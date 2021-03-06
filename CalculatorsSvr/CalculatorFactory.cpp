// CalculatorFactory.cpp : Implementation of CCalculatorFactory

#include "stdafx.h"
#include "CalculatorFactory.h"
#include "Calculator.h"

class CGenericInterceptor : public CComObjectRoot, public ICallFrameEvents {
public:
	BEGIN_COM_MAP(CGenericInterceptor)
		COM_INTERFACE_ENTRY(ICallFrameEvents)
	END_COM_MAP()

	STDMETHOD(OnCall)(ICallFrame* pFrame) {
		PWSTR interfaceName, methodName;
		pFrame->GetNames(&interfaceName, &methodName);
		printf("%ws::%ws\n", interfaceName, methodName);
		::CoTaskMemFree(interfaceName);
		::CoTaskMemFree(methodName);

		// invoke the method on the real object

		pFrame->Invoke(m_spObject);

		return pFrame->GetReturnValue();
	}

	void SetObject(IUnknown* pObject) {
		m_spObject = pObject;
	}

	CComPtr<IUnknown> m_spObject;
};

// CCalculatorFactory

STDMETHODIMP CCalculatorFactory::CreateCalculator(ICalculator** ppCalculator) {
	CComPtr<ICallInterceptor> spInterceptor;
	auto hr = ::CoGetInterceptor(__uuidof(ICalculator), nullptr, __uuidof(ICallInterceptor), reinterpret_cast<void**>(&spInterceptor));
	if (FAILED(hr))
		return hr;

	CComObject<CCalculator>* pCalculator;
	hr = pCalculator->CreateInstance(&pCalculator);
	if (FAILED(hr))
		return hr;

	CComObject<CGenericInterceptor>* pGenericInterceptor;
	pGenericInterceptor->CreateInstance(&pGenericInterceptor);
	pGenericInterceptor->SetObject(pCalculator);

	spInterceptor->RegisterSink(pGenericInterceptor);

	return spInterceptor->QueryInterface(ppCalculator);
}
