## Self-defined toolhead example
**Introduction**

This example is for self-defined toolhead, which develops by using [arduino leonardo](https://www.arduino.cc/en/Main/ArduinoBoardLeonardo), you can use any 
compatible arduino you like.

There are files to interact with FluxSDK:
* command.h-command class declaration
* commnad.cpp-command class implement
* FluxSDK_Example.ino-arduino entrypoint

**Quick start**
* Hardware configuration:

> Connects the Flux delta's extension port "TX" & "RX" to the arduino leonardo "RX1" & "TX1",also the power(3.3v) and ground.

> ![Flux delta extension port](https://github.com/flux3dp/PrinterHead/blob/master/selfdefinedtoolhead_example/doc/pics/Extension%20Port%20Graphics.png)

> ![](https://www.arduino.cc/en/uploads/Main/ArduinoLeonardoFront_2.jpg)

* Download the [arduino IED](http://www.arduino.cc/en/Main/Software) ,select leonardo board and serialport ,click download the firmware to your arduino and use the FluxSDK to communicate with the new tool head!

**Funtions**
* codeSeen() : use it to find command
* codeValueShort() : get value after found command
* getChecksum() : before send back response ,you should attach the checksum

>     if (codeSeen("HELLO")) {

>         sprintf(sendBack, "1 OK HELLO TYPE:USER/MYTOOLHEAD ID:0000 VENDOR:BANANA VERSION : 0.0.1 ");

>         sprintf(sendBack, "%s*%d", sendBack, getChecksum(sendBack));

>         serialPort.println(sendBack);

>     }

* commandHandler() : handle your command here

**Protocol**
* HELLO

> 1 HELLO *[CHECKSUM]

> 1 OK HELLO TYPE:USER/MYTOOLHEAD ID:0000 VENDOR:BANANA VERSION : 0.0.1 *[CHECKSUM]

* PING

> 1 PING *[CHECKSUM]

> 1 OK PONG ER:0 *[CHECKSUM]

* YOUR_COMMAND

> 1 [COMMNAD] *[CHECKSUM]

> 1 OK *[CHECKSUM]

* MULTI_PARAMETER

> 1 [COMMNAD] [PARAMETER] [COMMNAD] [PARAMETER] *[CHECKSUM]

> 1 OK [RETURN_VALUE] *[CHECKSUM]


