#pragma once

#include "Helpers.h"
#include "CompileTimeLoop.h"
#include "Products.h"

#include <array>
#include <tuple>
#include <stdexcept>

namespace constga
{
	using namespace detail;

	namespace literals
	{
		constexpr unsigned long long operator""_basis(unsigned long long n)
		{
			return 1ULL << n;
		}
	}

	template <auto basisBladeBits, auto signature>
	class MV
	{
	private:
		constexpr static auto N = basisBladeBits.size();

	public:
		constexpr static auto BasisBladeBits = basisBladeBits;
		constexpr static auto Signature = signature;

		std::array<float, N> values;

		constexpr MV() = default;
		constexpr MV(const std::array<float, N>&values)
			: values(values)
		{
		}

		template <auto blade>
		constexpr float Get() const
		{
			//for_range<0, N>([this]<auto i>() bug
			for (int i = 0; i < N; i++)
			{
				if (basisBladeBits[i] == blade)
				{
					return values[i];
				}
			}

			throw std::invalid_argument("MV does not have blade.");
		}
	};

	namespace detail
	{
		template <auto signature, auto basisBladeBitsA, auto basisBladeBitsB, ProductType productType>
		constexpr static auto MVProduct(const MV<basisBladeBitsA, signature>& a, const MV<basisBladeBitsB, signature>& b)
		{
			constexpr auto meta = ProductMeta(basisBladeBitsA, basisBladeBitsB, productType);

			return MV<TrimArray<meta.GetResultCount()>(meta.GetResultTypes()), signature>(Product<signature, basisBladeBitsA, basisBladeBitsB, meta.GetResultCount(), meta.GetResultIndices()>(a.values, b.values));
		}
	}

	template <auto signature, auto basisBladeBitsA, auto basisBladeBitsB>
	constexpr static auto Gp(const MV<basisBladeBitsA, signature>& a, const MV<basisBladeBitsB, signature>& b)
	{
		return MVProduct<signature, basisBladeBitsA, basisBladeBitsB, ProductType::GP>(a, b);
	}

	template <auto signature, auto basisBladeBitsA, auto basisBladeBitsB>
	constexpr static auto Ip(const MV<basisBladeBitsA, signature>& a, const MV<basisBladeBitsB, signature>& b)
	{
		return MVProduct<signature, basisBladeBitsA, basisBladeBitsB, ProductType::IP>(a, b);
	}

	template <auto signature, auto basisBladeBitsA, auto basisBladeBitsB>
	constexpr static auto Op(const MV<basisBladeBitsA, signature>& a, const MV<basisBladeBitsB, signature>& b)
	{
		return MVProduct<signature, basisBladeBitsA, basisBladeBitsB, ProductType::OP>(a, b);
	}

	template <auto basisBladeBitsA, auto basisBladeBitsB, auto signature>
	constexpr auto operator* (const MV<basisBladeBitsA, signature>& a, const MV<basisBladeBitsB, signature>& b)
	{
		return Gp<signature>(a, b);
	}

	template <auto basisBladeBitsA, auto basisBladeBitsB, auto signature>
	constexpr auto operator| (const MV<basisBladeBitsA, signature>& a, const MV<basisBladeBitsB, signature>& b)
	{
		return Ip<signature>(a, b);
	}

	template <auto basisBladeBitsA, auto basisBladeBitsB, auto signature>
	constexpr auto operator^ (const MV<basisBladeBitsA, signature>& a, const MV<basisBladeBitsB, signature>& b)
	{
		return Op<signature>(a, b);
	}
}