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

	double h = 0.1;

	while (1)
	{
		myspring.update();
		auto m = m2->getMass();
		auto k = myspring.getK();
		auto x = myspring.getDeltaX();
		auto v = m2->getVel();
		auto deltaV = -(x + v*h)*h*k / (m + h*h * k);
		auto deltaX = (v + deltaV) * h;
		
		m2->update(deltaX, deltaV);
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
