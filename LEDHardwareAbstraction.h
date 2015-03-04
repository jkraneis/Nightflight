#ifndef __INC_LEDHARDWAREABSTRACTION_H
#define __INC_LEDHARDWAREABSTRACTION_H



#include <stdint.h>
#include <FastLED.h>

#include "Vector.h"




struct LEDRow
{
	virtual CRGB& operator[] (uint16_t x);

	virtual int getNumLEDs();

	virtual void setLEDColor(int ledIndex, const CRGB& color, boolean addColor);
};



struct LEDStrip : public virtual LEDRow
{
	CRGB* _leds;
	int _numLeds;
	boolean _directionFrontToBack;
	int _offset;
	int _offsetEnd; //means offset from the end of the ledStrip: if the strip has 40 LEDs and _offsetEnd is set to 5, this means the end of the strip is 40-5

	LEDStrip(struct CRGB* leds, int numLEDs, boolean directionFrontToBack, int offset = 0, int offsetEnd = 0) 
		: _leds(leds), _numLeds(numLEDs), _directionFrontToBack(directionFrontToBack), _offset(offset), _offsetEnd(offsetEnd)
	{
	}

	virtual CRGB& operator[] (uint16_t x)
	{
		int realIndex = _directionFrontToBack ? x + _offset : _numLeds - _offsetEnd - 1 - x;
		return _leds[realIndex];
	}

	virtual int getNumLEDs()
	{
		return _numLeds - _offset - _offsetEnd;
	}

	boolean getDirectionFrontToBack()
	{
		return _directionFrontToBack;
	}

	//ledIndex is zero based!
	virtual void setLEDColor(int ledIndex, const CRGB& color, boolean addColor)
	{
		int realIndex = _directionFrontToBack ? ledIndex + _offset : _numLeds - _offsetEnd - 1 - ledIndex;
		if( !addColor )
		{
			_leds[realIndex] = CRGB::Black;
		}
		_leds[realIndex] += color;
	}
};


struct LEDVirtualStrip : public virtual LEDRow
{
	Vector<LEDStrip> stripVector;

	LEDVirtualStrip() 
		
	{
	}

	void addLEDStrip(struct LEDStrip& stripToAdd )
	{
		stripVector.push_back(stripToAdd);
	}

	virtual int getNumLEDs()
	{
		int returnValue = 0;
		size_t stripCount = stripVector.size();
		for( size_t i = 0; i < stripCount; i++ )
		{
			LEDStrip currentStrip = stripVector[i];
			returnValue += currentStrip.getNumLEDs();
		}
		return returnValue;
	}

	virtual CRGB& operator[] (uint16_t x)
	{
		size_t stripCount = stripVector.size();
		int currentStripIndex = 0;
		for( size_t i = 0; i < stripCount; i++ )
		{
			LEDStrip currentStrip = stripVector[i];
			int ledCount = currentStrip.getNumLEDs();
			if( ledCount > x )
			{
				currentStripIndex = i;
				break;
			}
			else
			{
				x -= ledCount;
			}
		}
		LEDStrip returnStrip = stripVector[currentStripIndex];
		return returnStrip[x];
	}

	//ledIndex is zero based!
	virtual void setLEDColor(int ledIndex, const CRGB& color, boolean addColor)
	{
		size_t stripCount = stripVector.size();

		for( size_t i = 0; i < stripCount; i++ )
		{
			LEDStrip currentStrip = stripVector[i];
			int ledCount = currentStrip.getNumLEDs();
			if( ledCount > ledIndex )
			{
				currentStrip.setLEDColor(ledIndex, color, addColor);
				break;
			}
			else
			{
				ledIndex -= ledCount;
			}
		}
	}

};

struct LEDRegion
{
	uint16_t _width;
	uint16_t _height;

	Vector<LEDVirtualStrip> stripVector;


	LEDRegion()
        : _width(0), _height(0)
    {

    }

	void addLEDStrip(struct LEDVirtualStrip& stripToAdd )
	{
		int numLEDs = stripToAdd.getNumLEDs();
		stripVector.push_back(stripToAdd);
		_height++;
		if(numLEDs > _width)
		{
			_width = numLEDs;
		}
	}

   /* virtual CRGB& operator[][] (uint16_t x, uint16_t y)
	{
		//Catch the case if a led strip is shorter than width
		size_t stripCount = stripVector.size();
		int currentStripIndex = 0;
		for( size_t i = 0; i < stripCount; i++ )
		{
			LEDStrip currentStrip = stripVector[i];
			int ledCount = currentStrip.getNumLEDs();
			if( ledCount > x )
			{
				currentStripIndex = i;
				break;
			}
			else
			{
				x -= ledCount;
			}
		}
		LEDStrip returnStrip = stripVector[currentStripIndex];
		return returnStrip[x];
	}*/

   // static CRGB** getLEDs( struct CRGB ** allLEDs, struct LEDRegion& region )

};


#endif
