/**************************************************************************/
/*! 
    @file     ACS7xx_Allegro.h
    @author   SOSAndroid.fr (E. Ha.)
	
    @section  HISTORY

    v1.0 - First release

    Set of methods to use measurements from ACS7xx sensors from Allegro Micro
	All those methods are made to be called periodically
	
    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, SOSAndroid.fr (E. Ha.)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _ACS7XX_ALLEGRO_H_
#define _ACS7XX_ALLEGRO_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

// Enabling debug - comment to disable / normal operations
#define SERIAL_DEBUG

// Default values - default is ACS712-20A
#define ACS7XX_BIDIR_DEFAULT true //Is the sensor bidirectional ?
#define ACS7XX_PIN_DEFAULT	0 //Pin used  for measurements - default A0 from Mega 2650
#define ACS7XX_SENSITIVITY_DEFAULT  0.1 //Sensitivity of the sensor in V/A (divide per 1000 the datasheet value)
#define BOARD_MEASURED_OFFSET 1 //ADC may shift its measurement. For instance a shitfted value of 1 out of 1024 steps represents 0.05A for ACS712. In perfect world, BOARD_MEASURED_OFFSET should be 0
#define BOARD_VOLTAGE_DEFAULT 5.0 //Power supply voltage
#define BOARD_ADC_DEPTH 1024.0 // 10bits: 1024.0, 12bits: 4096.0, 14bits: 16384.0


class ACS7XX_ALLEGRO {
 public:
	ACS7XX_ALLEGRO(void);
	ACS7XX_ALLEGRO(boolean bidir, int pintoread, double voltage, double sensitivity);
	
	void	instantCurrent(double *current);
	void	ampereHourCount(double *amperehc); // calls one shot
	void	resetCounters(void); // reset all counters
	void 	updateMillis(void); // when starting a new cycle, we need to init Lastmillis
	void	updateCounters(void);
	void	getAHCount(double *ahcount);
	void	getCoulombCount(double *ccount);
	void	printDebugDeviceInit(void);
  
 private:
	unsigned long	_lastMillis;
	double	_lastCurrent;
	double	_lastAmperehour;
	double	_lastCoulomb;
	double	_AHCounter;
	double	_CoulombCounter;
	
	boolean	_bidir;
	int		_pintoread;
	double	_voltage;
	double	_voltage_offset;
	double	_sensitivity;
	double	_resolution;
};

#endif