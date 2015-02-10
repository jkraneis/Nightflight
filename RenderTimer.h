#ifndef __INC_RENDERTIMER_H
#define __INC_RENDERTIMER_H


#include <stdint.h>

// Render Timer function pointer definition for callbacks
typedef void (*RenderTimerFunctionPointer)(void);


struct RenderTimerInfo
{
	uint64_t _lastUpdate;
	uint64_t _performUpdateTime;
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

	void setCallback(RenderTimerFunctionPointer userFunc)
	{
		callbackFunction = userFunc;
	}

	void performUpdate()
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
		_performUpdateTime = millis();
		callbackFunction();
		_lastUpdate = millis();
		_performUpdate = false;
	}

	boolean getPerformingUpdate()
	{
		return _performUpdate;
	}

	uint64_t getLastUpdate()
	{
		return _lastUpdate;
	}

	void setUpdateIntervalMilliSeconds(uint64_t interval)
	{
		_updateIntervalMilliSeconds = interval;
	}

	void setNumberRepetitions(uint16_t numberRepetitions)
	{
		_numberRepetitions = numberRepetitions;
		_isRepeatingTimer = true;
	}

	uint64_t getUpdateIntervalMilliSeconds()
	{
		return _updateIntervalMilliSeconds;
	}

};

struct RenderTimer
{
	uint64_t _lastTime;
	RenderTimerInfo _renderItems[4];
	uint8_t _renderItemCounter;

	RenderTimer()
		: _lastTime(millis()), _renderItemCounter(0)
	{
	}

	RenderTimerInfo& getRenderTimerInfo()
	{
		_renderItemCounter++;
		return (_renderItems[_renderItemCounter-1]);
	}

	void update()
	{
		uint64_t currentTime = millis();
		for( uint8_t i = 0; i < _renderItemCounter; i++ )
		{
			if( _renderItems[i].getUpdateIntervalMilliSeconds() == 0 //perform an update on every loop
				|| _renderItems[i].getLastUpdate() + _renderItems[i].getUpdateIntervalMilliSeconds() <= currentTime )
			{
				_renderItems[i].performUpdate();
			}
		}
		_lastTime = currentTime;
	}
};

struct BarBeatTicks
{
	uint16_t _bar = 0;
	uint16_t _beat = 0;
	uint16_t _tick = 0;
};

struct BPMTimer
{
	double _bpm;
	uint8_t _timeSignatureTop = 4; //For the beginning: only 4/4th
	uint8_t _timeSignatureBottom = 4;
	double _microsPerTick;
	BarBeatTicks _posititon;

	uint64_t _startTime;
	boolean _started = false;

	BPMTimer(double bpm)
		: _bpm(bpm)
	{
		_microsPerTick = 60.0 / _bpm / 1000.0;
	}

	void start()
	{
		_started = true;
		_startTime = micros();
	}

	void stop()
	{
		_started = false;
	}

	void update()
	{
//		uint64_t currentTime = micros();
	}
};




#endif
