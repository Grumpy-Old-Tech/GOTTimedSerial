# GOTTimedSerial
This library implements a serial comms methodology that throttles comms on the serial port to assist time critical applications. The library maintains send and receive buffers and processes a character each loop.

## INSTANTIATION
The SafeSerial class allows you to select the size of the send and receive buffers on instantiation. Select sizes that are application to your application

## EXECUTION
Call the execute method on the class every loop. A character will be read and sent each loop is required. If the buffer overflows, it will be cleared and a message automatically inserted.

## SENDING CHARACTERS
To send characters there are two functions, sendOnSerial and sendOnSerialLN. The second option automatically adds a new line character to the string to be sent. Both functions support String, int and float data types. Floats allow the precision to be specified.

## RECEIVING CHARACTERS
Received characters are processed into a string until a CR character is received. At that time the buffer will wait for the string to be retrieved. You can check if received data is available by using the isReceivedReady function and data can be retrieved by using the getReceivedBuffer function.
