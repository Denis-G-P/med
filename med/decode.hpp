/**
@file
decoding entry point

@copyright Denis Priyomov 2016-2017
Distributed under the MIT License
(See accompanying file LICENSE or visit https://github.com/cppden/med)
*/

#pragma once

#include <utility>

#include "error.hpp"
#include "optional.hpp"
#include "state.hpp"
#include "padding.hpp"
#include "debug.hpp"
#include "length.hpp"
#include "name.hpp"
#include "placeholder.hpp"
#include "value.hpp"


namespace med {

//structure layer
namespace sl {

struct default_handler
{
	template <class FUNC, class IE, class HEADER>
	constexpr MED_RESULT operator()(FUNC& func, IE const&, HEADER const& header) const
	{
		CODEC_TRACE("ERROR tag=%zu", std::size_t(get_tag(header)));
		return func(error::INCORRECT_TAG, name<IE>(), get_tag(header));
	}
};

template <class WRAPPER, class FUNC, class IE, class UNEXP>
constexpr MED_RESULT decode_ie(FUNC&, IE& ie, IE_NULL const&, UNEXP&)
{
	CODEC_TRACE("NULL %s", name<IE>());
	ie.set();
	MED_RETURN_SUCCESS; //do nothing
}

template <class WRAPPER, class FUNC, class IE, class UNEXP>
constexpr MED_RESULT decode_ie(FUNC& func, IE& ie, PRIMITIVE const&, UNEXP&)
{
	CODEC_TRACE("PRIMITIVE %s", name<IE>());
	if constexpr (is_peek_v<IE>)
	{
#if (MED_EXCEPTIONS)
		if (func(PUSH_STATE{}))
		{
			func(ie, typename WRAPPER::ie_type{});
			func(POP_STATE{});
		}
#else
		return func(PUSH_STATE{})
			&& func(ie, typename WRAPPER::ie_type{})
			&& func(POP_STATE{});
#endif //MED_EXCEPTIONS
	}
	else if constexpr (is_skip_v<IE>)
	{
		//TODO: need to support fixed octet_string here?
		return func.template advance<IE::traits::bits>();
	}
	else
	{
		return func(ie, typename WRAPPER::ie_type{});
	}
}

//Tag-Value
template <class WRAPPER, class FUNC, class IE, class UNEXP>
//std::enable_if_t<!is_optional_v<IE>, MED_RESULT>
inline MED_RESULT decode_ie(FUNC& func, IE& ie, IE_TV const&, UNEXP& unexp)
{
	CODEC_TRACE("TV %s", name<IE>());
	//convert const to writable
	using TAG = typename WRAPPER::tag_type::writable;
	TAG tag;
	MED_CHECK_FAIL(func(tag, typename TAG::ie_type{}));
	if (WRAPPER::tag_type::match(tag.get_encoded()))
	{
		CODEC_TRACE("TV[%zu : %s]", std::size_t(tag.get()), name<typename WRAPPER::field_type>());
		return decode(func, ref_field(ie), unexp);
	}
	//NOTE: this can only be called for mandatory field thus it's fail case (not unexpected)
	CODEC_TRACE("ERROR tag=%zu", std::size_t(tag.get_encoded()));
	return func(error::INCORRECT_TAG, name<typename WRAPPER::field_type>(), tag.get_encoded());
}

//Length-Value
template <class WRAPPER, class FUNC, class IE, class UNEXP>
inline MED_RESULT decode_ie(FUNC& func, IE& ie, IE_LV const&, UNEXP& unexp)
{
	typename WRAPPER::length_type len_ie;
	CODEC_TRACE("LV[%s]", name<IE>());
	MED_CHECK_FAIL((decode(func, len_ie, unexp)));
	std::size_t len_value = len_ie.get_encoded();
	MED_CHECK_FAIL((value_to_length(func, len_ie, len_value)));
	//CODEC_TRACE("LV[%zu] : %s", len, name<IE>());
	auto end = func(PUSH_SIZE{len_value});
	MED_CHECK_FAIL((decode(func, ref_field(ie), unexp)));
	//TODO: ??? as warning not error
	if (0 != end.size()) return func(error::OVERFLOW, name<typename WRAPPER::field_type>(), end.size() * FUNC::granularity);
	MED_RETURN_SUCCESS;
}

template <class FUNC, class IE, class UNEXP>
struct length_decoder
{
	using state_type = typename FUNC::state_type;
	using size_state = typename FUNC::size_state;
	using allocator_type = typename FUNC::allocator_type;
	static constexpr std::size_t granularity = FUNC::granularity;

	using length_type = typename IE::length_type;

	length_decoder(FUNC& decoder, IE& ie, UNEXP& unexp) noexcept
		: m_decoder{ decoder }
		, m_ie{ ie }
		, m_start{ m_decoder(GET_STATE{}) }
		, m_unexp{ unexp }
	{
		CODEC_TRACE("start %s with length...:", name<IE>());
	}

	void restore_length()
	{
		m_size_state.restore_end();
	}

#ifdef CODEC_TRACE_ENABLE
	~length_decoder()
	{
		CODEC_TRACE("finish %s with length...:", name<IE>());
	}
#endif

	template <int DELTA>
	MED_RESULT operator() (placeholder::_length<DELTA>&)
	{
		length_type len_ie;
		MED_CHECK_FAIL(decode(m_decoder, len_ie, m_unexp));
		//reduced size of the input buffer for current length and elements from the start of IE
		std::size_t len_value = len_ie.get_encoded();
		MED_CHECK_FAIL(value_to_length(m_decoder, len_ie, len_value));
		typename length_type::value_type const size = (len_value + DELTA) - (m_decoder(GET_STATE{}) - m_start);
		CODEC_TRACE("size(%zu)=length(%zu) + %d - %ld", std::size_t(size), len_value, DELTA, (m_decoder(GET_STATE{}) - m_start));
		m_size_state = m_decoder(PUSH_SIZE{size});
		if (!m_size_state) return m_decoder(error::OVERFLOW, name<get_field_type_t<IE>>(), m_size_state.size() * FUNC::granularity, size * FUNC::granularity);
		MED_RETURN_SUCCESS;
	}

	//check if placeholder was visited
	explicit operator bool() const                     { return static_cast<bool>(m_size_state); }
	auto size() const                                  { return m_size_state.size(); }

	template <class ...T>
	auto operator() (T&&... args)                      { return m_decoder(std::forward<T>(args)...); }

	allocator_type& get_allocator()                    { return m_decoder.get_allocator(); }

	FUNC&            m_decoder;
	IE&              m_ie;
	state_type const m_start;
	size_state       m_size_state; //to switch end of buffer for this IE
	UNEXP&           m_unexp;
};

template <class WRAPPER, class FUNC, class IE, class UNEXP>
inline MED_RESULT decode_ie(FUNC& func, IE& ie, CONTAINER const&, UNEXP& unexp)
{
	if constexpr (has_length_type<IE>::value)
	{
		if constexpr (has_padding<IE>::value)
		{
			using pad_t = padder<IE, FUNC>;
			pad_t pad{func};
			length_decoder<FUNC, IE, UNEXP> ld{ func, ie, unexp };
			MED_CHECK_FAIL(ie.decode(ld, unexp));
			//special case for empty elements w/o length placeholder
			pad.enable(static_cast<bool>(ld));
			CODEC_TRACE("%sable padding bits=%zu for len=%zu", ld?"en":"dis", pad.size(), ld.size());
			if constexpr (pad_t::pad_traits::inclusive)
			{
				MED_CHECK_FAIL(pad.add());
				ld.restore_length();
			}
			else
			{
				ld.restore_length();
				return pad.add();
			}
			//TODO: may treat this case as warning? happens only in case of last IE with padding ended prematuraly
			//if (std::size_t const left = ld.size() * FUNC::granularity - padding_size(pad))
			// return func(error::OVERFLOW, name<IE>(), left * FUNC::granularity);
		}
		else
		{
			length_decoder<FUNC, IE, UNEXP> ld{ func, ie, unexp };
			MED_CHECK_FAIL(ie.decode(ld, unexp));
			ld.restore_length();
		}
		MED_RETURN_SUCCESS;
	}
	else
	{
		CODEC_TRACE("%s w/o length...:", name<IE>());
		return ie.decode(func, unexp);
	}
}

}	//end: namespace sl

template <class FUNC, class IE, class UNEXP = sl::default_handler>
constexpr MED_RESULT decode(FUNC&& func, IE& ie, UNEXP&& unexp = sl::default_handler{})
{
	if constexpr (has_ie_type<IE>::value)
	{
		return sl::decode_ie<IE>(func, ie, typename IE::ie_type{}, unexp);
	}
	else
	{
		return func(ie);
	}
}

}	//end: namespace med
