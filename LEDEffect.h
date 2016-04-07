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
    Parameter<uint8_t> *_changeSpeedParameter;


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
    Parameter<CRGB>* _colorParameter;

    Line(Parameter<CRGB>* colorParameter) 
        : Line(0, 1 , colorParameter, true)
    {
    }

    Line(uint8_t start, uint8_t end, Parameter<CRGB>* colorParameter, boolean addRGB) 
        : LEDEffect(addRGB), _start(start), _end(end), _colorParameter(colorParameter)
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

    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

        int numLEDs = data.getNumLEDs();
        for( int i = offset; i < numLEDs; i++) 
        {
            if( (i >= ( offset + _start ) && i <= ( offset + _end )) )
            {
                data.setLEDColor( i, _colorParameter->getValue(), _addRGB );
            }
            else if ((_end < _start && (i >= offset && i<= offset + _end))||(_end < _start && (i < numLEDs && i>= offset + _start))) //this is the case if the end wraps around the max number of LEDs and gets displayed in the beginning of the LED strip whereas the start is still at the end of the strip
            {
                data.setLEDColor( i, _colorParameter->getValue(), _addRGB );
            }
            else
            {
                //This would be setting the background color but we switch to an extra effect of a color reset.
                //data.setLEDColor( i, _bgColor, _addRGB );
            }
        }
    }
};


struct Radius : public virtual LEDEffect
{
    Parameter<uint8_t>* _position;
    Parameter<uint8_t>* _radius;
    Parameter<CRGB>* _colorParameter;
    boolean _doCircle;

    Radius(Parameter<uint8_t>* position, Parameter<uint8_t>* radius, Parameter<CRGB>* colorParameter, boolean addRGB, boolean doCircle = false) 
        : LEDEffect(addRGB), _position(position), _radius(radius), _colorParameter(colorParameter), _doCircle(doCircle)
    {
    }


    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

        int numLEDs = data.getNumLEDs();
        for( int i = offset; i < numLEDs; i++) 
        {
            int start = _position->getValue() - _radius->getValue();
            int end = _position->getValue() + _radius->getValue(); 
            /*if(start < offset || end > numLEDs) //check if the led number is in bounds
            {
                continue; 
            }*/
            if( (i >= ( offset + start ) && i <= ( offset + end )) || (_doCircle && ( (((end % numLEDs-1) + offset >= i) && end % numLEDs <= _radius->getValue() ) || (start - offset + numLEDs <= i) ) ) )
            {
                data.setLEDColor( i, _colorParameter->getValue(), _addRGB );
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

struct Stroboscope : public virtual LEDEffect
{
    bool _on;

    Stroboscope(bool startOn) 
        : LEDEffect(true), _on(startOn)
    {
    }


    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

        int numLEDs = data.getNumLEDs();

        for(int i = 0 + offset; i < numLEDs; i++) 
        {
            if(!_on)
            {
                data.setLEDColor( i, CRGB(0, 0, 0), false );
            }
        } 
        _on = !_on;
    }
};



struct Fire : public virtual LEDEffect
{
    uint8_t _cooling;
    uint8_t _sparking;
    bool _coldFire;

    byte heat[256];

    Fire() 
        : LEDEffect(false), _cooling(50), _sparking(120), _coldFire(false)
    {
    }

    Fire(uint8_t cooling, uint8_t sparking, bool coldFire) 
        : LEDEffect(false), _cooling(cooling), _sparking(sparking), _coldFire(coldFire)
    {
    }

    CRGB ColdColor( uint8_t temperature)
    {
        CRGB heatcolor;

        // Scale 'heat' down from 0-255 to 0-191,
        // which can then be easily divided into three
        // equal 'thirds' of 64 units each.
        uint8_t t192 = scale8_video( temperature, 192);

        // calculate a value that ramps up from
        // zero to 255 in each 'third' of the scale.
        uint8_t heatramp = t192 & 0x3F; // 0..63
        heatramp <<= 2; // scale up to 0..252

        // now figure out which third of the spectrum we're in:
        if( t192 & 0x80) {
            // we're in the hottest third
            heatcolor.r = heatramp; // full red
            heatcolor.g = 255; // full green
            heatcolor.b = 255; // ramp up blue

        } else if( t192 & 0x40 ) {
            // we're in the middle third
            heatcolor.r = 0; // full red
            heatcolor.g = heatramp; // ramp up green
            heatcolor.b = 255; // no blue

        } else {
            // we're in the coolest third
            heatcolor.r = 0; // ramp up red
            heatcolor.g = 0; // no green
            heatcolor.b = heatramp; // no blue
        }

        return heatcolor;
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
            data.setLEDColor( j, _coldFire ? ColdColor( heat[j]) : HeatColor( heat[j]), _addRGB );
//          data[j] = HeatColor( heat[j]);
        }
   }
};


struct Twinkle : public virtual LEDEffect
{
    // Base background color
//    #define BASE_COLOR       CRGB(32,0,32)
    Parameter<CRGB>* _baseColorParameter;

    // Peak color to twinkle up to
//    #define PEAK_COLOR       CRGB(64,0,64)
    Parameter<CRGB>* _peakColorParameter;


    // Currently set to brighten up a bit faster than it dims down, 
    // but this can be adjusted.

    // Amount to increment the color by each loop as it gets brighter:
//    #define DELTA_COLOR_UP   CRGB(4,0,4)
    Parameter<CRGB>* _deltaUpColorParameter;

    // Amount to decrement the color by each loop as it gets dimmer:
//    #define DELTA_COLOR_DOWN CRGB(1,0,1)
    Parameter<CRGB>* _deltaDownColorParameter;


    // Chance of each pixel starting to brighten up.  
    // 1 or 2 = a few brightening pixels at a time.
    // 10 = lots of pixels brightening at a time.
//    #define CHANCE_OF_TWINKLE 1
    Parameter<uint8_t>* _chanceOfTwinkle;

    enum { SteadyDim, GettingBrighter, GettingDimmerAgain };
    uint8_t PixelState[256];


    Twinkle(Parameter<uint8_t>* chanceOfTwinkle, Parameter<CRGB>* baseColorParameter, Parameter<CRGB>* peakColorParameter, Parameter<CRGB>* deltaUpColorParameter, Parameter<CRGB>* deltaDownColorParameter) 
        : LEDEffect(false), _baseColorParameter(baseColorParameter), _peakColorParameter(peakColorParameter), _deltaUpColorParameter(deltaUpColorParameter), _deltaDownColorParameter(deltaDownColorParameter), _chanceOfTwinkle(chanceOfTwinkle)
    {
        memset( PixelState, sizeof(PixelState), SteadyDim); // initialize all the pixels to SteadyDim.
    }

    virtual void render( struct LEDRow &data, uint8_t offset)
    {
        LEDEffect::render(data, offset);

        int numLEDs = data.getNumLEDs();

        for( uint16_t i = 0; i < numLEDs; i++) 
        {
            if( PixelState[i] == SteadyDim) 
            {
                // this pixels is currently: SteadyDim
                // so we randomly consider making it start getting brighter
                if( random8() <= _chanceOfTwinkle->getValue()) 
                {
                    PixelState[i] = GettingBrighter;
                }
              
            } 
            else if( PixelState[i] == GettingBrighter ) 
            {
                // this pixels is currently: GettingBrighter
                // so if it's at peak color, switch it to getting dimmer again
                if( data[i] >= _peakColorParameter->getValue() ) 
                {
                    PixelState[i] = GettingDimmerAgain;
                } 
                else 
                {
                    // otherwise, just keep brightening it:
                    data[i] += _deltaUpColorParameter->getValue();
                }
          
            } 
            else 
            { // getting dimmer again
                // this pixels is currently: GettingDimmerAgain
                // so if it's back to base color, switch it to steady dim
                if( data[i] <= _baseColorParameter->getValue() ) 
                {
                    data[i] = _baseColorParameter->getValue(); // reset to exact base color, in case we overshot
                    PixelState[i] = SteadyDim;
                } 
                else 
                {
                // otherwise, just keep dimming it down:
                    data[i] -= _deltaDownColorParameter->getValue();
                }
            }
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
