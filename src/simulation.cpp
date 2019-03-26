#include <iostream>
#include <memory>
#include <unistd.h>

#include "spring.hpp"
#include "particle.hpp"
#include "vector.hpp"

using namespace std;
using namespace mymath;

int main(int argc, char const *argv[])
{
	Spring myspring(1, 5);
	shared_ptr<Particle> m2(new Particle(2, false, {3, 0, 0}));
	shared_ptr<Particle> m1(new Particle(0, true));
	myspring.bindLeft(m1);
	myspring.bindRight(m2);

	double timeStep = 0.1;

	while (1)
	{
		myspring.update();
		m1->update(timeStep);
		m2->update(timeStep);
		sleep(1);
		cout << "m2 : " << m2->getPos() << endl;
		cout << "len: " << myspring.getCurLen() << endl;
		cout << "vel: " << m2->getVel() << endl;
		cout << "acc: " << m2->getAcc() << endl;
		cout << "force: " << m2->getForce() << endl;
		cout << endl;
		m1->setForce({0, 0, 0});
		m2->setForce({0, 0, 0});
	}
	return 0;
}
