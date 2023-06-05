/*
 * A finite state machine controlling the execution of parallel, forward or reverse parks using the Pololu 
 * Zumo rover, a HM-10 bluetooth module and a BLE Joystick app for IOS. This program has been adapted from 
 * code used in the Zumo based labs.
 * Author: Chloe Hulme
 * BLE Joystick app (IOS): https://apps.apple.com/us/app/ble-joystick/id1098200556
 */

// defines pins numbers
const int echoPinLeft = 5;
const int trigPinLeft = 6;

const int echoPinFront = 11;
const int trigPinFront = 12;

const int echoPinRight = 2;
const int trigPinRight = 4;

// defines variables
long duration;
int distance;

int motorright = 9;                                                              
int motorrightdir  = 7;
int motorleft = 10;
int motorleftdir  = 8;

int right;
int front;
int left;

// parking type
String parking_type = "none";

// states in finite state machine, handled in loop()
enum State {
    STATE_SELECT_PARK_TYPE = 0,
    STATE_EXECUTE_FORWARD,
    STATE_EXECUTE_REVERSE,
    STATE_EXECUTE_PARALLEL
};

// initialise to first state
State state = STATE_SELECT_PARK_TYPE;

void setup() {
  pinMode(motorright, OUTPUT);                                                      
  pinMode(motorleft, OUTPUT);     
  pinMode(motorrightdir, OUTPUT);  
  pinMode(motorleftdir, OUTPUT);  

  pinMode(trigPinLeft, OUTPUT); 
  pinMode(echoPinLeft, INPUT); 
  pinMode(trigPinFront, OUTPUT); 
  pinMode(echoPinFront, INPUT); 
  pinMode(trigPinRight, OUTPUT); 
  pinMode(echoPinRight, INPUT); 
  Serial.begin(9600); 
}

void loop() 
{
  switch(state) {
    case STATE_SELECT_PARK_TYPE:
      select_parking_type();
   
      if (parking_type == "forward") {
        state = STATE_EXECUTE_FORWARD;
      }
      else if (parking_type == "reverse") {
        state = STATE_EXECUTE_REVERSE;
      }
      else if (parking_type == "parallel") {
        state = STATE_EXECUTE_PARALLEL;
      }
      break;

    case STATE_EXECUTE_FORWARD:
      execute_forward();
      parking_type = "none";
      state = STATE_SELECT_PARK_TYPE;
      break;

    case STATE_EXECUTE_REVERSE:
      execute_reverse();
      parking_type = "none";
      state = STATE_SELECT_PARK_TYPE;
      break;

    case STATE_EXECUTE_PARALLEL:
      execute_parallel();
      parking_type = "none";
      state = STATE_SELECT_PARK_TYPE;
      break;
  }
}

int ultrasonic(int echoPin, int trigPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  
  distance= duration*0.034/2;
  return distance;
}

// Select parking type by interacting with IOS app
void select_parking_type() {
  Serial.flush();
  while (parking_type == "none") {
    if(Serial.available() > 0) {
       // Read in data sent from connected device
       char data = Serial.read();
       Serial.println(data);
    
       // Up arrow on right controller in app 
       if(data =='E') {
         parking_type = "forward";
       }
       // Down arrow on right controller in app 
       if(data =='G') {
         parking_type = "reverse";
       }
       // Left arrow on right controller in app 
       if(data =='H') {
         parking_type = "parallel";
       }
     }
  }
}

// execute parking sequence for parallel park
void execute_parallel(){
  bool parked = false;
  while(!parked) {
    right = ultrasonic(echoPinRight,trigPinRight);
    if (right > 60) {
      while (right > 10) {
        move_forward(50);
        right = ultrasonic(echoPinRight,trigPinRight);
        delay(10);
      }
        parallel_park();
        move_stop();
        parked = true;
    }
    else {
      move_forward(50);
    }
  }
}

// parallel park algorithm
void parallel_park() {
  move_stop();
  delay(500);
  turn_left(320);
  delay(500);
  move_backward(50);
  delay(2800);
  move_stop();
  delay(500);
  turn_right(330);
}

// execute sequence for reverse park
void execute_reverse() {
  bool parked = false;
  while(!parked){
    right = ultrasonic(echoPinRight,trigPinRight);
    if (right > 80) {
      delay(800);
      reverse_park_rhs();
      move_stop();
      parked = true;
     }
     else {
        move_forward(50);
     }
  }
}

// reverse park algorithm
void reverse_park_rhs() {
  turn_left(500);
  delay(100);
  move_backward(50);
  delay(3000);
}

// execute sequence for forward park
void execute_forward() {
  bool parked = false;
  while(!parked) {
    right = ultrasonic(echoPinRight,trigPinRight);
    if (right > 80) {
      forward_park_rhs();
      move_stop();
      parked = true;
     }
     else {
        move_forward(50);
     }
  }
}

// forward park algorithm
void forward_park_rhs() {
  delay(1250);
  turn_right(400);
  delay(500);
  move_forward(50);
  delay(2500);
}

void move_forward(int speed)
{
  digitalWrite(motorrightdir, LOW);
  analogWrite(motorright,speed); 
  digitalWrite(motorleftdir, LOW);
  analogWrite(motorleft, speed); 
}

void move_backward(int speed)
{
  digitalWrite(motorrightdir, HIGH);
  analogWrite(motorright,speed); 
  digitalWrite(motorleftdir, HIGH);
  analogWrite(motorleft, speed);
}

void turn_right(int time)
{
  digitalWrite(motorrightdir, HIGH);
  analogWrite(motorright,180); 
  digitalWrite(motorleftdir, LOW);
  analogWrite(motorleft, 180);
  delay(time);
  move_stop();
}

void turn_left(int time)
{
  digitalWrite(motorrightdir, LOW);
  analogWrite(motorright,180); 
  digitalWrite(motorleftdir, HIGH);
  analogWrite(motorleft, 180);
  delay(time);
  move_stop();
}

void move_stop()
{
  analogWrite(motorright, 0); 
  analogWrite(motorleft, 0);   
}
