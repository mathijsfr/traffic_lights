bool startMessage = false;

byte storedByte = 0;
byte OnReceive() {
  byte returnByte = 0;

  if (mySerial.available() > 0)
  {
    byte readByte = mySerial.read();

    if (!startMessage) {
      if (readByte == START_MESSAGE_PROTOCOL) {
        startMessage = true;
        storedByte = 0;
      }
    }
    else {
      if (readByte == END_MESSAGE_PROTOCOL) {
        startMessage = false;
        returnByte = storedByte;
      }
      else
      {
        storedByte = readByte;
      }
    }
  }

  return returnByte;
}

void HeartbeatOnRequest() {
  switch (state) {
    case Red:
      SendAcknowledge(RED_STATE_PROTOCOL);
      break;
    case Orange:
      SendAcknowledge(ORANGE_STATE_PROTOCOL);
      break;
    case Green:
      SendAcknowledge(GREEN_STATE_PROTOCOL);
      break;
    case Disconnected:
      SendAcknowledge(DISCONNECTED_PROTOCOL);
      break;
  }
}

void CheckIfReceivedState(byte readByte) {
  switch (readByte) {
    case RED_STATE_PROTOCOL:
      SendAcknowledge(RED_STATE_PROTOCOL);
      state = Red;
      ChangeStates();
      break;
    case ORANGE_STATE_PROTOCOL:
      SendAcknowledge(ORANGE_STATE_PROTOCOL);
      state = Orange;
      ChangeStates();
      break;
    case GREEN_STATE_PROTOCOL:
      SendAcknowledge(GREEN_STATE_PROTOCOL);
      state = Green;
      ChangeStates();
      break;
    case DISCONNECTED_PROTOCOL:
      SendAcknowledge(DISCONNECTED_PROTOCOL);
      state = Disconnected;
      ChangeStates();
    default:
      break;
  }
}

void SendAcknowledge(byte message) {
  mySerial.write(START_MESSAGE_PROTOCOL);
  mySerial.write(message);
  mySerial.write(END_MESSAGE_PROTOCOL);
}

