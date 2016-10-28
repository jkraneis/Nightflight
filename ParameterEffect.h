#ifndef __INC_NFPARAMETEREFFECT_H
#define __INC_NFPARAMETEREFFECT_H

#include <stdint.h>
#include "ParameterChange.h"



struct ParameterEffect
{
    virtual void render()
    {
        //empty implementation of base class - should not get called
        calculateValue();
    }

    virtual void calculateValue()
    {
        //should be overridden by subclasses
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
  Serial.println("This should not happen");
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

    ParameterTimeLinear()
    {}

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

struct ParameterBeatGrid : public virtual ParameterEffect
{
    enum transitionType
    {
        LINEAR = 0
    };

    struct GridPoint
    {
        uint32_t beat = 0;
        uint8_t value = 0;
        uint8_t randomizeBy = 0;
        transitionType type = LINEAR;
    };

    Parameter<uint8_t>* _value;
    double _bpm;
    double _millisPerBeat;
    GridPoint _gridArray[256];
    uint8_t _count = 0;
    uint8_t _current = 0;
    uint32_t _beats = 0;

    ParameterTimeLinear _transition;

    uint64_t _lastBeatStart = 0;
    double _valueChangeBetweenGridEntries = 0.0;
    double _valueChangeBetweenBeats = 0.0;

    ParameterBeatGrid(double bpm, Parameter<uint8_t>* value)
    {
        _value = value;
        _bpm = bpm;
        _millisPerBeat = (double)(60.0 / _bpm * 1000.0);
        _transition = ParameterTimeLinear(0, 0, 0, _value, 0);
    }

    virtual void addGridEntry( uint32_t beat, uint8_t value, uint8_t randomizeBy, transitionType type = LINEAR)
    {
        _gridArray[_count].beat = beat;
        _gridArray[_count].value = value;
        _gridArray[_count].randomizeBy = randomizeBy;
        _gridArray[_count].type = type;
        _count++;
    }

    virtual void beat() //gets called every beat, calculates _value per beat between beat entries
    {
        _beats++;

        if(_gridArray[_current+1].beat==_beats && _current < _count) //set new current with the current beat
        {
            _transition.stop();
            _current++;
            uint32_t beatsBetweenGridEntries = _gridArray[_current+1].beat - _gridArray[_current].beat;
            uint32_t durationBetweenGridEntries = (uint32_t)(((double)beatsBetweenGridEntries) * _millisPerBeat);

            switch(_gridArray[_current].type)
            {
                case LINEAR:
                default:        _transition = ParameterTimeLinear(_gridArray[_current].value, _gridArray[_current+1].value, durationBetweenGridEntries, _value, _gridArray[_current].randomizeBy );
                                break;
            }
            _transition.start();
        }
         _transition.render();
    }

    virtual void calculateValue()   //gets called per render() and calculates the _value within the current beat. smoothens value changes between beats
    {
        if(_beats == 0)
        {
            return;
        }
        _transition.render();
    }
};




#endif
