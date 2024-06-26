#pragma once
#include "info.h"
#include "vectors.h"

#include <ostream>

template <typename CharT, typename Traits, vector vec>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, vec v)
{
	os << '(' << v[0] << ", " << v[1] << ')';
	return os;
}

template <typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os, const customer &c)
{
	os << '(' << c.pos() << ", " << c.demand() << ')';
	return os;
}

template <typename CharT, typename Traits, std::ranges::range Range>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Range &&r)
{
	os << '{';
	if (!std::ranges::empty(r))
		for (os << *std::ranges::begin(r); const auto &elem : r | std::views::drop(1))
			os << ", " << elem;
	os << '}';
	return os;
}