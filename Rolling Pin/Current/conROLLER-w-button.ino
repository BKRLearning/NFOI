#include <Encoder.h>
#include <Bounce.h>

#define RUMBLE_LEFT 21
#define RUMBLE_RIGHT 20
#define START_PIN 16
#define BUTTON_PIN 15
#define FOOD_PROCESSOR_PIN 14
#define BLENDER_PIN 13

Bounce pushbutton = Bounce(BUTTON_PIN, 5);  // 5ms debounce
Bounce startButton = Bounce(START_PIN, 5);  // 5ms debounce
byte buttonState, previousState;

Encoder knob(6, 7);
long currentPos, secondPos;

const long releaseDelay = 40;  // delay in ms before button releases

long startMillis;
long currentMillis;
long motorMillis;
const long period = 10; // delay in ms between analog reads

void setup() {
  Serial.begin(9600);
  startMillis = millis();  // start countdown clock for non blocking delay

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  //pinMode(START_PIN, INPUT_PULLUP);
  pinMode(FOOD_PROCESSOR_PIN, OUTPUT);
  pinMode(BLENDER_PIN, OUTPUT);
  pinMode(RUMBLE_LEFT, OUTPUT);
  pinMode(RUMBLE_RIGHT, OUTPUT);

  // get knob state
  currentPos = knob.read();
  secondPos = currentPos - 1;

  // get button state
  buttonState = digitalRead(BUTTON_PIN);
  previousState = HIGH;
}

void loop() {
  pushbutton.update();  // get button status
  startButton.update();

  // read encoder values at defined period (non blocking)
  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    currentPos = knob.read();
    startMillis = currentMillis;
  }

  updateMotorStatus();
  getStartButtonPress();
  getButtonPress();  // check if button was pressed, activate appliances
  getPinMovement(); // check if pin has been rolled, move player

}

void updateMotorStatus() {
  if (millis() - motorMillis >= (releaseDelay * 10))
  {
    analogWrite(RUMBLE_LEFT, 0);
    analogWrite(RUMBLE_RIGHT, 0);
  } else {
    analogWrite(RUMBLE_LEFT, 255);
    analogWrite(RUMBLE_RIGHT, 255);
  }
}

void getStartButtonPress() {
  if (startButton.fallingEdge()) {
    Keyboard.press(KEY_ENTER);
    delay(releaseDelay);  // has to be long enough that keypress is registered by computer
    Keyboard.release(KEY_ENTER);
  }
}

void getButtonPress() {
  if (pushbutton.fallingEdge()) {    // button was pressed

    Serial.println("Button pressed");
    
    // turn appliances on
    digitalWrite(FOOD_PROCESSOR_PIN, HIGH);
    digitalWrite(BLENDER_PIN, HIGH);
    Keyboard.press(KEY_A);
    
    motorMillis = millis(); // reset vibration timing
    delay(releaseDelay);  // has to be long enough that keypress is registered by computer & mechanical devices are powered on long enough for effect (min 20ms)

    // turn appliances off
    digitalWrite(FOOD_PROCESSOR_PIN, LOW);
    digitalWrite(BLENDER_PIN, LOW);
    Keyboard.release(KEY_A);
  }
}


void getPinMovement() {
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
    delay(releaseDelay);  // has to be long enough that keypress is registered by computer
    Keyboard.release(KEY_LEFT);
    Keyboard.release(KEY_RIGHT);
  }
}

