###############################################################################
###############################################################################
#### Blank Project 
####
#### Author: Renan Prata 										October 13, 2019
####
####
#### This makefile allows you to build the project from the command line
####
#### Instructions for using the makefile:
#### 
#### 1. Type "make" and press enter to compile/verify your program
#### 2. Check the USB port and change "PORT" value in makefile (/dev/*)
#### 3. Type "make flash-*" and press enter flash your program to the 
####    Arduino board. If you're using arduino UNO, type "make flash-uno"
###############################################################################
###############################################################################

#Compiler Settings
CC =avr-gcc
OBJCOPY =avr-objcopy
AVRDUDE =avrdude
STRIP =avr-strip

#MCU Settings
MCU =atmega328p
F_CPU =16000000L
FORMAT =ihex

LUA_ENABLED=yes
ifeq ($(LUA_ENABLED), yes)
ENABLE_LUA_MATH=yes
ENABLE_LUA_STRING=yes
endif


#Boot source
BOOT_SRC=src/boot/crt1.S

#Source Settings
SOURCE =main.c

SOURCE +=src/core/hserial.c \
		 src/core/io.c \
		 src/core/adc.c \
		 src/core/alarm.c \
		 src/core/pwm.c

ifeq ($(LUA_ENABLED), yes)
SOURCE +=src/lua/hash.c \
		 src/lua/inout.c \
		 src/lua/lex_yy.c \
		 src/lua/opcode.c \
		 src/lua/table.c \
		 src/lua/y_tab.c \
		 src/core/luino.c

ifeq ($(ENABLE_LUA_MATH), yes)
SOURCE +=src/lua/mathlib.c
endif

ifeq ($(ENABLE_LUA_STRING), yes)
SOURCE +=src/lua/strlib.c
endif

endif

CFLAGS =-w -Os -ffunction-sections -fdata-sections

USE_BOOT_FLAGS_OPT=no
ifeq ($(USE_BOOT_FLAGS_OPT), yes)
CFLAGS +=-fno-inline-small-functions -mrelax -nostartfiles boot/crt1.o
endif

ifeq ($(LUA_ENABLED), yes)
CFLAGS+=-DLUA_USE_C89 -DLUA_C89_NUMBERS -DENABLE_LUA

ifeq ($(ENABLE_LUA_MATH), yes)
CFLAGS+=-DENABLE_LUA_MATH 
endif

ifeq ($(ENABLE_LUA_STRING), yes)
CFLAGS+=-DENABLE_LUA_STRING
endif

endif

LDFLAGS=-Wl,--gc-sections

CINCS =-I./include/core
ifeq ($(LUA_ENABLED), yes)
CINCS +=-I./include/lua
endif

BIN =bin/firmware
HEX =hex/firmware.hex
BOOT =boot/crt1.o
MKDIR_P =mkdir -p bin/ hex/ boot/
RM_RF =rm -rf bin/ hex/
	
#Upload Settings
AVRDUDE_PROGRAMMER =arduino
AVR_PORT =/dev/ttyACM0
AVRDUDE_FLAGS =-F -V
AVRDUDE_FLASH =-U flash:w:$(HEX)
UPLOAD_RATE_DUEMILANOVE =57600
UPLOAD_RATE_UNO =115200

all:
	$(MKDIR_P)
	$(CC) -c $(BOOT_SRC) -o $(BOOT)
	$(CC) $(CFLAGS) -DF_CPU=$(F_CPU) $(CINCS) -mmcu=$(MCU) $(SOURCE) $(LDFLAGS) -o $(BIN)
	$(STRIP) -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $(BIN)
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $(BIN) $(HEX)

flash-uno:
	sudo $(AVRDUDE) $(AVRDUDE_FLAGS) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -P $(AVR_PORT) -b $(UPLOAD_RATE_UNO) $(AVRDUDE_FLASH)

flash-duemilanove:
	sudo $(AVRDUDE) $(AVRDUDE_FLAGS) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -P $(AVR_PORT) -b $(UPLOAD_RATE_DUEMILANOVE) $(AVRDUDE_FLASH)

clean:
	$(RM_RF)