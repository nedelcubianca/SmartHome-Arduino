#include <Servo.h>
#include <dht_nonblocking.h>
#include <LiquidCrystal_I2C.h>

// --- DHT + LCD + Relay ---
#define DHT_SENSOR_TYPE DHT_TYPE_11
const int DHT_SENSOR_PIN = 2;
const int FAN_PIN = 3;

DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool fan_on = false;

// --- Servo + HC-SR04 + Buzzer ---
Servo servo;
const int servoPin = 10;
const int trigPin = 12;
const int echoPin = 11;
const int buzzerPin = 9;

bool door_opening = false;
unsigned long door_timer = 0;

// --- Buzzer Tone States ---
enum BuzzerPlayState {
  BUZZER_IDLE,
  BUZZER_TONE1_PLAYING,
  BUZZER_TONE2_PLAYING,
  BUZZER_TONE3_PLAYING
};
BuzzerPlayState buzzer_state = BUZZER_IDLE;
unsigned long buzzer_event_timer = 0;

const int TONE1_FREQ = 523;
const unsigned long TONE1_DURATION = 150;
const int TONE2_FREQ = 659;
const unsigned long TONE2_DURATION = 150;
const int TONE3_FREQ = 784;
const unsigned long TONE3_DURATION = 200;

const unsigned long DELAY_AFTER_TONE1 = 200;
const unsigned long DELAY_AFTER_TONE2 = 200;
const unsigned long DELAY_AFTER_TONE3 = 250;

// --- LEDs + LDR ---
const int ldrPin = A0;
const int ledPins[] = {4, 5, 6, 7};
const int numLeds = 4;

void setup() {
  Serial.begin(9600);

  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW); // Ventilator OFF inițial (releu activ LOW)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  servo.attach(servoPin);
  servo.write(180);
  lcd.init();
  lcd.backlight();
}

// --- DHT11: măsurare la 3 secunde ---
static bool measure_environment(float *temperature, float *humidity) {
  static unsigned long measurement_timestamp = millis();

  if (millis() - measurement_timestamp > 3000ul) {
    if (dht_sensor.measure(temperature, humidity) == true) {
      measurement_timestamp = millis();
      return true;
    }
  }
  return false;
}

void loop() {
  float temperature, humidity;

  if (measure_environment(&temperature, &humidity) == true) {
    if (temperature > 26.0) {
      digitalWrite(FAN_PIN, LOW); // Ventilator ON (releu activ LOW)
      if (!fan_on) {
        Serial.println("Temperatura ridicata - ventilator PORNIT");
        fan_on = true;
      }
    } else {
      digitalWrite(FAN_PIN, HIGH); // Ventilator OFF
      if (fan_on) {
        Serial.println("Temperatura OK - ventilator OPRIT");
        fan_on = false;
      }
    }

    Serial.print("Temperatura: ");
    Serial.print(temperature, 1);
    Serial.print(" °C, Umiditate: ");
    Serial.print(humidity, 1);
    Serial.println(" %");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature, 1);
    lcd.print(" C");
    
    lcd.setCursor(0, 1);
    lcd.print("Umid: ");
    lcd.print(humidity, 1);
    lcd.print("%");
  }

  handleLDR();
  handleUltrasonicAndServo();
  handleBuzzer();
}



// --- LDR + LEDs ---
void handleLDR() {
  int brightness = analogRead(ldrPin);
  int level = map(brightness, 0, 1023, 0, 100);
  if (level < 10) {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
  } else {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

// --- HC-SR04 + Servo ---
void handleUltrasonicAndServo() {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance > 0 && distance < 3 && !door_opening) {
    servo.write(20); // Open door
    door_opening = true;
    door_timer = millis();
    buzzer_state = BUZZER_TONE1_PLAYING;
    tone(buzzerPin, TONE1_FREQ);
    buzzer_event_timer = millis();
  }

  if (door_opening && millis() - door_timer > 3000) {
    servo.write(180); // Close door
    door_opening = false;
  }
}

// --- Buzzer melodie confirmare ---
void handleBuzzer() {
  unsigned long now = millis();
  switch (buzzer_state) {
    case BUZZER_TONE1_PLAYING:
      if (now - buzzer_event_timer >= TONE1_DURATION) {
        noTone(buzzerPin);
        buzzer_event_timer = now;
        buzzer_state = BUZZER_TONE2_PLAYING;
        delay(DELAY_AFTER_TONE1);
        tone(buzzerPin, TONE2_FREQ);
      }
      break;
    case BUZZER_TONE2_PLAYING:
      if (now - buzzer_event_timer >= TONE2_DURATION) {
        noTone(buzzerPin);
        buzzer_event_timer = now;
        buzzer_state = BUZZER_TONE3_PLAYING;
        delay(DELAY_AFTER_TONE2);
        tone(buzzerPin, TONE3_FREQ);
      }
      break;
    case BUZZER_TONE3_PLAYING:
      if (now - buzzer_event_timer >= TONE3_DURATION) {
        noTone(buzzerPin);
        buzzer_state = BUZZER_IDLE;
      }
      break;
    case BUZZER_IDLE:
    default:
      break;
  }
}
