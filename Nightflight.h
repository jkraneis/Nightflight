#ifndef __INC_NIGHTFLIGHT_H
#define __INC_NIGHTFLIGHT_H


#define  NIGHTFLIGHT_VERSION 0001000
#ifndef NIGHTFLIGHT_INTERNAL
#warning Nightflight version 0001000  (Not really a warning, just telling you here.)
#endif

#include <stdint.h>
#include <FastLED.h>

#define NIGHTFLIGHTUSELSMDM9DF0

class LSM9DS0;

#include "LEDHardwareAbstraction.h"
#include "LEDEffect.h"
#include "ParameterChange.h"
#include "ParameterEffect.h"
#include "Vector.h"
#include "RenderTimer.h"


#define FRAMES_PER_SECOND 40

#define STATUS_LED 13

// This needs to change for input from XBee
#define RCCHANNELPIN1 3
#define RCCHANNELPIN2 11
#define RCCHANNELPIN3 4
#define NIGHTFLIGHT_BASESTATION true


#ifdef NIGHTFLIGHTUSELSMDM9DF0

// See MS5637-02BA03 Low Voltage Barometric Pressure Sensor Data Sheet
#define MS5637_RESET      0x1E
#define MS5637_CONVERT_D1 0x40
#define MS5637_CONVERT_D2 0x50
#define MS5637_ADC_READ   0x00

#define SDO 1
#if SDO
#define LSM9DS0_XM  0x1D // Would be 0x1D if SDO_XM is HIGH
#define LSM9DS0_G   0x6B // Would be 0x6B if SDO_G is HIGH
#define MS5637_ADDRESS 0x76   // Address of altimeter
#else 
#define LSM9DS0_XM  0x1E // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6A // Would be 0x6A if SDO_G is LOW
#define MS5637_ADDRESS 0x76   // Address of altimeter
#endif

#define ADC_256  0x00 // define pressure and temperature conversion rates
#define ADC_512  0x02
#define ADC_1024 0x04
#define ADC_2048 0x06
#define ADC_4096 0x08
#define ADC_8192 0x0A
#define ADC_D1   0x40
#define ADC_D2   0x50

#endif

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
	CNightflight(uint8_t fpsLEDs, uint8_t rcChannelPin, uint8_t rcChannelPin2, uint8_t rcChannelPin3);
	void loop();

	void setDebug(boolean debug) {_debug = debug;}
	boolean isDebug() {return _debug;}

	void setMainLoopCallback(RenderTimerFunctionPointer mainLoopCallback);
	void resetMainLoopCallback();

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
	double getAbsoluteAcceleration() {return _absoluteAcceleration; _numberGyroCalls = 0;}
	void setAbsoluteAcceleration(double absAcc) {_absoluteAcceleration = absAcc;}
	void updateGyroData();

	boolean isInBlockingLoop();



//RC Input Channel implementation - should be wrapped by setter and getter functions.
	volatile uint16_t channel = 1120;
	uint32_t channel_start;
	uint16_t currentChannelData;
	boolean wasUpdating = false;

	volatile uint16_t channel2 = 1120;
	uint32_t channel_start2;
	uint16_t currentChannelData2;
	boolean wasUpdating2 = false;

	volatile uint16_t channel3 = 1120;
	uint32_t channel_start3;
	uint16_t currentChannelData3;
	boolean wasUpdating3 = false;


//9DoF variables for direct access :(

	float pitch, yaw, roll, heading;
	float deltat = 0.0f;        // integration interval for both filter schemes
	uint32_t lastUpdate = 0;    // used to calculate integration interval
	uint32_t Now = 0;           // used to calculate integration interval
	double _absoluteAcceleration = 0.0;

	uint64_t _numberGyroCalls = 0;
	uint64_t _startTime = 0;
	bool _initialized = false;

	float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values 
	float q[4] = {1.0f, 0.0f, 0.0f, 0.0f};    // vector to hold quaternion
	float eInt[3] = {0.0f, 0.0f, 0.0f};       // vector to hold integral error for Mahony method
	float temperature;
	float abias[3] = {0, 0, 0}, gbias[3] = {0, 0, 0};


#ifdef NIGHTFLIGHTUSELSMDM9DF0
	// Specify sensor full scale
	uint8_t OSR = ADC_8192;     // set pressure amd temperature oversample rate
	uint16_t Pcal[8];         // calibration constants from MS5637 PROM registers
	unsigned char nCRC;       // calculated check sum to ensure PROM integrity
	uint32_t D1 = 0, D2 = 0;  // raw MS5637 pressure and temperature data
	double dT, OFFSET, SENS, T2, OFFSET2, SENS2, GLOBALALTOFFSET;  // First order and second order corrections for raw S5637 temperature and pressure data
	double Temperature, Pressure; // stores MS5637 pressures sensor pressure and temperature
	float Altitude;
	bool _readTempNext = true;	
#endif


//Base Station data retrieval
	uint8_t _rms[7] = {0, 0, 0, 0, 0, 0, 0};


	boolean _logHeaderWritten;
	boolean _statusLEDStatus = true;

private:

	boolean _debug;


	uint8_t _fpsLEDs;
	uint8_t _maxRenderFrequency;

	uint8_t _rcChannelPin;
	uint8_t _rcChannelPin2;
	uint8_t _rcChannelPin3;

	double _bpmDrift = 0.0;


	LSM9DS0* _dof; //(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);
	boolean _useLSM9DS0 = false;
	//LSM9DS0 PIN Declarations
	const byte INT1XM = 27; // INT1XM tells us when accel data is ready
	const byte INT2XM = 32; // INT2XM tells us when mag data is ready
	const byte DRDYG = 24;  // DRDYG tells us when gyro data is ready
	const byte SDOpin = 31;  // selects either of two I2C addresses


	RenderTimer renderTimer;
	RenderTimerInfo* renderTimerInfoGyroUpdateLoop;
	RenderTimerInfo* renderTimerInfoAltitudeUpdateLoop;
	RenderTimerInfo* renderTimerInfoJlogLoop;
	RenderTimerInfo* renderTimerInfoBaseStationDataLoop;
	RenderTimerInfo* renderTimerInfoMainLoop;
	RenderTimerInfo* renderTimerInfoLEDLoop;
	RenderTimerInfo* renderTimerRCInputLoop;
	BPMTimerInfo* bpmTimerInfoLoop;


	RenderTimerFunctionPointer _mainBackupFunctionPointer = NULL;
};






extern CNightflight Nightflight;



#endif
