GCC=g++ -std=c++11
APP=TLV
OUTFILE=$(APP)_server
SRC = $(wildcard src/*.cpp)
OBJS = $(SRC:%.cpp=%.o)

BOOST_LIB_DIR=/usr/lib64
#LIBEVENT2_LIB_DIR=/usr/local/lib

all:  server

server: $(OBJS)
	$(GCC) -o $(OUTFILE) $^ -Wall -L.  -L$(BOOST_LIB_DIR)  \
		-lpthread -lboost_thread -levent -levent_core -levent_extra -lboost_system \
		-ldl  -fPIC 

%.o: %.cpp
	$(GCC) -c -o $@ $< -Wall  -g 

clean :
	rm -f src/*.o $(OUTFILE) 

