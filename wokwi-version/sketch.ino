// ESP32-S3 Reaction Time Tester
// Wokwi version
// LED: GPIO 4
// Start Button: GPIO 5
// Reaction Button: GPIO 6

const int ledPin = 4;
const int startButtonPin = 5;
const int reactionButtonPin = 6;

unsigned long lightOnTime = 0;
unsigned long reactionTime = 0;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  // INPUT_PULLUP means:
  // not pressed = HIGH
  // pressed = LOW
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(reactionButtonPin, INPUT_PULLUP);

  digitalWrite(ledPin, LOW);

  randomSeed(micros());

  Serial.println("=== Reaction Time Tester ===");
  Serial.println("Press the START button to begin.");
}

void loop() {
  // Wait until the Start Button is pressed
  if (digitalRead(startButtonPin) == LOW) {
    delay(200); // Simple debounce

    Serial.println();
    Serial.println("Get ready...");
    digitalWrite(ledPin, LOW);

    // Random waiting time from 1 to 5 seconds
    int randomDelay = random(1000, 5001);

    unsigned long waitStart = millis();

    // During the waiting period, check if the player presses too early
    while (millis() - waitStart < randomDelay) {
      if (digitalRead(reactionButtonPin) == LOW) {
        Serial.println("Too early! You pressed before the light turned on.");
        Serial.println("Press START to try again.");

        // Wait until the player releases the button
        while (digitalRead(reactionButtonPin) == LOW) {
          delay(10);
        }

        return;
      }
    }

    // Turn on LED and start timing
    digitalWrite(ledPin, HIGH);
    lightOnTime = millis();

    Serial.println("GO! Press the REACTION button!");

    // Wait until Reaction Button is pressed
    while (digitalRead(reactionButtonPin) == HIGH) {
      // Keep waiting
    }

    reactionTime = millis() - lightOnTime;

    digitalWrite(ledPin, LOW);

    Serial.print("Your reaction time: ");
    Serial.print(reactionTime);
    Serial.println(" ms");

    Serial.println("Press START to play again.");

    // Wait until Reaction Button is released
    while (digitalRead(reactionButtonPin) == LOW) {
      delay(10);
    }

    delay(300);
  }
}
