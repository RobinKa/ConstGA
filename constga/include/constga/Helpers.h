#pragma once

#include <stdint.h>
#include <array>
#include <algorithm>

namespace constga::detail
{
	constexpr int NumberOfSetBits(uint32_t i)
	{
		i = i - ((i >> 1) & 0x55555555);
		i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
		return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}

	template <typename T>
	constexpr auto Abs(T x)
	{
		return x < 0 ? -x : x;
	}

	template <auto count>
	consteval auto TrimArray(const auto& a)
	{
		std::array<std::size_t, count> result{};
		std::copy(a.begin(), a.begin() + count, result.begin());
		return result;
	}
}