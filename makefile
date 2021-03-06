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
ARDUINO_MODEL= $(if $(model),$(model),uno)

ifeq ($(ARDUINO_MODEL), uno)
MCU =atmega328p
endif

ifeq ($(ARDUINO_MODEL), mega)
MCU =atmega2560
endif

F_CPU =16000000L
FORMAT =ihex

LUA_ENABLED=yes
ifeq ($(LUA_ENABLED), yes)
ENABLE_LUA_MATH=yes
ENABLE_LUA_DEBUG=yes
ENABLE_LUA_STRING=yes
ENABLE_LUA_TABLE=yes
ENABLE_LUA_COROUTINE=no
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
SOURCE +=src/lua/lapi.c \
		 src/lua/lcode.c \
		 src/lua/lctype.c \
		 src/lua/ldo.c \
		 src/lua/lfunc.c \
		 src/lua/lgc.c \
		 src/lua/llex.c \
		 src/lua/lmem.c \
		 src/lua/lobject.c \
		 src/lua/lopcodes.c \
		 src/lua/lparser.c \
		 src/lua/lundump.c \
		 src/lua/ldebug.c \
		 src/lua/lstate.c \
		 src/lua/ltable.c \
		 src/lua/lvm.c \
		 src/lua/ltm.c \
		 src/lua/lstring.c \
		 src/lua/lzio.c \
		 src/lua/lauxlib.c \
		 src/lua/lbaselib.c \
		 src/core/luino.c


ifeq ($(ENABLE_LUA_DEBUG), yes) 
SOURCE +=src/lua/ldblib.c
endif

ifeq ($(ENABLE_LUA_MATH), yes)
SOURCE +=src/lua/lmathlib.c
endif

ifeq ($(ENABLE_LUA_STRING), yes)
SOURCE +=src/lua/ldump.c \
		 src/lua/lstrlib.c
endif

ifeq ($(ENABLE_LUA_COROUTINE), yes)
SOURCE +=src/lua/lcorolib.c
endif

ifeq ($(ENABLE_LUA_TABLE), yes)
SOURCE +=src/lua/ltablib.c
endif

endif

CFLAGS =-w -Os -ffunction-sections -fdata-sections

ifeq ($(ARDUINO_MODEL), mega)
CFLAGS +=-D__AVR_ATmega1280__
endif

USE_BOOT_FLAGS_OPT=no
ifeq ($(USE_BOOT_FLAGS_OPT), yes)
CFLAGS +=-fno-inline-small-functions -mrelax -nostartfiles boot/crt1.o
endif

ifeq ($(LUA_ENABLED), yes)
CFLAGS+=-DLUA_USE_C89 -DLUA_C89_NUMBERS -DENABLE_LUA

ifeq ($(ENABLE_LUA_MATH), yes)
CFLAGS+=-DENABLE_LUA_MATH 
endif

ifeq ($(ENABLE_LUA_DEBUG), yes)
CFLAGS+=-DENABLE_LUA_DEBUG
endif

ifeq ($(ENABLE_LUA_STRING), yes)
CFLAGS+=-DENABLE_LUA_STRING
endif

ifeq ($(ENABLE_LUA_COROUTINE), yes)
CFLAGS+=-DENABLE_LUA_COROUTINE
endif

ifeq ($(ENABLE_LUA_TABLE), yes)
CFLAGS+=-DENABLE_LUA_TABLE
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
ifeq ($(ARDUINO_MODEL), uno)
AVRDUDE_PROGRAMMER =arduino
AVR_PORT =/dev/ttyACM0
AVRDUDE_FLAGS =-F -V
endif

ifeq ($(ARDUINO_MODEL), mega)
AVRDUDE_PROGRAMMER =stk500v2
AVR_PORT =/dev/ttyUSB0
AVRDUDE_FLAGS =-F -V -D
endif

AVRDUDE_FLASH +=-U flash:w:$(HEX)

UPLOAD_RATE_DUEMILANOVE =57600
UPLOAD_RATE_UNO =115200
UPLOAD_RATE_MEGA =115200

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


flash-mega:
	sudo $(AVRDUDE) $(AVRDUDE_FLAGS) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -P $(AVR_PORT) -b $(UPLOAD_RATE_MEGA) $(AVRDUDE_FLASH)

clean:
	$(RM_RF)