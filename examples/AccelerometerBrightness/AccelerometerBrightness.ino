
#include <FastLED.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h> // Included for SFE_LSM9DS0 library
#include <SFE_LSM9DS0.h>

#include <Nightflight.h> //requires the FastLED library for LED functionality



#define BRIGHTNESS  20
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



KnightRider rider11(0, 2, true, false, false, CRGB::White, CRGB::Black);
KnightRider rider12(29, 2, false, false, true, CRGB::Black, CRGB::Black);
Rainbow rainbow1(2);
Rainbow rainbow2(6);
Fire fire1;
Fire fire2;
Fire fire3;
Fade fade1(4);
Line line1(0, 2, CRGB::Red, CRGB::Black, false);

Brightness br1;

Parameter<uint8_t> brightnessParam;
//KnightRider rider21;
//KnightRider rider31(0, 0, true, false, CRGB::Red, CRGB::Black);
//KnightRider rider41;
//KnightRider rider42(59, 2, false, false, CRGB::Red, CRGB::Blue);
//KnightRider rider2(15, 1, true, false, CRGB::Green, CRGB::Black);
//KnightRider rider3(30, 1, false, true, CRGB::Blue, CRGB::Black);

// Define which pin we're using to accept the signal

StepParameter testParam(1120, 2400, 250);

//This has to be created in the sketch since including libraries in libraries does not work with the Arduino ide...
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

void setup() {
  //Perform all calculations on CRGB led values and Parameters in this callback.
  Nightflight.setDebug(false);

  Nightflight.setMainLoopCallback(renderTimerInfoMainLoopCallback);
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
  br1.setBrightnessParameter((Parameter<unsigned char>*)&testParam);
  FastLED.setBrightness( BRIGHTNESS );
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

void renderTimerInfoMainLoopCallback()
{
    testParam.setValue(Nightflight.currentChannelData);
    uint8_t absAccel = (uint8_t)fmap(Nightflight.getAbsoluteAcceleration(), 0.0, 16.0, 0.0, 100.0);
    Serial.println(absAccel);
    brightnessParam.setValue(absAccel);
    //first read the RX data if available
    rider11.render(vStripLinksOben, 0);
    fire2.render(vStripLinksUnten, 0);
    //rainbow2.render(vStripHeckFinne, 0);
    //line1.setStart((uint8_t)((Nightflight.currentChannelData-1120)/30) % 30);
    //line1.setEnd((uint8_t)((Nightflight.currentChannelData-1120)/30 + 3) % 30);
    //line1.render(vStripHeckFinne, 0);
    //  fire3.render(vStripHeckFinne, 0);
    rider12.render(vStripHeckFinne, 0);
    //  fade1.render(vStripHeckFinne, 0);
   // br1.render(vStripHeckFinne, 0);
  FastLED.setBrightness( brightnessParam.getValue() );
    if(fade1.fadeFinished())
    {
      fade1.reset();
    }
}




