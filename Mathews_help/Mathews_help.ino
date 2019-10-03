#define IRPin A0
#define LED 13 //Sets LED pin

//Speed Definitions
#define Slow 1
#define Fast 2

#include <math.h>

#define LED1 2 // Green
#define LED2 4 // Yellow
#define LED3 6 // Red

#define noObject 0
#define object 1
#define objectEntered 2
#define objectLeft 3

int currentState;

int timeObjectEntered;

void setup() {
  Serial.begin(9600);
  Serial.println("Mathews IR sensor program");

  pinMode(LED1, OUTPUT); //On for no object
  pinMode(LED2, OUTPUT); //On for an object
  pinMode(LED3, OUTPUT); //On after object

  pinMode(IRPin, INPUT); //input from sensor

}

void loop() 
{
  float NewRead = ReadValue(); //Read from sensor
  float Distance = FindDistance(NewRead); //Find distance
  bool isObject = FindisObject(Distance);

  currentState = states(isObject, currentState);
  updateLeds(currentState);
 }

int states(bool isObject, int previousState)
{
  int currentState = previousState;
  if(previousState == noObject) {
    if(isObject) {
      currentState = objectEntered;
      timeObjectEntered = millis();
    }
  }
  else if(previousState == objectEntered) {
    if(isObject) {
      if((timeObjectEntered+2000) <= millis()) {
        currentState = object;
      }
    }else {
      currentState = objectLeft;
    }
  }
  else if(previousState == object) {
    if(!isObject) {
      currentState = objectLeft;
    }
  }
  else if(previousState == objectLeft) {
    if(!isObject) {
      if((timeObjectEntered+2000) <= millis()) {
        currentState = noObject;
      }
    } else {
      currentState = object;
    }
  }
  
  return currentState;
}

void updateLeds(int currentState)
{
  if (currentState == objectLeft)//Newly  Left
  {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  }
  else if (currentState == object)//There
  {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  }
  
  else if (currentState == objectEntered)//Newly Entered
  {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  }
  else if (currentState == noObject)//gone 
  {
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  }
}

float ReadValue() {
  //Referenced from Wilhem's Practice Code
  int A2D = analogRead(IRPin);
  float SensorVolts = double(A2D);  //To double
  SensorVolts *= 5.0/1024;         // To Volts
  return SensorVolts;
}

float FindDistance(float ReadValue){
  float V;
  int Distance;
  V = ReadValue; //Sets Volts
  Distance = pow((V/14.475),(1/-0.8118)); //Sensor Calibration Curve
  //Serial.print("Distance: ");
  //Serial.println(Distance);
  return Distance;
}

bool FindisObject(float Distance){
  if(Distance >= 5 && Distance <=30) 
  return true;
  else 
  return false;
}

//float FindTimeOut( 
