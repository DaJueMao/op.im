PROJECT 	:= $(shell pwd)
LOG			:= $(PROJECT)/lib/log.cpp
SRC 		:= $(wildcard *.cc)
SUBSRC 		:= $(wildcard *.cpp)

CXX 		:= g++
FLAGS 		:= -O3 -Wall -g -std=c++11
LIB 		:= -ljsoncpp -lncurses -lpthread
BIN			:= $(PROJECT)/bin
OBJECT		:= $(patsubst %.cpp, %.o, $(LOG))

TARGET 		:= server
SUBTARGET 	:= client
SUBTARGET1 	:= Jarvis

all : $(BIN)/$(TARGET) $(BIN)/$(SUBTARGET) $(TEST)/$(SUBTARGET1)

objs : $(OBJECT)

$(BIN)/$(TARGET) : $(SRC) $(OBJECT)
	[ -e $(BIN) ] || mkdir -p $(BIN)
	$(CXX) $^ -o $@ $(FLAGS) $(LIB)

$(BIN)/$(SUBTARGET) : $(SUBSRC) $(OBJECT)
	$(CXX) $^ -o $@ $(FLAGS) $(LIB)

clean :
	find . -name '*.o' | xargs rm -f
	find . -name $(TARGET) | xargs rm -f
	find . -name $(SUBTARGET) | xargs rm -f
