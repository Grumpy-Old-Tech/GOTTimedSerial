//
//    GOTTimedSerial.cpp
//
//    MIT License
//
//    Copyright (c) Saturday 1st September 2018, Neville Kripp (Grumpy Old Tech)
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is
//    furnished to do so, subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.

#include "Arduino.h"
#include "GOTTimedSerial.h"

#if defined(BOARD_generic_stm32f103c)
    #include <itoa.h>
#endif

GOTTimedSerial::GOTTimedSerial(int txBuffSize, int rxBuffSize) {
    
    receiveBufferLen = rxBuffSize;
    sendBufferLen = txBuffSize;
    receiveReady = false;
}

void GOTTimedSerial::begin(int speed) {
    
    Serial.begin(speed);
    while (!Serial);
}

void GOTTimedSerial::execute() {
    
    byte receivedByte = 0;
    
    // Keep getting characters if a command is not ready
    if (!receiveReady) {
        
        // Read characters and build a command
        if (Serial.available()) {
            
            receivedByte = Serial.read();
            
            if (receivedByte == 13) {
                
                receiveReady = true;
            }
            else {
                
                receiveBuffer.concat((char)receivedByte);
            }
        }
    }
    
    // Send a char from the buffer if it contains characters
    if (sendBuffer.length() > 0) {
        
        int firstCharacter = sendBuffer.charAt(0);
        Serial.write(firstCharacter);
        sendBuffer.remove(0,1);
    }
}

bool GOTTimedSerial::isReceivedReady() {
    
    if (receiveReady) {
        
        return true;
    }
    return false;
}

String GOTTimedSerial::getReceiveBuffer() {
    
    if (receiveReady) {
        
        String returnString = String(receiveBuffer);
        receiveBuffer = "";
        receiveReady = false;
        return returnString;
    }
    else {
        
        return "";
    }
}

void GOTTimedSerial::sendOnSerial(String newString) {
    
    if ((sendBuffer.length() + newString.length()) > sendBufferLen) {
        
        sendBuffer = "Send Buffer Full, cleared content\n";
    }
    
    sendBuffer.concat(newString);
}

void GOTTimedSerial::sendOnSerial(int newInt) {
    
    String temp = String(newInt);
    sendOnSerial(temp);
}

void GOTTimedSerial::sendOnSerial(float newFloat, int precision) {
    
    char buffer[20];
    
    // Deposit the whole part of the number.
    long wholePart = (long) newFloat;
    itoa(wholePart,buffer,10);
    
    // Now work on the faction if we need one.
    if (precision > 0) {
        
        char *endOfString = buffer;
        while (*endOfString != '\0') endOfString++;
        *endOfString++ = '.';
        
        if (newFloat < 0) {
            
            newFloat *= -1;
            wholePart *= -1;
        }
        
        double fraction = newFloat - wholePart;
        while (precision > 0) {
            
            // Multiple by ten and pull out the digit.
            
            fraction *= 10;
            wholePart = (long) fraction;
            *endOfString++ = '0' + wholePart;
            
            // Update the fraction and move on to the
            // next digit.
            
            fraction -= wholePart;
            precision--;
        }
        
        // Terminate the string.
        *endOfString = '\0';
    }
    sendOnSerial(buffer);
}

void GOTTimedSerial::sendOnSerialLN(String newString) {
    
    sendOnSerial(newString);
    sendOnSerial("\n");
}

void GOTTimedSerial::sendOnSerialLN(int newInt) {
    
    sendOnSerial(newInt);
    sendOnSerial("\n");
}

void GOTTimedSerial::sendOnSerialLN(float newFloat, int precision) {
    
    sendOnSerial(newFloat, precision);
    sendOnSerial("\n");
}
