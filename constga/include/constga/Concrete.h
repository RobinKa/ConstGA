#pragma once

#include "constga/GA.h"

using namespace constga::literals;

namespace constga
{
	namespace pga3d
	{
		constexpr std::array<int, 4> signature { 1, 1, 1, 0 };

		constexpr std::array<size_t, 4> planeBasisBlades { 0_basis, 1_basis, 2_basis, 3_basis };
		constexpr std::array<size_t, 3> idealPointBasisBlades { 0_basis | 2_basis | 3_basis, 0_basis | 1_basis | 3_basis, 0_basis | 1_basis | 2_basis };
		constexpr std::array<size_t, 4> pointBasisBlades { 0_basis | 2_basis | 3_basis, 0_basis | 1_basis | 3_basis, 0_basis | 1_basis | 2_basis, 1_basis | 2_basis | 3_basis };

		using Plane = MV<planeBasisBlades, signature>;
		using IdealPoint = MV<idealPointBasisBlades, signature>;
		using Point = MV<pointBasisBlades, signature>;
	}

	namespace qcga
	{
		constexpr std::array<int, 15> signature
		{
			1, 1, 1,
			1, -1,
			1, -1,
			1, -1,
			1, -1,
			1, -1,
			1, -1
		};

		constexpr std::array<size_t, 15> pointBasisBlades
		{
			0_basis, 1_basis, 2_basis, 3_basis, 4_basis,
			5_basis, 6_basis, 7_basis, 8_basis, 9_basis,
			10_basis, 11_basis, 12_basis, 13_basis, 14_basis
		};

		using Point = MV<pointBasisBlades, signature>;

		constexpr Point MakePoint(float x, float y, float z)
		{
			const float xx = x * x;
			const float yy = y * y;
			const float zz = z * z;

			const float xy = x * y;
			const float xz = x * z;
			const float yz = y * z;

			return Point(std::array<float, 15>
			{
				x, y, z,
				0.5f * xx - 0.5f, 0.5f * xx + 0.5f,
				0.5f * yy - 0.5f, 0.5f * yy + 0.5f,
				0.5f * zz - 0.5f, 0.5f * zz + 0.5f,
				xy, xy,
				xz, xz,
				yz, yz
			});
		};
	}
}