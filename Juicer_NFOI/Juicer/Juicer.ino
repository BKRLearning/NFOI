#include "EmonLib.h"             // Include Emon Library
EnergyMonitor emon1;             // Create an instance

int baseline;

void setup()
{  
  Serial.begin(9600);

  pinMode(4, OUTPUT);
  baseline = analogRead(18);
  //emon1.current(18, 111.1);       // Current: input pin, calibration.
  Serial.print(baseline);
}

void loop()
{
  //Serial.println(analogRead(18));

  if(abs(baseline - analogRead(18)) > 10 ){
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }
  /*
  for(int i=0; i<analogRead(18); i++){
    Serial.print("-");
  }
  Serial.println("|");
  
  delay(100);
  */
  
//  emon1.calcVI(20,2000);         // Calculate all. No.of wavelengths, time-out
//  emon1.serialprint();           // Print out all variables
}


