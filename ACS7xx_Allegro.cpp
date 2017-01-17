/**************************************************************************/
/*!
    @file     CurrentIC_ACS7xx_Allegro.cpp
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Set of methods to use measurements from ACS7xx sensors from Allegro Micro
	All those methods are made to be called periodically

    @section  HISTORY

    v1.0 - First release
	V1.1 - Moving exponential average + robustness
	v1.2 - Modify the constructor call to avoid some errors
	v1.2.1 - Fix issue #1
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
	_bidir = ACS7XX_BIDIR_DEFAULT;
	_pintoread = ACS7XX_PIN_DEFAULT;
	_sensitivity = ACS7XX_SENSITIVITY_DEFAULT;
	_voltage = BOARD_VOLTAGE_DEFAULT;
}

ACS7XX_ALLEGRO::ACS7XX_ALLEGRO(boolean bidir, int pintoread, double voltage, double sensitivity) 
{
	_bidir = bidir;
	_pintoread = pintoread;
	_sensitivity = sensitivity;
	_voltage = voltage;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

void ACS7XX_ALLEGRO::begin(void) {


	if (_bidir) {
		_voltage_offset = _voltage / 2.0;
	} 
	else {
		_voltage_offset = 0.0;
	}
	_resolution = (double) BOARD_ADC_DEPTH;
	_adc_offset = (int) BOARD_MEASURED_OFFSET;
	_factor_value = (double) ACS7XX_FACTOR_VALUE;
	
	_lastMillis = 0; //need to add a function to start the counting by initializing lastmillis when starting a discharge / charge cycle
	_lastCurrent = 0.0;
	_AHCounter = 0.0;
	_CoulombCounter = 0.0;
	
	_movavgexp_alpha = 2.0 / (EXP_MOVAVG_N + 1.0);
	_movavgexp_loop = 0;
	_movavgexp_val = 0.0;
	
	#ifdef SERIAL_DEBUG
		if(!Serial) {
			Serial.begin(9600);
		}
	#endif
	return;
}

void ACS7XX_ALLEGRO::instantCurrent(double *current)
{
	int readvalue = analogRead(_pintoread) + _adc_offset;
	double readvolt = (((double) readvalue / _resolution) * _voltage) - _voltage_offset;
	double readcur = readvolt * _factor_value / _sensitivity;
	*current = readcur;
	_lastCurrent = readcur;
	ACS7XX_ALLEGRO::movingAvgExp(readcur);
	
	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.print("Read value on pin including offset: ");
			Serial.println(readvalue, DEC);
			Serial.print("Current: ");
			Serial.print(readcur, DEC);
			Serial.println(" mA");
			Serial.print("Moving average: ");
			Serial.print(ACS7XX_ALLEGRO::getMovingAvgExp(), DEC);
			Serial.println(" mA");
		}
    #endif
	
	return;
}

void ACS7XX_ALLEGRO::ampereHourCount(double *mamperehc)
{
	unsigned long currentmillis = millis();
	double timeframehour = (double)(currentmillis - _lastMillis) / 3600000.0;
	
	double readcurrent;
	ACS7XX_ALLEGRO::instantCurrent(&readcurrent);
	*mamperehc = readcurrent * timeframehour;
	
	_lastMillis = currentmillis;

	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.print("AmpHour: ");
			Serial.print(*mamperehc, DEC);
			Serial.println(" mAh");
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
	
	_lastCoulomb = amperehcTemp * 3.6;
	_CoulombCounter += _lastCoulomb;

	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.print("mAH counter ");
			Serial.print(_AHCounter, DEC);
			Serial.println(" mAH");
			Serial.print("Coulomb counter ");
			Serial.print(_CoulombCounter, DEC);
			Serial.println(" C");
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

void ACS7XX_ALLEGRO::movingAvgExp(double current) {

	//init moving average exponetial with simple average of 
	
	if (_movavgexp_loop < EXP_MOVAVG_LOOP) {
		_movavgexp_val += current;
		if (_movavgexp_loop == (EXP_MOVAVG_LOOP - 1)) _movavgexp_val = _movavgexp_val / (double) EXP_MOVAVG_LOOP;
		_movavgexp_loop ++;
	}
	else {
		double movavgexp = _movavgexp_val + _movavgexp_alpha * (current - _movavgexp_val);
		_movavgexp_val = movavgexp;
	}

	return;
}

double ACS7XX_ALLEGRO::getMovingAvgExp(void) {
	return _movavgexp_val;
}

void ACS7XX_ALLEGRO::resetMovingAvgExp(void) {
	_movavgexp_val = 0;
	_movavgexp_loop = 0;
	return;
}