/**************************************************************/
/*  Nom del fitxer: L1_5_practica.ino                         */
/*  Data: 15/11/2018                                          */
/*  Autor: Enyu Lin, OlegSokolov,                             */
/*  Descripcio: scketch per                                   */
/*                                                            */ 
/*                                                            */
/*                                                            */
/*   Arduino ADK Mega 2560.                                   */
/**************************************************************/

#include "Accelerometer.h"
#include <Event.h>
#include <Timer.h>

const int TILT_SENSOR = 2;
const int xpin = A0;                  // x-axis of the accelerometer
const int ypin = A1; 
const float start_running_speed = 1.66;


//int tilt_state = 0; 
int counter_steps = 0;

Timer timer;
int X_last, Y_last;
int X_tmp, Y_tmp, value;
Accelerometer accel(xpin,ypin);

// State:
// 0 - walking
// 1 - running
int state = 0;

float distance_m_running = 0;
float distance_m_walking = 0;
float distance_m_total = 0;

// 1 Step: The distance covered by a single step assuming a stride length of 0.762 meters
float step_length = 0.762; 
float move_speed = 0;

void increment_steps(){
  detachInterrupt(digitalPinToInterrupt(TILT_SENSOR));
  //Serial.println("INTERUPT");
  //Serial.println(counter_steps);
  //X_tmp = analogRead(xpin);
  //Y_tmp = analogRead(ypin);
  accel.readX();
  X_tmp = accel.xValue; 
  accel.readY();
  Y_tmp = accel.yValue;
 
  
   if (((X_tmp - X_last) > 50) || ((X_tmp - X_last) < -50)){
      counter_steps++;
   }
   else if (((Y_tmp - Y_last) > 50) || ((Y_tmp - Y_last) < -50)){
      counter_steps++;
   }
   X_last = X_tmp;
   Y_last = Y_tmp;

  attachInterrupt(digitalPinToInterrupt(TILT_SENSOR), increment_steps, CHANGE);
  //counter_steps++;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TILT_SENSOR, INPUT);
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT); 
   
  accel.readX();
  X_last = accel.xValue;
  accel.readY();
  Y_last = accel.yValue;

  int tickEvent = timer.every(1000, calcSpeed, (void*) 1);
  int tickEvent2 = timer.every(1000, printSteps, (void*) 1);
  int tickEvent3 = timer.every(1000, getDist, (void*) 1);
  

  attachInterrupt(digitalPinToInterrupt(TILT_SENSOR), increment_steps, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.update();
 
}

void printSteps(void* context){
   Serial.print("Steps: ");
   Serial.println(counter_steps);
   
   Serial.print("Distance walking (m): ");
   Serial.println(distance_m_walking);
   
   Serial.print("Distance running (m): ");
   Serial.println(distance_m_running);
   
   Serial.print("Distance total (m): ");
   Serial.println(distance_m_total);
}


void getDist(void* context){
  distance_m_total = distance_m_running + distance_m_walking; 
}

void calcSpeed(void* context){
  move_speed = counter_steps * step_length; 
  Serial.print("Move speed: ");
  Serial.println(move_speed);
  if (move_speed < start_running_speed){
    distance_m_walking += move_speed;
  } else {
    distance_m_running += move_speed;
  }
  counter_steps = 0;
}
