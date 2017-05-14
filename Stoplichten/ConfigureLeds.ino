//pins
#define LED_PIN_RED 9
#define LED_PIN_GREEN 11
#define LED_PIN_ORANGE 10

void InitializeLeds() {
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_ORANGE, OUTPUT);
}

void ChangeLeds(int red, int orange, int green){
  digitalWrite(LED_PIN_RED, red);
  digitalWrite(LED_PIN_ORANGE, orange);
  digitalWrite(LED_PIN_GREEN, green);
}
