
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <math.h>
#include <Servo.h>

#define WINDOW_SIZE 5

Servo myservo;
int servoPin = 8;

int US_INDEX = 0; 
int Accel_X_INDEX = 0;
int Accel_Y_INDEX = 0;
float US_READINGS[WINDOW_SIZE];
float Accel_X_READINGS[WINDOW_SIZE];
float Accel_Y_READINGS[WINDOW_SIZE];

int gameState = 0;

int sensorUltraTrig = 6;
int sensorUltraEcho = 7;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

float echoTime;
float echoTime_Average;
unsigned long distance;

float new_x_reading;
float new_y_reading;

float accel_x_average;
float accel_y_average;

float accel_angle;
float difficulty_value;

int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;
int led5 = 9;

String portRead;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(servoPin);

    /* Initialise the accelerometer */
    if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_2_G);

  for(int i = 0; i < WINDOW_SIZE; i++){
  US_READINGS[i] = 0;
  Accel_X_READINGS[i] = 0;
  Accel_Y_READINGS[i] = 0;

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
}


  //Ultrasonic sensor setup
  pinMode(sensorUltraTrig, OUTPUT);
  pinMode(sensorUltraEcho, INPUT);

  digitalWrite(sensorUltraTrig, LOW);
  

}

int running_average(float sensorValue, float READINGS[WINDOW_SIZE], int INDEX)
{
  float SUM = 0;

  READINGS[INDEX] = sensorValue;
  for (int i = 0; i<WINDOW_SIZE; i++){
    SUM += READINGS[i];
  }
  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size
    // Divide the sum of the window by the window size for the result
  return SUM / WINDOW_SIZE; 

}

void loop() {

  // pos = Serial.read(); 
  // if (pos > 0)
  // {
  //   pos = map(pos, 0, 5,0, 180);
  //   myservo.write(pos);
  // }
  digitalWrite(sensorUltraTrig, HIGH);
  delayMicroseconds(10);// delay to send a couple soundwaves
  digitalWrite(sensorUltraTrig, LOW);
  echoTime = pulseIn(sensorUltraEcho,HIGH);
  echoTime_Average = running_average(echoTime, US_READINGS, US_INDEX); //averages the value of the recieved soundwave by comparing to the last 5 values
  distance = (echoTime_Average*0.034)/2; // formula for changing the time measured into distance (cm)
    /* Get a new sensor event */ 

  sensors_event_t event; 
  accel.getEvent(&event);

  /* reading sensor values */
  new_x_reading = event.acceleration.x;
  new_y_reading = event.acceleration.y;
  new_z_reading = event.acceleration.z;

  accel_x_average = running_average(new_x_reading, Accel_X_READINGS, Accel_X_INDEX);
  accel_y_average = running_average(new_y_reading, Accel_Y_READINGS, Accel_Y_INDEX);

  accel_angle = atan2(accel_y_average,accel_x_average)*180/M_PI;
  if (accel_angle<=45){
    difficulty_value = 1;
  }
  else if (accel_angle>=135)
  {
    difficulty_value = 3;
  }
  else
  {
    difficulty_value = 2;
  }


  Serial.print(distance); //distance in cm
  Serial.print(",");
  Serial.print(difficulty_value); //
  Serial.println("");

  int val = Serial.read();
  controlLEDs(val);
  controllServo(val);
  
}

void controllServo(int state)
{
  if (state == 1) {
    myservo.write(0);
  }
  else if (state == 2) {
    myservo.write(45);
  }
  else if (state == 3) {
    myservo.write(90);
  }
  else if (state == 4) {
    myservo.write(135);
  }
  else if (state == 5) {
    myservo.write(180);
  }
}

void controlLEDs(int state) {

  // Turn all LEDs OFF first
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);

  // Turn ON LED depending on angle range
  if (state == 1) {
    digitalWrite(led1, HIGH);
  }
  else if (state == 2) {
    digitalWrite(led2, HIGH);
  }
  else if (state == 3) {
    digitalWrite(led3, HIGH);
  }
  else if (state == 4) {
    digitalWrite(led4, HIGH);
  }
  else if (state == 5) {
    digitalWrite(led5, HIGH);
  }
}
