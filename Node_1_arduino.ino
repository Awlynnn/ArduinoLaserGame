#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#define WINDOW_SIZE 3
#define PI 3.1415926535897932384626433832795

bool on;
// defining LED pins
#define RED  4   // pin that red led is connected to    
#define GREEN  5 // pin that green led is connected to     
#define BLUE  6  // pin that blue led is connected to   
#define DELAY 200 // blink delay in ms 


/* Filter variables photo */
int oldpReading = 0;
int olderpReading = 0;
int irDigital;

/* Accelerometer variables*/
//x-axis
int INDEX = 0;
int VALUE = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;
//y-axis
int INDEX_y= 0;
int VALUE_y = 0;
int SUM_y = 0;
int READINGS_y[WINDOW_SIZE];
int AVERAGED_y = 0;
//z-axis
int INDEX_z = 0;
int VALUE_z = 0;
int SUM_z = 0;
int READINGS_z[WINDOW_SIZE];
int AVERAGED_z = 0;

/*Accelerometer filter variables */
float new_x_reading, new_y_reading, new_z_reading;
float averaged_x, averaged_y, averaged_z;
float x_reading, y_reading, z_reading;

/*Angles*/
float filter_coeff, pitch, roll, angle;
String word_angle = "";

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void setup(void) 
{
  // LED connection pins to be set as an output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  turn_off_LEDs(); // LEDs are initially OFF
  blink_red();
  blink_green();

  Serial.begin(9600);
  pinMode(8, INPUT); // IR distance interrupter
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  // accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  accel.setRange(ADXL345_RANGE_2_G);

}

int running_average_x(float sensor_value)
{
  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  VALUE = sensor_value;
   // Read the next sensor value
  READINGS[INDEX] = VALUE;           // Add the newest reading to the window
  SUM = SUM + VALUE;                 // Add the newest reading to the sum
  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  return AVERAGED;


}
int running_average_y(float sensor_value)
{
  SUM_y = SUM_y - READINGS_y[INDEX_y];       // Remove the oldest entry from the sum
  VALUE_y = sensor_value;
   // Read the next sensor value
  READINGS_y[INDEX_y] = VALUE_y;           // Add the newest reading to the window
  SUM_y = SUM_y + VALUE_y;                 // Add the newest reading to the sum
  INDEX_y = (INDEX_y+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  AVERAGED = SUM_y / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  return AVERAGED;


}int running_average_z(float sensor_value)
{
  SUM_z = SUM_z - READINGS_z[INDEX_z];       // Remove the oldest entry from the sum
  VALUE_z = sensor_value;
   // Read the next sensor value
  READINGS_z[INDEX_z] = VALUE_z;           // Add the newest reading to the window
  SUM_z = SUM_z + VALUE_z;                 // Add the newest reading to the sum
  INDEX_z = (INDEX_z+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  AVERAGED = SUM_z / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  return AVERAGED;
}

float get_pitch(int x_reading, int y_reading, int z_reading) {
  pitch = atan2(x_reading, sqrt(y_reading^2 + z_reading^2));
  return pitch;
}

float get_roll(int x_reading, int y_reading, int z_reading) {
  roll = atan2(y_reading, sqrt(x_reading^2 + z_reading^2));
  return roll;
}

String get_word_angle(float angle) {
  /* conditions for checking angle*/
  if (angle > (3*PI/8) && angle < (3*PI/4)) {
    word_angle = "1";
  }
  else if (angle > PI/8 && angle < (3*PI/8)) { 
    word_angle = "2";
  } 
  else if (angle > -PI/8 && angle < PI/8) { 
    word_angle = "3";
  } 
  else if (angle < -PI/8 && angle > -(3*PI/8)) { 
    word_angle = "4";
  } 
  else if (angle > -(3*PI/4) && angle < -(3*PI/8)) {
    word_angle = "5";
  }
  else {
    word_angle = "-1"; // undefined angle
  }
  return word_angle;
}

int photo_interrupter_filter(int newpReading) {
  /* filter for photointeruppter */
  if (oldpReading == 0) {
    if (newpReading == 1 && oldpReading == 1 && olderpReading == 1) {
      irDigital = 1;
    }
    else {
      irDigital = 0;
    }
  }

  if (oldpReading == 1) {
    if (newpReading == 0 && oldpReading == 0 && olderpReading == 0) {
      irDigital = 0;
    }
    else {
      irDigital = 1;
    }
  }
  // updating old values for next iteration
  olderpReading = oldpReading;
  oldpReading = newpReading;

  return irDigital; // returning filtered value
}


//LED states / functions
void turn_off_LEDs() {
  analogWrite(RED, 255); // red OFF
  analogWrite(GREEN, 255); // green OFF
  analogWrite(BLUE, 255); // blue OFF
}

void fade_to_blue() {
  for(int i=0; i<255; i++) {
    analogWrite(RED, 255); // red OFF
    analogWrite(GREEN, 255); // green OFF
    analogWrite(BLUE, 255-i); // blue initially OFF
    delay(DELAY/30);
  }
}

void blue() {
  analogWrite(RED, 255); // red OFF
  analogWrite(GREEN, 255); // green OFF
  analogWrite(BLUE, 0); // blue ON
}

void blink_red() {
  for(int i=0; i<3; i++) {
    analogWrite(RED, 0); // red ON
    analogWrite(GREEN, 255); // green OFF
    analogWrite(BLUE, 255); // blue OFF
    delay(DELAY); 
    analogWrite(RED, 255); // red OFF
    delay(DELAY); 
  }
}

void blink_green() {
  for(int i=0; i<3; i++) {
    analogWrite(RED, 255); // red OFF
    analogWrite(GREEN, 0); // green ON
    analogWrite(BLUE, 255); // blue OFF
    delay(DELAY); 
    analogWrite(GREEN, 255); // green OFF
    delay(DELAY); 
  }
}


void loop(void) 
{
  
  /* reading photo interrupter input */
  int newpReading = digitalRead(8);
  irDigital = photo_interrupter_filter(newpReading);

  
  /* Get a new sensor acc event */ 
  sensors_event_t event; 
  accel.getEvent(&event);

  /* reading accelerometer values */
  new_x_reading = event.acceleration.x;
  new_y_reading = event.acceleration.y;
  new_z_reading = event.acceleration.z;

  x_reading = running_average_x(new_x_reading);
  y_reading = running_average_y(new_y_reading);
  z_reading = running_average_z(new_z_reading);


  angle = get_roll(z_reading, y_reading, z_reading);

  word_angle = get_word_angle(angle);

  /* print photo interrupter data (0 or 1) */
  Serial.print(irDigital);
  Serial.print(",");
  Serial.print(word_angle);
  Serial.println("");

  int read = Serial.read();

  if (read == 1)
  {
    on = true;
    fade_to_blue();
  }

  if (on == true)
  {
    blue();
  }

  if (read == 3)
  {
    blink_green();
    blue();
  }
  else if (read == 2)
  {
    blink_red();
    blue();
  }

}
