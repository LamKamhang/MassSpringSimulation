#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <Eigen/Core>
#include <Eigen/Dense>
// #include <cmath>

// #define DEBUG

#include "particle.hxx"
#include "spring.hxx"


#define GRAVITY_MODE

#define EPSILON 1e-4
#define NEWTON_STEPS 10
#define MINIMUM_LENGTH 0.1

class NetSystem
{
public:
	NetSystem(double h = 0.1, const std::vector<std::shared_ptr<Particle> > &particle_vec = {}, const std::vector<std::shared_ptr<Spring > > &spring_vec = {});
	~NetSystem();

	inline void assemble_complete();
	inline void setAssembleable(bool flag);
	inline bool getAssembleable();

	inline void setH(double h);
	inline double getH();

	inline const std::vector<std::shared_ptr<Particle> >& get_particle_vec() const;
	inline const std::vector<std::shared_ptr<Spring > >& get_spring_vec() const;
	inline const Eigen::VectorXd& get_x() const;
	inline const Eigen::VectorXd& get_v() const;

	inline unsigned add_particle(Eigen::Vector3d x, bool fixed = false, double mass = 1, Eigen::Vector3d v = {0, 0, 0});
	inline unsigned add_particle(Particle *ptr);

	inline void add_spring(double lid, double rid, double k = 5, double l = 10);
	inline void add_spring(Spring *ptr);

	inline void simulate();

	inline void render();
	
private:
	inline void _setup();
	inline void _update();
	
	inline Eigen::VectorXd _newton_iterate();
	inline Eigen::VectorXd _f(const Eigen::VectorXd &x);
	inline Eigen::MatrixXd _df(const Eigen::VectorXd &x);

private:
	std::vector<std::shared_ptr<Particle> > _particle_vec;
	std::vector<std::shared_ptr<Spring > > _spring_vec;

	// Eigen::MatrixXd _mass;
	Eigen::VectorXd _xt;
	Eigen::VectorXd _vt;

	double _h;
	bool _assembleable;
};

NetSystem::NetSystem(double h, const std::vector<std::shared_ptr<Particle> > &particle_vec, const std::vector<std::shared_ptr<Spring> > &spring_vec)
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

const std::vector<std::shared_ptr<Particle> >& NetSystem::get_particle_vec() const
{
	return _particle_vec;
}

const std::vector<std::shared_ptr<Spring > >& NetSystem::get_spring_vec() const
{
	return _spring_vec;
}

const Eigen::VectorXd& NetSystem::get_x() const
{
	return _xt;
}

const Eigen::VectorXd& NetSystem::get_v() const
{
	return _vt;
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

unsigned NetSystem::add_particle(Eigen::Vector3d x, bool fixed, double mass, Eigen::Vector3d v)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");

	unsigned id = _particle_vec.size();
	_particle_vec.emplace_back(new Particle(id, x, fixed, mass, v));
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
	// _mass = Eigen::MatrixXd(_particle_vec.size() * 3, _particle_vec.size() * 3);

	int i;
	for (auto item : _particle_vec)
	{
		i = 3*item->getID();
		Eigen::Vector3d X = item->getX();
		Eigen::Vector3d V = item->getV();
		for (int j = 0; j < 3; ++j)
		{
			// _mass(i+j, i+j) = item->getMass();
			_xt(i+j) = X(j);
			_vt(i+j) = V(j);
		}
	}
#ifdef DEBUG
	std::cout << "mass" << std::endl;
	std::cout << _mass << std::endl << std::endl;
#endif
}

void NetSystem::_update()
{
	// int i;
	// for (auto item : _particle_vec)
	// {
	// 	i = 3*item->getID();
	// 	Eigen::Vector3d X = item->getX();
	// 	Eigen::Vector3d V = item->getV();
	// 	if (item->getFixStatus())
	// 	{
	// 		for (int j = 0; j < 3; ++j)
	// 		{
	// 			_xt(i+j) = X(j);
	// 			_vt(i+j) = V(j);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		item->setX({_xt(i), _xt(i+1), _xt(i+2)});
	// 		item->setV({_vt(i), _vt(i+1), _vt(i+2)});
	// 	}
	// }

	for (auto spring : _spring_vec)
	{
		auto lid = spring->getLID();
		auto rid = spring->getRID();

		auto lp = _particle_vec[lid];
		auto rp = _particle_vec[rid];

		lid *= 3;
		rid *= 3;
		Eigen::Vector3d l_X = lp->getX();
		Eigen::Vector3d l_V = lp->getV();
		Eigen::Vector3d r_X = rp->getX();
		Eigen::Vector3d r_V = rp->getV();

		Eigen::Vector3d l_nX(_xt(lid), _xt(lid+1), _xt(lid+2));
		Eigen::Vector3d r_nX(_xt(rid), _xt(rid+1), _xt(rid+2));

		Eigen::Vector3d l_nV(_vt(lid), _vt(lid+1), _vt(lid+2));
		Eigen::Vector3d r_nV(_vt(rid), _vt(rid+1), _vt(rid+2));

		if (lp->getFixStatus())
		{
			for (int j = 0; j < 3; ++j)
			{
				_xt(lid+j) = l_X(j);
				_vt(lid+j) = l_V(j);
			}
		}
		else
		{
			if ((l_nX - r_nX).norm() <= MINIMUM_LENGTH)
			{
				for (int j = 0; j < 3; ++j)
				{
					_xt(lid+j) = l_X(j);
					_vt(lid+j) = _vt(rid+j);
				}
				lp->setV({_vt(lid), _vt(lid+1), _vt(lid+2)});
			}
			else
			{
				lp->setX({_xt(lid), _xt(lid+1), _xt(lid+2)});
				lp->setV({_vt(lid), _vt(lid+1), _vt(lid+2)});
			}
		}
		
		if (rp->getFixStatus())
		{
			for (int j = 0; j < 3; ++j)
			{
				_xt(rid+j) = r_X(j);
				_vt(rid+j) = r_V(j);
			}
		}
		else
		{
			if ((l_nX - r_nX).norm() <= MINIMUM_LENGTH)
			{
				for (int j = 0; j < 3; ++j)
				{
					_xt(rid+j) = r_X(j);
					_vt(rid+j) = _vt(lid+j);
				}
				rp->setV({_vt(rid), _vt(rid+1), _vt(rid+2)});
			}
			else
			{
				rp->setX({_xt(rid), _xt(rid+1), _xt(rid+2)});
				rp->setV({_vt(rid), _vt(rid+1), _vt(rid+2)});
			}
		}

		// if ((l_nX - r_nX).norm() <= MINIMUM_LENGTH)
		// {
		// 	i = lp->getID()*3;
		// 	if (lp->getFixStatus())
		// 	{
		// 		for (int j = 0; j < 3; ++j)
		// 		{
		// 			_xt(i+j) = l_X(j);
		// 			_vt(i+j) = l_V(j);
		// 		}
		// 	}
		// 	else
		// 	{
		// 		for (int j = 0; j < 3; ++j)
		// 		{
		// 			_xt(i+j) = l_X(j);
		// 			_vt(i+j) = -_vt(i+j);
		// 		_vt(i+j) = 0;
		// 		}
		// 		lp->setV({_vt(i), _vt(i+1), _vt(i+2)});
		// 	}
			
		// 	i = rp->getID()*3;
		// 	if (rp->getFixStatus())
		// 	{
		// 		for (int j = 0; j < 3; ++j)
		// 		{
		// 			_xt(i+j) = r_X(j);
		// 			_vt(i+j) = r_V(j);
		// 		}
		// 	}
		// 	else
		// 	{
		// 		for (int j = 0; j < 3; ++j)
		// 		{
		// 			_xt(i+j) = r_X(j);
		// 			_vt(i+j) = -_vt(i+j);
		// 			_vt(i+j) = 0;
		// 		}
		// 		rp->setV({_vt(i), _vt(i+1), _vt(i+2)});
		// 	}
		// }
		// else
		// {
		// 	i = lp->getID()*3;
		// 	if (lp->getFixStatus())
		// 	{
		// 		for (int j = 0; j < 3; ++j)
		// 		{
		// 			_xt(i+j) = l_X(j);
		// 			_vt(i+j) = l_V(j);
		// 		}
		// 	}
		// 	else
		// 	{
		// 		lp->setX({_xt(i), _xt(i+1), _xt(i+2)});
		// 		lp->setV({_vt(i), _vt(i+1), _vt(i+2)});
		// 	}
			
		// 	i = rp->getID()*3;
		// 	if (rp->getFixStatus())
		// 	{
		// 		for (int j = 0; j < 3; ++j)
		// 		{
		// 			_xt(i+j) = r_X(j);
		// 			_vt(i+j) = r_V(j);
		// 		}
		// 	}
		// 	else
		// 	{
		// 		rp->setX({_xt(i), _xt(i+1), _xt(i+2)});
		// 		rp->setV({_vt(i), _vt(i+1), _vt(i+2)});
		// 	}
		// }
	}
}

Eigen::VectorXd NetSystem::_f(const Eigen::VectorXd &x)
{
	if (x.size() != _xt.size())
		throw std::runtime_error("[net system] calculate f, x is not the same size with xt");
	
	Eigen::VectorXd grad = Eigen::VectorXd::Zero(_xt.size());
	for (auto spring : _spring_vec)
		spring->accumulate_grad(grad, x);
	
	double factor = 1 / _h / _h;
	double term;
	unsigned id;
	for (auto particle : _particle_vec)
	{
#ifdef GRAVITY_MODE
		particle->accumulate_grad(grad, x);
#endif
		id = 3*particle->getID();
		term = particle->getMass() * factor;
		grad(id) += term * (x(id) - _xt(id) - _h*_vt(id));
		grad(id+1) += term * (x(id+1) - _xt(id+1) - _h*_vt(id+1));
		grad(id+2) += term * (x(id+2) - _xt(id+2) - _h*_vt(id+2));
	}
#ifdef DEBUG
	std::cout << "grad" << std::endl;
	std::cout << grad << std::endl << std::endl;
#endif
	
	return grad;
	// return _mass * ((x - _xt)/(_h*_h) - _vt/_h) + grad;
}

Eigen::MatrixXd NetSystem::_df(const Eigen::VectorXd &x)
{
	if (x.size() != _xt.size())
		throw std::runtime_error("[net system] calculate df, x is not the same size with xt");
	
	Eigen::MatrixXd hessian = Eigen::MatrixXd::Zero(_xt.size(), _xt.size());
	for (auto spring: _spring_vec)
		spring->accumulate_hessian(hessian, x);
#ifdef DEBUG
	std::cout << "hessian" << std::endl;
	std::cout << hessian << std::endl << std::endl;
#endif

	// _mass / _h^2 + hessian;
	double factor = 1 / _h / _h;
	double term;
	unsigned id;
	for (auto particle : _particle_vec)
	{
		id = 3*particle->getID();
		term = particle->getMass() * factor;
		hessian(id, id) += term;
		hessian(id+1, id+1) += term;
		hessian(id+2, id+2) += term;
	}

	return hessian;
	// return _mass / (_h*_h) + hessian;
}

Eigen::VectorXd NetSystem::_newton_iterate()
{
#ifdef DEBUG
	std::cout << _xt << std::endl << std::endl;
	std::cout << _f(_xt) << std::endl << std::endl;
#endif
	Eigen::VectorXd xtt = _xt;
	Eigen::VectorXd b, deltaX;
	Eigen::MatrixXd A;

	for (int i = 0; i < NEWTON_STEPS; ++i)
	{
		b = -_f(xtt);
		double norm = b.norm();
		if (norm < EPSILON && norm > -EPSILON)
		{
#ifdef DEBUG
			std::cout << "finish newton iterate, " << i << " steps" << std::endl;
#endif
			break;
		}	
		A = _df(xtt);
		deltaX = A.ldlt().solve(b);
#ifdef DEBUG
	std::cout << b << std::endl;
	std::cout << A << std::endl;
	std::cout << deltaX << std::endl;
#endif
		xtt += deltaX;
	}
#ifdef DEBUG
	std::cout << xtt << std::endl << std::endl;
	std::cout << "newton iterate" << std::endl;
	std::cout << _f(xtt) << std::endl << std::endl;
#endif

	return xtt;
}

void NetSystem::simulate()
{
	if (_assembleable)
		throw std::runtime_error("[net system] simulate need not assembleable status.");

	Eigen::VectorXd xtt = _newton_iterate();
	Eigen::VectorXd deltaX = xtt - _xt;
	_xt = xtt;
	_vt = deltaX / _h;


	_update();
#ifdef DEBUG
	std::cout << "force: " << std::endl;
	Eigen::VectorXd grad = Eigen::VectorXd::Zero(_xt.size());
	for (auto spring : _spring_vec)
		spring->accumulate_grad(grad, _xt);
#ifdef GRAVITY_MODE
	for (auto particle : _particle_vec)
		particle->accumulate_grad(grad, _xt);
#endif
	std::cout << -grad << std::endl << std::endl;

	std::cout << "pos:" << std::endl;
	std::cout << _xt << std::endl << std::endl;

	std::cout << "vec:" << std::endl;
	std::cout << _vt << std::endl << std::endl;
#endif
}


void NetSystem::render()
{
	if (_assembleable)
		throw std::runtime_error("[net system] render need not assembleable status.");

	int i = 0;
	for (auto spring : _spring_vec)
	{
		unsigned lid = spring->getLID();
		unsigned rid = spring->getRID();

		std::cout << "spring " << i << ":" << std::endl;
		std::cout << "\tpos<left> : " << _xt(3*lid) << " " << _xt(3*lid+1) << " " << _xt(3*lid+2) << std::endl;
		std::cout << "\tvec<left> : " << _vt(3*lid) << " " << _vt(3*lid+1) << " " << _vt(3*lid+2) << std::endl;
		std::cout << "\tpos<right>: " << _xt(3*rid) << " " << _xt(3*rid+1) << " " << _xt(3*rid+2) << std::endl;
		std::cout << "\tvec<right>: " << _vt(3*rid) << " " << _vt(3*rid+1) << " " << _vt(3*rid+2) << std::endl;
		std::cout << "\tlength    : " << sqrt((_xt(3*lid) - _xt(3*rid)) * (_xt(3*lid) - _xt(3*rid))
										+ (_xt(3*lid+1) - _xt(3*rid+1)) * (_xt(3*lid+1) - _xt(3*rid+1))
										+ (_xt(3*lid+2) - _xt(3*rid+2)) * (_xt(3*lid+2) - _xt(3*rid+2))) << std::endl;
		std::cout << std::endl;
		i++;
	}
}