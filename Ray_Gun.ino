// Define Pins
#define ENABLE_SOUNDIE_PIN 4
#define TRIGGER_PIN A5
#define LASER_PIN 3
#define SOUND_1_PIN 5
#define SOUND_2_PIN 6
#define SOUND_3_PIN 7
#define SELECT_A_PIN 12
#define SELECT_B_PIN 13

/*
   Table of sounds, laser times and laser brightness
   format:
    { Pin to trigger sound, laser brightness, laser fade up time, laser time on, laser fade down time }
*/
int soundsAndLasers[3][5] = {
  {SOUND_1_PIN, 50, 150, 50, 150},
  {SOUND_2_PIN, 125, 80, 000, 80},
  {SOUND_3_PIN, 220, 5, 150, 5}
};

void setup() {
  Serial.begin(9600);
  Serial.println("startup");
  // Set pins
  pinMode(ENABLE_SOUNDIE_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(A4, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(SOUND_1_PIN, OUTPUT);
  pinMode(SOUND_2_PIN, OUTPUT);
  pinMode(SOUND_3_PIN, OUTPUT);
  pinMode(SELECT_A_PIN, INPUT_PULLUP);
  pinMode(SELECT_B_PIN, INPUT_PULLUP);
  digitalWrite(SOUND_1_PIN, LOW);
  digitalWrite(SOUND_2_PIN, LOW);
  digitalWrite(SOUND_3_PIN, LOW);
  digitalWrite(A4, LOW);

  enableSoundie();

}

void loop() {
  // All logic is inside a while loop to get good response from the trigger.
  if (digitalRead(TRIGGER_PIN) == LOW) {
  Serial.println("trigger pulled");
    int selection = checkSelection();
    playSound(selection);
    fireLaser(selection);
    
    // fire only once in stun mode
    if (selection == 0) {
      // fire only once in stun mode
      while (digitalRead(TRIGGER_PIN) == LOW) {
        delay(5);
      }
    }
  }
}

void enableSoundie() {
  // The Soundie board requires a pulse to its power button to fully start up.
  digitalWrite(ENABLE_SOUNDIE_PIN, HIGH);
  delay(50);
  digitalWrite(ENABLE_SOUNDIE_PIN, LOW);
}

int checkSelection() {
  int selection;
  if (digitalRead(SELECT_A_PIN) == LOW) {
    selection = 2;
  }
  else if (digitalRead(SELECT_B_PIN) == LOW) {
    selection = 1;
  }
  else {
    selection = 0;
  }
  return selection;
}

void playSound(int soundIndex) {
  Serial.print("playing sound ");
  Serial.println(soundIndex);
  digitalWrite(soundsAndLasers[soundIndex][0], HIGH);
  delay(45);
  digitalWrite(soundsAndLasers[soundIndex][0], LOW);
}

void fireLaser(int laserIndex) {
  Serial.print("firing laser ");
  Serial.println(laserIndex);
  int tempBrightness;
  for ( int fade = 0; fade <= soundsAndLasers[laserIndex][2]; fade++) {
    tempBrightness = map(fade, 0, soundsAndLasers[laserIndex][2], 0, soundsAndLasers[laserIndex][1]);
    analogWrite(LASER_PIN, tempBrightness);
    delayMicroseconds(900);
  }
  analogWrite(LASER_PIN, soundsAndLasers[laserIndex][1]);
  delay(soundsAndLasers[laserIndex][3]);

  for ( int fade = soundsAndLasers[laserIndex][4]; fade >= 0; fade--) {
    tempBrightness = map(fade, 0, soundsAndLasers[laserIndex][4], 0, soundsAndLasers[laserIndex][1]);
    analogWrite(LASER_PIN, tempBrightness);
    delayMicroseconds(900);
  }
}

