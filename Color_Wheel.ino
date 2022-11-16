
#include "pitches.h"

const unsigned int PIN_LED_RED = 5;
const unsigned int PIN_LED_GREEN = 6;
const unsigned int PIN_LED_BLUE = 9;

const unsigned int PIN_BUTTON_1 = 2;
const unsigned int PIN_BUTTON_2 = 3;
const unsigned int PIN_BUTTON_3 = 4;
const unsigned int PIN_BUTTON_4 = 7;
const unsigned int PIN_BUTTON_5 = 11;
const unsigned int PIN_BUTTON_6 = 10;

const int ANALOG_PIN = A0;
const int SPEAKER_PIN = 8;



int tempo = 120;

int melody[] = {
  REST,2, REST,4, REST,8, NOTE_DS4,8, 
  NOTE_E4,-4, REST,8, NOTE_FS4,8, NOTE_G4,-4, REST,8, NOTE_DS4,8,
  NOTE_E4,-8, NOTE_FS4,8,  NOTE_G4,-8, NOTE_C5,8, NOTE_B4,-8, NOTE_E4,8, NOTE_G4,-8, NOTE_B4,8,   
  NOTE_AS4,2, NOTE_A4,-16, NOTE_G4,-16, NOTE_E4,-16, NOTE_D4,-16, NOTE_E4,2, 
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;



const unsigned int ROTATION_DELAY = 1;
const unsigned int GUESS_LOOP_MAX = 10000;
const unsigned int GUESS_LOOP_INCREMENT = 1000;
const unsigned int DARK_PAUSE_DELAY = 250;
const unsigned int FAIL_DELAY = 3000;
const unsigned int WIN_DELAY = 3000;
const unsigned int BLINK_DELAY = 100;
const unsigned int BLINK_NUMBER = 12;

const unsigned int TONE_DURATION = 170;

unsigned int color[3];
boolean gameOver = true;
boolean correct = false;
unsigned int readCount;
unsigned int randomPrev;
unsigned int randomColor;
unsigned int guessLoopSpeed = GUESS_LOOP_MAX;

unsigned int button1;
unsigned int button2;
unsigned int button3;
unsigned int button4;
unsigned int button5;
unsigned int button6;

void setup () {
  Serial.begin(9600);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
  pinMode(PIN_BUTTON_1, INPUT);
  pinMode(PIN_BUTTON_2, INPUT);
  pinMode(PIN_BUTTON_3, INPUT);
  pinMode(PIN_BUTTON_4, INPUT);
  pinMode(PIN_BUTTON_5, INPUT);
  pinMode(PIN_BUTTON_6, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  randomSeed(analogRead(ANALOG_PIN));
  //playSong();
}

void loop () {
  if (gameOver) {
    // reset round
    setColor(0, 0, 0);
    gameOver = false;
    readCount = 0;
    rotateColors(3);
    setColor(0, 0, 0);
    delay(DARK_PAUSE_DELAY);
    tone(SPEAKER_PIN, 4000, TONE_DURATION / 2);
    pickRandomColor();
  } else if (readCount < guessLoopSpeed) {
    // accept input
    button1 = digitalRead(PIN_BUTTON_1);
    button2 = digitalRead(PIN_BUTTON_2);
    button3 = digitalRead(PIN_BUTTON_3);
    button4 = digitalRead(PIN_BUTTON_4);
    button5 = digitalRead(PIN_BUTTON_5);
    button6 = digitalRead(PIN_BUTTON_6);
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
        Serial.println("CORRECT");
        setColor(0, 0, 0);
        tone(SPEAKER_PIN, NOTE_C5, TONE_DURATION);
        delay(TONE_DURATION * 1.3);
        setColorInt(randomColor);
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, NOTE_C6, TONE_DURATION);
        delay(TONE_DURATION * 1.3);
        setColor(0, 0, 0);
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, NOTE_C7, TONE_DURATION * 1.3);
        delay(TONE_DURATION * 1.3);
        setColorInt(randomColor);
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, NOTE_C8, TONE_DURATION * 2);
        // playSong();
        blinkColor();
        delay(WIN_DELAY);
        gameOver = true;
      } else {
        Serial.println("WRONG");
        setColor(0, 0, 0);
        //playSong();
        tone(SPEAKER_PIN, NOTE_D6, TONE_DURATION * 2);
        delay(TONE_DURATION * 2.3);
        tone(SPEAKER_PIN, NOTE_D5, TONE_DURATION * 4);
        delay(TONE_DURATION * 4);
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

void playSong () {
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(SPEAKER_PIN, melody[thisNote], noteDuration * 0.9);   

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(SPEAKER_PIN);
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
    randomColor = random(6);
  }
  randomPrev = randomColor;
  setColorInt(randomColor);
}

void setColorInt (unsigned int color) {
Serial.println(color);
  
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

void blinkColor () {
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
