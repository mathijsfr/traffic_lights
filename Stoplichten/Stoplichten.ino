#include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 3);

//protocols
#define START_MESSAGE_PROTOCOL 0b10000000
#define END_MESSAGE_PROTOCOL 0b00000001
#define HEARTBEAT_PROTOCOL 0b01000000
#define RED_STATE_PROTOCOL 0b00001000
#define ORANGE_STATE_PROTOCOL 0b00000100
#define GREEN_STATE_PROTOCOL 0b00000010
#define DISCONNECTED_PROTOCOL 0b10101010

//interval
#define NOTHING_RECEIVED_INTERVAL 300

enum States {
  Red,
  Orange,
  Green,
  Disconnected
};

States state;

unsigned long prevMillis;

void setup() {
  InitializeLeds();

  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  byte receivedByte = OnReceive();

  unsigned long currentMillis = millis();
  
  if (receivedByte ==  HEARTBEAT_PROTOCOL) {
    HeartbeatOnRequest();
    prevMillis = currentMillis;
  }
  else{
    CheckIfReceivedState(receivedByte);
  }

  if (currentMillis - prevMillis >= NOTHING_RECEIVED_INTERVAL) {
    state = Disconnected;
    ChangeStates();
    prevMillis = currentMillis;
  }
}

void ChangeStates() {
  switch (state) {
    case Red:
      ChangeLeds(HIGH, 0, 0);
      break;
    case Orange:
      ChangeLeds(0, HIGH, 0);
      break;
    case Green:
      ChangeLeds(0, 0, HIGH);
      break;
    case Disconnected:
      ChangeLeds(0, HIGH, 0);
      break;
  }
}

