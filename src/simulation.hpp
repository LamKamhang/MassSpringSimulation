#pragma once

#include <vector>
#include <memory>

#include "particle.hpp"
#include "spring.hpp"
#include "vector.hpp"

class NaiveSystem
{
public:
	NaiveSystem(std::shared_ptr<Spring>& spring);
	void update(double timeStep);
	void reset(mymath::Vector<double, 3> leftPos, mymath::Vector<double, 3> rightPos);

private:
	// void _ExplicitEluler(double h);
	// void _SemiImplicitEuler(double h);
	void _ImplicitEuler(double h);

private:
	std::shared_ptr<Spring> _spring;
};

NaiveSystem::NaiveSystem(std::shared_ptr<Spring>& spring)
	: _spring(spring)
{}

void NaiveSystem::update(double timeStep)
{
	_ImplicitEuler(timeStep);
}

void NaiveSystem::reset(mymath::Vector<double, 3> leftPos, mymath::Vector<double, 3> rightPos)
{
	_spring->getLeft()->setPos(leftPos);
	_spring->getRight()->setPos(rightPos);
}

void NaiveSystem::_ImplicitEuler(double h)
{
	_spring->update();

	// get mass point
	auto left = _spring->getLeft();
	auto right = _spring->getRight();

	// get stiffness
	auto k = _spring->getK();
	// from left to right
	auto x = _spring->getDeltaX();

	// (m + h^2k)*DeltaV = -hk(x+hv)
	// DeltaX = h(v+DeltaV)
	// left
	auto m = left->getMass();
	auto v = left->getVel();
	auto deltaV = (-x + v*h)*(-h*k) / (m+h*h*k);
	auto deltaX = (v + deltaV) * h;
	left->addVel(deltaV);
	left->move(deltaX);

	// right
	m = right->getMass();
	v = right->getVel();
	deltaV = (x + v*h)*(-h*k) / (m+h*h*k);
	deltaX = (v + deltaV) * h;
	right->addVel(deltaV);
	right->move(deltaX);

	// clean force
	left->setForce({0, 0, 0});
	right->setForce({0, 0, 0});
}