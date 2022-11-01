const unsigned int PIN_LED_RED = 5;
const unsigned int PIN_LED_GREEN = 6;
const unsigned int PIN_LED_BLUE = 9;

const int ANALOG_PIN = A0;

const unsigned int BLINK_DELAY = 250;
const unsigned int BLINK_NUMBER = 7;
const unsigned int ROTATION_DELAY = 5;
const unsigned int GUESS_DELAY = 10000;

unsigned int color[3];
unsigned int randomColor;

void setup() {
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  randomSeed(analogRead(ANALOG_PIN));
}

void loop() {
  rotateColors(3);
  setColor(0, 0, 0);
  delay(1000);
  pickRandomColor();

  // accept input

  // detemine match

  // signal success
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
  randomColor = random(5);
  switch (randomColor) {
    case 0:
      setColor(255, 0, 0); // Red
    case 1:
      setColor(255, 80, 0); // Orange
    case 2:
      setColor(255, 255, 0); // Yellow
    case 3:
      setColor(0, 255, 1); // Green
    case 4:
      setColor(0, 0, 255); // Blue
    case 5:
      setColor(255, 0, 255); // Magenta
  }
  delay(GUESS_DELAY);
}

void blinkColor (unsigned int red, unsigned int green, unsigned int blue) {
  for (int i = 0; i < BLINK_NUMBER; i += 1) {
    setColor(0, 0, 0);
    delay(BLINK_DELAY);
    setColor(red, green, blue);
    delay(BLINK_DELAY);
  }
}

void setColor (unsigned int red, unsigned int green, unsigned int blue) {
 analogWrite(PIN_LED_RED, red);
 analogWrite(PIN_LED_GREEN, green);
 analogWrite(PIN_LED_BLUE, blue);
}
