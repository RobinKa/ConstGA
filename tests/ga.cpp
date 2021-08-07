#include <gtest/gtest.h>

#include <constga/GA.h>

using namespace constga;
using namespace constga::literals;

TEST(TestGa, 3DVectorTimesVector)
{
	constexpr std::array<int, 3> signature{ 1, 1, 1 };

	// 2e0 + 3e1 + 4e2
	constexpr auto basisBladesA = std::array<size_t, 3> { 0_basis, 1_basis, 2_basis };
	constexpr auto valuesA = std::array<float, 3>{2.0f, 3.0f, 4.0f};

	// 5e0 + 6e1 + 7e2
	constexpr auto basisBladesB = std::array<size_t, 3> { 0_basis, 1_basis, 2_basis };
	constexpr auto valuesB = std::array<float, 3>{5.0f, 6.0f, 7.0f};

	constexpr MV<basisBladesA, signature> mvA(valuesA);
	constexpr MV<basisBladesA, signature> mvB(valuesB);

	// 56 - 3e01 - 6e02 - 3 e12
	constexpr auto mvX = mvA * mvB;

	ASSERT_EQ(mvX.values.size(), 4);
	ASSERT_EQ(mvX.values[0], 56);
	ASSERT_EQ(mvX.values[1], -3);
	ASSERT_EQ(mvX.values[2], -6);
	ASSERT_EQ(mvX.values[3], -3);
}
