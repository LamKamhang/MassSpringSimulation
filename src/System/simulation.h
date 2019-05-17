#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <Eigen/Core>
#include <Eigen/Dense>

// #define DEBUG

#include "particle.h"
#include "spring.h"


#define GRAVITY_MODE

#define EPSILON 1e-4
#define NEWTON_STEPS 10
#define MINIMUM_LENGTH_FACTOR 0.1

class NetSystem
{
public:
	NetSystem(double h = 0.1, 
		const std::vector<std::shared_ptr<Particle> > 
			&particle_vec = {}, 
		const std::vector<std::shared_ptr<Spring > > 
			&spring_vec = {})
		: _particle_vec(particle_vec)
		, _spring_vec(spring_vec)
		, _h(h)
		, _assembleable(true)
	{}

	void assemble_complete();
	void set_assembleable(bool flag);
	bool get_assembleable() const;

	void setH(double h);
	double getH() const;

	const std::vector<std::shared_ptr<Particle> >& 
	get_particle_vec() const;

	const std::vector<std::shared_ptr<Spring > >& 
	get_spring_vec() const;

	const Eigen::VectorXd& get_x() const;
	const Eigen::VectorXd& get_v() const;

	unsigned add_particle(Eigen::Vector3d x, bool fixed = false, 
		double mass = 1, Eigen::Vector3d v = {0, 0, 0});
	unsigned add_particle(Particle *ptr);

	void add_spring(double lid, double rid, 
		double k = 5, double l = 10);
	void add_spring(Spring *ptr);

	void simulate();

	void render() const;
	
private:
	void _setup();
	void _update();

	Eigen::VectorXd _newton_iterate();
	Eigen::VectorXd _f(const Eigen::VectorXd &x);
	Eigen::MatrixXd _df(const Eigen::VectorXd &x);

private:
	std::vector<std::shared_ptr<Particle> > _particle_vec;
	std::vector<std::shared_ptr<Spring > > _spring_vec;

	// Eigen::MatrixXd _mass;
	Eigen::VectorXd _xt;
	Eigen::VectorXd _vt;

	double _h;
	bool _assembleable;
};


inline void NetSystem::assemble_complete()
{
	_assembleable = false;
	_setup();
}

inline void NetSystem::set_assembleable(bool flag)
{
	_assembleable = flag;
	if (!_assembleable)
		_setup();
}

inline const std::vector<std::shared_ptr<Particle> >& 
NetSystem::get_particle_vec() const
{
	return _particle_vec;
}

inline const std::vector<std::shared_ptr<Spring > >& 
NetSystem::get_spring_vec() const
{
	return _spring_vec;
}

inline const Eigen::VectorXd& NetSystem::get_x() const
{
	return _xt;
}

inline const Eigen::VectorXd& NetSystem::get_v() const
{
	return _vt;
}


inline bool NetSystem::get_assembleable() const
{
	return _assembleable;
}

inline void NetSystem::setH(double h)
{
	_h = h;
}

inline double NetSystem::getH() const
{
	return _h;
}

inline unsigned NetSystem::add_particle(Eigen::Vector3d x, bool fixed, double mass, Eigen::Vector3d v)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");

	unsigned id = _particle_vec.size();
	_particle_vec.emplace_back(new Particle(id, x, fixed, mass, v));
	return id;
}

inline unsigned NetSystem::add_particle(Particle *ptr)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");
	
	unsigned id = _particle_vec.size();
	ptr->setID(id);
	_particle_vec.emplace_back(ptr);
	return id;
}


inline void NetSystem::add_spring(double lid, double rid, double k, double l)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");

	_spring_vec.emplace_back(new Spring(lid, rid, k, l));
}

inline void NetSystem::add_spring(Spring *ptr)
{
	if (!_assembleable)
		throw std::runtime_error("[net system] it's not _assembleable");

	_spring_vec.emplace_back(ptr);
}
