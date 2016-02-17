FILES := main.cpp functions.cpp

ifeq ($(OS),Windows_NT)
	OUTPUT := /Fericey.exe 
	CCFLAGS += /EHsc $(FILES) windows.cpp
	CC = cl
	MFLAGS := -nologo -manifest ricey.manifest -outputresource:ricey.exe;\#1
	MT := mt
else
	CCFLAGS += -w
	OUTPUT += -o
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CCFLAGS += -lX11 -std=c++11 $(FILES) linux.cpp
		OUTPUT += "ricey"
		CC = g++
	endif
	ifeq ($(UNAME_S),Darwin)
		EXTRA_SHIZ := gcc -g -O -c display.mm
		OUTPUT += ricey display.o
		CC = g++
		CCFLAGS += -framework AppKit -framework CoreGraphics -framework CoreServices $(FILES) osx.cpp
	endif
	ifneq (,$(findstring BSD,$(UNAME_S)))
		CCFLAGS += -lX11 -std=c++11 -lmemstat $(FILES) bsd.cpp
		OUTPUT += "ricey"
		CC = g++49
	endif
endif

all:
	$(EXTRA_SHIZ)
	$(CC) $(CCFLAGS) $(OUTPUT)
	$(MT) $(MFLAGS)