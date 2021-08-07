#include <constga/GA.h>

#include <iostream>
#include <array>

using namespace constga;
using namespace constga::literals;

int main()
{
	constexpr int mvA = 0b100; // e2
	constexpr int mvB = 0b010; // e1
	constexpr std::array<int, 9+6> signature { 1, 1, 1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1 };

	// 10e14
	MV<std::array<std::size_t, 1> { 14_basis }, signature> u({ 10 });

	// 5e14
	MV<std::array<std::size_t, 1> { 14_basis }, signature> v({ 5 });

	// 10e14 5e14 = -50
	auto w = u * v;

	std::cout << w.values[0] << std::endl;
	std::cout << u.values[0] << " " << u.values.size() << std::endl;
}