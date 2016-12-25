/*!
TODO: define.

@copyright Denis Priyomov 2016
Distributed under the MIT License
(See accompanying file LICENSE or copy at https://opensource.org/licenses/MIT)
*/

#pragma once

#include "ie.hpp"
#include "state.hpp"
#include "container.hpp"
#include "encode.hpp"
#include "decode.hpp"
#include "debug.hpp"

namespace med {

namespace sl {

template <class Enable, class... IES>
struct seq_dec_imp;

template <class... IES>
using seq_decoder = seq_dec_imp<void, IES...>;

//single-instance mandatory field
template <class IE, class... IES>
struct seq_dec_imp<std::enable_if_t<
		!has_multi_field_v<IE>
		&& !is_optional_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		CODEC_TRACE("{%s}...", name<IE>());
		IE& ie = to;
		return med::decode(func, ie, unexp) && seq_decoder<IES...>::decode(to, func, unexp, vtag);
	}
};

//TODO: more flexible is to compare current size read and how many bits to rewind for RO IE.
template <class IE, class FUNC, class TAG>
std::enable_if_t<is_read_only_v<typename IE::tag_type>>
inline clear_tag(FUNC& func, TAG& vtag)
{
	func(POP_STATE{});
	vtag.clear();
}

template <class IE, class FUNC, class TAG>
std::enable_if_t<!is_read_only_v<typename IE::tag_type>>
inline clear_tag(FUNC&, TAG& vtag)
{
	vtag.clear();
}

//single-instance optional field with a tag
template <class IE, class... IES>
struct seq_dec_imp<std::enable_if_t<
		!has_multi_field_v<IE>
		&& is_optional_v<IE>
		&& !has_optional_type<IE>::value
		&& has_tag_type_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		//CODEC_TRACE("[%s]...", name<IE>());
		if (!vtag)
		{
			//save state before decoding a tag
			if (func(PUSH_STATE{}))
			{
				if (auto const tag = decode_tag<typename IE::tag_type>(func))
				{
					vtag.set_encoded(tag.get_encoded());
					CODEC_TRACE("pop tag=%zx", tag.get_encoded());
				}
				else
				{
					return false;
				}
			}
			else
			{
				CODEC_TRACE("EoF at %s", name<IE>());
				return true; //end of buffer
			}
		}

		if (IE::tag_type::match(vtag.get_encoded())) //check tag decoded
		{
			CODEC_TRACE("T=%zx[%s]", vtag.get_encoded(), name<IE>());
			clear_tag<IE>(func, vtag); //clear current tag as decoded
			IE& ie = to;
			if (!med::decode(func, ie.ref_field(), unexp)) return false;
		}
		return seq_decoder<IES...>::decode(to, func, unexp, vtag);
	}
};

//single-instance optional field without a tag (optional by end of data)
template <class IE, class... IES>
struct seq_dec_imp<std::enable_if_t<
		!has_multi_field_v<IE>
		&& is_optional_v<IE>
		&& !has_tag_type_v<IE>
		&& !has_optional_type<IE>::value
	>,
	IE, IES...>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		CODEC_TRACE("[%s]...", name<IE>());
		if (vtag)
		{
			CODEC_TRACE("discard tag=%zx", vtag.get_encoded());
			func(POP_STATE{}); //restore state
			vtag.clear();
		}

		if (func(CHECK_STATE{}))
		{
			IE& ie = to;
			return med::decode(func, ie.ref_field(), unexp)
				&& seq_decoder<IES...>::decode(to, func, unexp, vtag);
		}
		CODEC_TRACE("EOF at [%s]", name<IE>());
		return true; //end of buffer
	}
};

//single-instance optional field with a presence-functor
template <class IE, class... IES>
struct seq_dec_imp<std::enable_if_t<
		!has_multi_field_v<IE>
		&& is_optional_v<IE>
		&& has_optional_type<IE>::value
	>,
	IE, IES...>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		CODEC_TRACE("[%s]...", name<IE>());
		if (typename IE::optional_type{}(to))
		{
			CODEC_TRACE("[%s]", name<IE>());
			IE& ie = to;

			if (vtag)
			{
				CODEC_TRACE("discard tag=%zx", vtag.get_encoded());
				func(POP_STATE{}); //restore state
				vtag.clear();
			}

			if (!med::decode(func, ie.ref_field(), unexp)) return false;
		}
		return seq_decoder<IES...>::decode(to, func, unexp, vtag);
	}
};

//multi-instance optional or mandatory field with a tag
template <class IE, class... IES>
struct seq_dec_imp<std::enable_if_t<
		has_multi_field_v<IE>
		&& has_tag_type_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		IE& ie = to;

		if (!vtag)
		{
			if (func(PUSH_STATE{}))
			{
				if (auto const tag = decode_tag<typename IE::tag_type>(func))
				{
					vtag.set_encoded(tag.get_encoded());
					CODEC_TRACE("pop tag=%zx", vtag.get_encoded());
				}
				else
				{
					return false;
				}
			}
		}

		while (IE::tag_type::match(vtag.get_encoded()))
		{
			CODEC_TRACE("T=%zx[%s]*", vtag.get_encoded(), name<IE>());

			if (auto* field = func.allocate(ie))
			{
				if (!med::decode(func, *field, unexp)) return false;
			}
			else
			{
				return false;
			}

			if (func(PUSH_STATE{}))
			{
				if (auto const tag = decode_tag<typename IE::tag_type>(func))
				{
					vtag.set_encoded(tag.get_encoded());
					CODEC_TRACE("pop tag=%zx", vtag.get_encoded());
				}
				else
				{
					vtag.clear();
				}
			}
			else
			{
				vtag.clear();
				break;
			}
		}

		if (check_arity<IE>(field_count(ie)))
		{
			if (!vtag)
			{
				func(POP_STATE{}); //restore state
				func(error::SUCCESS); //clear error
			}
			return seq_decoder<IES...>::decode(to, func, unexp, vtag);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), IE::min, field_count(ie));
			return false;
		}
	}
};

//multi-instance field without tag, counter or count-getter
template <class IE, class... IES>
struct seq_dec_imp<std::enable_if_t<
		has_multi_field_v<IE> &&
		!has_tag_type_v<IE> &&
		!has_count_getter_v<IE> &&
		!is_counter_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		IE& ie = to;

		if (vtag)
		{
			CODEC_TRACE("discard tag=%zx", vtag.get_encoded());
			func(POP_STATE{}); //restore state
			vtag.clear();
		}

		CODEC_TRACE("[%s]*[%zu..%zu]", name<IE>(), IE::min, IE::max);

		std::size_t count = 0;
		while (func(CHECK_STATE{}) && count < IE::max)
		{
			if (auto* field = func.allocate(ie))
			{
				if (!sl::decode<IE>(func, *field, typename IE::ie_type{}, unexp)) return false;
			}
			else
			{
				return false;
			}
			++count;
		}

		if (check_arity<IE>(count))
		{
			return seq_decoder<IES...>::decode(to, func, unexp, vtag);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), IE::min, count);
			return false;
		}
	}
};


//multi-instance field without tag with count-getter
template <class IE, class... IES>
struct seq_dec_imp<
	std::enable_if_t<
		has_multi_field_v<IE> &&
		!has_tag_type_v<IE> &&
		has_count_getter_v<IE>
	>,
	IE, IES...
>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		CODEC_TRACE("[%s]...", name<IE>());
		IE& ie = to;

		if (vtag)
		{
			CODEC_TRACE("discard tag=%zx", vtag.get_encoded());
			func(POP_STATE{}); //restore state
			vtag.clear();
		}

		std::size_t count = typename IE::count_getter{}(to);

		CODEC_TRACE("[%s]*%zu", name<IE>(), count);
		while (count--)
		{
			if (auto* field = func.allocate(ie))
			{
				if (!med::decode(func, *field, unexp)) return false;
			}
			else
			{
				return false;
			}
		}

		if (check_arity<IE>(field_count(ie)))
		{
			return seq_decoder<IES...>::decode(to, func, unexp, vtag);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), IE::min, field_count(ie));
			return false;
		}
	}
};

//multi-instance field without tag with counter
template <class IE, class... IES>
struct seq_dec_imp<
	std::enable_if_t<
		has_multi_field_v<IE> &&
		!has_tag_type_v<IE> &&
		is_counter_v<IE>
	>,
	IE, IES...
>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static inline bool decode(TO& to, FUNC&& func, UNEXP& unexp, TAG& vtag)
	{
		CODEC_TRACE("[%s]...", name<IE>());
		IE& ie = to;

		if (vtag)
		{
			CODEC_TRACE("discard tag=%zx", vtag.get_encoded());
			func(POP_STATE{}); //restore state
			vtag.clear();
		}

		typename IE::counter_type counter_ie;
		if (!med::decode(func, counter_ie)) return false;

		auto count = counter_ie.get_encoded();
		if (check_arity<IE>(count))
		{
			while (count--)
			{
				CODEC_TRACE("[%s]*%zu", name<IE>(), count);

				if (auto* field = func.allocate(ie))
				{
					if (!med::decode(func, *field, unexp)) return false;
				}
				else
				{
					return false;
				}
			}

			return seq_decoder<IES...>::decode(to, func, unexp, vtag);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), IE::min, count);
			return false;
		}
	}
};


template <>
struct seq_dec_imp<void>
{
	template <class TO, class FUNC, class UNEXP, class TAG>
	static bool decode(TO&, FUNC&&, UNEXP&, TAG&)               { return true; }
};


template <class Enable, class... IES>
struct seq_enc_imp;
template <class... IES>
using seq_encoder = seq_enc_imp<void, IES...>;

template <class IE, class... IES>
struct seq_enc_imp<std::enable_if_t<
		!has_multi_field_v<IE> &&
		!is_optional_v<IE> &&
		!has_setter_type_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC>
	static inline bool encode(TO const& to, FUNC&& func)
	{
		IE const& ie = to;
		CODEC_TRACE("%c{%s}", ie.ref_field().is_set()?'+':'-', class_name<IE>());
		if (ie.ref_field().is_set())
		{
			return med::encode(func, ie) && seq_encoder<IES...>::encode(to, func);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), 1, 0);
			return false;
		}
	}
};

template <class IE, class... IES>
struct seq_enc_imp<std::enable_if_t<
		!has_multi_field_v<IE> &&
		!is_optional_v<IE> &&
		has_setter_type_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC>
	static inline bool encode(TO const& to, FUNC&& func)
	{
		CODEC_TRACE("{%s} with setter", name<IE>());
		typename IE::setter_type{}(const_cast<TO&>(to)); //TODO: copy of IE to not const_cast?
		IE const& ie = to;
		if (ie.ref_field().is_set())
		{
			return med::encode(func, ie) && seq_encoder<IES...>::encode(to, func);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), 1, 0);
			return false;
		}
	}
};

template <class IE, class... IES>
struct seq_enc_imp<std::enable_if_t<
		!has_multi_field_v<IE> &&
		is_optional_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC>
	static inline bool encode(TO const& to, FUNC&& func)
	{
		IE const& ie = to;
		CODEC_TRACE("%c[%s]", ie.ref_field().is_set()?'+':'-', name<IE>());
		if (ie.ref_field().is_set())
		{
			return med::encode(func, ie) && seq_encoder<IES...>::encode(to, func);
		}
		else
		{
			return seq_encoder<IES...>::encode(to, func);
		}
	}
};


//multi-field w/o counter
template <class IE, class... IES>
struct seq_enc_imp<std::enable_if_t<
		has_multi_field_v<IE> &&
		!is_counter_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC>
	static inline bool encode(TO const& to, FUNC&& func)
	{
		IE const& ie = to;
		int const count = med::encode_multi(func, ie);
		if (count < 0) return false;

		if (check_arity<IE>(count))
		{
			return seq_encoder<IES...>::encode(to, func);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), IE::min, count);
			return false;
		}
	}
};

//mandatory multi-field w/ counter
template <class IE, class... IES>
struct seq_enc_imp<std::enable_if_t<
		has_multi_field_v<IE> &&
		is_counter_v<IE> &&
		!is_optional_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC>
	static inline bool encode(TO const& to, FUNC&& func)
	{
		IE const& ie = to;
		std::size_t const count = field_count(ie);
		CODEC_TRACE("CV[%s]=%zu", name<IE>(), count);
		if (check_arity<IE>(count))
		{
			typename IE::counter_type counter_ie;
			counter_ie.set_encoded(count);

			return med::encode(func, counter_ie)
				&& med::encode_multi(func, ie) == (int)count
				&& seq_encoder<IES...>::encode(to, func);
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), IE::min, count);
		}
		return false;
	}
};

//optional multi-field w/ counter
template <class IE, class... IES>
struct seq_enc_imp<std::enable_if_t<
		has_multi_field_v<IE> &&
		is_counter_v<IE> &&
		is_optional_v<IE>
	>,
	IE, IES...>
{
	template <class TO, class FUNC>
	static inline bool encode(TO const& to, FUNC&& func)
	{
		IE const& ie = to;
		std::size_t const count = field_count(ie);
		CODEC_TRACE("CV[%s]=%zu", name<IE>(), count);
		if (check_arity<IE>(count))
		{
			if (count > 0)
			{
				typename IE::counter_type counter_ie;
				counter_ie.set_encoded(count);

				return med::encode(func, counter_ie)
					&& med::encode_multi(func, ie) == (int)count
					&& seq_encoder<IES...>::encode(to, func);
			}
			else
			{
				return seq_encoder<IES...>::encode(to, func);
			}
		}
		else
		{
			func(error::MISSING_IE, name<typename IE::field_type>(), IE::min, count);
		}
		return false;
	}
};

template <>
struct seq_enc_imp<void>
{
	template <class TO, class FUNC>
	static inline bool encode(TO&, FUNC&& func)
	{
		return true;
	}
};

}	//end: namespace sl

template <class ...IES>
struct sequence : container<IES...>
{
	template <class ENCODER>
	bool encode(ENCODER&& encoder) const
	{
		return sl::seq_encoder<IES...>::encode(this->m_ies, encoder);
	}

	template <class DECODER, class UNEXP>
	bool decode(DECODER&& decoder, UNEXP& unexp)
	{
		value<32> vtag; //todo: any clue from sequence?
		return sl::seq_decoder<IES...>::decode(this->m_ies, decoder, unexp, vtag);
	}
};


}

