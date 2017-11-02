#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>
#include <std_msgs/UInt16.h>
#include <sensor_msgs/JointState.h>
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

#define SONAR_NUM     3 // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
float cm[SONAR_NUM];
unsigned int last_cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.
bool publishDistance = false;

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(3, 2, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(5, 4, MAX_DISTANCE),
  NewPing(7, 6, MAX_DISTANCE)
};

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "/range_data", &range_msg);

bool published_distance = true;

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
    published_sensor = true;
  }

  pub_sensor.publish(&pushed_msg_sensor);
  last_reading = reading;
}

// distance
void setupDistance(){
  range_msg.radiation_type = sensor_msgs::Range::INFRARED;
  range_msg.field_of_view = 0.26;
  range_msg.min_range = 0.03;
  range_msg.max_range = 2;

  nh.advertise(pub_range);
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;

}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = (sonar[currentSensor].ping_result / US_ROUNDTRIP_CM);
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
 publishDistance = true;
}

void handleDistance(){

  if ( publishDistance == false ) {
    for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
      if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
        pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
        if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
        currentSensor = i;                          // Sensor being accessed.
        cm[currentSensor] =  sonar[currentSensor].ping_cm(); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
      }
    }
  }

  if ( publishDistance == true ) {
     range_msg.header.stamp = nh.now();
     for (uint8_t i = 0; i < SONAR_NUM; i++) {

        char frameid[4]="  ";

        if (i == 0) {
          strncpy(frameid, "fl", 4);
        }

         if (i == 1){
           strncpy(frameid, "ff", 4);
         }

         if (i == 2){
           strncpy(frameid, "fr", 4);
         }


        range_msg.range = cm[i]/100.0;
        range_msg.header.frame_id =  frameid;

        pub_range.publish(&range_msg);
        publishDistance = false;
      }
  }
}


char log_buffer[50];
#include <AccelStepper.h>
#include <MultiStepper.h>

// Define some steppers and the pins the will use
AccelStepper stepper5(AccelStepper::DRIVER,30,31,32); // wrist tilt
AccelStepper stepper4(AccelStepper::DRIVER,36,37,38); // wrist rotate
AccelStepper stepper1(AccelStepper::DRIVER,33,34,35); // elbow
AccelStepper stepper3(AccelStepper::DRIVER,39,40,41); // shoulder tilt
AccelStepper stepper2(AccelStepper::DRIVER,42,43,44); // shoulder rotate

MultiStepper steppers;

long joint_state[5];
long prev_state[5];

void setupSteppers(){
  //nh.loginfo("Setup steppers");
  stepper1.setMaxSpeed(500.0);
  stepper1.setAcceleration(3000.0);
  stepper2.setMaxSpeed(500.0);
  stepper2.setAcceleration(3000.0);
  stepper3.setMaxSpeed(500.0);
  stepper3.setAcceleration(3000.0);
  stepper4.setMaxSpeed(500.0);
  stepper4.setAcceleration(3000.0);
  stepper5.setMaxSpeed(500.0);
  stepper5.setAcceleration(3000.0);

  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
  steppers.addStepper(stepper5);
}

int convert_angle( int stepsPerRotation, double radians){
  float y;
  y=((radians / M_PI)*stepsPerRotation)+0.5;
  return y*49;
}

void moveSteppers(){
  if (prev_state[0] != joint_state[0] || prev_state[1] != joint_state[1] || prev_state[2] != joint_state[2] || prev_state[3] != joint_state[3] || prev_state[4] != joint_state[4]){
    steppers.moveTo(joint_state);
    // prev_state[0]=joint_state[0]; //E
    // prev_state[1]=joint_state[1]; //SR
    // prev_state[2]=joint_state[2]; //ST
    // prev_state[3]=joint_state[3]; //WR
    prev_state[4]=joint_state[4]; //WT
  }
  steppers.runSpeedToPosition();
}

void jointCallback( const sensor_msgs::JointState& cmd_msg ){
  // nh.loginfo("callback");
  // joint_state[0] = -convert_angle(64, cmd_msg.position[0]);
  // joint_state[1] = convert_angle(633, cmd_msg.position[1]);
  // joint_state[2] = -convert_angle(171, cmd_msg.position[2]);
  // joint_state[3] = convert_angle(200, cmd_msg.position[3]);
  joint_state[4] = convert_angle(880, cmd_msg.position[4]);
}

ros::Subscriber<sensor_msgs::JointState> sub("/move_group/fake_controller_joint_states", jointCallback );

void setup(){
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.subscribe(sub);
  while (!nh.connected()){
    nh.spinOnce();
  }

  setupSteppers();

  //setupDistance();
}


void loop()
{
    moveSteppers();
    nh.spinOnce();
}
