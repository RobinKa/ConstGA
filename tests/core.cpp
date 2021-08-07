#include <gtest/gtest.h>

#include <constga/GA.h>

using namespace constga;
using namespace constga::literals;
using namespace constga::detail;

TEST(TestBitReordering, NumberOfSetBitsCorrectForInt)
{
	constexpr int x = NumberOfSetBits(0b0101010111010);
	constexpr int y = NumberOfSetBits(0);
	constexpr int z = NumberOfSetBits(0b111);

	ASSERT_EQ(x, 7);
	ASSERT_EQ(y, 0);
	ASSERT_EQ(z, 3);
}

TEST(TestBitReordering, CanonicalReorderingSignEuclideanCorrect)
{
	constexpr int x = BladeProductSignEuclidean(1_basis, 1_basis);
	constexpr int y = BladeProductSignEuclidean(1_basis, 2_basis);
	constexpr int z = BladeProductSignEuclidean(2_basis, 1_basis);

	ASSERT_EQ(x, 1);
	ASSERT_EQ(y, 1);
	ASSERT_EQ(z, -1);
}

TEST(TestBitReordering, CanonicalReorderingSignEuclidean)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };
	constexpr int x = BladeProductSign(0_basis, 0_basis, signature);
	constexpr int y = BladeProductSign(1_basis, 2_basis, signature);
	constexpr int z = BladeProductSign(2_basis, 1_basis, signature);

	ASSERT_EQ(x, 1);
	ASSERT_EQ(y, 1);
	ASSERT_EQ(z, -1);
}

TEST(TestProducts, GmtElementCorrect)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };

	// e1 e1 = 1 (sign: 1, bits: 0)
	constexpr auto xSign = BladeProductSign(0_basis, 0_basis, signature);
	constexpr auto xBits = BladeProductBits(0_basis, 0_basis);

	// e1 e2 = e12 (sign: 1, bits: 11)
	constexpr auto ySign = BladeProductSign(0_basis, 1_basis, signature);
	constexpr auto yBits = BladeProductBits(0_basis, 1_basis);

	// e2 e1 = -e12 (sign: -1, bits: 11)
	constexpr auto zSign = BladeProductSign(1_basis, 0_basis, signature);
	constexpr auto zBits = BladeProductBits(1_basis, 0_basis);

	ASSERT_EQ(xSign, 1);
	ASSERT_EQ(xBits, 0);
	ASSERT_EQ(ySign, 1);
	ASSERT_EQ(yBits, 0b11);
	ASSERT_EQ(zSign, -1);
	ASSERT_EQ(zBits, 0b11);
}

TEST(TestProducts, GpUniqueResultTypeCountCorrectForBivectorResult)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };

	constexpr auto basisBladesA = std::array<size_t, 1> { 0_basis };
	constexpr auto basisBladesB = std::array<size_t, 1> { 1_basis };

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);

	ASSERT_EQ(x.GetResultCount(), 1);
}

TEST(TestProducts, GpUniqueResultTypeCountCorrectForScalarPlusBivectorResult)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };

	// (e1 + e2)(e1 + e2) = scalar + e12
	constexpr auto basisBladesA = std::array<size_t, 2> { 0_basis, 1_basis };
	constexpr auto basisBladesB = std::array<size_t, 2> { 0_basis, 1_basis };

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);

	ASSERT_EQ(x.GetResultCount(), 2);
}

TEST(TestProducts, GpUniqueResultTypeCountCorrectForScalarResult)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };

	constexpr auto basisBladesA = std::array<size_t, 1> { 1_basis };
	constexpr auto basisBladesB = std::array<size_t, 1> { 1_basis };

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);

	ASSERT_EQ(x.GetResultCount(), 1);
}

TEST(TestGpResultTypes, TwoSameVectorsGivesScalar)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };
	constexpr auto basisBladesA = std::array<size_t, 1> { 1_basis };
	constexpr auto basisBladesB = std::array<size_t, 1> { 1_basis };

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);

	ASSERT_EQ(x.GetResultCount(), 1);
	ASSERT_EQ(x.GetResultTypes()[0], 0);
}

TEST(TestGpResultTypes, TwoDifferentVectorsGivesBivector)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };
	constexpr auto basisBladesA = std::array<size_t, 1> { 0_basis };
	constexpr auto basisBladesB = std::array<size_t, 1> { 1_basis };

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);

	ASSERT_EQ(x.GetResultCount(), 1);
	ASSERT_EQ(x.GetResultTypes()[0], 0b11);
}

TEST(TestGpResultTypes, 2DVectorTimesVectorEqualsScalarPlusBivector)
{
	constexpr std::array<int, 2> signature{ 1, 1 };
	constexpr auto basisBladesA = std::array<size_t, 2> { 0_basis, 1_basis };
	constexpr auto basisBladesB = std::array<size_t, 2> { 0_basis, 1_basis };

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);

	ASSERT_EQ(x.GetResultCount(), 2);
	ASSERT_TRUE((x.GetResultTypes()[0] == 0 && x.GetResultTypes()[1] == 0b11) || (x.GetResultTypes()[1] == 0 && x.GetResultTypes()[0] == 0b11));
}

TEST(TestGpResultTypes, 3DVectorTimesVectorEqualsScalarPlusBivector)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };
	constexpr auto basisBladesA = std::array<size_t, 3> { 0_basis, 1_basis, 2_basis };
	constexpr auto basisBladesB = std::array<size_t, 3> { 0_basis, 1_basis, 2_basis };

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);

	ASSERT_EQ(x.GetResultCount(), 4);
}

TEST(TestGpValues, TwoDifferentVectors)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };

	// 2e0 * 3e1 = 6e01
	constexpr auto basisBladesA = std::array<size_t, 1> { 0_basis };
	constexpr auto basisBladesB = std::array<size_t, 1> { 1_basis };
	constexpr auto valuesA = std::array<float, 1>{2.0f};
	constexpr auto valuesB = std::array<float, 1>{3.0f};

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);
	ASSERT_EQ(x.GetResultCount(), 1);

	constexpr auto values = Product<signature, basisBladesA, basisBladesB, x.GetResultCount(), x.GetResultIndices()>(valuesA, valuesB);

	ASSERT_EQ(values[0], 6);
}

TEST(TestGpValues, TwoSameVectorsSquareToNegativeScalar)
{
	constexpr std::array<int, 1> signature{ -1 };

	// 2e0 * 3e1 = 6e01
	constexpr auto basisBladesA = std::array<size_t, 1> { 0_basis };
	constexpr auto basisBladesB = std::array<size_t, 1> { 0_basis };
	constexpr auto valuesA = std::array<float, 1>{2.0f};
	constexpr auto valuesB = std::array<float, 1>{3.0f};

	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);
	ASSERT_EQ(x.GetResultCount(), 1);

	constexpr auto values = Product<signature, basisBladesA, basisBladesB, x.GetResultCount(), x.GetResultIndices()>(valuesA, valuesB);

	ASSERT_EQ(values[0], -6);
}

TEST(TestGpValues, 3DVectorTimesVector)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };

	// 2e0 + 3e1 + 4e2
	constexpr auto basisBladesA = std::array<size_t, 3> { 0_basis, 1_basis, 2_basis };
	constexpr auto valuesA = std::array<float, 3>{2.0f, 3.0f, 4.0f};

	// 5e0 + 6e1 + 7e2
	constexpr auto basisBladesB = std::array<size_t, 3> { 0_basis, 1_basis, 2_basis };
	constexpr auto valuesB = std::array<float, 3>{5.0f, 6.0f, 7.0f};

	// 56 - 3e01 - 6e02 - 3 e12
	constexpr auto x = ProductMeta(basisBladesA, basisBladesB, ProductType::GP);
	ASSERT_EQ(x.GetResultCount(), 4);

	constexpr auto values = Product<signature, basisBladesA, basisBladesB, x.GetResultCount(), x.GetResultIndices()>(valuesA, valuesB);

	ASSERT_EQ(values[0], 56);
	ASSERT_EQ(values[1], -3);
	ASSERT_EQ(values[2], -6);
	ASSERT_EQ(values[3], -3);
}