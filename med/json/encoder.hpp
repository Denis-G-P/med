/**
@file
JSON encoder

@copyright Denis Priyomov 2018
Distributed under the MIT License
(See accompanying file LICENSE or visit https://github.com/cppden/med)
*/

#pragma once

#include "../debug.hpp"
#include "../name.hpp"
#include "../state.hpp"
#include "json.hpp"

namespace med::json {

template <class ENC_CTX>
struct encoder
{
	//required for length_encoder
	using state_type = typename ENC_CTX::buffer_type::state_type;
	enum : std::size_t { granularity = 1 };
	static constexpr auto codec_kind = codec_e::STRUCTURED;

	ENC_CTX& ctx;

	explicit encoder(ENC_CTX& ctx_) : ctx{ ctx_ } { }

	//state
	auto operator() (GET_STATE const&)                       { return ctx.buffer().get_state(); }
	void operator() (SET_STATE const&, state_type const& st) { ctx.buffer().set_state(st); }

	template <class IE>
	bool operator() (PUSH_STATE const&, IE const&)           { return ctx.buffer().push_state(); }
	void operator() (POP_STATE const&)                       { ctx.buffer().pop_state(); }

	//errors
	template <typename... ARGS>
	void operator() (error e, ARGS&&... args)
	{
		return ctx.error_ctx().set_error(ctx.buffer(), e, std::forward<ARGS>(args)...);
	}

	//CONTAINER
	template <class IE>
	void operator() (IE const&, ENTRY_CONTAINER const&)
	{
		constexpr auto open_brace = []()
		{
			if constexpr (is_multi_field_v<IE>) return '[';
			else return '{';
		};

		CODEC_TRACE("CONTAINER-%c-[%s]: %s", open_brace(), name<IE>(), ctx.buffer().toString());
		if (auto* out = ctx.buffer().template advance<1>())
		{
			out[0] = open_brace();
		}
		else
		{
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), 1);
		}
	}
	template <class IE>
	void operator() (IE const&, HEADER_CONTAINER const&)
	{
		CODEC_TRACE("HEADER_CONTAINER[%s]: %s", name<IE>(), ctx.buffer().toString());
		if (auto* out = ctx.buffer().template advance<1>())
		{
			out[0] = ':';
		}
		else
		{
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), 1);
		}
	}
	template <class IE>
	void operator() (IE const&, EXIT_CONTAINER const&)
	{
		constexpr auto closing_brace = []()
		{
			if constexpr (is_multi_field_v<IE>) return ']';
			else return '}';
		};

		CODEC_TRACE("CONTAINER-%c-[%s]: %s", closing_brace(), name<IE>(), ctx.buffer().toString());
		if (auto* out = ctx.buffer().template advance<1>())
		{
			out[0] = closing_brace();
		}
		else
		{
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), 1);
		}
	}
	template <class IE>
	void operator() (IE const&, NEXT_CONTAINER_ELEMENT const&)
	{
		CODEC_TRACE("CONTAINER,[%s]: %s", name<IE>(), ctx.buffer().toString());
		if (auto* out = ctx.buffer().template advance<1>())
		{
			out[0] = ',';
		}
		else
		{
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), 1);
		}
	}

	//IE_VALUE
	template <class IE>
	void operator() (IE const& ie, IE_VALUE const&)
	{
		constexpr std::size_t len = [](){
			if constexpr (std::is_same_v<bool, typename IE::value_type>)
			{
				return 5; //max len of 'true'/'false'
			}
			else
			{
				return 32; //TODO: more precise estimation?
			}
		}();

		CODEC_TRACE("VAL[%s]: value_len=%zu %s", name<IE>(), len, ctx.buffer().toString());
		if (auto* out = ctx.buffer().template advance<len>())
		{
			if constexpr (std::is_same_v<bool, typename IE::value_type>)
			{
				if (ie.get())
				{
					octets<4,4>::copy(out, "true", 4);
					ctx.buffer().template advance<-1>();
				}
				else
				{
					octets<5,5>::copy(out, "false", 5);
				}
			}
			else if constexpr (std::is_floating_point_v<typename IE::value_type>)
			{
				int const written = std::snprintf(out, len, "%g", ie.get());
				ctx.buffer().advance(written - len);
			}
			else if constexpr (std::is_signed_v<typename IE::value_type>)
			{
				int const written = std::snprintf(out, len, "%lld", static_cast<long long>(ie.get()));
				ctx.buffer().advance(written - len);
			}
			else if constexpr (std::is_unsigned_v<typename IE::value_type>)
			{
				int const written = std::snprintf(out, len, "%llu", static_cast<unsigned long long>(ie.get()));
				ctx.buffer().advance(written - len);
			}
			else
			{
				MED_RETURN_ERROR(error::UNKNOWN_TAG, (*this), name<IE>(), 1);
			}
		}
		else
		{
			//TODO: report valid size
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), 1);
		}
	}

	//IE_OCTET_STRING
	template <class IE>
	void operator() (IE const& ie, IE_OCTET_STRING const&)
	{
		if (auto* out = ctx.buffer().advance(ie.size() + 2)) //2 quotes
		{
			*out++ = '"';
			octets<IE::min_octets, IE::max_octets>::copy(out, ie.data(), ie.size());
			CODEC_TRACE("STR[%s] %zu octets: %s", name<IE>(), ie.size(), ctx.buffer().toString());
			out[ie.size()] = '"';
		}
		else
		{
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), ie.size());
		}
	}

private:
};

}	//end: namespace med::json
