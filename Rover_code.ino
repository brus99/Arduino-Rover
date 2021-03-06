#include <Stepper.h>
const int stepsPerRev = 180;                                 //steps per full turn of motor shaft
Stepper motorOne = Stepper(stepsPerRev, 5, 6, 7, 8);       //initialize an object of stepper class
Stepper motorTwo = Stepper(stepsPerRev, 9, 10, 11, 12);       //initialize an object of stepper class
int dur;
int arrLength = 0;

int distanceTravelled[5];
int startingCoordinates = 0;                      //0 = North, 1 = East, 2 = South, 3 = West

int stepCountOne = 0;
int stepCountTwo = 0;


const int trigPin = 12;           //sends out signal at 340 m/s
const int echoPin = 13;           //recieves signal
const int acceptableRange = 15; 


void setup() {
motorOne.setSpeed(20);   // RPM
motorTwo.setSpeed(20);  

pinMode(echoPin, INPUT);
pinMode(trigPin, OUTPUT);
Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin,LOW);                            //Starting our trig pin at a low
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);                           //sending out trig pin signal
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);                            //returning trig pin to low state               
  dur =pulseIn(echoPin,HIGH);                            //using built in pulse method to receive time between trig out and echo in
  int calcDist =calcCenti(dur);                     //calculate distance in cm
  Serial.println(String(calcDist));                 //display distance on serial monitor
   
  motorOne.step(30);
  motorTwo.step(30);
  delay(100);
  stepCountOne += 30;               //advance 30
  stepCountTwo += 30;               
  if(calcDist < acceptableRange) {
    distanceTravelled[arrLength] = stepCountOne;       //steps will always be equal since it resets to 0 after a turn, so one array is required
    arrLength += 1;
    motorOne.step(30);                       //turning left
    motorTwo.step(-30);
    delay(100);
    stepCountOne = 0;                       //reset and allow the robot to continue moving straight
    stepCountTwo = 0;
    
  } 
  if(stepCountOne >500) {
    lineUp180();
    returningHome(distanceTravelled, arrLength);
  }
  

  
}


long calcCenti(long duration){
  return (dur*.0343)/2;            //calculating distance knowing the signal sent out is 340 m/s
}

void lineUp180() {
  motorOne.step(-60);
  motorTwo.step(60);
  
}

void returningHome(int distanceTravelled[], int arrLength) {
  for(int i = arrLength; i > 0; --i) {
    motorOne.step(distanceTravelled[i]);
    motorTwo.step(distanceTravelled[i]);
    motorOne.step(-30);
    motorTwo.step(30);
  
    
  }
}
