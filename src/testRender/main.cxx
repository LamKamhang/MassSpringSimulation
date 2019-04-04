#include "renderer.hpp"
#include <unistd.h>

#include "../spring.hpp"
#include "../particle.hpp"
#include <memory>

using namespace std;

int main(int argc, char const *argv[])
{
		shared_ptr<Particle> m1(new Particle(10, false));
	shared_ptr<Particle> m2(new Particle(10, true, {1, 0, 0}));
	shared_ptr<Particle> m3(new Particle(10, false, {3, 0, 0}));
	shared_ptr<Particle> m4(new Particle(15, false, {6, 0, 0}));

	shared_ptr<Spring> s1(new Spring(5, 3));
	shared_ptr<Spring> s2(new Spring(4, 4, m2, m3));
	shared_ptr<Spring> s3(new Spring(3, 4, m3, m4));
	s1->bindRight(m2);
	s1->bindLeft(m1);
	vector<shared_ptr<Particle>* >mass{&m1, &m2, &m3, &m4};
	vector<shared_ptr<Spring>* > springs{&s1, &s2, &s3};

		ChainSystem1D test(mass, springs);
	shared_ptr<ChainSystem1D> system(&test);


	Renderer renderer;

	// Cylinder c(s1->getLeft()->getPos(), s1->getRight()->getPos());
	// renderer.AddActor(c.getData(), s1);

	int i = 0;
	for (auto item : springs)
	{
		i++;
		auto c = new Cylinder((*item)->getLeft()->getPos(), (*item)->getRight()->getPos(), {1.0*(i&0x1), 1.0*(i&0x2), 1.0*(i&0x4)});
		renderer.AddActor(c->getData(), *item);
	}

	// renderer.AddActor(m1.getData(), left);
	// renderer.AddActor(m2.getData(), right);
	renderer.AddTimerEvent(system);

	renderer.render();

	return 0;
}
