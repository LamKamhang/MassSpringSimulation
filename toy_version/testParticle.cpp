#include <iostream>

#include "particle.hpp"
#include "vector.hpp"
#include <vector>

using namespace mymath;

std::vector<int> testing()
{
	return std::vector<int> {1,2,3,4};
}

int main(int argc, char const *argv[])
{
	Particle m1(10);
	Particle m2(20, false, {1, 2, 3});

	m1.addForce({2,3,4});
	
	std::cout << m1 << std::endl;
	std::cout << m2 << std::endl;

	return 0;
}
