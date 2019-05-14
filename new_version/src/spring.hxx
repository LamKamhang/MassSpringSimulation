#pragma once

#include <Eigen/Core>
#include <stdexcept>

class Spring
{
public:
	Spring(double lid, double rid, double k = 5, double l = 10);
	~Spring();

	void accumulate_grad(Eigen::VectorXd &grad, const Eigen::VectorXd &x);
	void accumulate_hessian(Eigen::MatrixXd &hessian, const Eigen::VectorXd &x);
private:

private:
	double _k;
	double _l;
	double _left_id;
	double _right_id;
};

Spring::Spring(double lid, double rid, double k, double l)
	: _k(k)
	, _l(l)
	, _left_id(lid)
	, _right_id(rid)
{
}

Spring::~Spring()
{
}


void Spring::accumulate_grad(Eigen::VectorXd &grad, const Eigen::VectorXd &x)
{
	if (_left_id*3+2 >= x.size() || _right_id*3+2 >= x.size())
		throw std::range_error("[spring] left or right id larger then input x");
	if (grad.size() != x.size())
		throw std::logic_error("[spring] grad should have the same dim with x");
	
	Eigen::Vector3d left = {x(_left_id), x(_left_id+1), x(_left_id+2)};
	Eigen::Vector3d right = {x(_right_id), x(_right_id+1), x(_right_id+2)};

	double norm = (left - right).norm();
	double base = _k * (norm - _l) / norm;
	for (unsigned i = 0; i < 3; ++i)
	{
		grad(_left_id+i) += base * (left(i) - right(i));
		grad(_right_id+i) += base * (right(i) - left(i));
	}
}

void Spring::accumulate_hessian(Eigen::MatrixXd &hessian, const Eigen::VectorXd &x)
{
	if (_left_id*3+2 >= x.size() || _right_id*3+2 >= x.size())
		throw std::range_error("[spring] left or right id larger then input x");
	if (hessian.size() != x.size() * x.size())
		throw std::logic_error("[spring] hessian should have the same dim with x*x");
	
	Eigen::Vector3d left = {x(_left_id), x(_left_id+1), x(_left_id+2)};
	Eigen::Vector3d right = {x(_right_id), x(_right_id+1), x(_right_id+2)};

	double norm = (left - right).norm();

	double base1 = _k * (norm - _l) / norm;
	double base2 = _k * _l / (norm * norm * norm);

	for (unsigned i = 0; i < 3; ++i)
	{
		unsigned ni = (i+1)%3;
		unsigned nni = (i+2)%3;

		double pattern = (left(i) - right(i)) * (left(i) - right(i));
		double term = base1 + term * base2;
		hessian(_left_id+i, _left_id+i) += term;
		hessian(_left_id+i, _right_id+i) += -term;
		hessian(_right_id+i, _right_id+i) += term;
		hessian(_right_id+i, _left_id+i) += -term;

		pattern = (left(i) - right(i)) * (left(ni) - right(ni));
		term = pattern * base2;
		hessian(_left_id+i, _left_id+ni) += term;
		hessian(_left_id+i, _right_id+ni) += -term;
		hessian(_right_id+i, _right_id+ni) += term;
		hessian(_right_id+i, _left_id+ni) += -term;

		pattern = (left(i) - right(i)) * (left(nni) - right(nni));
		term = pattern * base2;
		hessian(_left_id+i, _left_id+nni) += term;
		hessian(_left_id+i, _right_id+nni) += -term;
		hessian(_right_id+i, _right_id+nni) += term;
		hessian(_right_id+i, _left_id+nni) += -term;
	}
}
