/**************************************************************************/
/*!
    @file     CurrentIC_ACS7xx_Allegro.cpp
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Set of methods to use measurements from ACS7xx sensors from Allegro Micro
	All those methods are made to be called periodically

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/

#include <stdlib.h>
#include "ACS7xx_Allegro.h"

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
ACS7XX_ALLEGRO::ACS7XX_ALLEGRO(void) 
{
	ACS7XX_ALLEGRO::ACS7XX_ALLEGRO(ACS7XX_BIDIR_DEFAULT, ACS7XX_PIN_DEFAULT, BOARD_VOLTAGE_DEFAULT, ACS7XX_SENSITIVITY_DEFAULT);
}

ACS7XX_ALLEGRO::ACS7XX_ALLEGRO(boolean bidir, int pintoread, double voltage, double sensitivity) 
{
	_bidir = bidir;
	_pintoread = pintoread;
	_sensitivity = sensitivity;
	_voltage = voltage;
	if (_bidir) {
		_voltage_offset = voltage / 2.0;
	} 
	else {
		_voltage_offset = 0.0;
	}
	_resolution = (double) BOARD_ADC_DEPTH;
	
	_lastMillis = 0; //need to add a function to start the counting by initializing lastmillis when starting a discharge / charge cycle
	_lastCurrent = 0.0;
	_AHCounter = 0.0;
	_CoulombCounter = 0.0;

}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

void ACS7XX_ALLEGRO::instantCurrent(double *current)
{
	int readvalue = analogRead(ACS7XX_ALLEGRO::_pintoread);
	double readvolt = (((double) readvalue / _resolution) * _voltage) - _voltage_offset;
	double readcur = readvolt / _sensitivity;
	*current = readcur;
	_lastCurrent = readcur;
	
	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.print("Current: ");
			Serial.print(readcur, DEC);
			Serial.println(" A");
		}
    #endif
	
	return;
}

void ACS7XX_ALLEGRO::ampereHourCount(double *amperehc)
{
	unsigned long currentmillis = millis();
	double timeframehour = (double)(currentmillis - _lastMillis) / 3600000.0;
	
	double readcurrent;
	ACS7XX_ALLEGRO::instantCurrent(&readcurrent);
	*amperehc = readcurrent * timeframehour;
	
	_lastMillis = currentmillis;

	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.print("AmpHour: ");
			Serial.print(*amperehc, DEC);
			Serial.println(" Ah");
			Serial.print("timeframe ");
			Serial.print(timeframehour, DEC);
			Serial.println(" hour");
		}
    #endif
	
	return;
}

void ACS7XX_ALLEGRO::updateCounters(void)
{
	double amperehcTemp;
	ACS7XX_ALLEGRO::ampereHourCount(&amperehcTemp);
	_lastAmperehour = amperehcTemp;
	_AHCounter += amperehcTemp;
	
	_lastCoulomb = amperehcTemp / 3600.0;
	_CoulombCounter += _lastCoulomb;

	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.println("Counters updated");
		}
    #endif
	
	return;
}

void ACS7XX_ALLEGRO::resetCounters(void)
{
	_lastAmperehour = 0.0;
	_AHCounter = 0.0;
	_lastCoulomb = 0.0;
	_CoulombCounter = 0.0;

	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.println("Counters reseted");
		}
    #endif	
	
	return;
}

void ACS7XX_ALLEGRO::updateMillis(void)
{
	_lastMillis = millis();

	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.println("Millis updated");
		}
    #endif
	
	return;
}

void ACS7XX_ALLEGRO::getAHCount(double *ahcount)
{
	*ahcount = _AHCounter;
	return;
}

void ACS7XX_ALLEGRO::getCoulombCount(double *ccount)
{
	*ccount = _CoulombCounter;
	return;
}

void ACS7XX_ALLEGRO::printDebugDeviceInit(void)
{	
	
	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.println("ACS7XX sensor object initialized");
			Serial.print("PIN: ");
			Serial.println(_pintoread, DEC);
			Serial.print("Sensitivity: ");
			Serial.print(_sensitivity, DEC);
			Serial.println(" V/A");
			Serial.print("device is ");
			if(!_bidir) Serial.print("not ");
			Serial.println("bidirectional");
			Serial.println("...... ...... ......");
		}
    #endif
	
	return;
}