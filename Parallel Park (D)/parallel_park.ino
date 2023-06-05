/*
 * This program executes a reverse park on both RHS and LHS using the Pololu Zumo rover. This
 * program has been adapted from code used in the Zumo based labs.
 * Author: Chloe Hulme
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

// global parking variable
bool parked = false;

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
  int left = ultrasonic(echoPinLeft,trigPinLeft);
  int front = ultrasonic(echoPinFront,trigPinFront);
  int right = ultrasonic(echoPinRight,trigPinRight);

  String message = "";
  message = message + "[" + left + "cm," + front + "cm," + right + "cm]\n";
  Serial.print(message);
  delay(500); //wait for 1/10th of a second.

  if (!parked) {
    if (right > 60) {
      while (right > 10) {
        move_forward(50);
        right = ultrasonic(echoPinRight,trigPinRight);
        delay(10);
      }
      parallel_park();
      parked = true;
    }
    else {
      move_forward(50);
    }
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

void parallel_park() {
  move_stop();
  delay(500);
  turn_left(250);
  delay(500);
  move_backward(50);
  delay(2700);
  move_stop();
  delay(500);
  turn_right(200);
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
