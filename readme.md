Routedowser
==========================================
Blind Assist
==========================================

A project by
__________________________________________
Belin Fieldson
__________________________________________

Use this to build your own haptic visual assistance system.  The code can be modified and used in any way you want to.
__________________________________________

Target Functionality for v1:
__________________________________________
* Indicate via vibration patterns the distance to object
* Indicate via vibration patterns the presence of human directly in front of the user
* Recharge via USB and operate for up to 8 hours
* Add audio feedback option using clicks
* Add video capture module with encoder
* Connect to Android system via Bluetooth or usb
* Broadcast video stills via bluetooth
* Read translatable material from video/stills via AR library directly to user
* Create routes via the web and add cues to follow - "route dowser"
* Create volunteer mapping system where walking routes can be recorded to follow

RouteDowser Wearable API Use Example:
__________________________________________
* master device connects to routeDowser app via bluetooth
* master device establishes local wireless connection to additional sensors via TTL over 9600Mhz
* sensors for ad-hoc network connect to master device according to their 9600Mhz config
* feedback modules connect to master device according to their 9600 mhz config
* Sensors send sensor identity, sensor type and data of sensor to master via 9600Mhz.
* Master device sends each sensor data into routedowser bluetooth app
* Master device receives update from app according to sensor activity
* master device plays any audio data recieved from either audible sensors or from routeDowser bluetooth connection through audio out port on master module.
* master device sends any feedback updates to connected feedback modules


Sensor Types:
__________________________________________

* pingLeftLeg
	
	This is an example "trip" sensor which is designed to keep the user from tripping
	trip sensors are attached to the leg and give feedback of distance to nearest obstruction.
	These can be mounted at an angle facing down so that a high value indicates an upcoming precipice

	{
		id: 123456,
		type: 'pingLeftLeg',
		value: 129
	}

* pingRightLeg

	{
		id: 123456,
		type: 'pingRightLeg',
		value: 129
	}

* pingCane
	The ping cane has an accelerometer and a ping.  It contains a feedback vibrator which indicates distance 
	as read by the sensor:

	{
		id: 123456,
		type: 'pingCane',
		value: 230,
		location: 999999.99999999,
		rotation: 9999999.999999
	}
	feedback input
	{
		type: 'pingCane',
		pwmOn: 250,
		pwmOff: 30
	}

* humanFace

	This is a sensor placed near the face which can find heat sources.  It's used to keep your face pointed towards a person when interacting with them

	{
		id: 123456,
		type: 'humanFace',
		value: 0
	}
