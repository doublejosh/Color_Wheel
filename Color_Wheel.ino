const unsigned int PIN_LED_RED = 5;
const unsigned int PIN_LED_GREEN = 6;
const unsigned int PIN_LED_BLUE = 9;
const unsigned int PIN_BUTTON_1 = 2;
const unsigned int PIN_BUTTON_2 = 3;
const unsigned int PIN_BUTTON_3;
const unsigned int PIN_BUTTON_4;
const unsigned int PIN_BUTTON_5;
const unsigned int PIN_BUTTON_6;

const int ANALOG_PIN = A0;

const unsigned int ROTATION_DELAY = 1;
const unsigned int GUESS_LOOP_MAX = 10000;
const unsigned int DARK_PAUSE_DELAY = 250;
const unsigned int FAIL_DELAY = 3000;
const unsigned int WIN_DELAY = 3000;
const unsigned int BLINK_DELAY = 100;
const unsigned int BLINK_NUMBER = 20;

unsigned int color[3];
boolean gameOver = true;
boolean correct = false;
unsigned int readCount;
unsigned int randomPrev;
unsigned int randomColor;

unsigned int button1;
unsigned int button2;
unsigned int button3;
unsigned int button4;
unsigned int button5;
unsigned int button6;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_BUTTON_1, INPUT);
  randomSeed(analogRead(ANALOG_PIN));
}

void loop() {
  if (gameOver) {
    // reset round
    gameOver = false;
    readCount = 0;
    rotateColors(3);
    setColor(0, 0, 0);
    delay(DARK_PAUSE_DELAY);
    pickRandomColor();
  } else if (readCount < GUESS_LOOP_MAX) {
    // accept input
    button1 = digitalRead(PIN_BUTTON_1);
    button2 = digitalRead(PIN_BUTTON_2);
//    button3 = digitalRead(PIN_BUTTON_3);
//    button4 = digitalRead(PIN_BUTTON_4);
//    button5 = digitalRead(PIN_BUTTON_5);
//    button6 = digitalRead(PIN_BUTTON_6);
    // detemine match
    if (button1 == HIGH ||
        button2 == HIGH ||
        button3 == HIGH || 
        button4 == HIGH || 
        button5 == HIGH || 
        button6 == HIGH) {

      if ((randomColor == 0 && button1 == HIGH) || 
          (randomColor == 1 && button2 == HIGH) ||
          (randomColor == 2 && button3 == HIGH) || 
          (randomColor == 3 && button4 == HIGH) ||
          (randomColor == 4 && button5 == HIGH) ||
          (randomColor == 5 && button6 == HIGH)) {
        // CORRECT!
        Serial.println("CORRECT");
        blinkColor(255, 255, 255);
        setColor(0, 0, 0);
        delay(WIN_DELAY);
        gameOver = true; 
      } else {
        // WRONG!
        Serial.println("WRONG");
        setColor(0, 0, 0);
        delay(FAIL_DELAY);
        gameOver = true;
      }
    }
    readCount++;
    delay(1);
  } else {
    // end of listening loop
    gameOver = true;
    readCount = 0;
    correct = false;
  }
}

void rotateColors (unsigned int count) {
  for (int i = 0; i < count; i += 1) {
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;

    for (int decColour = 0; decColour < 3; decColour += 1) {
      int incColour = decColour == 2 ? 0 : decColour + 1;
      // cross-fade the two colours.
      for (int i = 0; i < 255; i += 1) {
        color[decColour] -= 1;
        color[incColour] += 1;
        setColor(color[0], color[1], color[2]);
        delay(ROTATION_DELAY);
      }
    }
  }
}

void pickRandomColor () {
  while (randomPrev == randomColor) {
    randomColor = random(5);
  }
  randomPrev = randomColor;
  Serial.println(randomColor);
  setColorInt(randomColor);
}

void setColorInt (unsigned int color) {
  switch (color) {
    case 0:
      setColor(255, 0, 0); // Red
      break;
    case 1:
      setColor(255, 80, 0); // Orange
      break;
    case 2:
      setColor(255, 255, 0); // Yellow
      break;
    case 3:
      setColor(0, 255, 1); // Green
      break;
    case 4:
      setColor(0, 0, 255); // Blue
      break;
    case 5:
      setColor(255, 0, 255); // Magenta
      break;
  }
}

void blinkColor (unsigned int red, unsigned int green, unsigned int blue) {
  for (int i = 0; i < BLINK_NUMBER; i += 1) {
    setColor(0, 0, 0);
    delay(BLINK_DELAY);
    setColorInt(randomColor);
    delay(BLINK_DELAY);
  }
}

void setColor (unsigned int red, unsigned int green, unsigned int blue) {
 analogWrite(PIN_LED_RED, red);
 analogWrite(PIN_LED_GREEN, green);
 analogWrite(PIN_LED_BLUE, blue);
}
