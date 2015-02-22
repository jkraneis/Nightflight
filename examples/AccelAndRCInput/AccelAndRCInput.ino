
#include <FastLED.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h> // Included for SFE_LSM9DS0 library
#include <SFE_LSM9DS0.h>

#include <Nightflight.h> //requires the FastLED library for LED functionality



#define BRIGHTNESS  30
#define CHIPSET     WS2811
#define COLOR_ORDER GRB

#define PIN_HAUBE_LINKS_OBEN  2
#define PIN_HAUBE_LINKS_UNTEN 14
#define PIN_HECK_LINKS_OBEN   7
#define PIN_HECK_LINKS_UNTEN  8

#define NUM_LEDS_HAUBE_LINKS_OBEN 30
#define NUM_LEDS_HAUBE_LINKS_UNTEN 30
#define NUM_LEDS_HECK_LINKS_OBEN 30
#define NUM_LEDS_HECK_LINKS_UNTEN 60



CRGB ledsHaubeLinksOben[NUM_LEDS_HAUBE_LINKS_OBEN];
CRGB ledsHaubeLinksUnten[NUM_LEDS_HAUBE_LINKS_UNTEN];
CRGB ledsHeckLinksOben[NUM_LEDS_HECK_LINKS_OBEN];
CRGB ledsHeckLinksUnten[NUM_LEDS_HECK_LINKS_UNTEN];


LEDStrip stripHaubeLinksOben(ledsHaubeLinksOben, NUM_LEDS_HAUBE_LINKS_OBEN, false);
LEDStrip stripHaubeLinksUnten(ledsHaubeLinksUnten, NUM_LEDS_HAUBE_LINKS_UNTEN, true);
LEDStrip stripHeckLinksOben(ledsHeckLinksOben, NUM_LEDS_HECK_LINKS_OBEN, true);
LEDStrip stripHeckLinksUnten(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN, false, 0, 30);
LEDStrip stripHeckFinne(ledsHeckLinksUnten, NUM_LEDS_HECK_LINKS_UNTEN, false, 30, 0);

LEDVirtualStrip vStripLinksOben;
LEDVirtualStrip vStripLinksUnten;
LEDVirtualStrip vStripHeckFinne;


Parameter<CRGB> riderColor(CRGB::Red);
Parameter<CRGB> riderColor2(CRGB::Green);
Parameter<CRGB> riderColor3(CRGB::Blue);
Parameter<CRGB> riderResetColor(CRGB::Black);
ColorReset riderReset(&riderResetColor);
Rainbow rainbow1(6);
KnightRider rider11(0, 2, true, false, &riderColor, &riderReset);
KnightRider rider12(59, 2, false, true, &riderColor, &rainbow1);

KnightRider rider21(0, 2, true, true, &riderColor3, &riderReset);
KnightRider rider22(59, 2, false, true, &riderColor2, &rider21);


Rainbow rainbow2(6);
Fire fire1;
Fire fire2;
Fire fire3;
Fade fade1(4);
Line line1(0, 2, CRGB::Red, false);

Parameter<uint8_t> brightnessParam(BRIGHTNESS);
Brightness br1(&brightnessParam);

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
  
  vStripLinksOben.addLEDStrip(stripHaubeLinksOben);
  vStripLinksOben.addLEDStrip(stripHeckLinksOben);
  //vStripLinksOben.addLEDStrip(stripHeckLinksUnten);
  //vStripLinksOben.addLEDStrip(stripHaubeLinksUnten);
  
  vStripHeckFinne.addLEDStrip(stripHeckFinne);
  vStripLinksUnten.addLEDStrip(stripHaubeLinksUnten);
  vStripLinksUnten.addLEDStrip(stripHeckLinksUnten);
 Serial.println(vStripLinksUnten.getNumLEDs());
  
  
  FastLED.setBrightness( BRIGHTNESS );
 /*   while (!Serial.available())
    ;*/

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
void bpmTimerInfoLoopCallback()
{
  beats++;
  
  if(!bpmRender)
  {
    return;
  }
  if(Nightflight.getBeatIsDownbeat())
  {
    Serial.print(" isDownbeat ");
  }
  //bpmRender = false;
  bpmRenderNext = true;
}

uint8_t rColorHue = 0;
void waitStartSequenceRender()
{
    //CHSV rColor = CHSV(rColorHue, 255, 255);
   // riderColor.setValue(rColor);
  //  rider11.render(vStripLinksOben, 0);
   //   rider12.render(vStripHeckFinne, 0);
    
    rider22.render(vStripLinksUnten, 0); //rider21.render gets called inside since it's a background effect
    
  //  rColorHue+=10;  
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
  
  if( !bpmRender || (bpmRender && bpmRenderNext) )
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




