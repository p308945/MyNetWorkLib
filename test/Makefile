CXXFLAGS=-Wall -std=c++11 -g -Wparentheses -Wl,-rpath,../lib
#CXXFLAGS=-Wall -std=c++11 -g -gstabs+
INCLUDE= -I../base -I../server -I../client -I../lib/tinyxml
LDFLAGS= -L../lib -lhyServer -L../lib -lhyClient -L../lib/tinyxml -ltinyxml
.PHONY : all
all : server
server : main.o Server.o XmlconfigParse.o
	g++ $(CXXFLAGS) $(INCLUDE) main.o Server.o XmlconfigParse.o -o $@ $(LDFLAGS) 
main.o : main.cpp
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@
Server.o : Server.cpp Server.h
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

XmlconfigParse.o : XmlconfigParse.cpp XmlconfigParse.h
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf server
	rm -rf Server.o
	rm -rf main.o
	rm -rf XmlconfigParse.o
