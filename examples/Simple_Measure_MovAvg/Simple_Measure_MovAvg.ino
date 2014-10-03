/**************************************************************************/
/*!
    @file     Simple_Measure_MovAvg.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Example sketch that reads periodically values from ACS7xx sensor and compute consumption data

    @section  HISTORY

    v1.0 - First release
	v1.1 - Integrate constructor call fix
	
*/
/**************************************************************************/

#include <ACS7xx_Allegro.h>

// example for ACS712 : bidir = true, A0 is the sensor pin, 5.0 is the volatge board, 0.1 is the sensibility of the chip
ACS7XX_ALLEGRO mysensor(true, 0, 5.0, 0.1);

double courant;

void setup() {

	Serial.begin(9600);
	while (!Serial) ; //wait until Serial ready
	
	mysensor.begin();
	
	Serial.println("Starting...");
	//check serial output now
	mysensor.printDebugDeviceInit();
}

void loop() {
	mysensor.instantCurrent(&courant);
	Serial.print("courant mesure: ");
	Serial.println(courant, DEC);
	Serial.print("moyenne mouvante: ");
	Serial.println(mysensor.getMovingAvgExp(), DEC);
	
	delay(3000);
}