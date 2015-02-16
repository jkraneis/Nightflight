#define NIGHTFLIGHT_INTERNAL
#include "Nightflight.h"

//Instance of CNightflight
CNightflight Nightflight;


//Define standard callbacks

void renderTimerInfoRCInputLoopCallback()
{
    Nightflight.currentChannelData = Nightflight.channel;
    if(Nightflight.isDebug())
    {
		Serial.print("Channel: ");
		Serial.println(Nightflight.currentChannelData);
	}
}

void renderTimerInfoLEDLoopCallback()
{
    FastLED.show(); // display this frame
}

void renderTimerInfoAccelGyroMagnLoopCallback()
{
	LSM9DS0* dof = Nightflight.getLSM9DS0();
	const byte accelPin = Nightflight.getAccelInterruptPin();
	if (digitalRead(accelPin))
	{
		// Use the readAccel() function to get new data from the accel.
		// After calling this function, new values will be stored in
		// the ax, ay, and az variables.
		dof->readAccel();
		double ax = dof->calcAccel(dof->ax);
		double ay = dof->calcAccel(dof->ay);
		double az = dof->calcAccel(dof->az);

		double absAcc = fabs(ax)+fabs(ay)+fabs(az);
		Nightflight.setAbsoluteAcceleration(absAcc);

		if(Nightflight.isDebug())
		{
			Serial.print("A: ");
			// Using the calcAccel helper function, we can get the
			// accelerometer readings in g's.
			Serial.print(ax);
			Serial.print(", ");
			Serial.print(ay);
			Serial.print(", ");
			Serial.println(az);

			Serial.print("AbsolutAcceleration: ");
			Serial.println(absAcc);
		}
	}
}

//RC Input Channel implementations

void RCchannel1() 
{
	// If the pin is HIGH, start a timer
	boolean ledUpdating = Nightflight.isInBlockingLoop();
	if(ledUpdating)
	{
		Nightflight.wasUpdating = true;
		if(Nightflight.isDebug())
		{
			Serial.println("RC Channel read skipped due to blocking LED loop.");
		}
		return;
	}
	int chn = digitalRead(RCCHANNELPIN1);
	if (chn == HIGH) 
	{
		Nightflight.channel_start = micros();
	} 
	else 
	{
		// The pin is now LOW so output the difference
		// between when the timer was started and now
		if(!Nightflight.wasUpdating)
		{
			Nightflight.channel = (uint16_t) (micros() - Nightflight.channel_start);
		}
		else
		{
			Nightflight.wasUpdating = false;
		}
	}
}




CNightflight::CNightflight() 
	: CNightflight(FRAMES_PER_SECOND, RCCHANNELPIN1)
{
}

CNightflight::CNightflight(uint8_t fpsLEDs, uint8_t rcChannelPin) 
	: _debug(false), _fpsLEDs(fpsLEDs), _rcChannelPin(rcChannelPin)
{
	delay(3000); // sanity delay
	Serial.begin(115200);

	//set up pin for RC input
	pinMode(_rcChannelPin, INPUT);

	// Attach an interrupt handler to be called whenever
	// the pin changes from LOW to HIGH or vice versa
	attachInterrupt(_rcChannelPin, RCchannel1, CHANGE);





	renderTimerInfoAccelGyroMagnLoop = &renderTimer.getRenderTimerInfo();
	renderTimerInfoAccelGyroMagnLoop->setCallback(renderTimerInfoAccelGyroMagnLoopCallback);
	renderTimerInfoAccelGyroMagnLoop->setUpdateIntervalMilliSeconds(25);
	renderTimerInfoMainLoop = &renderTimer.getRenderTimerInfo();
	renderTimerInfoMainLoop->setUpdateIntervalMilliSeconds(25);
	renderTimerRCInputLoop = &renderTimer.getRenderTimerInfo();
	renderTimerRCInputLoop->setCallback(renderTimerInfoRCInputLoopCallback);
	renderTimerRCInputLoop->setUpdateIntervalMilliSeconds(25);

	bpmTimerInfoLoop = &renderTimer.getBPMTimerInfo();

	renderTimerInfoLEDLoop = &renderTimer.getRenderTimerInfo();
	renderTimerInfoLEDLoop->setCallback(renderTimerInfoLEDLoopCallback);
	renderTimerInfoLEDLoop->setUpdateIntervalMilliSeconds(1000/_fpsLEDs);
}

void CNightflight::setLSM9DS0(LSM9DS0* dof) 
{
	if(_debug)
	{
		Serial.println("setLSM9DS0 start");
	}
	_dof = dof; 
	_useLSM9DS0 = true; 
	// Set up interrupt pins as inputs:
	pinMode(INT1XM, INPUT);
	pinMode(INT2XM, INPUT);
	pinMode(DRDYG, INPUT);

	// Use the begin() function to initialize the LSM9DS0 library.
	// You can either call it with no parameters (the easy way):
	uint16_t status = _dof->begin(_dof->G_SCALE_2000DPS, _dof->A_SCALE_6G, _dof->M_SCALE_2GS);
	// Or call it with declarations for sensor scales and data rates:  
	//uint16_t status = dof.begin(dof.G_SCALE_2000DPS, dof.A_SCALE_6G, dof.M_SCALE_2GS);

	// begin() returns a 16-bit value which includes both the gyro and
	// accelerometers WHO_AM_I response. You can check this to make sure
	// communication was successful.
	if(_debug)
	{
		Serial.println(status, HEX);
		Serial.println("setLSM9DS0 end");
	}
}


void CNightflight::setMainLoopCallback(RenderTimerFunctionPointer mainLoopCallback)
{
	renderTimerInfoMainLoop->setCallback(mainLoopCallback);
}

void CNightflight::setBPMLoopCallback(RenderTimerFunctionPointer bpmLoopCallback)
{
	bpmTimerInfoLoop->setCallback(bpmLoopCallback);
}

void CNightflight::startBPMTimer()
{
	bpmTimerInfoLoop->start();
}

void CNightflight::stopBPMTimer()
{
	bpmTimerInfoLoop->stop();
}

void CNightflight::setBPM(double bpm)
{
	bpmTimerInfoLoop->setBPM(bpm);
}

boolean CNightflight::isInBlockingLoop()
{
	return renderTimerInfoLEDLoop->getPerformingUpdate();
}

void CNightflight::loop()
{
	//drive the differently timed loops
	renderTimer.update();
}


