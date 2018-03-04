Arduino library for ACS7xx & ACS712 current IC sensors from Allegromicro
==============

ACS712 and ACS7XX sensors are hall effect sensors IC ranging from 0A to 50A for AC or DC current. They are suitable for use with Arduino boards either 5V or 3.3V according to your choice.

[ACS7xx](http://allegromicro.com/en/Products/Current-Sensor-ICs/Zero-To-Fifty-Amp-Integrated-Conductor-Sensor-ICs.aspx) Allegromicro page.


	v1.0 - First release
    v1.1 - Moving exponential average + robustness
	v1.2 - Modify the constructor call to avoid some errors
	v1.2.1 - Fix issue #1


## Features ##
- Get instant current
- Get Ampere.hour counter
- Get Coulomb counter
- Retreive last readings
- Reset counters
- Moving exponential average on current readings

## Testing ##
- Tested only against ACS712-20A
- Tested on Arduino Mega with Arduino IDE 1.0.5
- Please comment about other devices

