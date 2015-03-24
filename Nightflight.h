#ifndef __INC_NIGHTFLIGHT_H
#define __INC_NIGHTFLIGHT_H


#define  NIGHTFLIGHT_VERSION 0001000
#ifndef NIGHTFLIGHT_INTERNAL
#warning Nightflight version 0001000  (Not really a warning, just telling you here.)
#endif

#include <stdint.h>
#include <FastLED.h>
#include <SPI.h> // Included for SFE_LSM9DS0 library
#include <i2c_t3.h>
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


// global constants for 9 DoF fusion and AHRS (Attitude and Heading Reference System)
#define GyroMeasError PI * (40.0f / 180.0f)       // gyroscope measurement error in rads/s (shown as 3 deg/s)
#define GyroMeasDrift PI * (0.0f / 180.0f)      // gyroscope measurement drift in rad/s/s (shown as 0.0 deg/s/s)
// There is a tradeoff in the beta parameter between accuracy and response speed.
// In the original Madgwick study, beta of 0.041 (corresponding to GyroMeasError of 2.7 degrees/s) was found to give optimal accuracy.
// However, with this value, the LSM9SD0 response time is about 10 seconds to a stable initial quaternion.
// Subsequent changes also require a longish lag time to a stable output, not fast enough for a quadcopter or robot car!
// By increasing beta (GyroMeasError) by about a factor of fifteen, the response time constant is reduced to ~2 sec
// I haven't noticed any reduction in solution accuracy. This is essentially the I coefficient in a PID control sense; 
// the bigger the feedback coefficient, the faster the solution converges, usually at the expense of accuracy. 
// In any case, this is the free parameter in the Madgwick filtering and fusion scheme.
#define beta sqrt(3.0f / 4.0f) * GyroMeasError   // compute beta
#define zeta sqrt(3.0f / 4.0f) * GyroMeasDrift   // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value
#define Kp 2.0f * 5.0f // these are the free parameters in the Mahony filter and fusion scheme, Kp for proportional feedback, Ki for integral
#define Ki 0.0f




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
	const byte getGyroInterruptPin() const {return DRDYG;}
	const byte getMaglInterruptPin() const {return INT2XM;}
	double getAbsoluteAcceleration() {return _absoluteAcceleration;}
	void setAbsoluteAcceleration(double absAcc) {_absoluteAcceleration = absAcc;}
	void updateGyroData();

	boolean isInBlockingLoop();

//RC Input Channel implementation - should be wrapped by setter and getter functions.
	volatile uint16_t channel = 1120;
	uint32_t channel_start;
	uint16_t currentChannelData;
	boolean wasUpdating = false;


//9DoF variables for direct access :(

	float pitch, yaw, roll, heading;
	float deltat = 0.0f;        // integration interval for both filter schemes
	uint32_t lastUpdate = 0;    // used to calculate integration interval
	uint32_t Now = 0;           // used to calculate integration interval

	float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values 
	float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};    // vector to hold quaternion
	float eInt[3] = {0.0f, 0.0f, 0.0f};       // vector to hold integral error for Mahony method
	float temperature;
	float abias[3] = {0, 0, 0}, gbias[3] = {0, 0, 0};


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
	RenderTimerInfo* renderTimerInfoGyroUpdateLoop;
	RenderTimerInfo* renderTimerInfoAccelGyroMagnLoop;
	RenderTimerInfo* renderTimerInfoMainLoop;
	RenderTimerInfo* renderTimerInfoLEDLoop;
	RenderTimerInfo* renderTimerRCInputLoop;
	BPMTimerInfo* bpmTimerInfoLoop;
};






extern CNightflight Nightflight;



#endif
