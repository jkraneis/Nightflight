#ifndef __INC_NFPARAMETERCHANGE_H
#define __INC_NFPARAMETERCHANGE_H

#include <stdint.h>



template <typename ParameterType>
struct Parameter
{
public:

	ParameterType _parameterValue;

	Parameter()
	{

	}

	Parameter(ParameterType initialValue)
	{
		_parameterValue = initialValue;
	}

	virtual void setValue(ParameterType value)
	{
		_parameterValue = value;
	}

	virtual ParameterType getValue()
	{
		return _parameterValue;
	}

};



#endif