#pragma once

#include <Eigen/Core>

#define FIXED_PARTICLE_MASS	1e10
#define UNSET_ID	-1
#define GRAVITY 10.0
#define EPSILON 1e-4

class Particle
{
public:
	Particle(Eigen::Vector3d x, Eigen::Vector3d v = {0, 0, 0}, bool fixed = false, double mass = 1);
	Particle(unsigned id, Eigen::Vector3d x, Eigen::Vector3d v = {0, 0, 0}, bool fixed = false, double mass = 1);
	~Particle();

	inline void accumulate_grad(Eigen::VectorXd &grad, const Eigen::VectorXd &x);
	inline void accumulate_hessian(Eigen::MatrixXd &hessian, const Eigen::VectorXd &x);

	inline void updateX(Eigen::Vector3d deltaX);
	inline void updateV(Eigen::Vector3d deltaV);

	inline void setX(Eigen::Vector3d x);
	inline void setV(Eigen::Vector3d v);
	inline void setFix(bool fixed);
	inline void setMass(double mass);
	inline void setID(unsigned id);

	inline Eigen::Vector3d getX();
	inline Eigen::Vector3d getV();
	inline bool getFixStatus();
	inline double getMass();
	inline unsigned getID();
private:

private:
	Eigen::Vector3d _x;
	Eigen::Vector3d _v;
	double _mass;
	unsigned _id;
	bool _fixed;
};

Particle::Particle(Eigen::Vector3d x, Eigen::Vector3d v, bool fixed, double mass)
	: _x(x)
	, _v(v)
	, _mass(mass)
	, _id(UNSET_ID)
	, _fixed(fixed)
{
	if (_fixed)
	{
		_v = {0, 0, 0};
		_mass = FIXED_PARTICLE_MASS;
	}
}

Particle::Particle(unsigned id, Eigen::Vector3d x, Eigen::Vector3d v, bool fixed, double mass)
	: _x(x)
	, _v(v)
	, _mass(mass)
	, _id(id)
	, _fixed(fixed)
{
	if (_fixed)
	{
		_v = {0, 0, 0};
		_mass = FIXED_PARTICLE_MASS;
	}
}

Particle::~Particle()
{

}

void Particle::accumulate_grad(Eigen::VectorXd &grad, const Eigen::VectorXd &x)
{
	if (x(3*_id + 2)  > EPSILON)
		grad(3*_id+2) += _mass * GRAVITY;
}

void Particle::accumulate_hessian(Eigen::MatrixXd &hessian, const Eigen::VectorXd &x)
{

}


inline void Particle::updateX(Eigen::Vector3d deltaX)
{
	if (!_fixed)
		_x += deltaX;
}

inline void Particle::updateV(Eigen::Vector3d deltaV)
{
	if (!_fixed)
		_v += deltaV;
}


inline void Particle::setX(Eigen::Vector3d x)
{
	if (!_fixed)
		_x = x;
}

inline void Particle::setV(Eigen::Vector3d v)
{
	if (!_fixed)
		_v = v;
}

inline void Particle::setFix(bool fixed)
{
	_fixed = fixed;
}

inline void Particle::setMass(double mass)
{
	if (!_fixed)
		_mass = mass;
}

inline void Particle::setID(unsigned id)
{
	_id = id;
}


inline Eigen::Vector3d Particle::getX()
{
	return _x;
}

inline Eigen::Vector3d Particle::getV()
{
	return _v;
}

inline bool Particle::getFixStatus()
{
	return _fixed;
}

inline double Particle::getMass()
{
	return _mass;
}

inline unsigned Particle::getID()
{
	return _id;
}
