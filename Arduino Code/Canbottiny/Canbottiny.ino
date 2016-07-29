// 2015 Coretech Robotics 
// Servo Library: http://www.cunningturtle.com/attiny4585-servo-library/
// IR Code: David Cuartielles: http://forum.arduino.cc/index.php?topic=17965.0

#include "Servo8bit.h" 
#define trigPin 0
#define echoPin 3

int ir_pin = 4;
int motorPinL = 1;
int motorPinR = 2;
int key;            
int start_bit = 2000; 
int bin_1 = 1000; 
int bin_0 = 400;   
unsigned long stoptimer;
boolean autoMode;
Servo8Bit motorL, motorR;

void setup() {

   pinMode(ir_pin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  motorL.attach(motorPinL);
  motorR.attach(motorPinR);
  //Serial.begin(9600);
  digitalWrite(0, HIGH);
}

void loop() {
  key = getIRKey();
  if (key != -1) {
    //Serial.println(key);
    if (key < 755 && key > 750){moveMotors(160, 160);} //up
    if (key < 2805 && key > 2795){moveMotors(10, 10);} //down
    if (key < 725 && key > 715){moveMotors(10, 160);} //left
    if (key < 3285 && key > 3275){moveMotors(160, 10);} //right
    if (key < 2677 && key > 2667){
      autoMode = !autoMode;
      moveMotors(94, 85);
      delay(400);
    }
    if (key < 2710 && key > 2695){
      motorL.detach();
      motorR.detach();
    }
  }
  stoptimer = millis();
}

void moveMotors(int speedL, int speedR){
  motorL.write(speedL);
  motorR.write(180-speedR);
}

int getIRKey() {
 int data[12];
 while(pulseIn(ir_pin, LOW, 20000) < 2200) {
   if(stoptimer+20 < millis() && !autoMode)   moveMotors(94, 85);
   if(autoMode){
     if(getDistance() < 10){
       moveMotors(94, 20);
       delay(400); 
     }
     else{
       moveMotors(120, 120); 
     }
   }
   key = 0;
 }
 stoptimer = millis()+1000;
 data[0] = pulseIn(ir_pin, LOW);
 data[1] = pulseIn(ir_pin, LOW);
 data[2] = pulseIn(ir_pin, LOW);
 data[3] = pulseIn(ir_pin, LOW);
 data[4] = pulseIn(ir_pin, LOW);
 data[5] = pulseIn(ir_pin, LOW);
 data[6] = pulseIn(ir_pin, LOW);
 data[7] = pulseIn(ir_pin, LOW);
 data[8] = pulseIn(ir_pin, LOW);
 data[9] = pulseIn(ir_pin, LOW);
 data[10] = pulseIn(ir_pin, LOW);
 data[11] = pulseIn(ir_pin, LOW);

 for(int i=0;i<=11;i++) {

   if(data[i] > bin_1) {
       data[i] = 1;
   }  else {
       if(data[i] > bin_0) { 
         data[i] = 0;
       } else {
        data[i] = 2; 
       }
   }
 }

 for(int i=0;i<=11;i++) {
   if(data[i] > 1) {
       return -1;     
   }
 }

 int result = 0;
 int seed = 1;
 for(int i=11;i>=0;i--) {    
   if(data[i] == 1) {
       result += seed;
   }
   seed = seed * 2;
 }

 return result;                  
}

int getDistance(){
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return (duration/2) / 29.1;
}  
