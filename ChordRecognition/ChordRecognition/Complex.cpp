#include "Complex.h"


Complex::Complex()
	:m_real(0.0),
	m_imaginary(0.0)
{

}

Complex::Complex(const Complex& c)
	:m_real(c.m_real),
	m_imaginary(c.m_imaginary)
{

}

Complex::Complex(f64 re, f64 im)
	:m_real(re),
	m_imaginary(im)
{

}

Complex::~Complex()
{

}