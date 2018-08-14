// define input pins for juicers
#define JUICER_P1_LEFT 18
#define JUICER_P1_RIGHT 17
#define JUICER_P1_UP 16
#define JUICER_P1_DOWN 15
#define JUICER_P1_START 14

#define JUICER_P2_LEFT 13
#define JUICER_P2_RIGHT 12
#define JUICER_P2_UP 11
#define JUICER_P2_DOWN 10
#define JUICER_P2_START 9


// define output pins for LEDs
#define LED_P1_LEFT 4
#define LED_P1_RIGHT 5
#define LED_P1_UP 6
#define LED_P1_DOWN 7

#define LED_P2_LEFT 8
#define LED_P2_RIGHT 9
#define LED_P2_UP 10
#define LED_P2_DOWN 11


// define juicer key presses
#define p1Left KEY_LEFT
#define p1Right KEY_RIGHT
#define p1Up KEY_UP
#define p1Down KEY_DOWN
#define p1Start KEY_SPACE

#define p2Left KEY_A
#define p2Right KEY_D
#define p2Up KEY_W
#define p2Down KEY_S
#define p2Start KEY_ENTER


#define totalJuicers 8    // 4 juicers per player times 2 players
uint8_t inputPins[totalJuicers] = {JUICER_P1_LEFT, JUICER_P1_RIGHT, JUICER_P1_UP, JUICER_P1_DOWN, JUICER_P2_LEFT, JUICER_P2_RIGHT, JUICER_P2_UP, JUICER_P2_DOWN};
uint8_t ledOutputs[totalJuicers] = {LED_P1_LEFT, LED_P1_RIGHT, LED_P1_UP, LED_P1_DOWN, LED_P2_LEFT, LED_P2_RIGHT, LED_P2_UP, LED_P2_DOWN};
uint8_t outputKeys[totalJuicers] = {p1Left, p1Right, p1Up, p1Down, p2Left, p2Right, p2Up, p2Down};

// arrays for juicer pressed/released bools to be in order L, R, U, D for p1 & p2
byte juicersPressed[totalJuicers];
byte juicersReleased[totalJuicers];

// arrays for juicer analog readings to be in order L, R, U, D for p1 & p2
uint16_t baselineVals[totalJuicers];
uint16_t currentVals[totalJuicers];
uint16_t previousVals[totalJuicers];

// arrays for pressed juicer running avg values
const byte arraySize = 6;
long pushedVals[totalJuicers][arraySize];    // 2D array contains array for each juicer with the last "arraySize" readings of each juicer to use for averaging
byte readIndexes[totalJuicers];
long totals[totalJuicers];
float averages[totalJuicers];

// setup millis delay
long startMillis;
long currentMillis;
const long period = 7;


void setup() {
  Serial.begin(9600);

  startMillis = millis();

  initJuicers();  // initialize juicer array vals
}

void loop() {

  currentMillis = millis();
  if (currentMillis - startMillis >= period) {  // run this code every "period" (in ms)

    getVals(); // get current vals for each juicer

    for (int i = 0; i < 4; i++) {
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

void initJuicers() {  // get juicer baseline readings & initialize arrays and LEDs
  for (int i = 0; i < totalJuicers; i++) {
    baselineVals[i] = analogRead(inputPins[i]);

    pinMode(ledOutputs[i], OUTPUT);

    // arrays in order L, R, U, D for p1 & p2
    juicersPressed[i] = false;
    juicersReleased[i] = false;
    currentVals[i] = 0;
    previousVals[i] = 0;
    readIndexes[i] = 0;
    totals[i] = 0;
    averages[i] = 0;

    // init running avg 2D array
    for (int j = 0; j < arraySize; j++) {
      pushedVals[i][j] = 0;
    }
  }
}


void getVals() {  // get current values for each juicer
  for (int i = 0; i < totalJuicers; i++) {
    previousVals[i] = currentVals[i];
    currentVals[i] = analogRead(inputPins[i]);
  }

  delay(1);
}



void getJuicersPressed() {  // check if player has pressed juicer
  for (int i = 0; i < totalJuicers; i++) {
    if (!juicersReleased[i] && abs(previousVals[i] - currentVals[i]) >= 15) { // actively pressing juicer if dif between previous and current val within 15
      juicersPressed[i] = true;

      Keyboard.press(outputKeys[i]); // press corresponding output key, in order p1 L, R, U, D -> p2 L, R, U, D
      digitalWrite(ledOutputs[i], HIGH);
    }
    delay(1);
  }
}

void getJuicersReleased() {  // check if juicer has been released after being pressed
  for (int i = 0; i < totalJuicers; i++) {
    if (!juicersPressed[i] && juicersReleased[i]) {
      Keyboard.release(outputKeys[i]);
      digitalWrite(ledOutputs[i], LOW);

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

