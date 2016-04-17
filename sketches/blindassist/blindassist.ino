/*
  Blind Assistance module
  @author Belin Fieldson <thebelin@gmail.com>
  @copyright This example code is in the public domain.

   The circuit:
  Rangefinder connection:
	* VCC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 2
	* ECHO connection of the sensor attached to digital pin 4
 
  Vibration Feedback connection
  * VCC connection of the vibration motor to A0
  * GND connection of the vibration motor to ground

  Neopixel connection
  * 5V to 5V
  * GND to GND
  * Data to D10
*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Rangefinder Trigger
const int trigPin = 2;

// Rangefinder Echo
const int echoPin = 4;

// neopixel indicators
const int neoPixelPin = 6;

// Haptic pin for showing distance
const int hapticPin = A0;

// The ranges of the depth sensor in cm
const int maxRange = 300;
const int minRange = 2;

// Minimum pulse length for PWM driven vibrator feedback mechanism 
const int minPulse = 5;
const int maxPulse = 200;

// The length of the pyro haptic pulse
const int pyroPulseLength = 250;

// The length of time between ping sequences
const int pingRate = 500;

// Declare the neopixel library, for controlling the neopixel indicator
Adafruit_NeoPixel light = Adafruit_NeoPixel(1, neoPixelPin, NEO_GRB + NEO_KHZ800);

// The current state of the distanceVibrator
boolean vibeState = false;

// The duration of the PWM wave which offers a signal to the distance feedback
long stateDuration = 0,

// The time in ms to remain off to form the PWM
  offDuration = 0,

// The last time an event happened for the distance haptic
  lastMillis = 0,
  
// The last time an event happened for the ping haptic
  pingLastMillis = 0,

// The distance in cm to the nearest object
  cm = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  // init the neoPixel
  light.begin();
  light.show();

  // Configure the ping sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

// Get the duration of the the PWM
// This should directly conform to the 
long getPWMDuration(long measurement, long minVal, long maxVal)
{
  if (measurement < maxVal) {
      if (measurement > minVal) {
        // Return a mapped value from the distance measurement
        // at a minimum of the minPulse duration constant
        return map(measurement, minVal, maxVal, minPulse, maxPulse);
      }
  }
  // If the value is outside of min/max params then return 0
  return 0;
}

// Get a color from the specified distance
// assign the distance to a color and brightness:
//  * blue for first meter,
//  * green for second meter,
//  * red for last meter
//  * make the intensity vary for entire set from 0 to 255
// @todo: ended up just making it red, and dim / bright according to distance read
uint32_t colorFromDistance (long distance) {
  // This powerLever is inversely proportionate to the distance
  float powerLevel = map (distance, minRange, maxRange, 255, 0);
  return light.Color (powerLevel, 0, 0);
}

void loop()
{
  // The duration of the ping
  long duration;

  // Only check distance every pingRate milliseconds
  if (pingLastMillis + pingRate < millis()) {
    // record the time of this ping
    pingLastMillis = millis();

    // @todo There's a total of 12 ms spent in delay here, possibly enhance system by
    // eliminating this delay
 
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(trigPin, OUTPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
  
    // convert the time into a distance
    cm = microsecondsToCentimeters(duration);
 
    // Debug the distance
    // Serial.print(cm);
    // Serial.print("cm");
    // Serial.println();
    
    // Get a stateDuration for the distance measured
    stateDuration = getPWMDuration(cm, minRange, maxRange);

    if (cm < maxRange && cm > minRange) {
      // assign the distance to a color (and brightness)
      // send the indicator light the color
      light.setPixelColor(0, colorFromDistance(cm));
    } else {
      light.setPixelColor(0, 0);
    }
  }

  // change the haptic indicator based on the range
  // This is a change to the time spent at high voltage vs in low
  // The adjustement is essentially a PWM (Pulse Width Modulation) signal, but very slow
  
  // If there's a valid state duration to supply
  // If the timer has expired the off duration
  if (vibeState == false && stateDuration != 0 && millis() > lastMillis + offDuration) {
    // reverse map the duration of the off state,
    // should be 3 times longer than on state
    offDuration = map (stateDuration, minPulse, maxPulse, maxPulse, minPulse) * 3;

    // Debug the stateDuration
    // Serial.print(stateDuration);
    // Serial.print(" / off:");
    // Serial.print(offDuration);
    // Serial.println();
    
    // Change the state
    analogWrite(hapticPin, 255);
    
    // Record the time the state changed
    lastMillis = millis();
    
    // Record the state
    vibeState = true;
  }
  
  // Vibe shutdown routine, only run if the vibe is operational
  // Only turn off the vibe if it's past the time it's supposed to be on
  if (vibeState == true && millis() > stateDuration + lastMillis) {
  
    // turn off the vibe
    analogWrite(hapticPin, 0);

    //Reset the last state change tracker
    lastMillis = millis();

    // Set the vibe state to 0 for a new reading 
    vibeState = false;
  } 
}
