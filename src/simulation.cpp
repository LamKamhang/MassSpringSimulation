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
	shared_ptr<Spring> spring(new Spring(50, 20));
	shared_ptr<Particle> left(new Particle(300, false, {10, 0, 0}));
	shared_ptr<Particle> right(new Particle(200, false, {20, 0, 0}));
	
	spring->bindLeft(left);
	spring->bindRight(right);

	NaiveSystem ns(spring);

	while (1)
	{
		ns.update(0.1);
		unsigned x0 = left->getPos().getItem(0)*5;
		unsigned x1 = right->getPos().getItem(0)*5;
		// cout << x0 << " " << x1 << endl;
		render(x0, x1);
		usleep(100000);
	}

	return 0;
}
