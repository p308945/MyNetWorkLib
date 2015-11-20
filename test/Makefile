CXXFLAGS=-Wall -std=c++11 -g -Wparentheses -Wl,-rpath,../lib
#CXXFLAGS=-Wall -std=c++11 -g -gstabs+
INCLUDE= -I../base -I../server -I../client  -I../proto -I../lib/tinyxml -I../include
LDFLAGS= -L../lib -lhyServer -L../lib -lhyClient -L../lib/tinyxml -ltinyxml -lprotobuf -lpthread
.PHONY : all
all : server
server : main.o Server.o XmlconfigParse.o LoginProcess.o test.pb.o MyCsvParse.o
	g++ $(CXXFLAGS) $(INCLUDE) main.o Server.o test.pb.o LoginProcess.o XmlconfigParse.o MyCsvParse.o -o $@ $(LDFLAGS) 
main.o : main.cpp
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@
Server.o : Server.cpp Server.h
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

XmlconfigParse.o : XmlconfigParse.cpp XmlconfigParse.h
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@
LoginProcess.o : LoginProcess.cpp LoginProcess.h
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

test.pb.o : ../proto/protobuf/test.pb.cc
	g++ $(CXXFLAGS) -c $< -o $@

MyCsvParse.o : ../base/MyCsvParse.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf server
	rm -rf Server.o
	rm -rf main.o
	rm -rf XmlconfigParse.o
	rm -rf LoginProcess.o
	rm -rf MyCsvParse.o
