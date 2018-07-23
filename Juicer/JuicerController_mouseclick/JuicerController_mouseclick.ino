#include <Bounce.h> // mouse functionality
Bounce space = Bounce(4, 10);

int baseline;

void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT); // LED
  baseline = analogRead(18); // initial reading from CT Sensor
  Serial.print(baseline);
}

void loop() {
  space.update();

  if (abs(baseline - analogRead(18)) > 10 ) { // when sensor reading exceeds initial reading by +/- 10,
    digitalWrite(4, HIGH); // turn on LED
    Mouse.click();         // click mouse
    Serial.println(abs(baseline - analogRead(18)));
  } else {
    digitalWrite(4, LOW);
  }
}


