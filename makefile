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

#Source Settings
SOURCE =main.c

SOURCE +=src/core/hserial.c \
		 src/core/io.c \
		 src/core/adc.c \
		 src/core/alarm.c \
		 src/core/pwm.c

ifeq ($(LUA_ENABLED), yes)
SOURCE +=src/lua/lapi.c \
		 src/lua/lcode.c \
		 src/lua/lctype.c \
		 src/lua/ldebug.c \
		 src/lua/ldo.c \
		 src/lua/lfunc.c \
		 src/lua/lgc.c \
		 src/lua/llex.c \
		 src/lua/lmem.c \
		 src/lua/ldump.c \
		 src/lua/lobject.c \
		 src/lua/lopcodes.c \
		 src/lua/lparser.c \
		 src/lua/lundump.c \
		 src/lua/ldblib.c \
		 src/lua/lstate.c \
		 src/lua/lstring.c \
		 src/lua/ltable.c \
		 src/lua/ltm.c \
		 src/lua/lvm.c \
		 src/lua/lstrlib.c \
		 src/lua/lzio.c \
		 src/lua/lmathlib.c \
		 src/lua/lauxlib.c \
		 src/lua/lbaselib.c \
		 src/lua/lcorolib.c \
		 src/lua/ltablib.c \
		 src/core/luino.c 
endif

CFLAGS =-w -Os -ffunction-sections -fdata-sections

ifeq ($(LUA_ENABLED), yes)
CFLAGS+=-DLUA_USE_C89 -DLUA_C89_NUMBERS -DENABLE_LUA
# CFLAGS+=-DENABLE_LUA_MATH 
# CFLAGS+=-DENABLE_LUA_DEBUG 
# CFLAGS+=-DENABLE_LUA_STRING 
# CFLAGS+=-DENABLE_LUA_TABLE
endif 

LDFLAGS=-Wl,--gc-sections

CINCS =-I./include/core
ifeq ($(LUA_ENABLED), yes)
CINCS +=-I./include/lua
endif

BIN =bin/firmware
HEX =hex/firmware.hex
MKDIR_P =mkdir -p bin/ hex/
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
	$(CC) $(CFLAGS) -DF_CPU=$(F_CPU) $(CINCS) -mmcu=$(MCU) $(SOURCE) $(LDFLAGS) -o $(BIN)
	$(STRIP) -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $(BIN)
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $(BIN) $(HEX)

flash-uno:
	sudo $(AVRDUDE) $(AVRDUDE_FLAGS) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -P $(AVR_PORT) -b $(UPLOAD_RATE_UNO) $(AVRDUDE_FLASH)

flash-duemilanove:
	sudo $(AVRDUDE) $(AVRDUDE_FLAGS) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -P $(AVR_PORT) -b $(UPLOAD_RATE_DUEMILANOVE) $(AVRDUDE_FLASH)

clean:
	$(RM_RF)