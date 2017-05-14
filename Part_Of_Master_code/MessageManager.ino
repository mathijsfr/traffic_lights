SoftwareSerial mySerial(3, 5);//rx, tx

bool startMessage = false;

void InitializeMessageManager() {
  mySerial.begin(9600);
}

void SendCommand(byte command) {
  mySerial.write(START_MESSAGE_PROTOCOL);
  mySerial.write(command);
  mySerial.write(END_MESSAGE_PROTOCOL);
}

byte storedByte = 0;

byte ReceivedCommand() {
  byte returnByte = 0;

  if (mySerial.available() > 0) {
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
      else {
        storedByte = readByte;
      }
    }
  }

  return returnByte;
}
