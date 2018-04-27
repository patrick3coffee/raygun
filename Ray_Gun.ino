// Define Pins
#define ENABLE_SOUNDIE_PIN 4
#define TRIGGER_PIN A5
#define LASER_PIN 3
#define SOUND_1_PIN 5
#define SOUND_2_PIN 6
#define SOUND_3_PIN 7
#define SELECT_A_PIN 12
#define SELECT_B_PIN 13

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
  digitalWrite(LASER_PIN, LOW);

  enableSoundie();

}

void loop() {
  // All logic is inside a while loop to get good response from the trigger.
  if (digitalRead(TRIGGER_PIN) == LOW) {
    Serial.println("trigger pulled");
    int selection = checkSelection();
    fireLaser(selection);

    // fire only once in stun mode
    if (selection == 1) {
      // fire only once in stun mode
      while (digitalRead(TRIGGER_PIN) == LOW) {
        delay(5);
      }
    }
    delay(1500);
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
    selection = 3;
  }
  else if (digitalRead(SELECT_B_PIN) == LOW) {
    selection = 2;
  }
  else {
    selection = 1;
  }
  return selection;
}

void playSound(int soundIndex) {
  Serial.print("playing sound: ");
  Serial.println(soundIndex);
  digitalWrite(SOUND_1_PIN, soundIndex & 0x01);
  digitalWrite(SOUND_2_PIN, soundIndex & 0x02);
  digitalWrite(SOUND_3_PIN, soundIndex & 0x04);
  delay(55);
  digitalWrite(SOUND_1_PIN, LOW);
  digitalWrite(SOUND_2_PIN, LOW);
  digitalWrite(SOUND_3_PIN, LOW);
}

void fireLaser(int index) {
  Serial.print("firing laser at brightness: ");
  int brightness, pulses;

  switch (index) {
    // stun brightness
    case 1:
      brightness = 10;
      pulses = 1;
      break;

    //kill brightness
    case 2:
      brightness = 20;
      pulses = 5;
      break;

    // vaporize brightness
    case 3:
      brightness = 200;
      pulses = 5;
      break;
  }

  Serial.println(brightness);


  playSound(index);
  for (int pulse = 1; pulse <= pulses; pulse++) {
    fadeLaserUp(brightness);
    delay(170);
    fadeLaserDown(brightness);
  }
  digitalWrite(LASER_PIN, LOW);

}

void fadeLaserUp(int maxBrightness) {
  for (int currentBrightness = 0; currentBrightness <= maxBrightness; currentBrightness++) {
    analogWrite(LASER_PIN, currentBrightness);
    delayMicroseconds(50);
  }
}

void fadeLaserDown(int maxBrightness) {
  for (int currentBrightness = maxBrightness; currentBrightness >= 0; currentBrightness--) {
    analogWrite(LASER_PIN, currentBrightness);
    delayMicroseconds(50);
  }
}

