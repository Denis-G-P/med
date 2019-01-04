/**
@file
multi-instance field (aka sequence-of) and its aggregates

@copyright Denis Priyomov 2016-2017
Distributed under the MIT License
(See accompanying file LICENSE or visit https://github.com/cppden/med)
*/

#pragma once

#include <iterator>
#include <limits>

#include "exception.hpp"
#include "field.hpp"
#include "debug.hpp"


namespace med {

template <std::size_t MIN>
struct min : std::integral_constant<std::size_t, MIN> {};

template <std::size_t MAX>
struct max : std::integral_constant<std::size_t, MAX> {};

template <std::size_t MAX>
struct pmax : std::integral_constant<std::size_t, MAX> {};

using inf = pmax< std::numeric_limits<std::size_t>::max() >;

template <std::size_t N>
struct arity : std::integral_constant<std::size_t, N> {};


namespace {

template <std::size_t MIN, class CMAX>
struct get_inplace;

template <std::size_t MIN, std::size_t MAX>
struct get_inplace<MIN, max<MAX>> : std::integral_constant<std::size_t, MAX> {};

template <std::size_t MIN, std::size_t MAX>
struct get_inplace<MIN, pmax<MAX>> : std::integral_constant<std::size_t, MIN> {};

}

template <class FIELD, std::size_t MIN, class CMAX>
class multi_field
{
	static_assert(MIN > 0, "MIN SHOULD BE GREATER ZERO");
	static_assert(CMAX::value >= MIN, "MAX SHOULD BE GREATER OR EQUAL TO MIN");
	//static_assert(MIN <= INPLACE && INPLACE <= MAX, "INPLACE SHOULD BE BETWEEN MIN AND MAX");
	static_assert(is_field_v<FIELD>, "FIELD IS REQUIRED");

public:
	using ie_type = typename FIELD::ie_type;
	using field_type = FIELD;

	struct field_value
	{
		field_type   value;
		field_value* next;
	};

	enum bounds : std::size_t
	{
		min     = MIN,
		max     = CMAX::value,
		inplace = get_inplace<MIN, CMAX>::value,
	};

	multi_field() = default;

private:
	template <class T>
	class iter_type
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = std::conditional_t<std::is_const_v<T>, field_type const*, field_type*>;
		using reference = std::conditional_t<std::is_const_v<T>, field_type const&, field_type&>;

		explicit iter_type(value_type* p = nullptr) : m_curr{p} { }
		iter_type& operator++()                     { m_curr = m_curr ? m_curr->next : nullptr; return *this; }
		iter_type operator++(int)                   { iter_type ret = *this; ++(*this); return ret;}
		bool operator==(iter_type const& rhs) const { return m_curr == rhs.m_curr; }
		bool operator!=(iter_type const& rhs) const { return !(*this == rhs); }
		reference operator*() const                 { return *get(); }
		pointer operator->() const                  { return get(); }
		pointer get() const                         { return m_curr ? &m_curr->value : nullptr; }
		explicit operator bool() const              { return nullptr != m_curr; }

	private:
		value_type* m_curr;
	};

public:
	using iterator = iter_type<field_value>;
	iterator begin()                                        { return iterator{empty() ? nullptr : m_fields}; }
	iterator end()                                          { return iterator{}; }
	using const_iterator = iter_type<field_value const>;
	const_iterator begin() const                            { return const_iterator{empty() ? nullptr : m_fields}; }
	const_iterator end() const                              { return const_iterator{}; }

	std::size_t count() const                               { return m_count; }
	bool empty() const                                      { return 0 == m_count; }
	//NOTE: clear won't return items allocated from external storage, use reset there
	void clear()                                            { m_count = 0; }
	bool is_set() const                                     { return not empty() && m_fields[0].value.is_set(); }

	field_type const* first() const                         { return empty() ? nullptr : &m_fields[0].value; }
	field_type const* last() const                          { return empty() ? nullptr : &m_tail->value; }

	[[deprecated("inefficient, will be removed")]]
	field_type const* at(std::size_t index) const
	{
		if (index >= count()) return nullptr;
		if (index < inplace) return &m_fields[index].value;
		const_iterator it = begin(), ite = end();
		while (index && it != ite) { --index; ++it; }
		return (it && it->is_set()) ? it.get() : nullptr;
	}

	//uses inplace storage only
	field_type* push_back()
	{
		if (count() < inplace)
		{
			return inplace_push_back();
		}
		throw out_of_memory("No space for in-place '%.32s': %zu bytes", name<field_type>(), sizeof(field_type));
	}

	//uses inplace or external storage
	//NOTE: check for max is done during encode/decode
	template <class CTX>
	field_type* push_back(CTX& ctx)
	{
		//try inplace 1st then external
		if (count() < inplace)
		{
			return inplace_push_back();
		}
		else
		{
			auto* piv = get_allocator_ptr(ctx)->template allocate<field_value>();
			++m_count;
			m_tail->next = piv;
			piv->next = nullptr;
			m_tail = piv;
			return &m_tail->value;
		}
	}

	//won't recover space if external storage was used
	void pop_back()
	{
		if (auto const num = count())
		{
			//clear the last
			m_tail->value.clear();
			//locate previous before last
			auto* prev = m_fields;
			for (std::size_t i = 2; i < num; ++i)
			{
				prev = prev->next;
			}
			CODEC_TRACE("%s: %s[%zu]=%p", __FUNCTION__, name<field_type>(), num-1, (void*)prev->next);
			prev->next = nullptr;
			m_tail = (--m_count) ? prev : nullptr;
		}
	}

private:
	//checks are done by caller
	field_type* inplace_push_back()
	{
		auto* piv = m_fields + count();
		CODEC_TRACE("%s: inplace %s[%zu]=%p", __FUNCTION__, name<field_type>(), count(), (void*)piv);
		if (m_count++) { m_tail->next = piv; }
		piv->next = nullptr;
		m_tail = piv;
		return &m_tail->value;
	}

	multi_field(multi_field const&) = delete;
	multi_field& operator= (multi_field const&) = delete;

	field_value  m_fields[inplace];
	std::size_t  m_count {0};
	field_value* m_tail {nullptr};
};


template <class TAG, class VAL, std::size_t MIN, class CMAX>
struct multi_tag_value : multi_field<VAL, MIN, CMAX>, tag_t<TAG>
{
	using ie_type = IE_TV;
};

template <class LEN, class VAL, std::size_t MIN, class CMAX>
struct multi_length_value : multi_field<VAL, MIN, CMAX>, LEN
{
	using ie_type = IE_LV;
};

template <class, class Enable = void>
struct is_multi_field : std::false_type { };
template <class T>
struct is_multi_field<T, std::void_t<typename T::field_value>> : std::true_type { };
template <class T>
constexpr bool is_multi_field_v = is_multi_field<T>::value;

}	//end: namespace med
