#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Wire.h> 

float vol = 0.0,l_minute;
unsigned long currentTime;
unsigned long cloopTime;
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour

int pir1 = 4;
int pir2 = 5;// the pin that the sensor is atteched to
int state1 = LOW;             // by default, no motion detected
int state2 = LOW;
int pump1 = 6;
int pump2 = 7;
int flowmeter = A0;

void flow () // Interrupt function
{
   flow_frequency++;
}
void setup() {
  
Serial.begin(9600);

   currentTime = millis();
   cloopTime = currentTime;
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei();
   currentTime = millis();
   cloopTime = currentTime;


    pinMode(pir1,INPUT);
    pinMode(pir2,INPUT);
    pinMode(flowmeter, INPUT);
    pinMode(pump1,OUTPUT);
    pinMode(pump2,OUTPUT);
    digitalWrite(pump1,LOW);
    digitalWrite(pump2,LOW);

    lcd.begin();
    cloopTime = currentTime; // Updates cloopTime
    l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
    flow_frequency = 0; // Reset Counter
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Rate: ");
    lcd.print(l_hour);
    lcd.print(" L/H");
    l_minute = l_hour/60;
    lcd.setCursor(0,1);
    vol = vol +l_minute;
    lcd.print("Vol:");
    lcd.print(vol);
}

void loop() {
 
  int motion2 = digitalRead(pir2);
  int motion = digitalRead(pir1);
  
  interrupts();   //Enables interrupts on the Arduino
  delay(100);   //Wait 100m second 
  noInterrupts(); //Disable the interrupts on the Arduino
 
  if(motion){   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; 
    
      l_hour = (flow_frequency * 60 / 7.5);
      flow_frequency = 0; // Reset Counter
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(l_hour);
      lcd.print(" L/H");
      l_minute = l_hour/60;
      lcd.setCursor(0,1);
      vol = vol +l_minute;
      lcd.print("Vol:");
      lcd.print(vol);
      digitalWrite(pump1, HIGH); 

   }   
   
   }
  else {
    Serial.println("NOT MOVE 1 ");
    digitalWrite(pump1, LOW);
  }
  
  if(motion2){
    Serial.println("pir 2 : sabun ");
    digitalWrite(pump2, HIGH);

   //delay(100);
    }
  else {
    Serial.println("NOT MOVE 2");
    digitalWrite(pump2, LOW);
  
    }}
