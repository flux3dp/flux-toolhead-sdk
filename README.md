## Self-defined toolhead example
**Introduction**

This example is for self-defined toolhead, which develops by using [arduino pro mini 3.3v version](https://www.arduino.cc/en/Main/ArduinoBoardProMini), you can use any 
compatible arduino board you like.

There are files to interact with FluxSDK:
* command.h - command class declaration
* commnad.cpp - command class implement
* selfdefined_toolhead_example.ino - arduino program entrypoint

**Quick start**

* Download the [arduino IDE](http://www.arduino.cc/en/Main/Software) ,select arduino pro mini board and serialport ,click "Upload" to burn the firmware to your arduino and use the [FluxSDK](http://dev.flux3dp.com/tutorials/toolhead_control.html#communicating-with-hardware-extensions) to communicate with the new tool head!

* Hardware configuration:

> Connect the Flux delta's extension port "TX" & "RX" to the arduino pro mini "RX1" & "TX1",also the power supply(3.3v and ground) for arduino board.

> You can plug a led or any ttl component to arduino digital pin 2 and groud , then it would be driven.

> ![](https://github.com/flux3dp/selfdefined_toolhead_example/blob/master/doc/pics/connection.png)

**Using the FluxSDK**
> [See the FluxSDK document](http://dev.flux3dp.com/tutorials/toolhead_control.html#communicating-with-hardware-extensions)

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
