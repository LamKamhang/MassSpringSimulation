#include <iostream>
#include <memory>
#include <unistd.h>

#include "spring.hpp"
#include "particle.hpp"
#include "vector.hpp"
#include "simulation.hpp"

using namespace std;
using namespace mymath;

void render(unsigned left, unsigned right)
{
	for (unsigned i = 0; i < left; ++i)
	{
		cout << " ";
	}
	for (unsigned i = left; i <= right; ++i)
	{
		cout << "*";
	}
	cout  << endl;
}

int main(int argc, char const *argv[])
{
	// shared_ptr<Spring> spring(new Spring(50, 20));
	// shared_ptr<Particle> left(new Particle(300, true, {10, 0, 0}));
	// shared_ptr<Particle> right(new Particle(200, false, {20, 0, 0}));
	
	// spring->bindLeft(left);
	// spring->bindRight(right);

	// // cout << spring->getDeltaX() << endl;
	// NaiveSystem ns(spring);

	// while (1)
	// {
	// 	ns.update(0.1);
	// 	unsigned x0 = left->getPos().getItem(0)*5;
	// 	unsigned x1 = right->getPos().getItem(0)*5;
	// 	// cout << x0 << " " << x1 << endl;
	// 	render(x0, x1);
	// 	usleep(10000);
	// }


	// shared_ptr<Spring> s1(new Spring(4, 100));
	// shared_ptr<Spring> s2(new Spring(8, 100));
	// shared_ptr<Spring> s3(new Spring(16, 100));

	// shared_ptr<Particle> m1(new Particle(1, false, {0, 0, 0}));
	// shared_ptr<Particle> m2(new Particle(2, false, {100, 0, 0}));
	// shared_ptr<Particle> m3(new Particle(4, false, {200, 0, 0}));
	// shared_ptr<Particle> m4(new Particle(8, false, {250, 0, 0}, {-50, 0, 0}));

	shared_ptr<Spring> spring(new Spring(50, 20));
	shared_ptr<Particle> left(new Particle(300, true, {10, 0, 0}));
	shared_ptr<Particle> right(new Particle(200, false, {20, 0, 0}));
	
	spring->bindLeft(left);
	spring->bindRight(right);
	
	vector<shared_ptr<Particle>* >mass{&left, &right};
	vector<shared_ptr<Spring>* > springs{&spring};

	ChainSystem1D test(mass, springs);
	
	while (1)
	{
		test.update();
		unsigned x0 = left->getPos().getItem(0)*5;
		unsigned x1 = right->getPos().getItem(0)*5;
		// cout << x0 << " " << x1 << endl;
		render(x0, x1);
		usleep(100000);
	}

	return 0;
}
