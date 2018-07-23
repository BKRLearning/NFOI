#define JUICER_LEFT 18
#define JUICER_RIGHT 17
#define JUICER_UP 16
#define JUICER_DOWN 15

// Arrays of juicer vals in order L, R, U, D
byte juicersPressed[] = {false, false, false, false};
byte juicersReleased[] = {false, false, false, false};

// Arrays of juicer vals in order L, R, U, D
uint16_t baselineVals[4] = {0, 0, 0, 0};
uint16_t currentVals[4] = {0, 0, 0, 0};
uint16_t previousVals[4] = {0, 0, 0, 0};

// Arrays for juicer pressed values
const byte arraySize = 6;
long pushedVals[4][arraySize];


byte readIndexes[] = {0, 0, 0, 0};
long totals[] = {0, 0, 0, 0};
float averages[]  = {0, 0, 0, 0};

// Setup millis delay
long startMillis;
long currentMillis;
const long period = 7;


void setup() {
  Serial.begin(9600);

  startMillis = millis();

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < arraySize; j++) {
      pushedVals[i][j] = 0;
    }
  }

  // LED outputs
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // get juicer baseline readings
  baselineVals[0] = analogRead(JUICER_LEFT);
  baselineVals[1] = analogRead(JUICER_RIGHT);
  baselineVals[2] = analogRead(JUICER_UP);
  baselineVals[3] = analogRead(JUICER_DOWN);
}

void loop() {

  currentMillis = millis();
  if (currentMillis - startMillis >= period) {

    getVals(); // get current vals for each juicer

    // Check if any juicers have been pressed
    for (int i = 0; i < 4; i++) {
      if (juicersPressed[i]) {  // if a particular juicer was pressed, then...
        totals[i] = totals[i] - pushedVals[i][readIndexes[i]];  // subtract the last value from the running total
        pushedVals[i][readIndexes[i]] = currentVals[i]; // get the new pressed value from the current value array
        totals[i] = totals[i] + pushedVals[i][readIndexes[i]]; // add that value to the running total
        readIndexes[i]++; // update the index

        if (readIndexes[i] >= arraySize) {  // reset the index number one it gets to the array size
          readIndexes[i] = 0;
        }

        averages[i] = totals[i] / arraySize;
        delay(2);

        if (abs(averages[i] - baselineVals[i]) <= 2) {  // check if the difference between the running avg and the baseline val for any juicer is within 2
          juicersReleased[i] = true;
          juicersPressed[i] = false;
        }
      }
      delay(1);
    }

    startMillis = currentMillis;
  }


  getJuicersPressed(); // Check if player has pressed juicer
  getJuicersReleased(); // Check if player has released juicer

  delay(1);
}

// Get current values for each juicer
void getVals() {
  previousVals[0] = currentVals[0];
  currentVals[0] = analogRead(JUICER_LEFT);

  previousVals[1] = currentVals[1];
  currentVals[1] = analogRead(JUICER_RIGHT);

  previousVals[2] = currentVals[2];
  currentVals[2] = analogRead(JUICER_UP);

  previousVals[3] = currentVals[3];
  currentVals[3] = analogRead(JUICER_DOWN);

  delay(1);
}



void getJuicersPressed() {  // check if player has pressed juicer
  for (int i = 0; i < 4; i++) {
    if (!juicersReleased[i] && abs(previousVals[i] - currentVals[i]) >= 15) { // actively pressing juicer if dif between previous and current val within 5
      juicersPressed[i] = true;

      switch (i) { 
        case 0:
          Keyboard.press(KEY_LEFT);
          break;
        case 1:
          Keyboard.press(KEY_RIGHT);
          break;
        case 2:
          Keyboard.press(KEY_UP);
          break;
        case 3:
          Keyboard.press(KEY_DOWN);
          break;
      }
    }
    delay(1);
  }
}

void getJuicersReleased() {  // check if juicer has been pressed and then released
  for (int i = 0; i < 4; i++) {
    if (!juicersPressed[i] && juicersReleased[i]) {
      switch (i) { 
        case 0:
          delay(1);
          Keyboard.release(KEY_LEFT);
          break;
        case 1:
          delay(1);
          Keyboard.release(KEY_RIGHT);
          break;
        case 2:
          delay(1);
          Keyboard.release(KEY_UP);
          break;
        case 3:
          delay(1);
          Keyboard.release(KEY_DOWN);
          break;
      }

      // reset array setup for that juicer
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

