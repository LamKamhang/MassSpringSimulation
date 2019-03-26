#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <initializer_list>

namespace mymath
{
	class mathException
	{
	public:
		mathException(std::string info): _info(info){}
		inline void print(void)
		{
			std::cerr << _info << std::endl;
		}
	private:
		std::string _info;
	};

	template<class T, unsigned D>
	class Vector
	{
	public:
		// constructor
		Vector(T val = 0);
		Vector(T array[], int len, T val = 0);
		Vector(const std::vector<T> &vec, T val = 0);
		Vector(const Vector<T, D> &right, T val = 0);
		Vector(const std::initializer_list<T>& list, T val = 0);

		// operators:
		inline const Vector operator+(const Vector<T, D> &right)	const;
		inline const Vector operator-(const Vector<T, D> &right)	const;
		inline const Vector operator*(T right)						const;
		inline const Vector operator*(const Vector<T, D> &right)	const;
		inline const Vector operator/(T right)						const;
		inline bool		 	operator==(const Vector<T, D> &right)	const;
		inline Vector &	 	operator+=(const Vector<T, D> &right);
		inline Vector &	 	operator-=(const Vector<T, D> &right);
		inline Vector &	 	operator*=(T right);
		inline Vector &	 	operator*=(const Vector<T, D> &right);
		inline Vector &	 	operator/=(T right);

		inline operator std::string() const;

		// getter
		inline T getItem (unsigned index) const;

		// inner product
		double innerProduct(const Vector<T, D> &right);
		
	private:
		void _isValidDim();
		void _setDefault(T val, unsigned left = 0, unsigned right = D);
	
	private:
		T _items[D];
	};

	// constructor
	template<class T, unsigned D>
	Vector<T, D>::Vector(T val)
	{
		_isValidDim();
		_setDefault(val);
	}

	template<class T, unsigned D>
	Vector<T, D>::Vector(T array[], int len, T val)
	{
		_isValidDim();
		if (len > D)
			throw mathException("VectorConstructorError");
		else
		{
			for (unsigned i = 0; i < len; ++i)
			{
				_items[i] = array[i];
			}
			_setDefault(val, len);
		}
	}
	
	template<class T, unsigned D>
	Vector<T, D>::Vector(const std::vector<T> &vec, T val)
	{
		_isValidDim();
		if (vec.size() > D)
			throw mathException("VectorConstructorError");
		else
		{
			for (unsigned i = 0; i < vec.size(); ++i)
			{
				_items[i] = vec[i];
			}
			_setDefault(val, vec.size());
		}
	}
	
	template<class T, unsigned D>
	Vector<T, D>::Vector(const Vector<T, D> &right, T val)
	{
		for (unsigned i = 0; i < D; ++i)
		{
			_items[i] = right._items[i];
		}
	}

	template<class T, unsigned D>
	Vector<T, D>::Vector(const std::initializer_list<T>& list, T val)
	{
		_isValidDim();
		if (list.size() > D)
			throw mathException("VectorConstructorError");
		else
		{
			unsigned i = 0;
			for (auto item : list)
			{
				_items[i++] = item;
			}
			_setDefault(val, list.size());
		}
	}

	// operators:
	template<class T, unsigned D>
	inline const Vector<T, D> Vector<T, D>::operator+(const Vector<T, D> &right) const
	{
		Vector<T, D> res(*this);
		for (unsigned i = 0; i < D; ++i)
		{
			res._items[i] += right._items[i];
		}
		return res;
	}

	template<class T, unsigned D>
	inline const Vector<T, D> Vector<T, D>::operator-(const Vector<T, D> &right) const
	{
		Vector<T, D> res(*this);
		for (unsigned i = 0; i < D; ++i)
		{
			res._items[i] -= right._items[i];
		}
		return res;
	}

	template<class T, unsigned D>
	inline const Vector<T, D> Vector<T, D>::operator*(T right) const
	{
		Vector<T, D> res(*this);
		for (unsigned i = 0; i < D; ++i)
		{
			res._items[i] *= right;
		}
		return res;
	}

	template<class T, unsigned D>
	inline const Vector<T, D> Vector<T, D>::operator*(const Vector<T, D> &right) const
	{
		Vector<T, D> res(*this);
		for (unsigned i = 0; i < D; ++i)
		{
			res._items[i] *= right._items[i];
		}
		return res;
	}

	template<class T, unsigned D>
	inline const Vector<T, D> Vector<T, D>::operator/(T right) const
	{
		Vector<T, D> res(*this);
		for (unsigned i = 0; i < D; ++i)
		{
			res._items[i] /= right;
		}
		return res;
	}

	template<class T, unsigned D>
	inline bool		 Vector<T, D>::operator==(const Vector<T, D> &right) const
	{
		bool flag = true;
		for (unsigned i = 0; i < D; ++i)
		{
			if (_items[i] != right._items[i])
			{
				flag = false;
				break;
			}
		}
		return flag;
	}

	template<class T, unsigned D>
	inline Vector<T, D> &	 Vector<T, D>::operator+=(const Vector<T, D> &right)
	{
		for (unsigned i = 0; i < D; ++i)
		{
			_items[i] += right._items[i];
		}
		return *this;
	}

	template<class T, unsigned D>
	inline Vector<T, D> &	 Vector<T, D>::operator-=(const Vector<T, D> &right)
	{
		for (unsigned i = 0; i < D; ++i)
		{
			_items[i] -= right._items[i];
		}
		return *this;
	}

	template<class T, unsigned D>
	inline Vector<T, D> &	 Vector<T, D>::operator*=(T right)
	{
		for (unsigned i = 0; i < D; ++i)
		{
			_items[i] *= right;
		}
		return *this;
	}

	template<class T, unsigned D>
	inline Vector<T, D> &	 Vector<T, D>::operator*=(const Vector<T, D> &right)
	{
		for (unsigned i = 0; i < D; ++i)
		{
			_items[i] *= right._items[i];
		}
		return *this;
	}

	template<class T, unsigned D>
	inline Vector<T, D> &	 Vector<T, D>::operator/=(T right)
	{
		for (unsigned i = 0; i < D; ++i)
		{
			_items[i] /= right;
		}
		return *this;
	}

	template<class T, unsigned D>
	inline Vector<T, D>::operator std::string() const
	{
		std::stringstream ss;
		ss << "[";
		ss << _items[0];
		for (int i = 1; i < D; ++i)
		{
			ss << ", " << _items[i];
		}
		ss << "]";
		return ss.str();
	}

	// getter
	template<class T, unsigned D>
	inline T Vector<T, D>::getItem (unsigned index) const
	{
		if (index >= D)
		{
			throw mathException("IndexOutOfBound");
		}
		else
		{
			return _items[index];
		}
	}

	// inner product
	template<class T, unsigned D>
	double Vector<T, D>::innerProduct(const Vector<T, D> &right)
	{
		double res = 0;
		for (unsigned i = 0; i < D; ++i)
		{
			res += _items[i] * right._items[i];
		}
		return res;
	}

	template<class T, unsigned D>
	void Vector<T, D>::_isValidDim()
	{
		if (D == 0)
			throw mathException("VectorDimensionError");
	}

	template<class T, unsigned D>
	void Vector<T, D>::_setDefault(T val, unsigned left, unsigned right)
	{
		if (right > D)
			throw mathException("IndexOutOfBound");
		for (unsigned i = left; i < right; ++i)
		{
			_items[i] = val;
		}
	}

	template<class T, unsigned D>
	inline std::ostream& operator<<(std::ostream & os, const Vector<T, D> &right)
	{
		return os << (std::string)right;
	}
}