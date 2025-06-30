
#include <LiquidCrystal.h> 
LiquidCrystal LCD(11,10,9,2,3,4,5);  

#define trigPin 13 
#define echoPin 12 
int const buzzPin = 8;
void setup() 
{  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  
  LCD.begin(16,2); 
  LCD.setCursor(0,0);  
  LCD.print("Target Distance:");  
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  if (distance <= 10 && distance >= 0) {
    	// Buzz
    digitalWrite(buzzPin, HIGH);
    delay(50);
    digitalWrite(buzzPin, LOW);
    delay(50);
  }
  else if (distance <= 20 && distance >= 10){
    digitalWrite(buzzPin, HIGH);
    delay(100);
    digitalWrite(buzzPin, LOW);
    delay(100);
  }
 else if (distance <= 40 && distance >= 20){
    digitalWrite(buzzPin, HIGH);
    delay(200);
    digitalWrite(buzzPin, LOW);
    delay(200);
  }
  else if (distance <= 50 && distance >= 40){
    digitalWrite(buzzPin, HIGH);
    delay(500);
    digitalWrite(buzzPin, LOW);
    delay(500);
  }
  else {
    digitalWrite(buzzPin, LOW);
  }


  LCD.setCursor(0,1); 
  LCD.print("                "); 
  LCD.setCursor(0,1);   
  LCD.print(distance); 
  LCD.print(" cm");  
  delay(250); 
}

