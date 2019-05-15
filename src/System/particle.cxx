#include "particle.h"


Particle::Particle(const Eigen::Vector3d &x, bool fixed, 
                   double mass, const Eigen::Vector3d &v)
	: _x(x)
	, _v(v)
	, _mass(mass)
	, _id(UNSET_ID)
	, _fixed(fixed)
{
	_init();
}

Particle::Particle(unsigned id, const Eigen::Vector3d &x, bool fixed, 
                   double mass, const Eigen::Vector3d &v)
	: _x(x)
	, _v(v)
	, _mass(mass)
	, _id(id)
	, _fixed(fixed)
{
	_init();
}
