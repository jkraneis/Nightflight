#ifndef __INC_RENDERTIMER_H
#define __INC_RENDERTIMER_H


#include <stdint.h>

// Render Timer function pointer definition for callbacks
typedef void (*RenderTimerFunctionPointer)(void);


struct RenderTimerInfo
{
	uint64_t _lastUpdate;
	uint64_t _updateIntervalMilliSeconds;
	uint16_t _numberRepetitions;
	boolean _performUpdate;
	boolean _isRepeatingTimer = false;
	RenderTimerFunctionPointer callbackFunction = NULL;

	RenderTimerInfo()
	: RenderTimerInfo(0)
	{
	}

	RenderTimerInfo(uint64_t updateIntervalMilliSeconds)
		: _lastUpdate(millis()), _updateIntervalMilliSeconds(updateIntervalMilliSeconds), _performUpdate(false)
	{
	}

	virtual void setCallback(RenderTimerFunctionPointer userFunc)
	{
		callbackFunction = userFunc;
	}

	virtual void performUpdate()
	{
		if(_isRepeatingTimer && _numberRepetitions == 0)
		{
			return;
		}
		else if(_isRepeatingTimer && _numberRepetitions > 0)
		{
			_numberRepetitions--;
		}
		_performUpdate = true;
		_lastUpdate = millis();
		callbackFunction();
		_performUpdate = false;
	}

	virtual boolean getPerformingUpdate()
	{
		return _performUpdate;
	}

	virtual uint64_t getLastUpdate()
	{
		return _lastUpdate;
	}

	virtual void setUpdateIntervalMilliSeconds(uint64_t interval)
	{
		_updateIntervalMilliSeconds = interval;
	}

	virtual void setNumberRepetitions(uint16_t numberRepetitions)
	{
		_numberRepetitions = numberRepetitions;
		_isRepeatingTimer = true;
	}

	virtual uint64_t getUpdateIntervalMilliSeconds()
	{
		return _updateIntervalMilliSeconds;
	}

};

struct BPMTimerInfo : public virtual RenderTimerInfo
{
	double _bpm;
	uint8_t _timeSignatureTop = 4; //For the beginning: only 4/4th
	uint8_t _timeSignatureBottom = 4;
	double _millisPerBeat = 0.0;
	double _millisDriftPerBeat = 0.0;
	double _millisTotalDrift = 0.0;

	double _millisTotalExpected = 0.0;
	uint64_t _calculatedUpdateIntervalMilliseconds = 0;
	uint16_t _beatsElapsed = 0;
	uint8_t _updatesWithoutAdjustment = 0;
	uint64_t _millisReallyElapsed = 0;
	uint64_t _elapsedSinceLastUpdate = 0;

	uint64_t _startTime;
	boolean _started = false;

	BPMTimerInfo()
		: BPMTimerInfo(120.0, 4)
	{
	}

	BPMTimerInfo(double bpm, uint8_t timeSignatureTop)
		: RenderTimerInfo(), _timeSignatureTop(timeSignatureTop)
	{
		setBPM(bpm);
	}

	void setBPM(double bpm)
	{
		_bpm = bpm;
		_millisPerBeat = (double)(60.0 / _bpm * 1000.0);
		_millisDriftPerBeat = fmod(_millisPerBeat, (double)((uint64_t) _millisPerBeat));
		_updateIntervalMilliSeconds = (uint64_t)_millisPerBeat;
		_calculatedUpdateIntervalMilliseconds = _updateIntervalMilliSeconds;
	//	calculateUpdateInterval();		
	}

	void setTimeSignatureTop(uint8_t timeSignatureTop)
	{
		_timeSignatureTop = timeSignatureTop;
	}

	void start()
	{
		_started = true;
		_beatsElapsed = 0;
		_millisTotalDrift = 0.0;
		_startTime = millis();
		_lastUpdate = _startTime;
	}

	void stop()
	{
		_started = false;
	}

	double getBPMDrift()
	{
		return _millisTotalDrift;
	}

	double getMillisExpected()
	{
		return _millisTotalExpected;
	}

	uint64_t getMillisElapsed()
	{
		return _millisReallyElapsed;
	}

	boolean isDownbeat()
	{
		return (_beatsElapsed % _timeSignatureTop) == 1;
	}

	void calculateUpdateInterval()
	{
		uint64_t now = millis();
		_elapsedSinceLastUpdate = now - _lastUpdate;
		_millisReallyElapsed = now - _startTime;

		_beatsElapsed++;
		_millisTotalExpected = _millisPerBeat * (double)_beatsElapsed;

		
		_millisTotalDrift = ((double)_millisReallyElapsed) - _millisTotalExpected;

		double tweakValue = abs(_millisTotalDrift);
		if(_millisTotalDrift < -0.1)
		{
			_calculatedUpdateIntervalMilliseconds = _updateIntervalMilliSeconds + tweakValue;
		}
		else if(_millisTotalDrift > 0.1)
		{
			_calculatedUpdateIntervalMilliseconds = _updateIntervalMilliSeconds - tweakValue;
		}
		else
		{
			_calculatedUpdateIntervalMilliseconds = _updateIntervalMilliSeconds;
		}
	}

	virtual void setUpdateIntervalMilliSeconds(uint64_t interval)
	{
		//do nothing since we calculate the update interval from the set beats
	}

	virtual void performUpdate()
	{
		if(_started)
		{
			calculateUpdateInterval();

			RenderTimerInfo::performUpdate();
		}
	}

	//overwrite to make sure the rounding error doesn't get high
	virtual uint64_t getUpdateIntervalMilliSeconds()
	{

		return _calculatedUpdateIntervalMilliseconds;
	}
};

struct RenderTimer
{
	uint64_t _lastTime;
	RenderTimerInfo _renderTimerInfoItems[7];
	BPMTimerInfo _bpmTimerInfoItems[1];
	uint8_t _renderTimerItemCounter;
	uint8_t _bpmTimerItemCounter;
	boolean _isRenderTimerMask[16]; //true for RenderTimerInfo, false for BPMTimerInfo

	RenderTimer()
		: _lastTime(millis()), _renderTimerItemCounter(0)
	{
	}

	RenderTimerInfo& getRenderTimerInfo()
	{
		_isRenderTimerMask[_renderTimerItemCounter + _bpmTimerItemCounter] = true;
		_renderTimerItemCounter++;
		return _renderTimerInfoItems[_renderTimerItemCounter-1];
	}

	BPMTimerInfo& getBPMTimerInfo()
	{
		_isRenderTimerMask[_renderTimerItemCounter + _bpmTimerItemCounter] = false;
		_bpmTimerItemCounter++;
		return _bpmTimerInfoItems[_bpmTimerItemCounter-1];
	}

	void update()
	{
		uint64_t currentTime = millis();
		size_t numberItems = _renderTimerItemCounter + _bpmTimerItemCounter;
		uint8_t renderTimerItems = 0;
		uint8_t bpmTimerItems = 0;
		for( uint8_t i = 0; i < numberItems; i++ )
		{
			RenderTimerInfo* currentItem;
			if(_isRenderTimerMask[i])
			{
				currentItem = &_renderTimerInfoItems[renderTimerItems];
			}
			else
			{
				currentItem = &_bpmTimerInfoItems[bpmTimerItems];
			}
			uint64_t currentUpdateInterval = currentItem->getUpdateIntervalMilliSeconds();

			if( currentUpdateInterval == 0 //perform an update on every loop
				|| currentItem->getLastUpdate() + currentUpdateInterval <= currentTime )
			{
				currentItem->performUpdate();
			}

			if(_isRenderTimerMask[i])
			{
				renderTimerItems++;
			}
			else
			{
				bpmTimerItems++;
			}
		}
		_lastTime = currentTime;
	}
};




#endif
