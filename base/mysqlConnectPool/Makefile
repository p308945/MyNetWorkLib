CPPFLAGS = -std=c++11 -Wall -g -gstabs -I ../
.PHONY : all clean
all : DBPool.o
test : test.o DBPool.o
	g++ $(CPPFLAGS) -o test test.o DBPool.o -lmysqlclient -lpthread

test.o : test.cpp DBDefine.h
	g++ $(CPPFLAGS) -c test.cpp

DBPool.o : DBPool.cpp
	g++ $(CPPFLAGS) -c DBPool.cpp 

clean:
	rm -rf test
	rm -rf test.o
	rm -rf DBPool.o
