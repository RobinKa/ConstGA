#include <gtest/gtest.h>

#include <constga/Concrete.h>

#include <string>
#include <vector>

using namespace constga;
using namespace constga::qcga;
using namespace constga::literals;

TEST(QCGA, BasisVectorTimesBasisVector)
{
	// 2e14
	constexpr auto basisBladesA = std::array<size_t, 1> { 14_basis };
	constexpr auto valuesA = std::array<float, 1>{2.0f };

	// 3e1
	constexpr auto basisBladesB = std::array<size_t, 1> { 1_basis };
	constexpr auto valuesB = std::array<float, 1>{3.0f};

	constexpr MV<basisBladesA, signature> mvA(valuesA);
	constexpr MV<basisBladesB, signature> mvB(valuesB);

	// 6e14e1 = -6e1e14
	constexpr auto mvX = mvA * mvB;

	ASSERT_EQ(mvX.values[0], -6);
	ASSERT_EQ(mvX.values.size(), 1);
	ASSERT_EQ(mvX.BasisBladeBits.size(), 1);
	ASSERT_EQ(mvX.BasisBladeBits[0], 1_basis | 14_basis);
}

TEST(QCGA, FullVectorTimesFullVector)
{
	constexpr std::array<float, 15> valuesA
	{
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	};

	constexpr std::array<float, 15> valuesB
	{
		91, 92, 93, 94, 95, 96, 97, 98, 99, 910, 911, 912, 913, 914, 915
	};

	constexpr constga::MV<constga::qcga::pointBasisBlades, signature> mvA(valuesA);
	constexpr constga::MV<constga::qcga::pointBasisBlades, signature> mvB(valuesB);

	constexpr auto mvX = mvA * mvB;
}

TEST(QCGA, PointsInnerProductEqualsMinusHalfDistance)
{
	constexpr auto p1 = MakePoint(1, 2, 3);
	constexpr auto p2 = MakePoint(4, 5, 6);

	const float distanceSquared = 27;

	constexpr auto ipAB = p1 | p2;

	ASSERT_EQ(ipAB.values.size(), 1);
	ASSERT_FLOAT_EQ(ipAB.Get<0>(), -0.5f * distanceSquared);
}

TEST(QCGA, FivePointsOuterProductGivesQuadric)
{
	auto f = []()
	{
		return float(rand() % 10000) / 5000.0f - 1.0f;
	};

	auto p1 = MakePoint(f(), f(), f());
	auto p2 = MakePoint(f(), f(), f());
	auto p3 = MakePoint(f(), f(), f());
	auto p4 = MakePoint(f(), f(), f());
	auto p5 = MakePoint(f(), f(), f());

	auto x = p1 ^ p2 ^ p3;
	//auto quadric = p1 ^ p2 ^ p3 ^ p4 ^ p5;
	//auto quadric2 = quadric ^ p3;
	//auto quadric = qcga::op(qcga::op(qcga::op(qcga::op(qcga::op(mvA, mvB), mvC), mvD), mvE);
}