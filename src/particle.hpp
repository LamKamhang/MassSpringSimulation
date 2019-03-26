#pragma once

#include "vector.hpp"
#include <sstream>
#include <string>

class Particle
{
public:
	// constructor
	Particle(double mass = 0, bool fixed = false, 
			 mymath::Vector<double, 3> pos = mymath::Vector<double, 3>(),
			 mymath::Vector<double, 3> vel = mymath::Vector<double, 3>(),
			 mymath::Vector<double, 3> force = mymath::Vector<double, 3>());
	
	inline void addForce(mymath::Vector<double, 3> force);
	inline void addVel(mymath::Vector<double, 3> deltaVel);
	inline void move(mymath::Vector<double, 3> deltaPos);

	// getter
	inline mymath::Vector<double, 3> getPos()	const;
	inline mymath::Vector<double, 3> getVel()	const;
	inline mymath::Vector<double, 3> getAcc()	const;
	inline mymath::Vector<double, 3> getForce()	const;
	inline double					 getMass()	const;
	inline bool						 getState()	const;

	// setter
	inline void setPos(mymath::Vector<double, 3> pos);
	inline void setVel(mymath::Vector<double, 3> vec);
	inline void setForce(mymath::Vector<double, 3> force);
	inline void setMass(double mass);
	inline void setState(bool fixed);

	// toggle state
	inline void toggleFixed();

	inline operator std::string() const;
private:

private:
	mymath::Vector<double, 3> _velocity;
	mymath::Vector<double, 3> _position;
	mymath::Vector<double, 3> _force;
	double _mass;
	bool _isFixed;
};

Particle::Particle(double mass, bool fixed, 
			mymath::Vector<double, 3> pos,
			mymath::Vector<double, 3> vec,
			mymath::Vector<double, 3> force)
	: _velocity(vec)
	, _position(pos)
	, _force(force)
	, _mass(mass)
	, _isFixed(fixed)
{}

inline void Particle::addForce(mymath::Vector<double, 3> force)
{
	_force += force;
}

inline void Particle::addVel(mymath::Vector<double, 3> deltaVel)
{
	_velocity += deltaVel;
}

inline void Particle::move(mymath::Vector<double, 3> deltaPos)
{
	_position += deltaPos;
}

// getter
inline mymath::Vector<double, 3> Particle::getPos() const
{
	return _position;
}
inline mymath::Vector<double, 3> Particle::getVel() const
{
	return _velocity;
}
inline mymath::Vector<double, 3> Particle::getAcc() const
{
	return _force / _mass;
}
inline mymath::Vector<double, 3> Particle::getForce() const
{
	return _force;
}
inline double					 Particle::getMass() const
{
	return _mass;
}
inline bool						 Particle::getState() const
{
	return _isFixed;
}

// setter
inline void Particle::setPos(mymath::Vector<double, 3> pos)
{
	_position = pos;
}
inline void Particle::setVel(mymath::Vector<double, 3> vec)
{
	_velocity = vec;
}
inline void Particle::setForce(mymath::Vector<double, 3> force)
{
	_force = force;
}
inline void Particle::setMass(double mass)
{
	_mass = mass;
}
inline void Particle::setState(bool fixed)
{
	_isFixed = fixed;
}

// toggle state
inline void Particle::toggleFixed()
{
	_isFixed = !_isFixed;
}

inline Particle::operator std::string() const
{
	std::stringstream ss;
	ss << "[\n";
	ss << "  " << "state: " << _isFixed << "\n";
	ss << "  " << "mass : " << _mass << "\n";
	ss << "  " << "pos  : " << _position << "\n";
	ss << "  " << "vel  : " << _velocity << "\n";
	ss << "  " << "force: " << _force << "\n";
	ss << "]";
	return ss.str();
}

std::ostream& operator<< (std::ostream& os, const Particle &right)
{
	return os << (std::string) right;
}