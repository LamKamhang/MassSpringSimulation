#include "simulation.h"
#include <Eigen/IterativeLinearSolvers>

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
		double min_length = spring->get_l() * MINIMUM_LENGTH_FACTOR;
		auto lid = spring->get_lid();
		auto rid = spring->get_rid();

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
			if ((l_nX - r_nX).norm() <= min_length)
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
			if ((l_nX - r_nX).norm() <= min_length)
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
	Eigen::ConjugateGradient<Eigen::MatrixXd> cg;

	for (int i = 0; i < NEWTON_STEPS; ++i)
	{
		b = -_f(xtt);
		double norm = b.norm();
		if (norm < EPSILON && norm > -EPSILON)
		{
			break;
		}	
		A = _df(xtt);
		// deltaX = A.ldlt().solve(b);
		cg.compute(A);
		deltaX = cg.solve(b);
		xtt += deltaX;
	}
#ifdef DEBUG
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

void NetSystem::render() const
{
	if (_assembleable)
		throw std::runtime_error("[net system] render need not assembleable status.");

	int i = 0;
	for (auto spring : _spring_vec)
	{
		unsigned lid = spring->get_lid();
		unsigned rid = spring->get_rid();

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