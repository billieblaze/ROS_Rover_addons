
/*
 * Button Example for Rosserial
 */

#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>
#include <sensor_msgs/Range.h>
ros::NodeHandle nh;

// Sensor Config
std_msgs::Bool pushed_msg_sensor;
ros::Publisher pub_sensor("/ping", &pushed_msg_sensor);

const int button_pin = 8;
const int led_pin = 13;

bool last_reading;
long last_debounce_time=0;
long debounce_delay=50;
bool published_sensor = true;


// // Distance Config
#include <NewPing.h>
#include "RunningMedian.h"
RunningMedian samples_1 = RunningMedian(10);
RunningMedian samples_2 = RunningMedian(10);
RunningMedian samples_3 = RunningMedian(10);

#define SONAR_NUM     3 // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(2, 3, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(4, 5, MAX_DISTANCE),
  NewPing(6, 7, MAX_DISTANCE)
};

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "range_data", &range_msg);
bool published_distance = true;

// Stepper Config
volatile int stepCount = 0;
int maxSteps = 200 * 10;
int endstopPin = A3;
int directionPin = A4;
int stepPin = A5;
int initializingEndstop = 0;

// sensor
void setupSensor(){
  nh.advertise(pub_sensor);

  //initialize an LED output pin
  //and a input pin for our push button
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);

  //Enable the pullup resistor on the button
  digitalWrite(button_pin, HIGH);

  //The button is a normally button
  last_reading = ! digitalRead(button_pin);
}

void handleSensor(){
  bool reading = ! digitalRead(button_pin);

  if (last_reading!= reading){
      last_debounce_time = millis();
      published_sensor = false;
  }

  //if the button value has not changed during the debounce delay
  // we know it is stable
  if ( !published_sensor && (millis() - last_debounce_time)  > debounce_delay) {
    digitalWrite(led_pin, reading);
    pushed_msg_sensor.data = reading;
    pub_sensor.publish(&pushed_msg_sensor);
    published_sensor = true;
  }

  last_reading = reading;
}

// distance
void setupDistance(){
  range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  range_msg.field_of_view = 0.26;
  range_msg.min_range = 0.03;
  range_msg.max_range = 0.2;

  nh.advertise(pub_range);
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;

}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  int range = 0;

  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    range_msg.header.stamp = nh.now();

    char frameid[4]="/  ";
    if (i == 0) {
      strncpy(frameid, "/fl", 4);
      samples_1.add(cm[i]);
      range = (int)samples_1.getMedian();
    }

    if (i == 1){
      strncpy(frameid, "/ff", 4);
      samples_2.add(cm[i]);
      range = (int)samples_2.getMedian();
    }

    if (i == 2){
      strncpy(frameid, "/fr", 4);
      samples_3.add(cm[i]);
      range = (int)samples_3.getMedian();
    }

    range_msg.range = range;
    range_msg.header.frame_id =  frameid;

    pub_range.publish(&range_msg);
  }

}

void handleDistance(){
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
}

// stepper

void moveStepper(int steps) {

  if (steps > 0){
    digitalWrite(directionPin, LOW);
  } else {
    digitalWrite(directionPin, HIGH);
  }

  int targetSteps = stepCount + steps;

  if ( maxSteps < targetSteps ) {
    //Serial.println(maxSteps);
    stepCount = maxSteps;
    return;
  }

  if ( targetSteps < maxSteps || initializingEndstop == 1) {
    //myStepper.step(steps);
    for (int i = 0; i < abs(steps); i++){
      digitalWrite(stepPin, LOW);
      delay(1);
      digitalWrite(stepPin, HIGH);
      delay(1);
    }
    stepCount += steps;
    //Serial.println(stepCount);
  }
}

void setupStepper() {
  initializingEndstop = 1;

  // setup pins
  pinMode(endstopPin, INPUT);           // set pin to input
  digitalWrite(endstopPin, HIGH);       // turn on pullup resistors

  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  digitalWrite(directionPin, HIGH);
  digitalWrite(stepPin, LOW);

  // until we hit the endstop, keep moving back
  digitalWrite(13, HIGH);
  while (digitalRead(endstopPin)) {
    moveStepper(-100);
  }
  initializingEndstop = 0;
  digitalWrite(13, LOW);
  // hit it,  move off the endstop and reset the counter
  delay(10);
  moveStepper(200);
  stepCount = 0;

}


void setup(){

  nh.initNode();

  setupSensor();
  setupDistance();
  setupStepper();
}

void loop(){

  handleSensor();
  handleDistance();

  nh.spinOnce();
}
