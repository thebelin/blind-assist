/*
  Blind Assistance module
  @author Belin Fieldson <thebelin@gmail.com>
  @copyright This example code is in the public domain.

   Original code for Ping))) example was created by David A. Mellis
   Adapted for HC-SR04 by Tautvidas Sipavicius

   The circuit:
  Rangefinder connection:
	* VCC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 2
	* ECHO connection of the sensor attached to digital pin 4

  Pyroelectric sensor connection:
	* VCC connection of the sensor attached to +5V
	* GND connection of the sensor attached to ground
	* TRIG connection of the sensor attached to digital pin 6
  
  Vibration depth sensor Feedback connection
  * VCC connection of the vibration motor to A0
  * GND connection of the vibration motor to ground

  Vibration heat sensor Feedback connection
  * VCC connection of the vibration motor to A2
  * GND connection of the vibration motor to ground

*/
// Rangefinder Trigger
const int trigPin = 2;

// Rangefinder Echo
const int echoPin = 4;

// Pyro digital sensor
const int pyroPin = 6;

// PWM vibe feedback pin
// Note that some vibration modules need more volts than the 3 the analog port provides,
// one way to do that is to tie two of these analog pins together and juice them in series.
const int hapticPin = A0;

// vibe indicator for human (pyroelectric) detector
const int hapticPyroPin = A2;

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

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

// The current state of the distanceVibrator
boolean vibeState = false,

// The current state of the pyroVibrator
  pyroVibeState = false;

// The previous state of the pyroelectric sensor
int lastPyroState = 0;

// The duration of the PWM wave which offers a signal to the distance feedback
long stateDuration = 0,

// The time in ms to remain off to form the PWM
  offDuration = 0,

// The last time an event happened for the distance haptic
  lastMillis = 0,
  
// The last time an event happened for the pyro haptic
  pyroLastMillis = 0,
  
  pingLastMillis = 0,

// The duration of the PWM wave for the pyroelectric feedback
  heatDuration = 0,

// The time of the last heat measurement
  lastHeatMillis = 0,

// The distance in cm to the nearest object
  cm = 0;

void loop()
{
  // The duration of the ping
  long duration,
    
  // The time in ms of the last loop
  lastLoop,
  
  // The state of the pyroelectric sensor
  pyroState = 0;
  
  // Only check distance every pingRate milliseconds
  if (pingLastMillis + pingRate < millis()) {
    // record the time of this ping
    pingLastMillis = millis();

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
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    
    // Get a stateDuration for the distance measured
    stateDuration = getPWMDuration(cm, minRange, maxRange);
  }

  // change the analog out buzzer indicator based on the range
  // This is a change to the time spent at high voltage vs in low
  // The adjustement is essentially a PWM (Pulse Width Modulation) signal
  if (vibeState == false) {
    // If there's a valid state duration to supply
    if (stateDuration != 0) {
      
      // If the timer has expired the off duration
      if (millis() > lastMillis + offDuration) {

        // reverse map the duration of the off state
        offDuration = map(stateDuration, minPulse, maxPulse, maxPulse, minPulse)* 3;

        // Debug the stateDuration
        Serial.print(stateDuration);
        Serial.print(" / off:");
        Serial.print(offDuration);
        Serial.println();
        
        // Change the state
        analogWrite(hapticPin, 255);
        
        // Record the time the state changed
        lastMillis = millis();
        
        // Record the state
        vibeState = true;
      }
    }
  }
  
  // Vibe shutdown routine, only run if the vibe is operational
  if (vibeState == true) {

    // Only turn off the vibe if it's past the time it's supposed to be on
    if (millis() > stateDuration + lastMillis) {
      
      // turn off the vibe
      analogWrite(hapticPin, 0);

      //Reset the last state change tracker
      lastMillis = millis();
 
      // Set the vibe state to 0 for a new reading 
      vibeState = false;
    }
  }
 
  // This delay exists for the sake of sanity while debugging
  // @todo remove the delay
  //delayMicroseconds(100);
  
  pyroState = digitalRead(pyroPin);
  
  if (pyroVibeState == false) {
    if (pyroState != lastPyroState) {
      // Record the pyro state to sense the next change
      lastPyroState = pyroState;
  
      // Do pyro buzz
      analogWrite(hapticPyroPin, 255);
  
      // Turn the state on
      pyroVibeState = true;

      // Mark the time 
      pyroLastMillis = millis();

      // pyro debug
  //    Serial.print("pyroPin: ");
  //    Serial.print(pyroState);
  //    Serial.println();
    }
  }

  if (pyroVibeState == true) {
    if (pyroLastMillis + pyroPulseLength < millis()) {
      // Stop pyro buzz
      analogWrite(hapticPyroPin, 0);
  
      // Turn the state off
      pyroVibeState = false;
    }
  }
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

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

