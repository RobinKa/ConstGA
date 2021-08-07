#pragma once

#include "Helpers.h"
#include "CompileTimeLoop.h"

#include <algorithm>
#include <tuple>

namespace constga::detail
{
	enum class ProductType
	{
		GP,
		IP,
		OP
	};

	consteval int BladeProductSignEuclidean(int bitA, int bitB)
	{
		int a = bitA >> 1;
		int sumValue = 0;

		for (int i = 0; i < 32; i++)
		{
			sumValue += NumberOfSetBits(a & bitB);
			a >>= 1;
		}

		if ((sumValue & 1) == 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	consteval int BladeProductSign(int bitA, int bitB, auto signature)
	{
		int bitX = bitA & bitB;
		int outputSign = BladeProductSignEuclidean(bitA, bitB);

		for (int i = 0; i < signature.size(); i++)
		{
			if ((bitX & 1) != 0)
			{
				outputSign *= signature[i];
			}

			bitX >>= 1;
		}

		return outputSign;
	}

	consteval int BladeProductBits(int bitA, int bitB)
	{
		return bitA ^ bitB;
	}

	consteval bool IsAppropriateProduct(const auto& basisBladeA, const auto& basisBladeB, const auto& basisBladeResult, ProductType productType)
	{
		if (productType == ProductType::GP)
		{
			return true;
		}

		auto bitCountA = NumberOfSetBits(basisBladeA);
		auto bitCountB = NumberOfSetBits(basisBladeB);
		auto bitCountResult = NumberOfSetBits(basisBladeResult);

		return (productType == ProductType::IP && bitCountResult == Abs(bitCountA - bitCountB)) ||
			(productType == ProductType::OP && bitCountResult == bitCountA + bitCountB);
	}

	template <std::size_t sizeA, std::size_t sizeB>
	class ProductMetaData
	{
	private:
		constexpr static std::size_t resultSize = sizeA * sizeB;
		std::array<std::size_t, resultSize> resultTypes;
		int resultCount;
		std::array<std::array<std::size_t, sizeB>, sizeA> resultIndices;

	public:
		[[nodiscard]] constexpr const std::array<std::size_t, sizeA* sizeB>& GetResultTypes() const noexcept
		{
			return resultTypes;
		}

		[[nodiscard]] constexpr int GetResultCount() const noexcept
		{
			return resultCount;
		}

		[[nodiscard]] constexpr const std::array<std::array<std::size_t, sizeB>, sizeA>& GetResultIndices() const noexcept
		{
			return resultIndices;
		}

		constexpr ProductMetaData(
			const std::array<std::size_t, resultSize>& resultTypes,
			int resultCount,
			const std::array<std::array<std::size_t, sizeB>, sizeA>& resultIndices
		) : resultTypes(resultTypes), resultCount(resultCount), resultIndices(resultIndices)
		{

		}
	};

	template <std::size_t sizeA, std::size_t sizeB>
	consteval ProductMetaData<sizeA, sizeB> ProductMeta(const std::array<std::size_t, sizeA>& basisBladesA, const std::array<std::size_t, sizeB>& basisBladesB, ProductType productType)
	{
		std::array<std::size_t, sizeA * sizeB> resultTypes{};
		resultTypes.fill(-1);

		std::array<std::array<std::size_t, sizeB>, sizeA> resultIndices{};

		int nextIndex = 0;

		for (std::size_t i = 0; i < sizeA; i++)
		{
			for (std::size_t j = 0; j < sizeB; j++)
			{
				std::size_t gpResultBits = basisBladesA[i] ^ basisBladesB[j];

				if (IsAppropriateProduct(basisBladesA[i], basisBladesB[j], gpResultBits, productType))
				{
					auto last = resultTypes.begin() + nextIndex + 1;
					auto matchIndexIt = std::find(resultTypes.begin(), last, gpResultBits);
					if (matchIndexIt == last)
					{
						resultIndices[i][j] = nextIndex;
						resultTypes[nextIndex++] = gpResultBits;
					}
					else
					{
						resultIndices[i][j] = std::distance(resultTypes.begin(), matchIndexIt);
					}
				}
				else
				{
					resultIndices[i][j] = -1;
				}
			}
		}

		return ProductMetaData(resultTypes, nextIndex, resultIndices);
	}

	template <auto signature, auto basisBladesA, auto basisBladesB, std::size_t resultSize, auto resultIndices>
	constexpr std::array<float, resultSize> Product(const std::array<float, basisBladesA.size()>& a, const std::array<float, basisBladesB.size()>& b)
	{
		std::array<float, resultSize> resultValues{};
		resultValues.fill(0);

		for_index<basisBladesA.size()>([&]<auto i>()
		{
			for_index<basisBladesB.size()>([&]<auto j>()
			{
				constexpr int k = resultIndices[i][j];

				if constexpr (k != -1)
				{
					constexpr int gpSign = BladeProductSign(basisBladesA[i], basisBladesB[j], signature);
					if constexpr (gpSign != 0)
					{
						if constexpr (gpSign == 1)
						{
							resultValues[k] += a[i] * b[j];
						}
						else if constexpr (gpSign == -1)
						{
							resultValues[k] -= a[i] * b[j];
						}
						else
						{
							resultValues[k] += a[i] * b[j] * gpSign;
						}
					}
				}
			});
		});

		return resultValues;
	}
}