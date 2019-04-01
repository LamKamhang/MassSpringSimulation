#pragma once

#include <vector>
#include <memory>

#include "particle.hpp"
#include "spring.hpp"
#include "vector.hpp"

#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Cholesky>

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

class ChainSystem1D
{
public:
	ChainSystem1D(const std::vector<std::shared_ptr<Particle>* > &mass,
				  const std::vector<std::shared_ptr<Spring>* > &springs,
				  double h = 0.1);
	void update();

private:
	void _ImplicitEuler();

private:
	std::vector<std::shared_ptr<Particle>* > _mass;
	std::vector<std::shared_ptr<Spring>* > _springs;
	double timeStep;
	Eigen::MatrixXd A;
	Eigen::MatrixXd K;
};

ChainSystem1D::ChainSystem1D(const std::vector<std::shared_ptr<Particle>* > &mass,
							 const std::vector<std::shared_ptr<Spring>* > &springs,
							 double h)
	: _mass(mass)
	, _springs(springs)
	, timeStep(h)
	, A(_mass.size(), _mass.size())
	, K(_mass.size(), _mass.size())
{
	auto size = _mass.size();

	// frst line
	K(0, 0) = (*springs[0])->getK();
	K(0, 1) = -(*springs[0])->getK();
	for (unsigned i = 2; i < size; ++i)
	{
		K(0, i) = 0;
	}

	// 2 to n-1
	for (unsigned i = 1; i < size-1; ++i)
	{
		for (unsigned j = 0; j < i-1; ++j)
		{
			K(i, j) = 0;
		}
		K(i, i-1) = -(*springs[i-1])->getK();
		K(i, i) = (*springs[i-1])->getK()+(*springs[i])->getK();
		K(i, i+1) = -(*springs[i])->getK();
		for (unsigned j = i+2; j < size; ++j)
		{
			K(i, j) = 0;
		}
	}

	// last line
	for (unsigned i = 0; i < size - 2; ++i)
	{
		K(size-1, i) = 0;
	}

	K(size-1, size-2) = -(*springs[size-2])->getK();
	K(size-1, size-1) = (*springs[size-2])->getK();

	std::cout << K << std::endl;

	for (unsigned i = 0; i < size; ++i)
	{
		for (unsigned j = 0; j < i; ++j)
		{
			A(i, j) = 0;
		}
		A(i, i) = (*_mass[i])->getMass();
		for (unsigned j = i+1; j < size; ++j)
		{
			A(i, j) = 0;
		}
	}

	std::cout << "A" << std::endl;
	std::cout << A << std::endl;

	A += h*h*K;

	std::cout << "A" << std::endl;
	std::cout << A << std::endl;
}

void ChainSystem1D::_ImplicitEuler()
{

}