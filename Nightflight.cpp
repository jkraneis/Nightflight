#define NIGHTFLIGHT_INTERNAL
#include "Nightflight.h"

/******************************************************************************
Nightflight.h
Nightflight Library Header File
Jan Kraneis 

This file prototypes the Nightflight class, implemented in Nightflight.cpp.

Development environment specifics:
    IDE: Arduino 1.0.5
    Hardware Platform: Teensy 3.1

9DoF implementation taken from kriswiners implementation from Sparkfun.
https://github.com/kriswiner/LSM9DS0

This code is beerware; if you see me at the local, 
and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

//Forward declaration for LSM9DS0 class - if not used
class LSM9DS0;
// includes for the LSM9DS0 library
#ifdef NIGHTFLIGHTUSELSMDM9DF0
#include <SPI.h>
#include <i2c_t3.h>
#include <SFE_LSM9DS0.h>
#else
//Forward declaration for LSM9DS0 class - if not used
class LSM9DS0;
#endif

#define THESERIAL Serial1

//Instance of CNightflight
CNightflight Nightflight;


//Define standard callbacks

void renderTimerInfoRCInputLoopCallback()
{
    Nightflight.currentChannelData = Nightflight.channel;
    Nightflight.currentChannelData2 = Nightflight.channel2;
    if(Nightflight.isDebug())
    {
		Serial.print("Channel: ");
		Serial.println(Nightflight.currentChannelData);
	}
}

void renderTimerInfoLEDLoopCallback()
{
    digitalWrite(STATUS_LED, Nightflight._statusLEDStatus);
    Nightflight._statusLEDStatus = !Nightflight._statusLEDStatus;
    FastLED.show();
}

void initializerCallback()
{

}

void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
void MS5637Reset();       
void MS5637PromRead(uint16_t * destination);
uint32_t MS5637Read(uint8_t CMD, uint8_t OSR);
unsigned char MS5637checkCRC(uint16_t * n_prom);



void renderTimerInfoGyroUpdateLoopCallback()
{
#ifdef NIGHTFLIGHTUSELSMDM9DF0
	LSM9DS0* dof = Nightflight.getLSM9DS0();
	
	const byte INT1XM = Nightflight.getAccelInterruptPin();
	const byte DRDYG = Nightflight.getGyroInterruptPin();
	const byte INT2XM = Nightflight.getMaglInterruptPin();

 if(digitalRead(DRDYG)) {  // When new gyro data is ready
  	dof->readGyro();           // Read raw gyro data
    Nightflight.gx = dof->calcGyro(dof->gx) - Nightflight.gbias[0];   // Convert to degrees per seconds, remove gyro biases
    Nightflight.gy = dof->calcGyro(dof->gy) - Nightflight.gbias[1];
    Nightflight.gz = dof->calcGyro(dof->gz) - Nightflight.gbias[2];
  }
  
  if(digitalRead(INT1XM)) {  // When new accelerometer data is ready
    dof->readAccel();         // Read raw accelerometer data
    double oldAcc = Nightflight._absoluteAcceleration * Nightflight._numberGyroCalls;
    Nightflight.ax = dof->calcAccel(dof->ax) - Nightflight.abias[0];   // Convert to g's, remove accelerometer biases
    Nightflight.ay = dof->calcAccel(dof->ay) - Nightflight.abias[1];
    Nightflight.az = dof->calcAccel(dof->az) - Nightflight.abias[2];

	double absAcc = (fabs(Nightflight.ax)+fabs(Nightflight.ay)+fabs(Nightflight.az));
    double absAccCalc = (oldAcc + absAcc) / (Nightflight._numberGyroCalls + 1);
	Nightflight.setAbsoluteAcceleration(absAccCalc);
  }
  
  if(digitalRead(INT2XM)) {  // When new magnetometer data is ready
    dof->readMag();           // Read raw magnetometer data
    Nightflight.mx = dof->calcMag(dof->mx);     // Convert to Gauss and correct for calibration
    Nightflight.my = dof->calcMag(dof->my);
    Nightflight.mz = dof->calcMag(dof->mz);
    
    dof->readTemp();
    Nightflight.temperature = 21.0 + (float) dof->temperature/8.; // slope is 8 LSB per degree C, just guessing at the intercept
  }

  Nightflight.Now = micros();
  Nightflight.deltat = ((Nightflight.Now - Nightflight.lastUpdate)/1000000.0f); // set integration time by time elapsed since last filter update
  Nightflight.lastUpdate = Nightflight.Now;
  // Sensors x- and y-axes are aligned but magnetometer z-axis (+ down) is opposite to z-axis (+ up) of accelerometer and gyro!
  // This is ok by aircraft orientation standards!  
  // Pass gyro rate as rad/s
   MadgwickQuaternionUpdate(Nightflight.ax, Nightflight.ay, Nightflight.az, Nightflight.gx*PI/180.0f, Nightflight.gy*PI/180.0f, Nightflight.gz*PI/180.0f, Nightflight.mx, Nightflight.my, Nightflight.mz);
 //MahonyQuaternionUpdate(Nightflight.ax, Nightflight.ay, Nightflight.az, Nightflight.gx*PI/180.0f, Nightflight.gy*PI/180.0f, Nightflight.gz*PI/180.0f, Nightflight.mx, Nightflight.my, Nightflight.mz);
 
 Nightflight.updateGyroData();
#endif 
}

void renderTimerInfoAltitudeCallback()
{
/*    D1 = MS5637Read(ADC_D1, OSR);  // get raw pressure value
    D2 = MS5637Read(ADC_D2, OSR);  // get raw temperature value
    dT = D2 - Pcal[5]*pow(2,8);    // calculate temperature difference from reference
    OFFSET = Pcal[2]*pow(2, 17) + dT*Pcal[4]/pow(2,6);
    SENS = Pcal[1]*pow(2,16) + dT*Pcal[3]/pow(2,7);
 
    Temperature = (2000 + (dT*Pcal[6])/pow(2, 23))/100;           // First-order Temperature in degrees Centigrade
//
// Second order corrections
    if(Temperature > 20) 
    {
      T2 = 5*dT*dT/pow(2, 38); // correction for high temperatures
      OFFSET2 = 0;
      SENS2 = 0;
    }
    if(Temperature < 20)                   // correction for low temperature
    {
      T2      = 3*dT*dT/pow(2, 33); 
      OFFSET2 = 61*(Temperature - 2000)*(Temperature - 2000)/16;
      SENS2   = 29*(Temperature - 2000)*(Temperature - 2000)/16;
    } 
    if(Temperature < -15)                      // correction for very low temperature
    {
      OFFSET2 = OFFSET2 + 17*(Temperature + 1500)*(Temperature + 1500);
      SENS2 = SENS2 + 9*(Temperature + 1500)*(Temperature + 1500);
    }
 // End of second order corrections
 //
     Temperature = Temperature - T2;
     OFFSET = OFFSET - OFFSET2;
     SENS = SENS - SENS2;
 
     Pressure = (((D1*SENS)/pow(2, 21) - OFFSET)/pow(2, 15))/100;  // Pressure in mbar or kPa
  
    const int station_elevation_m = 1050.0*0.3048; // Accurate for the roof on my house; convert from feet to meters

    float baroin = Pressure; // pressure is now in millibars

    // Formula to correct absolute pressure in millbars to "altimeter pressure" in inches of mercury 
    // comparable to weather report pressure
    float part1 = baroin - 0.3; //Part 1 of formula
    const float part2 = 0.0000842288;
    float part3 = pow(part1, 0.190284);
    float part4 = (float)station_elevation_m / part3;
    float part5 = (1.0 + (part2 * part4));
    float part6 = pow(part5, 5.2553026);
    float altimeter_setting_pressure_mb = part1 * part6; // Output is now in adjusted millibars
    baroin = altimeter_setting_pressure_mb * 0.02953;

    float altitude = 145366.45*(1. - pow((Pressure/1013.25), 0.190284));
   
    Serial.print("Digital pressure value = "); Serial.print((float) Pressure, 2);  Serial.println(" mbar");// pressure in millibar
    Serial.print("Altitude = "); Serial.print(altitude, 2); Serial.println(" feet");
*/
}

void renderTimerInfoJlogLoopCallback()
{
    if(!Nightflight._initialized || Nightflight.isInBlockingLoop())
    {
        return;
    }
    if(!Nightflight._logHeaderWritten)
    {
        THESERIAL.print("$N$;My Data\r\n");
        THESERIAL.print("$C$;AccX [G];AccY [G];AccZ [G];GyX [DPS];GyY [DPS];GyZ [DPS];Pitch [Deg];Yaw [Deg]; Roll [Deg];AbsAcc [G]\r\n");
        THESERIAL.print("$I$;12\r\n");
 
        Nightflight._logHeaderWritten = true;
    }
    //Datenausgabe LogView kompatibel 
    THESERIAL.print("$");
    THESERIAL.print(Nightflight.ax);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.ay);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.az);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.gx);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.gy);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.gz);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.pitch);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.yaw);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight.roll);
    THESERIAL.print(";");
    THESERIAL.print(Nightflight._absoluteAcceleration);
    THESERIAL.print("\r\n");
}


void renderTimerInfoBaseStationDataLoopCallback()
{
  if (Serial1.available() >= 7)//  && (millis() - startTime) < 1000/60 ) 
  {
    Nightflight._rms[0] = Serial1.read();
    Nightflight._rms[1] = Serial1.read();
    Nightflight._rms[2] = Serial1.read();
    Nightflight._rms[3] = Serial1.read();
    Nightflight._rms[4] = Serial1.read();
    Nightflight._rms[5] = Serial1.read();
    Nightflight._rms[6] = Serial1.read();
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

//RC Input Channel implementations

void RCchannel2() 
{
    // If the pin is HIGH, start a timer
    boolean ledUpdating = Nightflight.isInBlockingLoop();
    if(ledUpdating)
    {
        Nightflight.wasUpdating2 = true;
        if(Nightflight.isDebug())
        {
            Serial.println("RC Channel read skipped due to blocking LED loop.");
        }
        return;
    }
    int chn = digitalRead(RCCHANNELPIN2);
    if (chn == HIGH) 
    {
        Nightflight.channel_start2 = micros();
    } 
    else 
    {
        // The pin is now LOW so output the difference
        // between when the timer was started and now
        if(!Nightflight.wasUpdating2)
        {
            Nightflight.channel2 = (uint16_t) (micros() - Nightflight.channel_start2);
        }
        else
        {
            Nightflight.wasUpdating2 = false;
        }
    }
}




CNightflight::CNightflight() 
	: CNightflight(FRAMES_PER_SECOND, RCCHANNELPIN1, RCCHANNELPIN2)
{
}

CNightflight::CNightflight(uint8_t fpsLEDs, uint8_t rcChannelPin, uint8_t rcChannelPin2) 
	: _logHeaderWritten(false), _debug(false), _fpsLEDs(fpsLEDs), _rcChannelPin(rcChannelPin), _rcChannelPin2(rcChannelPin2)
{
    _startTime = millis();
	//delay(3000); // sanity delay
	Serial.begin(115200);
    Serial1.begin(57600);

	//set up pin for RC input
    pinMode(_rcChannelPin, INPUT);
    pinMode(_rcChannelPin2, INPUT);

    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, _statusLEDStatus);
    _statusLEDStatus = !_statusLEDStatus;
	// Attach an interrupt handler to be called whenever
	// the pin changes from LOW to HIGH or vice versa
    attachInterrupt(_rcChannelPin, RCchannel1, CHANGE);
    attachInterrupt(_rcChannelPin2, RCchannel2, CHANGE);



	//update always
	renderTimerInfoGyroUpdateLoop = &renderTimer.getRenderTimerInfo();
	renderTimerInfoGyroUpdateLoop->setCallback(renderTimerInfoGyroUpdateLoopCallback);
	renderTimerInfoGyroUpdateLoop->setUpdateIntervalMilliSeconds(0);

    renderTimerInfoJlogLoop = &renderTimer.getRenderTimerInfo();
    renderTimerInfoJlogLoop->setCallback(renderTimerInfoJlogLoopCallback);
    renderTimerInfoJlogLoop->setUpdateIntervalMilliSeconds(12);

	renderTimerInfoBaseStationDataLoop = &renderTimer.getRenderTimerInfo();
	renderTimerInfoBaseStationDataLoop->setCallback(renderTimerInfoBaseStationDataLoopCallback);
	renderTimerInfoBaseStationDataLoop->setUpdateIntervalMilliSeconds(1000/_fpsLEDs);

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
#ifdef NIGHTFLIGHTUSELSMDM9DF0
    digitalWrite(STATUS_LED, _statusLEDStatus);
    _statusLEDStatus = !_statusLEDStatus;

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
    pinMode(SDOpin,   OUTPUT);
    digitalWrite(SDOpin, SDO);	// Use the begin() function to initialize the LSM9DS0 library.
	// You can either call it with no parameters (the easy way):
	uint32_t status = _dof->begin(_dof->G_SCALE_500DPS, _dof->A_SCALE_16G, _dof->M_SCALE_8GS);
	// Or call it with declarations for sensor scales and data rates:  

	if(_debug)
	{
		// begin() returns a 16-bit value which includes both the gyro and
		// accelerometers WHO_AM_I response. You can check this to make sure
		// communication was successful.
		Serial.println(status, HEX);
		Serial.println("setLSM9DS0 end");
	}
    digitalWrite(STATUS_LED, _statusLEDStatus);
    _statusLEDStatus = !_statusLEDStatus;
    delay(5000);
    digitalWrite(STATUS_LED, _statusLEDStatus);
    _statusLEDStatus = !_statusLEDStatus;
	// Set output data rates  
	// Accelerometer output data rate (ODR) can be: A_ODR_3125 (3.225 Hz), A_ODR_625 (6.25 Hz), A_ODR_125 (12.5 Hz), A_ODR_25, A_ODR_50, 
	//                                              A_ODR_100,  A_ODR_200, A_ODR_400, A_ODR_800, A_ODR_1600 (1600 Hz)
	_dof->setAccelODR(_dof->A_ODR_100); // Set accelerometer update rate at 100 Hz
	// Accelerometer anti-aliasing filter rate can be 50, 194, 362, or 763 Hz
	// Anti-aliasing acts like a low-pass filter allowing oversampling of accelerometer and rejection of high-frequency spurious noise.
	// Strategy here is to effectively oversample accelerometer at 100 Hz and use a 50 Hz anti-aliasing (low-pass) filter frequency
	// to get a smooth ~150 Hz filter update rate
	//TODO!
	_dof->setAccelABW(_dof->A_ABW_50); // Choose lowest filter setting for low noise

	// Gyro output data rates can be: 95 Hz (bandwidth 12.5 or 25 Hz), 190 Hz (bandwidth 12.5, 25, 50, or 70 Hz)
	//                                 380 Hz (bandwidth 20, 25, 50, 100 Hz), or 760 Hz (bandwidth 30, 35, 50, 100 Hz)
	_dof->setGyroODR(_dof->G_ODR_190_BW_125);  // Set gyro update rate to 190 Hz with the smallest bandwidth for low noise

	// Magnetometer output data rate can be: 3.125 (ODR_3125), 6.25 (ODR_625), 12.5 (ODR_125), 25, 50, or 100 Hz
	_dof->setMagODR(_dof->M_ODR_625); // Set magnetometer to update every 80 ms
	// Use the FIFO mode to average accelerometer and gyro readings to calculate the biases, which can then be removed from
	// all subsequent measurements.
//    delay(2000);
	_dof->calLSM9DS0(gbias, abias);

  // Reset the MS5637 pressure sensor
     digitalWrite(STATUS_LED, _statusLEDStatus);
    _statusLEDStatus = !_statusLEDStatus;
   // MS5637Reset();
    delay(100);
    digitalWrite(STATUS_LED, _statusLEDStatus);
    _statusLEDStatus = !_statusLEDStatus;
    /*Serial.println("MS5637 pressure sensor reset...");
    // Read PROM data from MS5637 pressure sensor
    MS5637PromRead(Pcal);
    Serial.println("PROM data read:");
    Serial.print("C0 = "); Serial.println(Pcal[0]);
    unsigned char refCRC = Pcal[0] >> 12;
    Serial.print("C1 = "); Serial.println(Pcal[1]);
    Serial.print("C2 = "); Serial.println(Pcal[2]);
    Serial.print("C3 = "); Serial.println(Pcal[3]);
    Serial.print("C4 = "); Serial.println(Pcal[4]);
    Serial.print("C5 = "); Serial.println(Pcal[5]);
    Serial.print("C6 = "); Serial.println(Pcal[6]);

    nCRC = MS5637checkCRC(Pcal);  //calculate checksum to ensure integrity of MS5637 calibration data
    Serial.print("Checksum = "); Serial.print(nCRC); Serial.print(" , should be "); Serial.println(refCRC);  
  */
    digitalWrite(STATUS_LED, _statusLEDStatus);
    _statusLEDStatus = !_statusLEDStatus;
    delay(1000);  
#endif
}

void CNightflight::updateGyroData()
{
    yaw   = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);   
    pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
    roll  = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
    pitch *= 180.0f / PI;
    yaw   *= 180.0f / PI; 
    yaw   += 1.9; // Declination at Osnabrück, Germany
    roll  *= 180.0f / PI;

}

void CNightflight::setMainLoopCallback(RenderTimerFunctionPointer mainLoopCallback)
{
    _mainBackupFunctionPointer = mainLoopCallback;
    renderTimerInfoMainLoop->setCallback(initializerCallback);
}

void CNightflight::resetMainLoopCallback()
{
    renderTimerInfoMainLoop->setCallback(_mainBackupFunctionPointer);
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
    uint64_t diff = millis() - _startTime;
    if(!_initialized && diff > 15000)
    {
        _initialized = true;
        Nightflight.resetMainLoopCallback();
    }
	//drive the differently timed loops
	renderTimer.update();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//Useful functions
////////////////////////////////////////////////////////////////////////////////////////////////////
// I2C communication with the MS5637 is a little different from that with the MPU9250 and most other sensors
// For the MS5637, we write commands, and the MS5637 sends data in response, rather than directly reading
// MS5637 registers

void MS5637Reset()
{
#ifdef NIGHTFLIGHTUSELSMDM9DF0
    Wire1.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
    Wire1.write(MS5637_RESET);                // Put reset command in Tx buffer
    Wire1.endTransmission();                  // Send the Tx buffer
#endif
}

void MS5637PromRead(uint16_t * destination)
{
#ifdef NIGHTFLIGHTUSELSMDM9DF0
    uint8_t data[2] = {0,0};
    for (uint8_t ii = 0; ii < 7; ii++) 
    {
        Wire1.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
        Wire1.write(0xA0 | ii << 1);              // Put PROM address in Tx buffer
        Wire1.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
        uint8_t i = 0;
        Wire1.requestFrom(MS5637_ADDRESS, 2);   // Read two bytes from slave PROM address 
        while ( Wire1.available()) 
        {
            data[i++] =  Wire1.read(); 
        }               // Put read results in the Rx buffer
        destination[ii] = (uint16_t) (((uint16_t) data[0] << 8) | data[1]); // construct PROM data for return to main program
    }
#endif
}

uint32_t MS5637Read(uint8_t CMD, uint8_t OSR)  // temperature data read
{
#ifdef NIGHTFLIGHTUSELSMDM9DF0
    Wire1.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
    Wire1.write(CMD | OSR);                  // Put pressure conversion command in Tx buffer
    Wire1.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive

    switch (OSR)
    {
        case ADC_256: delay(1); break;  // delay for conversion to complete
        case ADC_512: delay(3); break;
        case ADC_1024: delay(4); break;
        case ADC_2048: delay(6); break;
        case ADC_4096: delay(10); break;
        case ADC_8192: delay(20); break;
    }

    uint8_t data[3] = {0,0,0};
    Wire1.beginTransmission(MS5637_ADDRESS);  // Initialize the Tx buffer
    Wire1.write(0x00);                        // Put ADC read command in Tx buffer
    Wire1.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
    uint8_t i = 0;
    Wire1.requestFrom(MS5637_ADDRESS, 3);     // Read three bytes from slave PROM address 
    while ( Wire1.available()) 
    {
        data[i++] =  Wire1.read(); 
    }               // Put read results in the Rx buffer
    return (uint32_t) (((uint32_t) data[0] << 16) | (uint32_t) data[1] << 8 | data[2]); // construct PROM data for return to main program
#endif
}



unsigned char MS5637checkCRC(uint16_t * n_prom)  // calculate checksum from PROM register contents
{
  int cnt;
  unsigned int n_rem = 0;
  unsigned char n_bit;
  
  n_prom[0] = ((n_prom[0]) & 0x0FFF);  // replace CRC byte by 0 for checksum calculation
  n_prom[7] = 0;
  for(cnt = 0; cnt < 16; cnt++)
  {
    if(cnt%2==1) n_rem ^= (unsigned short) ((n_prom[cnt>>1]) & 0x00FF);
    else         n_rem ^= (unsigned short)  (n_prom[cnt>>1]>>8);
    for(n_bit = 8; n_bit > 0; n_bit--)
    {
        if(n_rem & 0x8000)    n_rem = (n_rem<<1) ^ 0x3000;
        else                  n_rem = (n_rem<<1);
    }
  }
  n_rem = ((n_rem>>12) & 0x000F);
  return (n_rem ^ 0x00);
}


// Implementation of Sebastian Madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
// (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
// which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
// device orientation -- which can be converted to yaw, pitch, and roll. Useful for stabilizing quadcopters, etc.
// The performance of the orientation filter is at least as good as conventional Kalman-based filtering algorithms
// but is much less computationally intensive---it can be performed on a 3.3 V Pro Mini operating at 8 MHz!
void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
    float q1 = Nightflight.q[0], q2 = Nightflight.q[1], q3 = Nightflight.q[2], q4 = Nightflight.q[3];   // short name local variable for readability
    float norm;
    float hx, hy, _2bx, _2bz;
    float s1, s2, s3, s4;
    float qDot1, qDot2, qDot3, qDot4;

    // Auxiliary variables to avoid repeated arithmetic
    float _2q1mx;
    float _2q1my;
    float _2q1mz;
    float _2q2mx;
    float _4bx;
    float _4bz;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q4 = 2.0f * q4;
    float _2q1q3 = 2.0f * q1 * q3;
    float _2q3q4 = 2.0f * q3 * q4;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;

    // Normalise accelerometer measurement
    norm = sqrt(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Normalise magnetometer measurement
    norm = sqrt(mx * mx + my * my + mz * mz);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    mx *= norm;
    my *= norm;
    mz *= norm;

    // Reference direction of Earth's magnetic field
    _2q1mx = 2.0f * q1 * mx;
    _2q1my = 2.0f * q1 * my;
    _2q1mz = 2.0f * q1 * mz;
    _2q2mx = 2.0f * q2 * mx;
    hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
    hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
    _2bx = sqrt(hx * hx + hy * hy);
    _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
    _4bx = 2.0f * _2bx;
    _4bz = 2.0f * _2bz;

    // Gradient decent algorithm corrective step
    s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
    norm = 1.0f/norm;
    s1 *= norm;
    s2 *= norm;
    s3 *= norm;
    s4 *= norm;

    // Compute rate of change of quaternion
    qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
    qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
    qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
    qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

    // Integrate to yield quaternion
    q1 += qDot1 * Nightflight.deltat;
    q2 += qDot2 * Nightflight.deltat;
    q3 += qDot3 * Nightflight.deltat;
    q4 += qDot4 * Nightflight.deltat;
    norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
    norm = 1.0f/norm;
    Nightflight.q[0] = q1 * norm;
    Nightflight.q[1] = q2 * norm;
    Nightflight.q[2] = q3 * norm;
    Nightflight.q[3] = q4 * norm;

}



// Similar to Madgwick scheme but uses proportional and integral filtering on the error between estimated reference vectors and
// measured ones. 
void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
    float q1 = Nightflight.q[0], q2 = Nightflight.q[1], q3 = Nightflight.q[2], q4 = Nightflight.q[3];   // short name local variable for readability
    float norm;
    float hx, hy, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;
    float pa, pb, pc;

    // Auxiliary variables to avoid repeated arithmetic
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;   

    // Normalise accelerometer measurement
    norm = sqrt(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f / norm;        // use reciprocal for division
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Normalise magnetometer measurement
    norm = sqrt(mx * mx + my * my + mz * mz);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f / norm;        // use reciprocal for division
    mx *= norm;
    my *= norm;
    mz *= norm;

    // Reference direction of Earth's magnetic field
    hx = 2.0f * mx * (0.5f - q3q3 - q4q4) + 2.0f * my * (q2q3 - q1q4) + 2.0f * mz * (q2q4 + q1q3);
    hy = 2.0f * mx * (q2q3 + q1q4) + 2.0f * my * (0.5f - q2q2 - q4q4) + 2.0f * mz * (q3q4 - q1q2);
    bx = sqrt((hx * hx) + (hy * hy));
    bz = 2.0f * mx * (q2q4 - q1q3) + 2.0f * my * (q3q4 + q1q2) + 2.0f * mz * (0.5f - q2q2 - q3q3);

    // Estimated direction of gravity and magnetic field
    vx = 2.0f * (q2q4 - q1q3);
    vy = 2.0f * (q1q2 + q3q4);
    vz = q1q1 - q2q2 - q3q3 + q4q4;
    wx = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
    wy = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
    wz = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);  

    // Error is cross product between estimated direction and measured direction of gravity
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
    if (Ki > 0.0f)
    {
        Nightflight.eInt[0] += ex;      // accumulate integral error
        Nightflight.eInt[1] += ey;
        Nightflight.eInt[2] += ez;
    }
    else
    {
        Nightflight.eInt[0] = 0.0f;     // prevent integral wind up
        Nightflight.eInt[1] = 0.0f;
        Nightflight.eInt[2] = 0.0f;
    }

    // Apply feedback terms
    gx = gx + Kp * ex + Ki * Nightflight.eInt[0];
    gy = gy + Kp * ey + Ki * Nightflight.eInt[1];
    gz = gz + Kp * ez + Ki * Nightflight.eInt[2];

    // Integrate rate of change of quaternion
    pa = q2;
    pb = q3;
    pc = q4;
    q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5f * Nightflight.deltat);
    q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5f * Nightflight.deltat);
    q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5f * Nightflight.deltat);
    q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5f * Nightflight.deltat);

    // Normalise quaternion
    norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
    norm = 1.0f / norm;
    Nightflight.q[0] = q1 * norm;
    Nightflight.q[1] = q2 * norm;
    Nightflight.q[2] = q3 * norm;
    Nightflight.q[3] = q4 * norm;

}

