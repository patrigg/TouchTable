CC=g++-4.9
LD=ld
MKDIR_P=mkdir -p
RM=rm -f
SOURCES=$(shell echo *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=libTouchDetector.so
#SONAME=libTouchDetector.so.1
INCLUDE=
LIBDIRS=
LIBRARIES=

BIN_DIR=lib
OBJ_DIR=obj

CFLAGS=-fPIC -c -std=c++14 -Wall $(INCLUDES) $(LIB_DIRS)
LDFLAGS=-shared $(LIBRARIES)

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
#	ln -f -s $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(SONAME)

$(OBJ_DIR)/%.o: %.cpp
#	$(dirguard)
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(BIN_DIR)/*
	$(RM) $(OBJ_DIR)/*
