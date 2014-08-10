Arduino library for ACS7xx current IC sensors from Allegromicro
==============

    v1.0 - First release

ACS7xx sensors are hall effect sensors IC ranging from 0A to 50A for AC or DC current. They are suitable for use with Arduino boards either 5V or 3.3V according to your choice.

[ACS7xx](http://allegromicro.com/en/Products/Current-Sensor-ICs/Zero-To-Fifty-Amp-Integrated-Conductor-Sensor-ICs.aspx) Allegromicro page.

## Features ##
- Get instant current
- Get one shot Ampere.hour measurement
- Get Ampere.hour counter
- Get Coulomb counter
- Retreive last readings
- Reset counters


## Testing ##
- Tested only against ACS712-20A on DC current
- Tested on Arduino Mega with Arduino IDE 1.0.5
- Please comment about other devices

## Usage ##
This lib has not been tested against AC current and may need some improvement for such usage.

This lib is mainly done to control battery consumption / charging over the time. The use of the [Timer Arduino Lib](http://playground.arduino.cc/code/timer) may be usefull to update values periodiically. The more often you'll update them, the more accurate measurements will be. The dedicated chips for battery monitoring are working on a 250ms basis. Feel free to adapt to your needs.

To use it, just download a [copy](https://github.com/sosandroid/ACS7XX_ALLEGRO/archive/master.zip) and extract it in an ACS7XX_ALLEGRO forlder inside your libraries forder in the Arduino IDE folder tree.
