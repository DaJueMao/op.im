PROJECT 	:= $(shell pwd)
INCLUDE 	:= -I $(PROJECT)/speech
LOG			:= $(PROJECT)/lib/log.cpp
TEST		:= $(PROJECT)/test
SRC 		:= $(wildcard *.cc)
SUBSRC 		:= $(wildcard *.cpp)
CXX 		:= g++
FLAGS 		:= -O3 -Wall -g -std=c++11
LIB 		:= -ljsoncpp -lncurses -lpthread
SUBLIB		:= -lcurl -lcrypto
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
	$(CXX) $^ -o $@ $(FLAGS) $(LIB) $(SUBLIB)

$(TEST)/$(SUBTARGET1) : $(TEST)/jarvis.cpp
	$(CXX) $^ -o $@ $(FLAGS) $(LIB) $(SUBLIB)

clean :
	find . -name '*.o' | xargs rm -f
	find . -name $(TARGET) | xargs rm -f
	find . -name $(SUBTARGET) | xargs rm -f
	find . -name $(SUBTARGET1) | xargs rm -f
