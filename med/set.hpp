/**
@file
set IE container - tagged elements in any order

@copyright Denis Priyomov 2016-2017
Distributed under the MIT License
(See accompanying file LICENSE or visit https://github.com/cppden/med)
*/

#pragma once

#include "config.hpp"
#include "error.hpp"
#include "optional.hpp"
#include "container.hpp"
#include "encode.hpp"
#include "decode.hpp"
#include "name.hpp"
#include "tag.hpp"
#include "debug.hpp"

namespace med {

namespace sl {

/**
 * structural decoder of set
 */
template <class... IES>
struct set_decoder;

template <class IE, class... IES>
struct set_decoder<IE, IES...>
{
	template <class TO, class FUNC, class UNEXP, class HEADER>
	static inline MED_RESULT decode(TO& to, FUNC& func, UNEXP& unexp, HEADER const& header)
	{
		if constexpr (is_multi_field<IE>::value)
		{
			if (tag_type_t<IE>::match( get_tag(header) ))
			{
				IE& ie = static_cast<IE&>(to);
				CODEC_TRACE("[%s]@%zu", name<typename IE::field_type>(), ie.count());
				if (ie.count() >= IE::max)
				{
					return func(error::EXTRA_IE, name<typename IE::field_type>(), IE::max, ie.count());
				}
				auto* field = ie.push_back(func);
				return MED_EXPR_AND(field) med::decode(func, *field, unexp);
			}
			return set_decoder<IES...>::decode(to, func, unexp, header);
		}
		//single-instance field
		else
		{
			if (tag_type_t<IE>::match( get_tag(header) ))
			{
				IE& ie = static_cast<IE&>(to);
				CODEC_TRACE("[%s] = %u", name<typename IE::field_type>(), ie.ref_field().is_set());
				if (!ie.ref_field().is_set())
				{
					return med::decode(func, ie.ref_field(), unexp);
				}
				return func(error::EXTRA_IE, name<typename IE::field_type>(), 1);
			}
			else
			{
				return set_decoder<IES...>::decode(to, func, unexp, header);
			}
		}
	}

	template <class TO, class FUNC>
	static MED_RESULT check(TO const& to, FUNC& func)
	{
		if constexpr (is_multi_field<IE>::value)
		{
			IE const& ie = static_cast<IE const&>(to);
			return check_arity(func, ie)
				MED_AND set_decoder<IES...>::check(to, func);
		}
		//single-instance field
		else
		{
			IE const& ie = static_cast<IE const&>(to);
			if (is_optional_v<IE> || ie.ref_field().is_set())
			{
				return set_decoder<IES...>::check(to, func);
			}
			return func(error::MISSING_IE, name<typename IE::field_type>(), 1, 0);
		}
	}
};

template <>
struct set_decoder<>
{
	template <class TO, class FUNC, class UNEXP, class HEADER>
	static MED_RESULT decode(TO& to, FUNC& func, UNEXP& unexp, HEADER const& header)
	{
		return unexp(func, to, header);
	}

	template <class TO, class FUNC>
	static constexpr MED_RESULT check(TO&, FUNC&)       { MED_RETURN_SUCCESS; }
};

/**
 * structural encoder of set
 */
template <class... IES>
struct set_encoder;

template <class HEADER, class IE, class FUNC>
constexpr MED_RESULT encode_header(FUNC& func)
{
	if constexpr (std::is_base_of<PRIMITIVE, typename HEADER::ie_type>::value)
	{
		HEADER header;
		header.set_encoded(tag_type_t<IE>::get_encoded());
		return encode(func, header);
	}
	else if constexpr (std::is_base_of<CONTAINER, typename HEADER::ie_type>::value)
	{
		MED_RETURN_SUCCESS;
	}
}

template <class TO, class FUNC, class HEADER, class IE, class... IES>
inline MED_RESULT continue_encode(TO const& to, FUNC& func)
{
	if constexpr (is_optional_v<IE>)
	{
		return set_encoder<IES...>::template encode<HEADER>(to, func);
	}
	else
	{
		return func(error::MISSING_IE, name<typename IE::field_type>(), 1, 0);
	}
}

template <class IE, class... IES>
struct set_encoder<IE, IES...>
{
	template <class HEADER, class TO, class FUNC>
	static inline MED_RESULT encode(TO const& to, FUNC& func)
	{
		if constexpr (is_multi_field<IE>::value)
		{
			IE const& ie = static_cast<IE const&>(to);
			CODEC_TRACE("[%s]*%zu", name<typename IE::field_type>(), ie.count());

			MED_CHECK_FAIL(check_arity(func, ie));
			for (auto& field : ie)
			{
				//TODO: actually this field was pushed but not set... do we need a new error?
				if (!field.is_set())
				{
					return func(error::MISSING_IE, name<typename IE::field_type>(), ie.count(), ie.count()-1);
				}
				MED_CHECK_FAIL((encode_header<HEADER, IE>(func)) MED_AND med::encode(func, field));
			}
			return set_encoder<IES...>::template encode<HEADER>(to, func);
		}
		//single-instance field
		else
		{
			IE const& ie = static_cast<IE const&>(to);
			if (ie.ref_field().is_set())
			{
				CODEC_TRACE("[%s]", name<IE>());
				return encode_header<HEADER, IE>(func) MED_AND med::encode(func, ie.ref_field())
					MED_AND set_encoder<IES...>::template encode<HEADER>(to, func);
			}
			return continue_encode<TO, FUNC, HEADER, IE, IES...>(to, func);
		}
	}
};

template <>
struct set_encoder<>
{
	template <class HEADER, class TO, class FUNC>
	static constexpr MED_RESULT encode(TO&, FUNC&)       { MED_RETURN_SUCCESS; }
};

template <class T, class FUNC>
constexpr void pop_state(FUNC&& func)
{
	if constexpr (std::is_base_of<CONTAINER, typename T::ie_type>::value)
	{
		func(POP_STATE{});
	}
}

}	//end: namespace sl


template <class HEADER, class ...IES>
struct set : container<IES...>
{
	using header_type = HEADER;

	template <class ENCODER>
	MED_RESULT encode(ENCODER& encoder) const
	{
		return sl::set_encoder<IES...>::template encode<header_type>(this->m_ies, encoder);
	}

	template <class DECODER, class UNEXP>
	MED_RESULT decode(DECODER& decoder, UNEXP& unexp)
	{
		while (decoder(PUSH_STATE{}))
		{
			header_type header;
#if (MED_EXCEPTIONS)
			//TODO: avoid try/catch
			try
			{
				med::decode(decoder, header, unexp);
			}
			catch (med::exception const& ex)
			{
				decoder(POP_STATE{});
				decoder(error::SUCCESS);
				break;
			}
			sl::pop_state<header_type>(decoder);
			CODEC_TRACE("tag=%#zx", std::size_t(get_tag(header)));
			MED_CHECK_FAIL(sl::set_decoder<IES...>::decode(this->m_ies, decoder, unexp, header));
#else
			if (med::decode(decoder, header, unexp))
			{
				sl::pop_state<header_type>(decoder);
				CODEC_TRACE("tag=%#zx", std::size_t(get_tag(header)));
				MED_CHECK_FAIL(sl::set_decoder<IES...>::decode(this->m_ies, decoder, unexp, header));
			}
			else
			{
				decoder(POP_STATE{});
				decoder(error::SUCCESS);
				break;
			}
#endif //MED_EXCEPTIONS
		}

		return sl::set_decoder<IES...>::check(this->m_ies, decoder);
	}
};

}	//end: namespace med
