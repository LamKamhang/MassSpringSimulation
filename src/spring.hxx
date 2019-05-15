#pragma once

#include <Eigen/Core>
#include <stdexcept>

class Spring
{
public:
	Spring(unsigned lid, unsigned rid, double k = 5, double l = 10);
	~Spring();

	inline unsigned getLID();
	inline unsigned getRID();

	inline void accumulate_grad(Eigen::VectorXd &grad, const Eigen::VectorXd &x);
	inline void accumulate_hessian(Eigen::MatrixXd &hessian, const Eigen::VectorXd &x);
private:

private:
	double _k;
	double _l;
	unsigned _left_id;
	unsigned _right_id;
};

Spring::Spring(unsigned lid, unsigned rid, double k, double l)
	: _k(k)
	, _l(l)
	, _left_id(lid)
	, _right_id(rid)
{
}

Spring::~Spring()
{
}

unsigned Spring::getLID()
{
	return _left_id;
}

unsigned Spring::getRID()
{
	return _right_id;
}

void Spring::accumulate_grad(Eigen::VectorXd &grad, const Eigen::VectorXd &x)
{
	if (_left_id*3+2 >= x.size() || _right_id*3+2 >= x.size())
		throw std::range_error("[spring] left or right id larger then input x");
	if (grad.size() != x.size())
		throw std::logic_error("[spring] grad should have the same dim with x");
	
	Eigen::Vector3d left = {x(3*_left_id), x(3*_left_id+1), x(3*_left_id+2)};
	Eigen::Vector3d right = {x(3*_right_id), x(3*_right_id+1), x(3*_right_id+2)};

	double norm = (left - right).norm();
	double base = _k * (norm - _l) / norm;
	for (unsigned i = 0; i < 3; ++i)
	{
		grad(3*_left_id+i) += base * (left(i) - right(i));
		grad(3*_right_id+i) += base * (right(i) - left(i));
	}
}

void Spring::accumulate_hessian(Eigen::MatrixXd &hessian, const Eigen::VectorXd &x)
{
	if (_left_id*3+2 >= x.size() || _right_id*3+2 >= x.size())
		throw std::range_error("[spring] left or right id larger then input x");
	if (hessian.size() != x.size() * x.size())
		throw std::logic_error("[spring] hessian should have the same dim with x*x");
	
	Eigen::Vector3d left = {x(3*_left_id), x(3*_left_id+1), x(3*_left_id+2)};
	Eigen::Vector3d right = {x(3*_right_id), x(3*_right_id+1), x(3*_right_id+2)};

	double norm = (left - right).norm();

	double base1 = _k * (norm - _l) / norm;
	double base2 = _k * _l / (norm * norm * norm);

	unsigned ni, nni;
	double pattern, term;

	for (unsigned i = 0; i < 3; ++i)
	{
		ni = (i+1)%3;
		nni = (i+2)%3;

		pattern = (left(i) - right(i)) * (left(i) - right(i));
		term = base1 + pattern * base2;
		hessian(3*_left_id+i, 3*_left_id+i) += term;
		hessian(3*_left_id+i, 3*_right_id+i) += -term;
		hessian(3*_right_id+i, 3*_right_id+i) += term;
		hessian(3*_right_id+i, 3*_left_id+i) += -term;

		pattern = (left(i) - right(i)) * (left(ni) - right(ni));
		term = pattern * base2;
		hessian(3*_left_id+i, 3*_left_id+ni) += term;
		hessian(3*_left_id+i, 3*_right_id+ni) += -term;
		hessian(3*_right_id+i, 3*_right_id+ni) += term;
		hessian(3*_right_id+i, 3*_left_id+ni) += -term;

		pattern = (left(i) - right(i)) * (left(nni) - right(nni));
		term = pattern * base2;
		hessian(3*_left_id+i, 3*_left_id+nni) += term;
		hessian(3*_left_id+i, 3*_right_id+nni) += -term;
		hessian(3*_right_id+i, 3*_right_id+nni) += term;
		hessian(3*_right_id+i, 3*_left_id+nni) += -term;
	}
}
