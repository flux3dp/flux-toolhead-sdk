/*
Self-defined toolhead firmware.
Copyright (C) 2016 Flux Software Team

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _COMMAND_h
#define _COMMAND_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/* Macros of command level error message*/
#define UNKNOW_COMMAND          "UNKNOW_COMMAND"
#define WRONG_NO_PARM           "WRONG_NO_PARM"
#define PARAM_OUT_OF_RANGE      "PARAM_OUT_OF_RANGE"
#define COMMAND_CANNOT_BE_PROCESSSED    "COMMAND_CANNOT_BE_PROCESSSED"
#define FAILLED                 "FAILLED"

/* Commnads*/
#define COMMAND_HELLO           "HELLO"
#define COMMAND_PING            "PING"
#define COMMAND_SET_LED         "SET_LED"
#define COMMNAD_DIGITALREAD     "DIGITALREAD"
#define COMMAND_DIGITALWRITE    "DIGITALWRITE"

/* Switch your prefered serial port here*/
#define MY_SERIAL Serial

class CommandClass
{
 protected:
     String receive_buffer;
     String receive_command;
     int command_pointer;
 public:
	CommandClass();
	void set_baudrate(unsigned long baud);
    void command_handler(void);
 protected:
     bool read_command_line(void);
     int16_t code_value_short(void);
     bool code_seen(String code);
     bool is_string(String str1, String str2);
     bool command_validate(String command);
     uint8_t get_checksum(String str);
     uint8_t find_command_checksum(String command);
};

#endif

