#include <GOTTimedSerial.h>

GOTTimedSerial serialPort(100,100); // 100 char tx buffer, 100 char rx buffer

unsigned long lastProcessedTime;

void setup() {

  serialPort.begin(9600);         // 9600 baud

  lastProcessedTime = millis();
}

void loop() {

  unsigned long loopTime = millis();
  
  if ((loopTime - lastProcessedTime) > 100) { // 100 milliSeconds

    lastProcessedTime = loopTime;
    timedSendFunction(loopTime);
    timedReceiveFunction();
  }
  serialPort.execute();
}

void timedSendFunction(unsigned long loopTime) {

  serialPort.sendOnSerial("Timer of loop: ");
  serialPort.sendOnSerial(loopTime);
  serialPort.sendOnSerialLN(".");
}

void timedReceiveFunction() {

  if (serialPort.isReceivedReady()) {

    serialPort.sendOnSerialLN(serialPort.getReceiveBuffer());
  }
}

