CXXFLAGS=-Wall -std=c++11 -g -gstabs -Wparentheses -Wl,-rpath,../lib
INCLUDE= -I../base -I../server -I../client
LDFLAGS= -L../lib -lhyServer -L../lib -lhyClient
.PHONY : all
all : server
server : main.o Server.o
	g++ $(CXXFLAGS) $(INCLUDE) main.o Server.o -o $@ $(LDFLAGS) 
main.o : main.cpp
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@
Server.o : Server.cpp Server.h
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf server
	rm -rf Server.o
	rm -rf main.o
