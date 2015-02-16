#ifndef __INC_NIGHTFLIGHT_H
#define __INC_NIGHTFLIGHT_H


#define  NIGHTFLIGHT_VERSION 0001000
#ifndef NIGHTFLIGHT_INTERNAL
#warning Nightflight version 0001000  (Not really a warning, just telling you here.)
#endif

#include <stdint.h>
#include <FastLED.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <Wire.h>
#include <SFE_LSM9DS0.h>



#include "LEDHardwareAbstraction.h"
#include "LEDEffect.h"
#include "ParameterChange.h"
#include "Vector.h"
#include "RenderTimer.h"


#define FRAMES_PER_SECOND 40
#define RCCHANNELPIN1 0

// SDO_XM and SDO_G are both grounded, therefore our addresses are:
#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW


class CNightflight 
{


public:
	CNightflight();
	CNightflight(uint8_t fpsLEDs, uint8_t rcChannelPin);
	void loop();

	void setDebug(boolean debug) {_debug = debug;}
	boolean isDebug() {return _debug;}

	void setMainLoopCallback(RenderTimerFunctionPointer mainLoopCallback);

	void setBPMLoopCallback(RenderTimerFunctionPointer bpmLoopCallback);
	void setBPM(double bpm);
	void startBPMTimer();
	void stopBPMTimer();
	double getBPMDrift() {return bpmTimerInfoLoop->getBPMDrift();}
	double getMillisExpected() {return bpmTimerInfoLoop->getMillisExpected();}
	uint64_t getMillisElapsed() {return bpmTimerInfoLoop->getMillisElapsed();}
	uint32_t getBPMTimerUpdateMillis() {return bpmTimerInfoLoop->getUpdateIntervalMilliSeconds();}
	boolean getBeatIsDownbeat() {return bpmTimerInfoLoop->isDownbeat();	}


	void setLSM9DS0(LSM9DS0* dof);
	LSM9DS0* getLSM9DS0() {return _dof;}
	const byte getAccelInterruptPin() const {return INT1XM;}
	double getAbsoluteAcceleration() {return _absoluteAcceleration;}
	void setAbsoluteAcceleration(double absAcc) {_absoluteAcceleration = absAcc;}

	boolean isInBlockingLoop();

//RC Input Channel implementation - should be wrapped by setter and getter functions.
	volatile uint16_t channel;
	uint32_t channel_start;
	uint16_t currentChannelData;
	boolean wasUpdating = false;



private:

	boolean _debug;

	uint8_t _fpsLEDs;
	uint8_t _maxRenderFrequency;

	uint8_t _rcChannelPin;

	double _bpmDrift = 0.0;


	LSM9DS0* _dof; //(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
	double _absoluteAcceleration = 0.0;
	boolean _useLSM9DS0 = false;
	const byte INT1XM = 23; // INT1XM tells us when accel data is ready
	const byte INT2XM = 22; // INT2XM tells us when mag data is ready
	const byte DRDYG = 1;  // DRDYG tells us when gyro data is ready

	RenderTimer renderTimer;
	RenderTimerInfo* renderTimerInfoAccelGyroMagnLoop;
	RenderTimerInfo* renderTimerInfoMainLoop;
	RenderTimerInfo* renderTimerInfoLEDLoop;
	RenderTimerInfo* renderTimerRCInputLoop;
	BPMTimerInfo* bpmTimerInfoLoop;
};






extern CNightflight Nightflight;



#endif
