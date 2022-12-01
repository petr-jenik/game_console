TARGET = game_console

CC = g++
CFLAGS = -g -DARDUINO=101 -D__cplusplus1

OUTDIR = ./bin
DATADIR = ./data
SUBDIR = src ArduinoCore-API/api ArduinoCore-API/test/include adafruit_driver
DIR_OBJ = ./obj

INCS = $(wildcard libs/*.h $(foreach fd, $(SUBDIR), $(fd)/*.h))
INCS += $(wildcard libs/*.hpp $(foreach fd, $(SUBDIR), $(fd)/*.hpp))
CPP_SRCS = $(wildcard $(foreach fd, $(SUBDIR), $(fd)/*.cpp))
C_SRCS += $(wildcard $(foreach fd, $(SUBDIR), $(fd)/*.c))


#ARDUINO_CORE_IMPLEMENTATION_INC=ArduinoCore-API/test/include
ARDUINO_CORE_IMPLEMENTATION=ArduinoCore-API/test/src

#INCS += $(wildcard $(foreach fd, $(ARDUINO_CORE_IMPLEMENTATION_INC), $(fd)/*.h))
CPP_SRCS += $(ARDUINO_CORE_IMPLEMENTATION)/dtostrf.cpp
CPP_SRCS += $(ARDUINO_CORE_IMPLEMENTATION)/itoa.cpp
CPP_SRCS += $(ARDUINO_CORE_IMPLEMENTATION)/MillisFake.cpp

SRCS = $(C_SRCS) $(CPP_SRCS)
#SRCS += $(ARDUINO_CORE_IMPLEMENTATION)/PrintMock.cpp
#SRCS += $(ARDUINO_CORE_IMPLEMENTATION)/StreamMock.cpp

NODIR_SRC = $(notdir $(SRCS))
CPP_OBJS = $(addprefix $(DIR_OBJ)/, $(CPP_SRCS:cpp=o)) # obj/xxx.o obj/folder/xxx .o
C_OBJS += $(addprefix $(DIR_OBJ)/, $(C_SRCS:c=o)) # obj/xxx.o obj/folder/xxx .o
OBJS = $(CPP_OBJS) $(C_OBJS)

INC_DIRS = -Ilibs $(addprefix -I, $(SUBDIR))
LIBS = -llibzmq-v141-mt-4_3_4
LIB_DIRS = -Llibs

PHONY := $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(OUTDIR)/$@ $(OBJS) $(LIB_DIRS) $(LIBS)

$(DIR_OBJ)/%.o: %.cpp $(INCS)
	mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $< $(INC_DIRS)

$(DIR_OBJ)/%.o: %.c $(INCS)
	mkdir -p $(@D)
	$(CC) -o $@ $(CFLAGS) -c $< $(INC_DIRS)

PHONY += clean
clean:
	rm -rf $(OUTDIR)/* $(DATADIR)/* $(DIR_OBJ)/*

PHONY += echoes
echoes:
	@echo "INC files: $(INCS)"
	@echo "SRC files: $(SRCS)"
	@echo "OBJ files: $(OBJS)"
	@echo "LIB files: $(LIBS)"
	@echo "INC DIR: $(INC_DIRS)"
	@echo "LIB DIR: $(LIB_DIRS)"

.PHONY = $(PHONY)
