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
    uint8_t _randomizeBy;
    Parameter<uint8_t>* _value;

    ParameterTimeLinear(uint8_t start, uint8_t end, uint32_t durationInMillis, Parameter<uint8_t>* value, uint8_t randomizeBy = 0)
    {
        _start = start;
        _end = end;
        _duration = durationInMillis;
        _value = value;
        _randomize = randomizeBy > 0;
        _randomizeBy = randomizeBy;
    }

    virtual void calculateValue()
    {
        uint64_t now = millis();
        if(now > _startTime /*&& now < _startTime + _duration*/)
        {
            uint32_t elapsed = now - _startTime;
            if(now > _startTime + _duration)
            {
                elapsed = _duration;
            }
            uint8_t newValue = (uint8_t)map(elapsed, 0, _duration, _start, _end);
            if(_randomize)
            {
                uint8_t randMin = min(_randomizeBy, newValue);
                uint8_t randMax = min(_randomizeBy, 255-newValue);
                newValue = random(newValue-randMin, newValue+randMax);
            }
            _value->setValue(newValue);
        }
    }
};




#endif
