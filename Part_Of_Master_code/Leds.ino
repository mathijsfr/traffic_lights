#define LED_PIN_RED 9
#define LED_PIN_ORANGE 10
#define LED_PIN_GREEN 11

void InitializeLeds() {
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_ORANGE, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
}

void ChangeLeds(int red, int orange, int green) {
  digitalWrite(LED_PIN_RED, red);
  digitalWrite(LED_PIN_ORANGE, orange);
  digitalWrite(LED_PIN_GREEN, green);
}
