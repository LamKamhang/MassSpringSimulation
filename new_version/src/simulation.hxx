#pragma once

#include <stdexcept>
#include <vector>
#include <memory>
#include <Eigen/Core>

#include "particle.hxx"
#include "spring.hxx"

#define EPSILON 1e-4
#define NEWTON_STEPS 10

class NetSystem
{
public:
	NetSystem(double h = 0.1, const std::vector<std::shared_ptr<Particle>* > &particle_vec = {}, const std::vector<std::shared_ptr<Spring >* > &spring_vec = {});
	~NetSystem();

	void assemble_complete();
	void setAssembleable(bool flag);
	bool getAssembleable();

	void setH(double h);
	double getH();

	unsigned add_particle(Eigen::Vector3d x, Eigen::Vector3d v = {0, 0, 0}, bool fixed = false, double mass = 1);
	unsigned add_particle(Particle *ptr);

	void add_spring(double lid, double rid, double k = 5, double l = 10);
	void add_spring(Spring *ptr);

	void simulate();

	void render();
private:
	void _setup();
	void _update();
	Eigen::VectorXd _newton_iterate();
	Eigen::VectorXd _f(const Eigen::VectorXd &x);
	Eigen::MatrixXd _df(const Eigen::VectorXd &x);

private:
	std::vector<std::shared_ptr<Particle>* > _particle_vec;
	std::vector<std::shared_ptr<Spring >* > _spring_vec;

	Eigen::MatrixXd _mass;
	Eigen::VectorXd _xt;
	Eigen::VectorXd _vt;

	double _h;
	bool _assembleable;
};

NetSystem::NetSystem(double h, const std::vector<std::shared_ptr<Particle>* > &particle_vec, const std::vector<std::shared_ptr<Spring>* > &spring_vec)
	: _particle_vec(particle_vec)
	, _spring_vec(spring_vec)
	, _h(h)
	, _assembleable(true)
{
}

NetSystem::~NetSystem()
{
}

void NetSystem::assemble_complete()
{
	_assembleable = false;
	_setup();
}

void NetSystem::setAssembleable(bool flag)
{
	_assembleable = flag;
	if (!_assembleable)
		_setup();
}

bool NetSystem::getAssembleable()
{
	return _assembleable;
}

void NetSystem::setH(double h)
{
	_h = h;
}

double NetSystem::getH()
{
	return _h;
}

unsigned NetSystem::add_particle(Eigen::Vector3d x, Eigen::Vector3d v, bool fixed, double mass)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");

	unsigned id = _particle_vec.size();
	_particle_vec.emplace_back(new Particle(id, x, v, fixed, mass));
	return id;
}

unsigned NetSystem::add_particle(Particle *ptr)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");
	
	unsigned id = _particle_vec.size();
	ptr->setID(id);
	_particle_vec.emplace_back(ptr);
	return id;
}


void NetSystem::add_spring(double lid, double rid, double k, double l)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");

	_spring_vec.emplace_back(new Spring(lid, rid, k, l));
}

void NetSystem::add_spring(Spring *ptr)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");

	_spring_vec.emplace_back(ptr);
}

void NetSystem::_setup()
{
	_xt = Eigen::VectorXd(_particle_vec.size() * 3);
	_vt = Eigen::VectorXd(_particle_vec.size() * 3);
	_mass = Eigen::MatrixXd(_particle_vec.size(), _particle_vec.size());

	int i = 0;
	for (auto item : _particle_vec)
	{
		_mass(i, i) = (*item)->getMass();
		auto X = (*item)->getX();
		auto V = (*item)->getV();
		for (int j = 0; j < 3; ++j)
		{
			_xt(i+j) = X(j);
			_vt(i+j) = V(j);
		}
		i+=3;
	}
}

void NetSystem::_update()
{
	int i = 0;
	for (auto item : _particle_vec)
	{
		(*item)->setX({_xt(i), _xt(i+1), _xt(i+1)});
		(*item)->setV({_vt(i), _vt(i+1), _vt(i+1)});
		i+=3;
	}
}

Eigen::VectorXd NetSystem::_f(const Eigen::VectorXd &x)
{
	if (x.size() != _xt.size())
		throw std::runtime_error("[net system] calculate f, x is not the same size with xt");
	
	Eigen::VectorXd grad = Eigen::VectorXd::Zero(_xt.size());
	for (auto spring : _spring_vec)
		(*spring)->accumulate_grad(grad, x);
	
	return _mass * (x - _xt - _h*_vt) + _h*_h*grad;
}

Eigen::MatrixXd NetSystem::_df(const Eigen::VectorXd &x)
{
	if (x.size() != _xt.size())
		throw std::runtime_error("[net system] calculate df, x is not the same size with xt");
	
	Eigen::MatrixXd hessian = Eigen::MatrixXd::Zero(_xt.size(), _xt.size());
	for (auto spring: _spring_vec)
		(*spring)->accumulate_hessian(hessian, x);
	
	return _mass + _h*_h*hessian;
}

Eigen::VectorXd NetSystem::_newton_iterate()
{
	Eigen::VectorXd xtt = _xt;
	Eigen::VectorXd b, deltaX;
	Eigen::MatrixXd A;

	for (int i = 0; i < NEWTON_STEPS; ++i)
	{
		b = -_f(xtt);
		A = _df(xtt);
		deltaX = A.ldlt().solve(b);
		xtt += deltaX;
	}

	return xtt;
}

void NetSystem::simulate()
{
	if (_assembleable)
		throw std::runtime_error("[net system] simulate need not assembleable status.");
	
	auto xtt = _newton_iterate();
	auto deltaX = xtt - _xt;
	_xt = xtt;
	_vt = deltaX / _h;

	_update();
}


void NetSystem::render()
{
	if (_assembleable)
		throw std::runtime_error("[net system] render need not assembleable status.");
}