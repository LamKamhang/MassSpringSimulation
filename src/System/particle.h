#pragma once

#include <Eigen/Core>

#define FIXED_PARTICLE_MASS	1e10
#define UNSET_ID	-1
#define GRAVITY 1.0
#define EPSILON 1e-4

class Particle
{
public:
	Particle(const Eigen::Vector3d &x, bool fixed = false, 
		double mass = 1, const Eigen::Vector3d &v = {0, 0, 0});

	Particle(unsigned id, const Eigen::Vector3d &x, bool fixed = false, 
		double mass = 1, const Eigen::Vector3d &v = {0, 0, 0});

	void accumulate_grad(Eigen::VectorXd &grad, 
		const Eigen::VectorXd &x) const;

	void updateX(const Eigen::Vector3d &deltaX);
	void updateV(const Eigen::Vector3d &deltaV);

	void setX(const Eigen::Vector3d &x);
	void setV(const Eigen::Vector3d &v);

	void setFix(bool fixed);
	void setMass(double mass);
	void setID(unsigned id);

	Eigen::Vector3d getX() const;
	Eigen::Vector3d getV() const;
	bool getFixStatus() const;
	double getMass() const;
	unsigned getID() const;
private:
	void _init();

private:
	Eigen::Vector3d _x;
	Eigen::Vector3d _v;
	double _mass;
	unsigned _id;
	bool _fixed;
};

inline void Particle::_init()
{
	if (_fixed)
	{
		_v = {0, 0, 0};
		_mass = FIXED_PARTICLE_MASS;
	}
}

inline void Particle::accumulate_grad(Eigen::VectorXd &grad, 
                               const Eigen::VectorXd &x) const
{
	// if (x(3*_id + 1)  > EPSILON)
		grad(3*_id+1) += _mass * GRAVITY;
}

inline void Particle::updateX(const Eigen::Vector3d &deltaX)
{
	if (!_fixed)
		_x += deltaX;
}

inline void Particle::updateV(const Eigen::Vector3d &deltaV)
{
	if (!_fixed)
		_v += deltaV;
}

inline void Particle::setX(const Eigen::Vector3d &x)
{
	if (!_fixed)
		_x = x;
}

inline void Particle::setV(const Eigen::Vector3d &v)
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


inline Eigen::Vector3d Particle::getX() const
{
	return _x;
}

inline Eigen::Vector3d Particle::getV() const
{
	return _v;
}

inline bool Particle::getFixStatus() const
{
	return _fixed;
}

inline double Particle::getMass() const
{
	return _mass;
}

inline unsigned Particle::getID() const
{
	return _id;
}
