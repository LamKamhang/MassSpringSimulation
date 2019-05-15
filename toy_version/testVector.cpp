#include <iostream>
#include <vector>
#include "vector.hpp"

using namespace mymath;
int main(int argc, char const *argv[])
{
	// test constructor
	Vector<double, 3> vec1;
	int a[] = {1,2,3};
	Vector<int, 3> vec2(a, 3);
	std::vector<double> b{4,5,6};
	Vector<double, 3> vec3(b);
	Vector<double, 3> vec4(vec3);

	std::cout << (std::string)vec1 << std::endl;
	std::cout << (std::string)vec2 << std::endl;
	std::cout << (std::string)vec3 << std::endl;
	std::cout << (std::string)vec4 << std::endl;

	std::cout << vec1 << std::endl;

	std::cout << vec1 + vec3 << std::endl;
	std::cout << vec1 - vec3 << std::endl;
	std::cout << vec2 * 3 << std::endl;
	std::cout << vec3 / 2 << std::endl;
	std::cout << vec3 * vec4 << std::endl;

	std::cout << vec3.innerProduct(vec4) << std::endl;

	vec3 += vec4;
	std::cout << vec3 << std::endl;

	return 0;
}
