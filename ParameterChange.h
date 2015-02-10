#ifndef __INC_NFPARAMETERCHANGE_H
#define __INC_NFPARAMETERCHANGE_H

#include <stdint.h>



template <typename ParameterType>
struct Parameter
{
public:

	ParameterType _parameterValue;


	virtual void setValue(ParameterType value)
	{
		_parameterValue = value;
	}

	virtual ParameterType getValue()
	{
		return _parameterValue;
	}

};


struct StepParameter : public virtual Parameter<uint16_t>
{
public:
	StepParameter(uint16_t minimum, uint16_t maximum, uint16_t numSteps)
		: _minimum(minimum), _maximum(maximum), _numSteps(numSteps)
	{

	}

	virtual void setValue(uint16_t value)
	{
		if( value < _minimum )
		{
			value = _minimum;
		}
		else if ( value > _maximum )
		{
			value = _maximum;
		}
		Parameter::setValue(value);
	}

	virtual uint16_t getValue()
	{
		uint16_t stepToReturn = 0;
		uint16_t divisor = (_maximum - _minimum) / (_numSteps-1);
		uint16_t current = _minimum;
		for( uint16_t step = 0; step < _numSteps; step++ )
		{
			if( _parameterValue >= current )
			{
				stepToReturn++;
				current += divisor;
			}
		}
		return stepToReturn;
	}

private:
	uint16_t _minimum;
	uint16_t _maximum;
	uint16_t _numSteps;
};

#endif