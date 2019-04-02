#pragma once

#include "particle.hpp"
#include <memory>

class Spring
{
public:
	// constructor
	Spring(double k, double len);
    Spring(double k, double len, std::shared_ptr<Particle> &left, std::shared_ptr<Particle> &right);

	// update
	inline void update();

	// getter
	inline std::shared_ptr<Particle> getLeft()	const;
	inline std::shared_ptr<Particle> getRight()	const;
	inline double getK()						const;
	inline double getOrigLen()					const;
	inline double getCurLen()					const;
	inline mymath::Vector<double, 3> getDeltaX()const;

	// setter
	inline void bindLeft(std::shared_ptr<Particle> &left);
	inline void bindRight(std::shared_ptr<Particle> &right);
	inline void setK(double k);
	inline void setOrigLen(double len);

	// move
	inline void move (const mymath::Vector<double, 3> &deltaPos);
	inline void moveL(const mymath::Vector<double, 3> &deltaPos);
	inline void moveR(const mymath::Vector<double, 3> &deltaPos);

private:
	inline mymath::Vector<double, 3> _calElasticForce();

private:
	std::shared_ptr<Particle> _left, _right;
	double _stiffness;
	double _lenOrig;
};

Spring::Spring(double k, double len)
	: _stiffness(k)
	, _lenOrig(len)
{
	_left = _right = nullptr;
}

Spring::Spring(double k, double len, std::shared_ptr<Particle> &left, std::shared_ptr<Particle> &right)
	: _left(left)
	, _right(right)
	, _stiffness(k)
	, _lenOrig(len)
{}

// update
inline void Spring::update()
{
	auto force = _calElasticForce();

	_right->addForce(force);
	_left->addForce(-force);
}

// getter
inline std::shared_ptr<Particle> Spring::getLeft() const
{
	return _left;
}
inline std::shared_ptr<Particle> Spring::getRight() const
{
	return _right;
}
inline double Spring::getK() const
{
	return _stiffness;
}
inline double Spring::getOrigLen() const
{
	return _lenOrig;
}
inline double Spring::getCurLen() const
{
	return (_left->getPos() - _right->getPos()).norm2();
}
inline mymath::Vector<double, 3> Spring::getDeltaX() const
{
	return _right->getOffset() - _left->getOffset();
	// auto LtoR = _right->getPos() - _left->getPos();
	// return LtoR * (1 - _lenOrig / LtoR.norm2());
}

// setter
inline void Spring::bindLeft(std::shared_ptr<Particle> &left)
{
	_left = left;
	if (_right != nullptr)
	{
		auto LtoR = _right->getPos() - _left->getPos();
		auto dx = LtoR * (1 - _lenOrig / LtoR.norm2());
		_left->setOffset(_right->getOffset() - dx);
	}
	else
	{
		_left->setOffset({0, 0, 0});
	}
	
}
inline void Spring::bindRight(std::shared_ptr<Particle> &right)
{
	_right = right;
	if (_left != nullptr)
	{
		auto LtoR = _right->getPos() - _left->getPos();
		auto dx = LtoR * (1 - _lenOrig / LtoR.norm2());
		_right->setOffset(_left->getOffset() + dx);
	}
	else
	{
		_right->setOffset({0, 0, 0});
	}
}
inline void Spring::setK(double k)
{
	_stiffness = k;
}
inline void Spring::setOrigLen(double len)
{
	_lenOrig = len;
}

// move
inline void Spring::move (const mymath::Vector<double, 3> &deltaPos)
{
	_left->move(deltaPos);
	_right->move(deltaPos);
}
inline void Spring::moveL(const mymath::Vector<double, 3> &deltaPos)
{
	_left->move(deltaPos);
}
inline void Spring::moveR(const mymath::Vector<double, 3> &deltaPos)
{
	_right->move(deltaPos);
}

inline mymath::Vector<double, 3> Spring::_calElasticForce()
{
	return getDeltaX() * (-_stiffness);
}