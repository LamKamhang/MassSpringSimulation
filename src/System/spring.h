#pragma once

#include <Eigen/Core>
#include <stdexcept>

class Spring
{
public:
	Spring(unsigned lid, unsigned rid, 
		double k = 5, double l = 10)
		: _k(k)
		, _l(l)
		, _left_id(lid)
		, _right_id(rid)
	{}

	unsigned get_lid() const;
	unsigned get_rid() const;

	void accumulate_grad(Eigen::VectorXd &grad, 
		const Eigen::VectorXd &x) const;
	void accumulate_hessian(Eigen::MatrixXd &hessian, 
		const Eigen::VectorXd &x) const;

	void set_l(double l);
	void set_k(double k);
	void set_lid(unsigned lid);
	void set_rid(unsigned rid);
private:

private:
	double _k;
	double _l;
	unsigned _left_id;
	unsigned _right_id;
};

inline unsigned Spring::get_lid() const
{
	return _left_id;
}

inline unsigned Spring::get_rid() const
{
	return _right_id;
}

inline void Spring::set_l(double l)
{
	_l = l;
}

inline void Spring::set_k(double k)
{
	_k = k;
}

inline void Spring::set_lid(unsigned lid)
{
	_left_id = lid;
}

inline void Spring::set_rid(unsigned rid)
{
	_right_id = rid;
}