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

RouteDowser Wearable AAPI (Assistive Application Programming Interface):
__________________________________________
* master device connects to routeDowser app via bluetooth
* master device establishes local wireless connection to additional sensors via TTL over 9600Mhz
* 9600 mhz communication through TBclient (thoughtbot) library
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
	These should be mounted at an angle facing down so that a high value indicates an upcoming precipice.

	{
		id: 'ba123',
		type: 'pingLeftLeg',
		value: 129
	}

	feedback input
	{
		id: 'ba123',
		type: 'vibration',
		pwmOn: 250,
		pwmOff: 30
	}

* pingRightLeg

	{
		id: 'ba123',
		type: 'pingRightLeg',
		value: 129
	}

	feedback input
	{
		id: 'ba123',
		type: 'vibration',
		pwmOn: 250,
		pwmOff: 30
	}

* pingCane
	The ping cane has an accelerometer and a ping rangefinder.  It contains a feedback vibrator which indicates distance as read by the sensor in a PWM wave inversely attuned to the distance sensed.  This could be hand held or wrist mounted.  This device is both feedback and sensor.  If it's connected to a sensor master it should be able to provide haptic assistance feedback for the master.

	{
		id: 'ba123',
		type: 'pingCane',
		value: 230,
		location: 999999.99999999,
		rotation: 9999999.999999
	}

	feedback input
	{
		id: 'ba123',
		feedback: [
			{
			type: 'vibration',
			pwmOn: 250,
			pwmOff: 30
			},
			{
			type: 'sound',
			waveForm: ''
			},
			{
			type: 'beep',
			hz: 2500,
			pwm: 30
			}
		]

	}

* imageCane
	The same haptic feedback as the ping cane, plus audio via stereo plug. An additional video and still image capture input device can be pointed in this format.  Video is continuously transmitted to the master as TTL.Color stills are taken every 5 seconds and transmitted to the API if there's at least one open assistance session. Stills can be analyzed by the API layer before sending them on to the routeDowser server. 

	{
		id: 'ba123',
		type: 'pingCane',
		value: 230,
		location: 999999.99999999,
		rotation: 9999999.999999,
		videoBlob: '',
		imageBlob: ''
	}

	feedback input
	{
		id: 'ba123',
		feedback: [
			{
			type: 'vibration',
			pwmOn: 250,
			pwmOff: 30
			},
			{
			type: 'sound',
			waveForm: ''
			},
			{
			type: 'beep',
			hz: 2500,
			pwm: 30
			}
		]

	}

* humanFace

	This is a sensor placed on the user's face which can find heat sources directly in front of it.  It's used to keep your face pointed towards a person when interacting with them

	{
		id: 123456,
		type: 'humanFace',
		value: 0
	}

* imageFace
	This is a sensor placed on the user's face which can capture live black & white video and image stills. It functions the same as the ping cane, except doesn't include haptic feedback responders
