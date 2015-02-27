 
#include <FastLED.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h> // Included for SFE_LSM9DS0 library
#include <SFE_LSM9DS0.h>

#include <Nightflight.h> //requires the FastLED library for LED functionality



#define BRIGHTNESS  30
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

#define PIN_HAUBE_RECHTS_OBEN  2
#define PIN_HAUBE_LINKS_UNTEN 14
#define PIN_HAUBE_LINKS_OBEN   7
#define PIN_HAUBE_RECHTS_UNTEN  8

#define PIN_HECK_RECHTS_OBEN  6
#define PIN_HECK_LINKS_UNTEN 20
#define PIN_HECK_LINKS_OBEN   21
#define PIN_HECK_RECHTS_UNTEN  5

#define NUM_LEDS_HAUBE_LINKS_OBEN 39
#define NUM_LEDS_HAUBE_LINKS_UNTEN 37
#define NUM_LEDS_HECK_LINKS_OBEN 35
#define NUM_LEDS_HECK_LINKS_UNTEN 46  //11 for the fin

#define NUM_LEDS_HAUBE_RECHTS_OBEN 39
#define NUM_LEDS_HAUBE_RECHTS_UNTEN 37
#define NUM_LEDS_HECK_RECHTS_OBEN 35
#define NUM_LEDS_HECK_RECHTS_UNTEN 46  //11 for the fin



CRGB ledsHaubeLinksOben[NUM_LEDS_HAUBE_LINKS_OBEN];
CRGB ledsHaubeLinksUnten[NUM_LEDS_HAUBE_LINKS_UNTEN];
CRGB ledsHeckLinksOben[NUM_LEDS_HECK_LINKS_OBEN];
CRGB ledsHeckLinksUnten[NUM_LEDS_HECK_LINKS_UNTEN];

CRGB ledsHaubeRechtsOben[NUM_LEDS_HAUBE_RECHTS_OBEN];
CRGB ledsHaubeRechtsUnten[NUM_LEDS_HAUBE_RECHTS_UNTEN];
CRGB ledsHeckRechtsOben[NUM_LEDS_HECK_RECHTS_OBEN];
CRGB ledsHeckRechtsUnten[NUM_LEDS_HECK_RECHTS_UNTEN];


LEDStrip stripHaubeLinksOben(ledsHaubeLinksOben, NUM_LEDS_HAUBE_LINKS_OBEN, false);
LEDStrip stripHaubeLinksUnten(ledsHaubeLinksUnten, NUM_LEDS_HAUBE_LINKS_UNTEN, false);
LEDStrip stripHeckLinksOben(ledsHeckLinksOben, NUM_LEDS_HECK_LINKS_OBEN, true);
LEDStrip stripHeckLinksUnten(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN, true, 0, 35);
LEDStrip stripLinksFinne(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN, true, 35, 0);

LEDStrip stripHaubeRechtsOben(ledsHaubeLinksOben, NUM_LEDS_HAUBE_RECHTS_OBEN, false);
LEDStrip stripHaubeRechtsUnten(ledsHaubeLinksUnten, NUM_LEDS_HAUBE_RECHTS_UNTEN, false);
LEDStrip stripHeckRechtsOben(ledsHeckLinksOben, NUM_LEDS_HECK_RECHTS_OBEN, true);
LEDStrip stripHeckRechtsUnten(ledsHeckLinksUnten, NUM_LEDS_HECK_RECHTS_UNTEN, true, 0, 35);
LEDStrip stripRechtsFinne(ledsHeckLinksUnten, NUM_LEDS_HECK_RECHTS_UNTEN, true, 35, 0);

LEDVirtualStrip vStripLinksOben;
LEDVirtualStrip vStripLinksUnten;
LEDVirtualStrip vStripLinksFinne;

LEDVirtualStrip vStripRechtsOben;
LEDVirtualStrip vStripRechtsUnten;
LEDVirtualStrip vStripRechtsFinne;


Parameter<CRGB> lineColor(CRGB::Red);

Line line1(&lineColor);

Parameter<uint8_t> brightnessParam(BRIGHTNESS);
Parameter<uint8_t> rcChannelSteps(0);


//This has to be created in the sketch since including libraries in libraries does not work with the Arduino ide...
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

void setup() {
 //Perform all calculations on CRGB led values and Parameters in this callback.
  Nightflight.setDebug(false);

  Nightflight.setMainLoopCallback(renderTimerInfoMainLoopCallback);
  Nightflight.setBPMLoopCallback(bpmTimerInfoLoopCallback);
  Nightflight.setBPM(120.0);
  
  Nightflight.setLSM9DS0(&dof);
  dof.setAccelScale(dof.A_SCALE_8G);
  dof.setAccelODR(dof.A_ODR_100);
  dof.setGyroScale(dof.G_SCALE_2000DPS);
  dof.setGyroODR(dof.G_ODR_380_BW_50);
  dof.setMagScale(dof.M_SCALE_8GS);
  dof.setMagODR(dof.M_ODR_50);
  
  
  //set up the LED hardware
  FastLED.addLeds<CHIPSET, PIN_HAUBE_LINKS_OBEN, COLOR_ORDER>(ledsHaubeLinksOben, NUM_LEDS_HAUBE_LINKS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HAUBE_LINKS_UNTEN, COLOR_ORDER>(ledsHaubeLinksUnten, NUM_LEDS_HAUBE_LINKS_UNTEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_LINKS_OBEN, COLOR_ORDER>(ledsHeckLinksOben, NUM_LEDS_HECK_LINKS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_LINKS_UNTEN, COLOR_ORDER>(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN).setCorrection( TypicalLEDStrip );
  
  FastLED.addLeds<CHIPSET, PIN_HAUBE_RECHTS_OBEN, COLOR_ORDER>(ledsHaubeRechtsOben, NUM_LEDS_HAUBE_RECHTS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HAUBE_RECHTS_UNTEN, COLOR_ORDER>(ledsHaubeRechtsUnten, NUM_LEDS_HAUBE_RECHTS_UNTEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_RECHTS_OBEN, COLOR_ORDER>(ledsHeckRechtsOben, NUM_LEDS_HECK_RECHTS_OBEN).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_HECK_RECHTS_UNTEN, COLOR_ORDER>(ledsHeckRechtsUnten, NUM_LEDS_HECK_RECHTS_UNTEN).setCorrection( TypicalLEDStrip );
  
  vStripLinksOben.addLEDStrip(stripHaubeLinksOben);
  vStripLinksOben.addLEDStrip(stripHeckLinksOben);  
  vStripLinksFinne.addLEDStrip(stripLinksFinne);
  vStripLinksUnten.addLEDStrip(stripHaubeLinksUnten);
  vStripLinksUnten.addLEDStrip(stripHeckLinksUnten);

  vStripRechtsOben.addLEDStrip(stripHaubeRechtsOben);
  vStripRechtsOben.addLEDStrip(stripHeckRechtsOben);  
  vStripRechtsFinne.addLEDStrip(stripRechtsFinne);
  vStripRechtsUnten.addLEDStrip(stripHaubeRechtsUnten);
  vStripRechtsUnten.addLEDStrip(stripHeckRechtsUnten);

 
  FastLED.setBrightness( BRIGHTNESS );
 
  Nightflight.startBPMTimer();
}


//boolean to check if a FastLED Render was performed

void loop()
{
  Nightflight.loop();
}

float fmap (float sensorValue, float sensorMin, float sensorMax, float outMin, float outMax)
{
  return (sensorValue - sensorMin) * (outMax - outMin) / (sensorMax - sensorMin) + outMin;
}

uint32_t beats = 0;
boolean bpmRender = false;
boolean bpmRenderNext = false;
uint8_t rColorHue = 0;

void bpmTimerInfoLoopCallback()
{
  beats++;
  
}

void waitStartSequenceRender()
{
  line1.setEnd(2);
  line1.render(stripHaubeLinksOben, 0);
  line1.setEnd(1);
  line1.render(stripHaubeLinksUnten, 0);
  line1.setEnd(6);
  line1.render(stripHeckLinksOben, 0);
  line1.setEnd(5);
  line1.render(stripHeckLinksUnten, 0);

  line1.setEnd(0);
  line1.render(stripHaubeRechtsOben, 0);
  line1.setEnd(3);
  line1.render(stripHaubeRechtsUnten, 0);
  line1.setEnd(4);
  line1.render(stripHeckRechtsOben, 0);
  line1.setEnd(7);
  line1.render(stripHeckRechtsUnten, 0);
    
    
}

uint8_t brightnessByAccelfade = 20;

void brightnessByAccel()
{
    uint8_t currentBrightness = brightnessParam.getValue();
    uint8_t absAccel = (uint8_t)fmap(Nightflight.getAbsoluteAcceleration(), 0.0, 16.0, 0.0, 255.0);
    if(absAccel > currentBrightness)
    {
      brightnessParam.setValue(absAccel);
    }
    else
    {
      brightnessParam.setValue(currentBrightness >= brightnessByAccelfade ? currentBrightness - brightnessByAccelfade : 0);
    }
}

void renderTimerInfoMainLoopCallback()
{
  //first evaluate the input from the RC Channel, currently splitting to 6 steps
  rcChannelSteps.setValue(map(Nightflight.currentChannelData, 1120, 1920, 0, 5));
  //then switch programs according to RC Channel value
  uint32_t theValue = rcChannelSteps.getValue();
  Serial.print("RC LED Program Selection: ");
  Serial.println(theValue);
  
  //if( !bpmRender || (bpmRender && bpmRenderNext) )
  {
    switch(theValue)
    {
      case 5:  break;
      case 4:  break;
      case 3:  break;
      case 2:   bpmRender = true;
                bpmRenderNext = false;    
                brightnessParam.setValue(BRIGHTNESS);
                waitStartSequenceRender();
                break;
      case 1:   bpmRender = false;
                brightnessByAccel();
                waitStartSequenceRender();
                break;
      case 0:   bpmRender = false;
                brightnessParam.setValue(BRIGHTNESS);
                waitStartSequenceRender();
                break;
      default:  break;
     }
   }


    //fire2.render(vStripLinksUnten, 0);
    //rainbow2.render(vStripHeckFinne, 0);
    /*
    line1.setStart((uint8_t)((Nightflight.currentChannelData-1120)/30) % 30);
    line1.setEnd((uint8_t)((Nightflight.currentChannelData-1120)/30 + 3) % 30);
    line1.render(vStripHeckFinne, 0);*/
    
    //  fade1.render(vStripHeckFinne, 0);
    //br1.render(vStripHeckFinne, 0);
    /*if(fade1.fadeFinished())
    {
      fade1.reset();
    }*/
    FastLED.setBrightness( brightnessParam.getValue() );
}




