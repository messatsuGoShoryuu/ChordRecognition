#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include <math.h>
#include "types.h"
class Complex
{
public:
	Complex();
	Complex(f64 re, f64 im);
	Complex(const Complex& c);
	~Complex();


public:
	//Accessors
	inline	f64		real()		const   { return m_real; }
	inline	f64		imaginary()	const	{ return m_imaginary; }

public:
	//Operators
	inline	void	set(f64 re, f64 im)
	{
		m_real = re;
		m_imaginary = im;
	}

	inline void		setReal(f64 re)	{ m_real = re; }
	inline void		setIm(f64 im)	{ m_imaginary = im; }

	inline	Complex	operator+(const Complex& value) const
	{
		return	Complex(m_real + value.m_real, m_imaginary + value.m_imaginary);
	}

	inline	Complex	operator-(const Complex& value) const
	{
		return	Complex(m_real - value.m_real, m_imaginary - value.m_imaginary);
	}

	inline	void	operator+=(const Complex& value)
	{
		m_real += value.m_real;
		m_imaginary += value.m_imaginary;
	}

	inline	void	operator-=(const Complex& value)
	{
		m_real -= value.m_real;
		m_imaginary -= value.m_imaginary;
	}

	inline	Complex operator*(const Complex& value) const
	{
		f64 a	= m_real * value.m_real;
		f64 i	= m_real * value.m_imaginary + m_imaginary * value.m_real;
		f64 b	= m_imaginary * value.m_imaginary;
		return Complex(a - b, i);
	}

	inline void operator=(const Complex& value)
	{
		m_real = value.m_real;
		m_imaginary = value.m_imaginary;
	}

	Complex	reciprocal()	const
	{
		f64	x2y2 = m_real * m_real + m_imaginary * m_imaginary;
		x2y2 = 1.0 / x2y2;

		return Complex(m_real * x2y2, m_imaginary * x2y2);
	}

	Complex	conjugate()	const
	{
		return Complex(m_real, -m_imaginary);
	}

	inline	f64	arg()	const
	{
		return atan2(m_imaginary, m_real);
	}

	inline f64 mod()	const
	{
		return sqrt(m_real * m_real + m_imaginary * m_imaginary);
	}

	inline	Complex	operator/(const Complex& value)
	{
		f64	u2v2 = value.m_real * value.m_real + value.m_imaginary * value.m_imaginary;
		u2v2 = 1.0 / u2v2;

		f64	xu = m_real * value.m_real;
		f64 yv = m_imaginary * value.m_imaginary;
		f64 xv = m_real * value.m_imaginary;
		f64 yu = m_imaginary * value.m_real;

		f64 re = (xu + yv) * u2v2;
		f64 im = (-xv + yu) * u2v2;
		return Complex(re, im);
	}

	inline void operator/=(const Complex& value)
	{
		f64	u2v2 = value.m_real * value.m_real + value.m_imaginary * value.m_imaginary;
		u2v2 = 1.0 / u2v2;

		f64	xu = m_real * value.m_real;
		f64 yv = m_imaginary * value.m_imaginary;
		f64 xv = m_real * value.m_imaginary;
		f64 yu = m_imaginary * value.m_real;

		f64 re = (xu + yv) * u2v2;
		f64 im = (-xv + yu) * u2v2;

		m_real = re; 
		m_imaginary = im;
	}

private:
	f64	m_real;
	f64	m_imaginary;
};
#endif // !_COMPLEX_H_
