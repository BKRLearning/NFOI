#include <Encoder.h>
#include <Bounce.h>

#define BUTTON_PIN 15
#define FOOD_PROCESSOR_PIN 14
#define BLENDER_PIN 13

Bounce pushbutton = Bounce(BUTTON_PIN, 15);  // 5ms debounce
byte buttonState, previousState;

Encoder knob(6, 7);
long currentPos, secondPos;

const long releaseDelay = 40;  // delay in ms before button releases

long startMillis;
long currentMillis;
const long period = 10; // delay period in ms between analog reads

void setup() {
  Serial.begin(9600);
  startMillis = millis();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(FOOD_PROCESSOR_PIN, OUTPUT);
  pinMode(BLENDER_PIN, OUTPUT);

  currentPos = knob.read();
  secondPos = currentPos - 1;

  buttonState = digitalRead(BUTTON_PIN);
  previousState = HIGH;
}

void loop() {
  pushbutton.update();

  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    currentPos = knob.read();
    startMillis = currentMillis;
  }

  if (pushbutton.fallingEdge()) {    // button was pressed
    Serial.println("button pressed");

    digitalWrite(FOOD_PROCESSOR_PIN, HIGH);
    digitalWrite(BLENDER_PIN, HIGH);
    Keyboard.press(KEY_A);
    
    delay(releaseDelay);  // delay needs to be long enough that keypress is registered by computer & mechanical devices are powered on long enough for 
    
    digitalWrite(FOOD_PROCESSOR_PIN, LOW);
    digitalWrite(BLENDER_PIN, LOW);
    Keyboard.release(KEY_A);
  }

  if (currentPos != secondPos) {      // knob has been turned
    if (currentPos > secondPos) {     // moving forward
      Serial.println("Moving forward...");
      Keyboard.press(KEY_RIGHT);
    } else {                          // moving backwards
      Serial.println("Moving backward...");
      Keyboard.press(KEY_LEFT);
    }
    secondPos = currentPos;

  } else {
    delay(releaseDelay);
    Keyboard.release(KEY_LEFT);
    Keyboard.release(KEY_RIGHT);
  }
}
