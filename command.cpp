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

#include "command.h"

CommandClass::CommandClass()
{
    receive_buffer = "";
    receive_command = "";
    command_pointer = 0;
}

void CommandClass::set_baudrate(unsigned long baud)
{
	MY_SERIAL.begin(baud);
}

/**
* @brief  This function handles commands.
* @param  None
* @retval None
*/
void CommandClass::command_handler() {
    if (!read_command_line()) {
        return;
    }
    else {
        if (!command_validate(receive_command))
            return;
    }
    char sendBack[200];
    if (code_seen(COMMAND_HELLO)) {
        sprintf(sendBack, "1 OK HELLO TYPE:USER/MYTOOLHEAD ID:0000 VENDOR:BANANA VERSION : 0.0.1 ");
        sprintf(sendBack, "%s*%d", sendBack, get_checksum(sendBack));
        MY_SERIAL.println(sendBack);
    }
    else if (code_seen(COMMAND_PING)) {
        sprintf(sendBack, "1 OK PONG ER:0 ");
        sprintf(sendBack, "%s*%d", sendBack, get_checksum(sendBack));
        MY_SERIAL.println(sendBack);
    }
    else if (code_seen(COMMAND_SET_LED)) {
        uint8_t ledPWM=code_value_short();
        analogWrite(10,ledPWM);
        sprintf(sendBack, "1 OK ");
        sprintf(sendBack, "%s*%d", sendBack, get_checksum(sendBack));
        MY_SERIAL.println(sendBack);
    }
    else if (code_seen(COMMNAD_DIGITALREAD)) {
        uint8_t pin = code_value_short();
        int pinState = digitalRead(pin);
        sprintf(sendBack, "1 OK %d ", pinState);
        sprintf(sendBack, "%s*%d", sendBack, get_checksum(sendBack));
        MY_SERIAL.println(sendBack);
    }
    else if (code_seen(COMMAND_DIGITALWRITE)) {
        uint8_t pin = code_value_short();
        if (code_seen("STATE"))
            digitalWrite(pin,code_value_short());
        sprintf(sendBack, "1 OK ");
        sprintf(sendBack, "%s*%d", sendBack, get_checksum(sendBack));
        MY_SERIAL.println(sendBack);
	}
	else {
		sprintf(sendBack, "1 ER FAILLED ");
		sprintf(sendBack, "%s*%d", sendBack, get_checksum(sendBack));
		MY_SERIAL.println(sendBack);	
	}
}

/**
* @brief  This function validates the checksum of received string.
* @param  command: received string.
* @retval None
*/
bool CommandClass::command_validate(String command) {
    uint8_t checksum = 0;
    String str_temp = "";
    for (int strPtr = command.length() - 1; strPtr >= 0; strPtr--) {
        if (command.charAt(strPtr) == '*') {
            str_temp = command.substring(0, strPtr );
            
            checksum= get_checksum(str_temp);
            break;
        }
    }
    if (find_command_checksum(command) == checksum || 1)
        return true;
    else
        return false;
}

/**
* @brief  This function caculates a checksum of a string by xor all of charactors.
* @param  str: a string.
* @retval checksum.
*/
uint8_t CommandClass::get_checksum(String str) {
    uint8_t checksum = 0;
    for (int strPtr=0; strPtr<str.length(); strPtr++) {
        checksum = checksum^str.charAt(strPtr);
    }
    return checksum;
}

/**
* @brief  This function finds checksum of received string.
* @param  command: received string.
* @retval Received checksum.
*/
uint8_t CommandClass::find_command_checksum(String command) {
    for (int cmd_ptr = command.length() - 1; cmd_ptr >= 0; cmd_ptr--) {
        if (command.charAt(cmd_ptr) == '*')
            return atoi((command.substring(cmd_ptr + 1).c_str()));
    }
}

/**
* @brief  This function compares two strings.
* @param  str1,str2
* @retval return true if two strings are the same.
*/
bool CommandClass::is_string(String str1, String str2) {
    if (str1.compareTo(str2) == 0)
        return true;
    else
        return false;
}

/**
* @brief  This function finds substring from the received command and moves the command pointer.
* @param  code: string to be found.
* @retval return true if code is found in the command.
*/
bool CommandClass::code_seen(String code) {
    String str_target=" "+ code;
    for (int cmd_ptr = command_pointer; cmd_ptr < receive_command.length(); cmd_ptr++) {
        if (receive_command.charAt(cmd_ptr) == str_target.charAt(0)) {
            for (int code_ptr = 1; code_ptr < str_target.length(); code_ptr++) {
                if (receive_command.charAt(cmd_ptr + code_ptr) != str_target.charAt(0 + code_ptr)) {
                    return false;
                }
            }
            command_pointer = cmd_ptr + str_target.length()+1;
            return true;
        }
    }
    return false;
}

/**
* @brief  This function transforms command string to signed integer beginning with the command pointer.
* @param  NONE
* @retval Found integer.
*/
int16_t CommandClass::code_value_short(void) {
    char str_temp[125];
    receive_command.toCharArray(str_temp, 125, command_pointer);
    return strtod(str_temp, NULL);
}

/**
* @brief  This function reads all characters until received a line character.
* @param  NONE
* @retval Return true if received a line.
*/
bool CommandClass::read_command_line(void) {
    char get_char;
    if (MY_SERIAL.available() > 0) {
        get_char = MY_SERIAL.read();
        if (receive_buffer.compareTo("") == 0 && get_char == '1') {
            receive_buffer.concat('1');
        }
        else if (get_char != '\n') {
            if (get_char != '\r')
                receive_buffer.concat(get_char);
        }
        else if (get_char == '\n') {
            receive_command = receive_buffer;
            receive_buffer = "";
            command_pointer = 0;
            return true;
        }
        return false;
    }
    else {
        return false;
    }
}
