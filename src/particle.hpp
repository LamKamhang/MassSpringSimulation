#pragma once

#include "vector.hpp"
#include <sstream>
#include <string>

class Particle
{
public:
	// constructor
	Particle(double mass = 0, bool fixed = false, 
			 mymath::Vector<double, 3> pos = {0, 0, 0},
			 mymath::Vector<double, 3> vel = {0, 0, 0},
			 mymath::Vector<double, 3> force = {0, 0, 0});
	
	inline void addForce(const mymath::Vector<double, 3> &force);
	inline void addVel(const mymath::Vector<double, 3> &deltaV);
	inline void move(const mymath::Vector<double, 3> &deltaX);


	// getter
	inline mymath::Vector<double, 3> getPos()	const;
	inline mymath::Vector<double, 3> getOffset()const;
	inline mymath::Vector<double, 3> getVel()	const;
	inline mymath::Vector<double, 3> getAcc()	const;
	inline mymath::Vector<double, 3> getForce()	const;
	inline double					 getMass()	const;
	inline bool						 getState()	const;

	// setter
	inline void setPos(mymath::Vector<double, 3> pos);
	inline void setOffset(mymath::Vector<double, 3> offset);
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
	mymath::Vector<double, 3> _offset;
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
	, _offset({0, 0, 0})
{
	if (_isFixed)
	{
		_force = _velocity = mymath::Vector<double, 3> {0, 0, 0};
	}
}

inline void Particle::addForce(const mymath::Vector<double, 3> &force)
{
	if (_isFixed)
		return;
	_force += force;
}

inline void Particle::addVel(const mymath::Vector<double, 3> &deltaV)
{
	if (_isFixed)
		return;
	_velocity += deltaV;
}

inline void Particle::move(const mymath::Vector<double, 3> &deltaX)
{
	if (_isFixed)
		return;
	_offset += deltaX;
}

// getter
inline mymath::Vector<double, 3> Particle::getPos() const
{
	return _position + _offset;
}
inline mymath::Vector<double, 3> Particle::getOffset()const
{
	return _offset;
}
inline mymath::Vector<double, 3> Particle::getVel() const
{
	return _velocity;
}
inline mymath::Vector<double, 3> Particle::getAcc() const
{
	if (_isFixed)
		return {0, 0, 0};
	else
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
inline void Particle::setOffset(mymath::Vector<double, 3> offset)
{
	if (!_isFixed)
		_offset = offset;
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
	ss << "  " << "pos  : " << _position + _offset << "\n";
	ss << "  " << "vel  : " << _velocity << "\n";
	ss << "  " << "force: " << _force << "\n";
	ss << "]";
	return ss.str();
}

std::ostream& operator<< (std::ostream& os, const Particle &right)
{
	return os << (std::string) right;
}