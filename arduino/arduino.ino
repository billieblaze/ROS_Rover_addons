
/* 
 * Button Example for Rosserial
 */

#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>

ros::NodeHandle nh;

// Sensor Config
std_msgs::Bool pushed_msg_sensor;
ros::Publisher pub_sensor("/sensor/hit", &pushed_msg_sensor);

const int button_pin = 8;
const int led_pin = 13;

bool last_reading;
long last_debounce_time=0;
long debounce_delay=50;
bool published_sensor = true;


// // Distance Config
#include "RunningMedian.h"
RunningMedian samples = RunningMedian(10);

#include <NewPing.h>
#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

std_msgs::Int16 pushed_msg_distance;
int last_distance=0;
long last_distance_time=0;
long distance_delay=10;
bool published_distance = true;
ros::Publisher pub_distance("/sensor/distance", &pushed_msg_distance);

// Stepper Config
#include <Stepper.h>
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 14, 15, 16, 17);
volatile int stepCount = 0;
int maxSteps = 200 * 10;
int endstopPin = 53;
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
  nh.advertise(pub_distance);
}

void handleDistance(){
  int reading = sonar.ping_cm();
  samples.add(reading);
  if (last_distance != reading){
      last_distance_time = millis();
      published_distance = false;
  }
  
  if ( !published_distance && (millis() - last_distance_time)  > distance_delay) {
    
    pushed_msg_distance.data = (int)samples.getMedian();

    pub_distance.publish(&pushed_msg_distance);
    published_distance = true;
  }

  last_distance = reading;

}

// stepper

void moveStepper(int steps) {
  int targetSteps = stepCount + steps;

  if ( maxSteps < targetSteps ) {
    //Serial.println(maxSteps);
    stepCount = maxSteps;
    return;
  }

  if ( targetSteps < maxSteps || initializingEndstop == 1) {
    myStepper.step(steps);
    stepCount += steps;
    //Serial.println(stepCount);
  }
}

void setupStepper() {
  initializingEndstop = 1;
  // setup endstop pin
  pinMode(endstopPin, INPUT);           // set pin to input
  digitalWrite(endstopPin, HIGH);       // turn on pullup resistors

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
  
  myStepper.setSpeed(100);

  setupSensor(); 
  setupDistance();

  setupStepper();
}

void loop(){
  
  handleSensor();
  handleDistance();
  
  nh.spinOnce();
}


