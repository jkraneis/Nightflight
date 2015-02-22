#ifndef __INC_NFLEDEFFECT_H
#define __INC_NFLEDEFFECT_H

#include <stdint.h>
#include <FastLED.h>
#include "ParameterChange.h"


struct LEDEffect
{
protected:
	boolean _addRGB;

    boolean _hasBackgroundEffect = false;
    LEDEffect* _backgroundEffect = NULL;

public:
	LEDEffect() 
		: LEDEffect(false, NULL)
	{
	}

    LEDEffect(boolean addRGB) 
        : LEDEffect(addRGB, NULL)
    {
    }

    LEDEffect(boolean addRGB, LEDEffect* backgroundEffect) 
        : _addRGB(addRGB)
    {
        if(backgroundEffect != NULL)
        {
            _backgroundEffect = backgroundEffect;
            _hasBackgroundEffect = true;
        }
    }

    virtual void setBackgroundEffect(LEDEffect* backgroundEffect)
    {
        if(backgroundEffect != NULL)
        {
            _backgroundEffect = backgroundEffect;
            _hasBackgroundEffect = true;
        }
    }


	virtual void render( struct LEDRow &data, uint8_t offset)
    {
        if(_hasBackgroundEffect)
        {
            _backgroundEffect->render(data, offset);
        }
    }
};

/*struct LEDPattern : public virtual LEDEffect
{
    LEDPattern() 
        : LEDEffect(false)
    {
    }
};*/

/*
*   Linear Rainbow effect to switch through the whole Color Space. Set colorStepsPerFrame in Constructor bigger than 1 to get faster color changes.
*/
struct Rainbow : public virtual LEDEffect
{
    uint8_t _colorStepsPerFrame;
    uint8_t _hue;


    Rainbow() 
        : LEDEffect(false), _colorStepsPerFrame(1)
    {
    }

    Rainbow(uint8_t colorStepsPerFrame) 
        : LEDEffect(false),  _colorStepsPerFrame(colorStepsPerFrame)
    {
    }

    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

        int numLEDs = data.getNumLEDs();
        for( int i = offset; i < numLEDs; i++) 
        {
            data.setLEDColor( i, CHSV(_hue, 255, 255), _addRGB );
//            data[i] = CHSV(_hue, 255, 255);
        }
        _hue += _colorStepsPerFrame;
    }
};

struct ColorReset : public virtual LEDEffect
{
    Parameter<CRGB>* _colorParameter;

    ColorReset(Parameter<CRGB>* colorParameter) 
        : LEDEffect(false), _colorParameter(colorParameter)
    {
    }



    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset); //there should not be a background effect

        int numLEDs = data.getNumLEDs();
        for( int i = offset; i < numLEDs; i++) 
        {
            data.setLEDColor( i, _colorParameter->getValue(), _addRGB );
        }
    }
};

struct Line : public virtual LEDEffect
{
    uint8_t     _start;
    uint8_t     _end;
    CRGB        _color;

    Line() 
        : LEDEffect(false), _start(0), _end(0), _color(CRGB(255,0,0))
    {
    }

    Line(uint8_t start, uint8_t end, const CRGB& color, boolean addRGB) 
        : LEDEffect(addRGB), _start(start), _end(end), _color(color)
    {
    }

    uint8_t getStart()
    {
        return _start;
    }
    
    void setStart(uint8_t start)
    {
        _start = start;
    }

    uint8_t getEnd()
    {
        return _end;
    }
    
    void setEnd(uint8_t end)
    {
        _end = end;
    }

    CRGB& getColor()
    {
        return _color;
    }

    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

        int numLEDs = data.getNumLEDs();
        for( int i = offset; i < numLEDs; i++) 
        {
            if( (i >= ( offset + _start ) && i <= ( offset + _end )) )
            {
                data.setLEDColor( i, _color, _addRGB );
            }
            else if ((_end < _start && (i >= offset && i<= offset + _end))||(_end < _start && (i < numLEDs && i>= offset + _start))) //this is the case if the end wraps around the max number of LEDs and gets displayed in the beginning of the LED strip whereas the start is still at the end of the strip
            {
                data.setLEDColor( i, _color, _addRGB );
            }
            else
            {
                //This would be setting the background color but we switch to an extra effect of a color reset.
                //data.setLEDColor( i, _bgColor, _addRGB );
            }
        }
    }
};

struct Brightness : public virtual LEDEffect
{
    uint8_t     _initialBrightness;
    Parameter<uint8_t>* _brightnessParameter;

    Brightness(Parameter<uint8_t>* brightnessParameter) 
        : LEDEffect(true), _brightnessParameter(brightnessParameter)
    {
        _initialBrightness = _brightnessParameter->getValue();
    }


    void reset()
    {
        _brightnessParameter->setValue(_initialBrightness);
    }


    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

        int numLEDs = data.getNumLEDs();
        uint8_t valueToSet = _brightnessParameter->getValue();

        for(int i = 0 + offset; i < numLEDs; i++) 
        { 
            data[i].nscale8(valueToSet); 
        } 
    }
};



struct Fire : public virtual LEDEffect
{
	uint8_t _cooling;
	uint8_t _sparking;

	byte heat[256];

	Fire() 
        : LEDEffect(false), _cooling(50), _sparking(120)
    {
    }

	Fire(uint8_t cooling, uint8_t sparking) 
        : LEDEffect(false), _cooling(cooling), _sparking(sparking)
    {
    }

    virtual void render( struct LEDRow &data, uint8_t offset)
   	{
        LEDEffect::render(data, offset);

    	int numLEDs = data.getNumLEDs();
  // Step 1.  Cool down every cell a little
	    for( int i = offset; i < numLEDs; i++) {
	      heat[i] = qsub8( heat[i],  random8(0, ((_cooling * 10) / numLEDs) + 2));
	    }
	  
	    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
	    for( int k= numLEDs - 1; k >= 2; k--) {
	      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
	    }
	    
	    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	    if( random8() < _sparking ) {
	      int y = random8(7);
	      heat[y] = qadd8( heat[y], random8(160,255) );
	    }

	    // Step 4.  Map from heat cells to LED colors
	    for( int j = offset; j < numLEDs; j++) {
            data.setLEDColor( j, HeatColor( heat[j]), _addRGB );
//	        data[j] = HeatColor( heat[j]);
	    }
   }
};

struct Fade : public virtual LEDEffect
{

	uint16_t _counter;
	uint16_t _step;

	Fade(uint16_t step ) 
        : LEDEffect(true), _step(step)
    {
    	reset();
    }

    void reset()
    {
		_counter = 255;
    }

    boolean fadeFinished()
    {
    	return _counter == 0;
    }

    virtual void render( struct LEDRow &data, uint8_t offset)
   	{
        LEDEffect::render(data, offset);

    	int numLEDs = data.getNumLEDs();
    	if( _counter >= _step)
    	{
    		_counter -= _step;
    	}
    	else
    	{
    		_counter = 0;
    	}
    	for(int i = 0 + offset; i < numLEDs; i++) 
    	{ 
    		data[i].nscale8(_counter); 
    	} 
   }
};

struct KnightRider : public virtual LEDEffect
{

	uint8_t 	_lastPos;
	uint8_t 	_range;
	boolean 	_lastDirection;
	Parameter<CRGB>* _colorParameter;
	
	KnightRider(Parameter<CRGB>* colorParameter) 
        : KnightRider(0, 1, true, false, colorParameter, NULL)
    {
    }

	KnightRider(uint8_t krLastPos, uint8_t krRange, boolean krLastDirection, boolean krAddRGB, Parameter<CRGB>* colorParameter, LEDEffect* backgroundEffect = NULL) 
        : LEDEffect(krAddRGB, backgroundEffect), _lastPos(krLastPos), _range(krRange), _lastDirection(krLastDirection), _colorParameter(colorParameter)
    {
    }


    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

    	int numLEDs = data.getNumLEDs();
    	for( int i = offset; i < offset + numLEDs; i++) 
    	{
	    	if( i >= ( _lastPos - _range ) && i <= ( _lastPos + _range ) )
	      	{
        		data.setLEDColor( i, _colorParameter->getValue(), _addRGB );
	      	}
	    }
	    if( _lastDirection )
	    {
	      	if(_lastPos == offset + numLEDs - 1) //change direction
	      	{
		        _lastDirection = !_lastDirection;
		        _lastPos--;
	      	}
	      	_lastPos++;
	    }
	    else
	    {
	      	if(_lastPos == offset) //change direction
	      	{
		        _lastDirection = !_lastDirection;
		        _lastPos++;
	      	}
	      	_lastPos--;
	    }
    }
};





#endif
