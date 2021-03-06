// Calculator.cpp : Implementation of CCalculator

#include "stdafx.h"
#include "Calculator.h"


// CCalculator

static const double PI = ::atan(1) * 4;

STDMETHODIMP CCalculator::Add(double a, double b, double* result) {
	if (result == nullptr)
		return E_POINTER;

	*result = a + b;

	return S_OK;
}


STDMETHODIMP CCalculator::Sin(double angle, double* result) {
	if (result == nullptr)
		return E_POINTER;

	if (!m_Radians)
		angle = angle * PI / 180;

	*result = ::sin(angle);

	return S_OK;
}


STDMETHODIMP CCalculator::get_Radians(VARIANT_BOOL* pVal) {
	*pVal = m_Radians;

	return S_OK;
}


STDMETHODIMP CCalculator::put_Radians(VARIANT_BOOL newVal) {
	m_Radians = newVal;

	return S_OK;
}
