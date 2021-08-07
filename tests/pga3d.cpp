#include <gtest/gtest.h>

#include <constga/Concrete.h>

using namespace constga::pga3d;
using namespace constga::literals;

TEST(PGA3D, PointInnerPointEqualsSomething)
{
	constexpr Plane a({ 1, 0, 0, 5 });
	constexpr Plane b({ 0, 1, 0, 8 });
	constexpr Plane c({ 0, 0, 1, 9 });

	constexpr auto p = a ^ b ^ c;
}
