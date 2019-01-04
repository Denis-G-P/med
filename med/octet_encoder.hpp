/**
@file
octet encoder definition

@copyright Denis Priyomov 2016-2017
Distributed under the MIT License
(See accompanying file LICENSE or visit https://github.com/cppden/med)
*/

#pragma once

#include "debug.hpp"
#include "name.hpp"
#include "state.hpp"
#include "octet_string.hpp"

namespace med {

template <class ENC_CTX>
struct octet_encoder
{
	//required for length_encoder
	using state_type = typename ENC_CTX::buffer_type::state_type;
	static constexpr std::size_t granularity = 8;

	ENC_CTX& ctx;

	explicit octet_encoder(ENC_CTX& ctx_) : ctx{ ctx_ } { }

	//state
	auto operator() (GET_STATE const&)                       { return ctx.buffer().get_state(); }
	void operator() (SET_STATE const&, state_type const& st) { ctx.buffer().set_state(st); }
	template <class IE>
	void operator() (SET_STATE const&, IE const& ie)
	{
		if (auto const ss = ctx.snapshot(ie))
		{
			if (ss.validate_length(get_length(ie)))
			{
				ctx.buffer().set_state(ss);
			}
			else
			{
				MED_RETURN_ERROR(error::INVALID_VALUE, (*this), name<IE>(), 0);
			}
		}
		else
		{
			MED_RETURN_ERROR(error::MISSING_IE, (*this), name<IE>(), 0);
		}
	}

	template <class IE>
	bool operator() (PUSH_STATE const&, IE const&)      { return ctx.buffer().push_state(); }
	void operator() (POP_STATE const&)                  { ctx.buffer().pop_state(); }
	void operator() (ADVANCE_STATE const& ss)
	{
		if (nullptr == ctx.buffer().advance(ss.bits/granularity))
		{ MED_RETURN_ERROR(error::OVERFLOW, (*this), "advance", ss.bits/granularity); }
	}
	void operator() (ADD_PADDING const& pad)
	{
		if (not ctx.buffer().fill(pad.bits/granularity, pad.filler))
		{ MED_RETURN_ERROR(error::OVERFLOW, (*this), "padding", pad.bits/granularity); }
	}
	void operator() (SNAPSHOT const& ss)                { ctx.snapshot(ss); }


	//errors
	template <typename... ARGS>
	void operator() (error e, ARGS&&... args)
	{
		return ctx.error_ctx().set_error(ctx.buffer(), e, std::forward<ARGS>(args)...);
	}

	//IE_VALUE
	template <class IE>
	void operator() (IE const& ie, IE_VALUE const&)
	{
		static_assert(0 == (IE::traits::bits % granularity), "OCTET VALUE EXPECTED");
		CODEC_TRACE("VAL[%s]=%#zx %zu bits: %s", name<IE>(), std::size_t(ie.get_encoded()), IE::traits::bits, ctx.buffer().toString());
		if (uint8_t* out = ctx.buffer().template advance<IE::traits::bits / granularity>())
		{
			put_bytes(ie, out);
		}
		else
		{
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), IE::traits::bits/granularity);
		}
	}

	//IE_OCTET_STRING
	template <class IE>
	void operator() (IE const& ie, IE_OCTET_STRING const&)
	{
		if ( uint8_t* out = ctx.buffer().advance(ie.size()) )
		{
			octets<IE::min_octets, IE::max_octets>::copy(out, ie.data(), ie.size());
			CODEC_TRACE("STR[%s] %zu octets: %s", name<IE>(), ie.size(), ctx.buffer().toString());
		}
		else
		{
			MED_RETURN_ERROR(error::OVERFLOW, (*this), name<IE>(), ie.size());
		}
	}

private:
	template <std::size_t NUM_BYTES, typename VALUE>
	static constexpr void put_byte(uint8_t*, VALUE const) { }

	template <std::size_t NUM_BYTES, typename VALUE, std::size_t OFS, std::size_t... Is>
	static void put_byte(uint8_t* output, VALUE const value)
	{
		output[OFS] = static_cast<uint8_t>(value >> ((NUM_BYTES - OFS - 1) << 3));
		put_byte<NUM_BYTES, VALUE, Is...>(output, value);
	}

	template<std::size_t NUM_BYTES, typename VALUE, std::size_t... Is>
	static void put_bytes_impl(uint8_t* output, VALUE const value, std::index_sequence<Is...>)
	{
		put_byte<NUM_BYTES, VALUE, Is...>(output, value);
	}

	template <class IE>
	static void put_bytes(IE const& ie, uint8_t* output)
	{
		constexpr std::size_t NUM_BYTES = IE::traits::bits / granularity;
		put_bytes_impl<NUM_BYTES>(output, ie.get_encoded(), std::make_index_sequence<NUM_BYTES>{});
	}
};

template <class ENC_CTX>
auto make_octet_encoder(ENC_CTX& ctx) { return octet_encoder<ENC_CTX>{ctx}; }

}	//end: namespace med
