#include <Bounce.h>
Bounce space = Bounce(4, 10);

int baseline;

void setup()
{
  Serial.begin(9600);

  pinMode(4, OUTPUT);
  baseline = analogRead(18);
  Serial.print(baseline);
}

void loop() { 
  space.update();

  if (abs(baseline - analogRead(18)) > 10 ) {
    digitalWrite(4, HIGH);
    if (space.fallingEdge()) {
      Keyboard.press(KEY_SPACE);
      delay(20);
      Keyboard.release(KEY_SPACE);
      //Keyboard.println("space");
    }
  } else {
    digitalWrite(4, LOW);
  }
}


