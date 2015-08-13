CC=g++-4.9
LD=g++-4.9
MKDIR_P=mkdir -p
RM=rm -rf
SOURCES=$(shell echo *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=TouchServer

ifndef OPENNI2_INCLUDE
    $(error OPENNI2_INCLUDE is not defined. Please define it or 'source' the OpenNIDevEnvironment file from the installation)
else ifndef OPENNI2_REDIST
    $(error OPENNI2_REDIST is not defined. Please define it or 'source' the OpenNIDevEnvironment file from the installation)
endif

INCLUDES=$(OPENNI2_INCLUDE)
LIB_DIRS=$(OPENNI2_REDIST) ../TouchDetector/lib
USED_LIBS=OpenNI2 pthread TouchDetector boost_system


BIN_DIR=bin
OBJ_DIR=obj

LIB_DIRS_OPTION= $(foreach dir,$(LIB_DIRS),-L$(dir)) -L$(BIN_DIR)
USED_LIBS_OPTION = $(foreach lib,$(USED_LIBS),-l$(lib))
INCLUDES_OPTION = $(foreach inc,$(INCLUDES),-I$(inc))


CFLAGS=-c -std=c++14 -Wall $(INCLUDES_OPTION) $(LIB_DIRS_OPTION)
LDFLAGS=$(LIB_DIRS_OPTION) $(USED_LIBS_OPTION) -Wl,-rpath,./

dirguard=@mkdir -p $(@D)

OBJ=$(OBJECTS:%.o=$(OBJ_DIR)/%.o)

all: directories $(BIN_DIR)/$(TARGET)

directories: ${BIN_DIR} ${OBJ_DIR}

${BIN_DIR}:
	${MKDIR_P} ${BIN_DIR}

${OBJ_DIR}:
	${MKDIR_P} ${OBJ_DIR}

$(BIN_DIR)/$(TARGET): $(OBJ)
#	$(dirguard)
	$(LD) $(LDFLAGS) $(OBJ) -o $@
	cp -r $(OPENNI2_REDIST)/* $(BIN_DIR)
	cp -r ../TouchDetector/lib/* $(BIN_DIR)

$(OBJ_DIR)/%.o: %.cpp
#	$(dirguard)
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(BIN_DIR)/*
	$(RM) $(OBJ_DIR)/*
