#include <SoftwareSerial.h>

#define START_MESSAGE_PROTOCOL 0b10000000
#define END_MESSAGE_PROTOCOL 0b00000001
#define HEARTBEAT_PROTOCOL 0b01000000
#define RED_STATE_PROTOCOL 0b00001000
#define ORANGE_STATE_PROTOCOL 0b00000100
#define GREEN_STATE_PROTOCOL 0b00000010
#define SLAVE_DISCONNECTED_PROTOCOL 0b10101010

unsigned long Time = 0;
unsigned long TimeHeartbeat = 0;
unsigned long TimeHeartbeatSend = 0;

int divider = 2;

bool noConnectionLightOn = false;
bool doStateOnce = true;

enum SlaveStates {
  slaveRed,
  slaveOrange,
  slaveGreen,
  slaveDisconnected
};

SlaveStates slaveState;
SlaveStates expectedSlaveState;

enum MasterStates {
  masterRed,
  masterOrange,
  masterGreen,
  masterSlaveToRed,
  masterSlaveToOrange,
  masterSlaveToGreen,
  masterNoAcknowledge
};

MasterStates MasterState;

//SETUP
void setup() {
  Serial.begin(9600);

  InitializeMessageManager();
  InitializeLeds();
  InitializeProgram();
}

//INITIALIZEPROGRAM
void InitializeProgram() {
  MasterState = masterRed;
  expectedSlaveState = slaveRed;
  SendCommand(RED_STATE_PROTOCOL);
}

//LOOP
void loop() {
  byte command = ReceivedCommand();

  GetSlaveState(command);
  MeasureHeartbeat();

  switch (MasterState) {
    case masterRed:

      if (doStateOnce) {
        ChangeLeds(1, 0, 0);
        doStateOnce = false;
      }

      if (millis() - Time >= 3000 / divider) {
        Time = millis();
        MasterState = masterSlaveToGreen;
      }
      break;
    case masterSlaveToGreen:

      if (doStateOnce) {
        SendCommand(GREEN_STATE_PROTOCOL);
        expectedSlaveState = slaveGreen;
        doStateOnce = false;
      }

      if (millis() - Time > 10000 / divider) {
        if (slaveState == expectedSlaveState) {
          Time = millis();
          MasterState = masterSlaveToOrange;
        }
        else {
          MasterState = masterNoAcknowledge;
        }
      }
      break;
    case masterSlaveToOrange:

      if (doStateOnce) {
        SendCommand(ORANGE_STATE_PROTOCOL);
        expectedSlaveState = slaveOrange;
        doStateOnce = false;
      }

      if (millis() - Time > 3000 / divider) {
        if (slaveState == expectedSlaveState) {
          Time = millis();
          MasterState = masterSlaveToRed;
        }
        else {
          MasterState = masterNoAcknowledge;
        }
      }
      break;
    case masterSlaveToRed:

      if (doStateOnce) {
        SendCommand(RED_STATE_PROTOCOL);
        expectedSlaveState = slaveRed;
        doStateOnce = false;
      }

      if (millis() - Time > 3000 / divider) {
        if (slaveState == expectedSlaveState) {
          Time = millis();
          MasterState = masterGreen;
        }
        else {
          MasterState = masterNoAcknowledge;
        }
      }
      break;
    case masterGreen:

      if (doStateOnce) {
        ChangeLeds(0, 0, 1);
        doStateOnce = false;
      }

      if (millis() - Time > 10000 / divider) {
        Time = millis();
        MasterState = masterOrange;
      }
      break;
    case masterOrange:
      if (doStateOnce) {
        ChangeLeds(0, 1, 0);
        doStateOnce = false;
      }

      if (millis() - Time > 3000 / divider) {
        Time = millis();
        MasterState = masterRed;
      }
      break;
    case masterNoAcknowledge:
      NoConnectionMode();
      break;
  }
}

void NoConnectionMode() {
  if (!noConnectionLightOn) {
    SendCommand(SLAVE_DISCONNECTED_PROTOCOL);
    expectedSlaveState = slaveDisconnected;
    ChangeLeds(0, 1, 0);
    noConnectionLightOn = true;
  }
}

void MeasureHeartbeat() {
  if (millis() - TimeHeartbeatSend > 100) {
    SendCommand(HEARTBEAT_PROTOCOL);
    TimeHeartbeatSend = millis();
  }

  if (millis() - TimeHeartbeat > 500) {

    if (MasterState != masterNoAcknowledge) {
      MasterState = masterNoAcknowledge;
    }
    TimeHeartbeat = millis();
  }
  else {

    if (slaveState == expectedSlaveState) {
      TimeHeartbeat = millis();

      if (slaveState == slaveDisconnected) {
        InitializeProgram();
        noConnectionLightOn = false;
      }
    }
  }
}

void GetSlaveState(byte readByte) {
  switch (readByte) {
    case RED_STATE_PROTOCOL:
      slaveState = slaveRed;
      break;
    case ORANGE_STATE_PROTOCOL:
      slaveState = slaveOrange;
      break;
    case GREEN_STATE_PROTOCOL:
      slaveState = slaveGreen;
      break;
    case SLAVE_DISCONNECTED_PROTOCOL:
      slaveState = slaveDisconnected;
      break;
    default:
      break;
  }
}

