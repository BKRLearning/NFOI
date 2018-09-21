#define JUICER_LEFT 18
#define JUICER_RIGHT 17
#define JUICER_UP 16
#define JUICER_DOWN 15
#define ENTER 19

#define LED_LEFT 4
#define LED_RIGHT 5
#define LED_UP 6
#define LED_DOWN 7

// arrays for juicer pressed/released bools in order L, R, U, D, E
byte juicersPressed[] = {false, false, false, false, false};
byte juicersReleased[] = {false, false, false, false, false};

// arrays for juicer analog readings in order L, R, U, D
uint16_t baselineVals[] = {0, 0, 0, 0, 0};
uint16_t currentVals[] = {0, 0, 0, 0, 0};
uint16_t previousVals[] = {0, 0, 0, 0, 0};

// arrays for pressed juicer running avg values
const byte arraySize = 6;
long pushedVals[5][arraySize];    // 2D array contains 4 arrays with the last "arraySize" readings of each juicer to use for averaging
byte readIndexes[] = {0, 0, 0, 0, 0};
long totals[] = {0, 0, 0, 0, 0};
float averages[]  = {0, 0, 0, 0, 0};

// setup millis delay
long startMillis;
long currentMillis;
const long period = 7;



void setup() {
  Serial.begin(9600);

  startMillis = millis();

  // fill running avg arrays with zeros
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < arraySize; j++) {
      pushedVals[i][j] = 0;
    }
  }


  // get juicer baseline readings
  baselineVals[0] = analogRead(JUICER_LEFT);
  baselineVals[1] = analogRead(JUICER_RIGHT);
  baselineVals[2] = analogRead(JUICER_UP);
  baselineVals[3] = analogRead(JUICER_DOWN);
  baselineVals[4] = analogRead(ENTER);

  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_UP, OUTPUT);
  pinMode(LED_DOWN, OUTPUT);
}

//initialize the output pin for led



void loop() {

  currentMillis = millis();
  if (currentMillis - startMillis >= period) {  // run this code every "period" (in ms)

    getVals(); // get current vals for each juicer

    for (int i = 0; i < 5; i++) {
      if (juicersPressed[i]) {  // if a particular juicer was pressed, then...
        totals[i] = totals[i] - pushedVals[i][readIndexes[i]];  // subtract the last value from the running total
        pushedVals[i][readIndexes[i]] = currentVals[i]; // get the new pressed value from the current value array
        totals[i] = totals[i] + pushedVals[i][readIndexes[i]]; // add that value to the running total
        readIndexes[i]++; // update the index

        if (readIndexes[i] >= arraySize) {  // reset the index number once it gets to the max array size
          readIndexes[i] = 0;
        }

        averages[i] = totals[i] / arraySize;
        delay(1);

        if (abs(averages[i] - baselineVals[i]) <= 2) {  // check if the difference between the running avg and the baseline val for any juicer is within 2
          juicersReleased[i] = true;
          juicersPressed[i] = false;
        }
      }
      delay(1);
    }

    startMillis = currentMillis;
  }


  getJuicersPressed();    // check if player has pressed juicer
  getJuicersReleased();   // check if player has released juicer

  delay(1);
}


void getVals() {  // get current values for each juicer
  previousVals[0] = currentVals[0];
  currentVals[0] = analogRead(JUICER_LEFT);
  Serial.print("current value left: ");
  Serial.print(currentVals[0]);
  Serial.print("; ");
  previousVals[1] = currentVals[1];
  currentVals[1] = analogRead(JUICER_RIGHT);
  Serial.print("current value right: ");
  Serial.print(currentVals[1]);
  Serial.print("; ");
  previousVals[2] = currentVals[2];
  currentVals[2] = analogRead(JUICER_UP);
  Serial.print("current value up: ");
  Serial.print(currentVals[2]);
  Serial.print("; ");
  previousVals[3] = currentVals[3];
  currentVals[3] = analogRead(JUICER_DOWN);
  Serial.print("current value down: ");
  Serial.print(currentVals[3]);
  Serial.print("; ");
  previousVals[4] = currentVals[4];
  currentVals[4] = analogRead(ENTER);
  Serial.print("current value enter: ");
  Serial.print(currentVals[4]);
  Serial.println();

  delay(1);
}



void getJuicersPressed() {  // check if player has pressed juicer
  for (int i = 0; i < 5; i++) {
    if (!juicersReleased[i] && abs(previousVals[i] - currentVals[i]) >= 15) { // actively pressing juicer if dif between previous and current val within 15
      juicersPressed[i] = true;

      switch (i) {
        case 0:
          Keyboard.press(KEY_A);
          digitalWrite(LED_LEFT, HIGH);
          break;
        case 1:
          Keyboard.press(KEY_D);
          digitalWrite(LED_RIGHT, HIGH);
         break;
        case 2:
          Keyboard.press(KEY_W);
//          digitalWrite(LED_UP, HIGH);
          break;
        case 3:
          Keyboard.press(KEY_S);
//          digitalWrite(LED_DOWN, HIGH);
          break;
        case 4:
          Keyboard.press(KEY_ENTER);
          break;
      }
    }
    delay(1);
  }
}

void getJuicersReleased() {  // check if juicer has been released after being pressed
  for (int i = 0; i < 5; i++) {
    if (!juicersPressed[i] && juicersReleased[i]) {
      switch (i) {
        case 0:
          delay(1);
          Keyboard.release(KEY_A);
          digitalWrite(LED_LEFT, LOW);
          break;
        case 1:
          delay(1);
          Keyboard.release(KEY_D);
          digitalWrite(LED_RIGHT, LOW);
          break;
        case 2:
          delay(1);
          Keyboard.release(KEY_W);
//          digitalWrite(LED_UP, LOW);
          break;
        case 3:
          delay(1);
          Keyboard.release(KEY_S);
//          digitalWrite(LED_UP, LOW);
          break;
        case 4:
          delay(1);
          Keyboard.release(KEY_ENTER);
          break;
      }

      // reset averaging array setup for that juicer
      for (int j = 0; j < arraySize; j++) {
        pushedVals[i][j] = 0;
      }
      totals[i] = 0;

      // reset state vals for that juicer
      juicersPressed[i] = false;
      juicersReleased[i] = false;

    }

    delay(1);
  }
}
