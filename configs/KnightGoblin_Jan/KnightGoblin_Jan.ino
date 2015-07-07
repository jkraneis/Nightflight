#define NIGHTFLIGHTUSELSMDM9DF0


#include <FastLED.h>

#ifdef NIGHTFLIGHTUSELSMDM9DF0
#include <SPI.h>
#include <i2c_t3.h>
#include <SFE_LSM9DS0.h>
#endif

#include <Nightflight.h> //requires the FastLED library for LED functionality

Parameter<uint8_t> rcChannelSteps(0);
Parameter<uint8_t> rcChannelSteps2(0);

#include <Jan.h>
//#include <Ingbert.h>

Parameter<uint8_t> brightnessParam(BRIGHTNESS);


CRGB tronColor(24,202,230);

Parameter<CRGB> riderColor(tronColor);
Parameter<CRGB> riderColorWhite(CRGB::White);
Parameter<CRGB> riderColorGreen(CRGB::Green);
Parameter<CRGB> riderColorDarkBlue(CRGB::DarkBlue);
Parameter<CRGB> riderColorBlack(CRGB::Black);
Parameter<CRGB> riderResetColor(CRGB::Green);
ColorReset riderReset(&riderResetColor);
ColorReset riderResetBlack(&riderColorBlack);
KnightRider rider11(0, 4, true, false, &riderColor, &riderReset);
KnightRider rider12(0, 4, true, false, &riderColor, &riderReset);
KnightRider rider13(0, 4, true, false, &riderColor, &riderReset);
KnightRider rider14(0, 4, true, false, &riderColor, &riderReset);

Fire fire1(70, 120);
Fire fire2(80, 120);
Fire fire3(90, 120);
Fire fire4(80, 120);


Parameter<CRGB> lineColor(CRGB::Red);

Line line1(0, 255, &lineColor, false);

Parameter<uint8_t> position(40);
Parameter<uint8_t> radius(0);
ParameterTimeLinear radiusLinearRadius(0, 40, 2000, &radius, 1);
Radius radiusLinear(&position, &radius, &riderColorDarkBlue, false);
ParameterTimeLinear radiusLinearRadiusDown(40, 0, 300, &radius, 0);

Parameter<uint8_t> radius2(0);
ParameterTimeLinear radiusLinearRadius2(0, 36, 4000, &radius2, 2);
Radius radiusLinear2(&position, &radius2, &riderColorGreen, true);
ParameterTimeLinear radiusLinearRadius2Down(36, 0, 300, &radius2, 0);

Parameter<uint8_t> radius3(0);
ParameterTimeLinear radiusLinearRadius3(0, 32, 6629, &radius3, 3);
Radius radiusLinear3(&position, &radius3, &riderColorWhite, true);
ParameterTimeLinear radiusLinearRadius3Down(32, 0, 300, &radius3, 0);

//YAW thing
Parameter<uint8_t> positionYaw(30);
Parameter<uint8_t> positionYaw2(60);
Parameter<uint8_t> positionYaw3(90);
Parameter<uint8_t> positionYaw4(120);
Parameter<uint8_t> radiusYawWidth(15);
Radius radiusYaw(&positionYaw, &radiusYawWidth, &riderColorDarkBlue, false, true);
Radius radiusYaw2(&positionYaw2, &radiusYawWidth, &riderColorGreen, false, true);
Radius radiusYaw3(&positionYaw3, &radiusYawWidth, &lineColor, false, true);
Radius radiusYaw4(&positionYaw4, &radiusYawWidth, &riderColorWhite, false, true);

//Equalizer
uint8_t eqLength = 70;
uint8_t eqSegmentLength = eqLength/7;
Parameter<CRGB> equalColor(CRGB::Red);

Parameter<uint8_t> equalBrightnessParam0(0);
Line equalLine0(0, eqSegmentLength-1, &equalColor, false);
Brightness equalBrightness0(&equalBrightnessParam0);

Parameter<uint8_t> equalBrightnessParam1(0);
Line equalLine1(0, eqSegmentLength-1, &equalColor, false);
Brightness equalBrightness1(&equalBrightnessParam1);

Parameter<uint8_t> equalBrightnessParam2(0);
Line equalLine2(0, eqSegmentLength-1, &equalColor, false);
Brightness equalBrightness2(&equalBrightnessParam2);

Parameter<uint8_t> equalBrightnessParam3(0);
Line equalLine3(0, eqSegmentLength-1, &equalColor, false);
Brightness equalBrightness3(&equalBrightnessParam3);

Parameter<uint8_t> equalBrightnessParam4(0);
Line equalLine4(0, eqSegmentLength-1, &equalColor, false);
Brightness equalBrightness4(&equalBrightnessParam4);

Parameter<uint8_t> equalBrightnessParam5(0);
Line equalLine5(0, eqSegmentLength-1, &equalColor, false);
Brightness equalBrightness5(&equalBrightnessParam5);

Parameter<uint8_t> equalBrightnessParam6(0);
Line equalLine6(0, eqSegmentLength-1, &equalColor, false);
Brightness equalBrightness6(&equalBrightnessParam6);



//This has to be created in the sketch since including libraries in libraries does not work with the Arduino ide...
#ifdef NIGHTFLIGHTUSELSMDM9DF0
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
#endif

void setup() {
 //Perform all calculations on CRGB led values and Parameters in this callback.
  Nightflight.setDebug(false);

  Nightflight.setMainLoopCallback(renderTimerInfoMainLoopCallback);
  Nightflight.setBPMLoopCallback(bpmTimerInfoLoopCallback);
  Nightflight.setBPM(123.0);
  
#ifdef NIGHTFLIGHTUSELSMDM9DF0
  Nightflight.setLSM9DS0(&dof);
#endif
  
  
  setupIndividual();
  FastLED.setBrightness( BRIGHTNESS );
 
  radiusLinear.setBackgroundEffect(&riderResetBlack);
  radiusLinear2.setBackgroundEffect(&radiusLinear);
  radiusLinear3.setBackgroundEffect(&radiusLinear2);
 
  radiusYaw.setBackgroundEffect(&riderResetBlack);
  
  equalBrightness0.setBackgroundEffect(&equalLine0);
  equalBrightness1.setBackgroundEffect(&equalLine1);
  equalBrightness2.setBackgroundEffect(&equalLine2);
  equalBrightness3.setBackgroundEffect(&equalLine3);
  equalBrightness4.setBackgroundEffect(&equalLine4);
  equalBrightness5.setBackgroundEffect(&equalLine5);
  equalBrightness6.setBackgroundEffect(&equalLine6);
}



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
uint8_t _keepBrightness = 0;
void bpmTimerInfoLoopCallback()
{
  beats++;
 // if(Nightflight.getBeatIsDownbeat())
  {
    Serial.print("Beats: ");
    Serial.println(beats);
    if(beats == 21)
    {
      radiusLinearRadius.start();
      radiusLinearRadius2.start();
      radiusLinearRadius3.start();
    }
    if(beats == 44)
    {
      radiusLinearRadius.stop();
      radiusLinearRadius2.stop();
      radiusLinearRadius3.stop();
      radiusLinearRadiusDown.start();
      radiusLinearRadius2Down.start();
      radiusLinearRadius3Down.start();
    }
  }
  if(!bpmRender)
  {
    return;
  }
  if(Nightflight.getBeatIsDownbeat())
  {
    //Serial.print(" isDownbeat ");
    FastLED.setBrightness( 255 );
    _keepBrightness = 3;
  }
  else
  {
    FastLED.setBrightness( 70 );
  }
  FastLED.show();
  bpmRenderNext = true;
  
}


boolean bpmTimerStarted = false;
void startBPMTimer()
{
  if(!bpmTimerStarted)
  {
    Nightflight.startBPMTimer();
    bpmTimerStarted = true;
  }
}

void waitStartSequenceRender()
{
    rider11.render(vStripLinksOben, 0);
    rider12.render(vStripLinksUnten, 0);
    rider13.render(vStripRechtsOben, 0);
    rider14.render(vStripRechtsUnten, 0);
}

void fire()
{
    fire1.render(vStripLinksOben, 0);
    fire2.render(vStripLinksUnten, 0);
    fire3.render(vStripRechtsOben, 0);
    fire4.render(vStripRechtsUnten, 0);
}

void THX()
{
    radiusLinearRadius.render();
    radiusLinearRadius2.render();
    radiusLinearRadius3.render();
    radiusLinearRadiusDown.render();
    radiusLinearRadius2Down.render();
    radiusLinearRadius3Down.render();
    radiusLinear3.render(vStripRechtsOben,0);
    radiusLinear3.render(vStripRechtsUnten,0);
    radiusLinear3.render(vStripLinksOben,0);
    radiusLinear3.render(vStripLinksUnten,0);
    brightnessParam.setValue(radius3.getValue()*7);
}

void Yaw()
{
    uint8_t yaw = (uint8_t)fmap(Nightflight.yaw, -180.0, 180.0, 0.0, 152.0);
/*    Serial.print("Yaw: ");
    Serial.println(Nightflight.yaw);*/
    positionYaw.setValue(yaw);
    positionYaw2.setValue((yaw + 37) % 152);
    positionYaw3.setValue((yaw + 74) % 152);
    positionYaw4.setValue((yaw + 111) % 152);
    radiusYaw.render(vStripObenRundrum, 0);
    radiusYaw2.render(vStripObenRundrum, 0);
    radiusYaw3.render(vStripObenRundrum, 0);
    radiusYaw4.render(vStripObenRundrum, 0);
    radiusYaw.render(vStripUntenRundrum, 0);
    radiusYaw2.render(vStripUntenRundrum, 0);
    radiusYaw3.render(vStripUntenRundrum, 0);
    radiusYaw4.render(vStripUntenRundrum, 0);
}

void equalizer()
{
/*  Serial.print(Nightflight._rms[0]);
  Serial.print(" ");
  Serial.print(Nightflight._rms[1]);
  Serial.print(" ");
  Serial.print(Nightflight._rms[2]);
  Serial.print(" ");
  Serial.print(Nightflight._rms[3]);
  Serial.print(" ");
  Serial.print(Nightflight._rms[4]);
  Serial.print(" ");
  Serial.print(Nightflight._rms[5]);
  Serial.print(" ");
  Serial.print(Nightflight._rms[6]);
  Serial.println();*/
  equalBrightnessParam0.setValue(Nightflight._rms[0]);
  equalBrightnessParam1.setValue(Nightflight._rms[1]);
  equalBrightnessParam2.setValue(Nightflight._rms[2]);
  equalBrightnessParam3.setValue(Nightflight._rms[3]);
  equalBrightnessParam4.setValue(Nightflight._rms[4]);
  equalBrightnessParam5.setValue(Nightflight._rms[5]);
  equalBrightnessParam6.setValue(Nightflight._rms[6]);
  equalBrightness0.render(vStripRechtsOben, eqSegmentLength*0);
  equalBrightness1.render(vStripRechtsOben, eqSegmentLength*1);
  equalBrightness2.render(vStripRechtsOben, eqSegmentLength*2);
  equalBrightness3.render(vStripRechtsOben, eqSegmentLength*3);
  equalBrightness4.render(vStripRechtsOben, eqSegmentLength*4);
  equalBrightness5.render(vStripRechtsOben, eqSegmentLength*5);
  equalBrightness6.render(vStripRechtsOben, eqSegmentLength*6);
  equalBrightness0.render(vStripLinksOben, eqSegmentLength*0);
  equalBrightness1.render(vStripLinksOben, eqSegmentLength*1);
  equalBrightness2.render(vStripLinksOben, eqSegmentLength*2);
  equalBrightness3.render(vStripLinksOben, eqSegmentLength*3);
  equalBrightness4.render(vStripLinksOben, eqSegmentLength*4);
  equalBrightness5.render(vStripLinksOben, eqSegmentLength*5);
  equalBrightness6.render(vStripLinksOben, eqSegmentLength*6);
  equalBrightness0.render(vStripRechtsUnten, eqSegmentLength*0);
  equalBrightness1.render(vStripRechtsUnten, eqSegmentLength*1);
  equalBrightness2.render(vStripRechtsUnten, eqSegmentLength*2);
  equalBrightness3.render(vStripRechtsUnten, eqSegmentLength*3);
  equalBrightness4.render(vStripRechtsUnten, eqSegmentLength*4);
  equalBrightness5.render(vStripRechtsUnten, eqSegmentLength*5);
  equalBrightness6.render(vStripRechtsUnten, eqSegmentLength*6);
  equalBrightness0.render(vStripLinksUnten, eqSegmentLength*0);
  equalBrightness1.render(vStripLinksUnten, eqSegmentLength*1);
  equalBrightness2.render(vStripLinksUnten, eqSegmentLength*2);
  equalBrightness3.render(vStripLinksUnten, eqSegmentLength*3);
  equalBrightness4.render(vStripLinksUnten, eqSegmentLength*4);
  equalBrightness5.render(vStripLinksUnten, eqSegmentLength*5);
  equalBrightness6.render(vStripLinksUnten, eqSegmentLength*6);
}

uint8_t brightnessByAccelfade = 11;

void brightnessByAccel()
{
    uint8_t currentBrightness = brightnessParam.getValue();
    uint8_t absAccel = (uint8_t)fmap(Nightflight.getAbsoluteAcceleration(), 0.0, 16.0, 0.0, 255.0);

    brightnessParam.setValue(absAccel);
}

boolean _initialized = false;
int _initializerCounter = 50;
void renderTimerInfoMainLoopCallback()
{
  //first evaluate the input from the RC Channel, currently splitting to 6 steps
  setRcChannelValues();
  //then switch programs according to RC Channel value
  uint32_t theValue = rcChannelSteps.getValue();
//  uint32_t theValue2 = rcChannelSteps2.getValue();
/*  Serial.print("RC PWM1 value: ");
  Serial.println(Nightflight.currentChannelData);
  Serial.print("RC PWM2 value: ");
  Serial.println(Nightflight.currentChannelData2);
  Serial.print("RC LED Program Selection: ");
  Serial.println(theValue);*/
  //Serial.print("RC LED Program Selection2: ");
  //Serial.println(theValue2);
  
//  if( _initialized )
  {
    switch(theValue)
    {
      case 7:   
      case 8:    bpmRender = false;
                bpmRenderNext = false;    
                brightnessByAccel();
                fire();
                break;

      case 9:   bpmRender = true;
                bpmRenderNext = false;
                brightnessByAccel();
                riderColor.setValue(CRGB::Red);
                riderResetColor.setValue(tronColor);
                waitStartSequenceRender();
                break;
      case 6:   bpmRender = false;
                bpmRenderNext = false;    
                brightnessParam.setValue(BRIGHTNESS);
                fire();
                break;
      case 5:
      case 4:   bpmRender = true;
                bpmRenderNext = false;    
                //brightnessByAccel();
                riderColor.setValue(CRGB::Red);
                riderResetColor.setValue(tronColor);
                waitStartSequenceRender();
                break;
      case 3:   bpmRender = true;
                bpmRenderNext = false;    
                riderColor.setValue(tronColor);
                riderResetColor.setValue(CRGB::Green);
               // brightnessByAccel();
                brightnessParam.setValue(BRIGHTNESS);
                waitStartSequenceRender();
                break;
      case 2:  bpmRender = true;
                bpmRenderNext = false;
                brightnessParam.setValue(BRIGHTNESS);
                Yaw();
                break;
      case 1:   THX();
                break;
      case 0:   brightnessParam.setValue(255);equalizer();//startBPMTimer();
                break;
      default:  break;
     }
//     if(theValue2 == 1)
     {
      // startBPMTimer();
     }
   }
/*   else
   {
     if(_initializerCounter % 5 == 0)
     {
       //do initStuff
       line1.render(vStripObenRundrum, 0);
         _initializerCounter--;
     }
     else
     {
       if(_initializerCounter <= 0)
       {
         riderResetBlack.render(vStripObenRundrum, 0);
         _initialized = true;
       }
       else
       {
         riderResetBlack.render(vStripObenRundrum, 0);
         _initializerCounter--;
       }
     }
   }*/
   
    if(_keepBrightness > 0)
    {
      _keepBrightness--;
    }
    else
    {
      FastLED.setBrightness( brightnessParam.getValue() );
    }
}




