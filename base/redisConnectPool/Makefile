CPPFLAGS = -std=c++11 -Wall -g -gstabs -I ../../lib/hiredis -L../../lib/hiredis -lhiredis
.PHONY : all clean
all : test redisOpt.o
test : test.o redisOpt.o
	g++ $(CPPFLAGS) -o test test.o redisOpt.o

test.o : test.cpp redisOpt.h
	g++ $(CPPFLAGS) -c test.cpp

redisOpt.o : redisOpt.cpp
	g++ $(CPPFLAGS) -c redisOpt.cpp 

clean:
	rm -rf test
	rm -rf test.o
	rm -rf redisOpt.o
