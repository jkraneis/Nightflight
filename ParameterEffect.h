#ifndef __INC_NFPARAMETEREFFECT_H
#define __INC_NFPARAMETEREFFECT_H

#include <stdint.h>
#include "ParameterChange.h"



struct ParameterEffect
{
    virtual void render()
    {
        //empty implementation of base class - should not get called
    }
};

struct ParameterTimeEffect : public virtual ParameterEffect
{
    uint64_t _startTime;
    boolean _started = false;

    virtual void start()
    {
        _started = true;
        _startTime = millis();
    }

    virtual void stop()
    {
        _started = false;
    }

    virtual void render()
    {
        if(_started)
        {
            calculateValue();
        }
    }

    virtual void calculateValue()
    {
        //should be overridden by subclasses
    }

};

struct ParameterTimeLinear : public virtual ParameterTimeEffect
{
    uint8_t _start;
    uint8_t _end;
    uint32_t _duration;
    boolean _randomize;
    Parameter<uint8_t>* _value;

    ParameterTimeLinear(uint8_t start, uint8_t end, uint32_t durationInMillis, Parameter<uint8_t>* value, boolean randomize = false)
    {
        _start = start;
        _end = end;
        _duration = durationInMillis;
        _value = value;
        _randomize = randomize;
    }

    virtual void calculateValue()
    {
        uint64_t now = millis();
        if(now > _startTime && now < _startTime + _duration)
        {
            uint32_t elapsed = now - _startTime;
            uint8_t newValue = (uint8_t)map(elapsed, 0, _duration, _start, _end);
            if(_randomize && newValue < 255)
            {
                newValue = random(newValue-1, newValue+1);
            }
            _value->setValue(newValue);
        }
    }
};




#endif
