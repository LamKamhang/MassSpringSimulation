#include <iostream>
#include <memory>
#include <unistd.h>

#include "spring.hpp"
#include "particle.hpp"
#include "vector.hpp"
#include "simulation.hpp"

using namespace std;
using namespace mymath;

// void render(unsigned left, unsigned right)
// {
// 	for (unsigned i = 0; i < left; ++i)
// 	{
// 		cout << " ";
// 	}
// 	for (unsigned i = left; i <= right; ++i)
// 	{
// 		cout << "*";
// 	}
// 	cout  << endl;
// }

int main(int argc, char const *argv[])
{
	// shared_ptr<Spring> spring(new Spring(50, 20));
	// shared_ptr<Particle> left(new Particle(300, false, {10, 0, 0}));
	// shared_ptr<Particle> right(new Particle(200, false, {20, 0, 0}));
	
	// spring->bindLeft(left);
	// spring->bindRight(right);

	// NaiveSystem ns(spring);

	// while (1)
	// {
	// 	ns.update(0.1);
	// 	unsigned x0 = left->getPos().getItem(0)*5;
	// 	unsigned x1 = right->getPos().getItem(0)*5;
	// 	// cout << x0 << " " << x1 << endl;
	// 	render(x0, x1);
	// 	usleep(100000);
	// }

	shared_ptr<Particle> m1(new Particle(100, true));
	shared_ptr<Particle> m2(new Particle(200, true));
	shared_ptr<Particle> m3(new Particle(300, true));
	shared_ptr<Particle> m4(new Particle(400, true));

	shared_ptr<Spring> s1(new Spring(4, 100));
	shared_ptr<Spring> s2(new Spring(8, 100));
	shared_ptr<Spring> s3(new Spring(16, 100));
	
	vector<shared_ptr<Particle>* >mass{&m1, &m2, &m3, &m4};
	vector<shared_ptr<Spring>* > springs{&s1, &s2, &s3};

	ChainSystem1D(mass, springs);

	return 0;
}
