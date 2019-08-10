/**
@file
length type definition and traits

@copyright Denis Priyomov 2016-2017
Distributed under the MIT License
(See accompanying file LICENSE or visit https://github.com/cppden/med)
*/

#pragma once

#include "exception.hpp"
#include "ie_type.hpp"
#include "state.hpp"
#include "value_traits.hpp"
#include "name.hpp"


namespace med {

template <class LENGTH>
struct length_t
{
	using length_type = LENGTH;
};

namespace detail {

template <class, class Enable = void>
struct has_length_type : std::false_type { };
template <class T>
struct has_length_type<T, std::void_t<typename T::length_type>> : std::true_type { };

template <class, typename Enable = void>
struct has_get_length : std::false_type { };
template <class T>
struct has_get_length<T, std::void_t<decltype(std::declval<T const>().get_length())>> : std::true_type { };

template <class, typename Enable = void>
struct has_size : std::false_type { };
template <class T>
struct has_size<T, std::void_t<decltype(std::declval<T const>().size())>> : std::true_type { };

template <class, typename Enable = void>
struct has_set_length : std::false_type { };
template <class T>
struct has_set_length<T, std::void_t<decltype(std::declval<T>().set_length(0))>> : std::true_type { };

template <class WRAPPER>
struct length_getter;

} //end: namespace detail


template <class T>
constexpr bool is_length_v = detail::has_length_type<T>::value;


template <class FIELD, class ENCODER>
constexpr std::size_t field_length(FIELD const& field, ENCODER& encoder)
{
	return detail::length_getter<FIELD>::get(field, encoder);
}
//wrapper is med::mandatory or med::optional
template <class WRAPPER, class FIELD, class ENCODER, typename Enable = std::enable_if_t<!std::is_same_v<WRAPPER, FIELD>>>
constexpr std::size_t field_length(FIELD const& field, ENCODER& encoder)
{
	return detail::length_getter<WRAPPER>::get(field, encoder);
}

namespace detail {

template <class WRAPPER>
struct length_getter
{
	template <class FIELD, class ENCODER>
	static constexpr std::size_t get(FIELD const& field, ENCODER& encoder)
	{
		if constexpr (is_peek_v<WRAPPER>)
		{
			return 0;
		}
		else
		{
			if constexpr (std::is_base_of_v<CONTAINER, typename WRAPPER::ie_type>)
			{
				CODEC_TRACE("%s[%s] : SEQ", __FUNCTION__, name<FIELD>());
				return field.calc_length(encoder);
			}
			else if constexpr (std::is_base_of_v<IE_TV, typename WRAPPER::ie_type>)
			{
				using tag_t = typename WRAPPER::tag_type;
				CODEC_TRACE("%s[%s] : TV", __FUNCTION__, name<FIELD>());
				return field_length(tag_t{}, encoder) + field_length<FIELD>(ref_field(field), encoder);
			}
			else if constexpr (std::is_base_of_v<IE_LV, typename WRAPPER::ie_type>)
			{
				using len_t = typename WRAPPER::length_type;
				CODEC_TRACE("%s[%s] : LV", __FUNCTION__, name<FIELD>());
				return field_length(len_t{}, encoder) + field_length<FIELD>(ref_field(field), encoder);
			}
			else if constexpr (std::is_base_of_v<PRIMITIVE, typename WRAPPER::ie_type>)
			{
				CODEC_TRACE("%s[%s] : PRIMITIVE", __FUNCTION__, name<FIELD>());
				return encoder(GET_LENGTH{}, field);
			}
			else
			{
				static_assert (std::is_void_v<typename WRAPPER::ie_type>, "unexpected");
			}
		}
	}
};


template<class T>
static auto test_value_to_length(int, std::size_t val = 0) ->
	std::enable_if_t<
		std::is_same_v<bool, decltype(T::value_to_length(val))>, std::true_type
	>;

template<class>
static auto test_value_to_length(long) -> std::false_type;

template <class T>
using has_length_converters = decltype(detail::test_value_to_length<T>(0));

}	//end: namespace detail


template <class FIELD>
constexpr void length_to_value(FIELD& field, std::size_t len)
{
	//convert length to raw value if needed
	if constexpr (detail::has_length_converters<FIELD>::value)
	{
		if (not FIELD::length_to_value(len))
		{
			MED_THROW_EXCEPTION(invalid_value, name<FIELD>(), len)
		}
	}

	CODEC_TRACE("L=%zx[%s]:", len, name<FIELD>());

	//set the length IE with the value
	if constexpr (detail::has_set_length<FIELD>::value)
	{
		if constexpr (std::is_same_v<bool, decltype(field.set_length(len))>)
		{
			if (not field.set_length(len))
			{
				MED_THROW_EXCEPTION(invalid_value, name<FIELD>(), len)
			}
		}
		else
		{
			field.set_length(len);
		}
	}
	else if constexpr (std::is_same_v<bool, decltype(field.set_encoded(len))>)
	{
		if (not field.set_encoded(len))
		{
			MED_THROW_EXCEPTION(invalid_value, name<FIELD>(), len)
		}
	}
	else
	{
		field.set_encoded(len);
	}
}

template <class FIELD>
constexpr void value_to_length(FIELD& field, std::size_t& len)
{
	//use proper length accessor
	if constexpr (detail::has_get_length<FIELD>::value)
	{
		len = field.get_length();
	}
	else
	{
		len = field.get_encoded();
	}

	CODEC_TRACE("L=%zu [%s]", len, name<FIELD>());

	//convert raw value to length if needed
	if constexpr (detail::has_length_converters<FIELD>::value)
	{
		if (not FIELD::value_to_length(len))
		{
			MED_THROW_EXCEPTION(invalid_value, name<FIELD>(), len)
		}
	}
}

}	//end: namespace med
