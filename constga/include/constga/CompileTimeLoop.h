#pragma once

#include <type_traits>

namespace constga::detail
{
	/*template <auto... Xs>
	constexpr void for_values(std::index_sequence<Xs...>, auto f)
	{
		(f.template operator() < Xs > (), ...);
	}

	template <auto C, typename F>
	constexpr void for_index(F&& f)
	{
		for_values(std::make_index_sequence<C>{}, std::forward<F>(f));
	}*/

	template <int C, typename F>
	constexpr void for_index(F&& f)
	{
		if constexpr (C > 0)
		{
			for_index<C - 1>(f);
			f.template operator() <C - 1>();
		}
	}
}