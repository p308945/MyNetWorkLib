CXXFLAGS=-Wall -std=c++11 -g -Wparentheses -Wsequence-point
LIBFLAGS=-fPIC -shared
.PHONY : all
all : test libhyServer.so
test : test.o MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTcpServer.o MyBaseServer.o MySockTask.o MyServer.o  MyTask.o MyServerMsgProcess.o MyServerTask.o MyHashFunction.o ConnectionLoadBalance.o
	g++ $(CXXFLAGS)  -o  $@ MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTcpServer.o MyBaseServer.o MyServer.o MyTask.o test.o MyServerMsgProcess.o MyServerTask.o MyHashFunction.o ConnectionLoadBalance.o -lmysqlclient -lpthread

libhyServer.so : MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTcpServer.o MyBaseServer.o MySockTask.o MyTask.o MyServerMsgProcess.o MyServerTask.o MyHashFunction.o ConnectionLoadBalance.o
	g++ $(CXXFLAGS) $(LIBFLAGS) -o $@ MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTcpServer.o MyBaseServer.o MyServer.o MyTask.o test.o MyServerMsgProcess.o MyServerTask.o MyHashFunction.o ConnectionLoadBalance.o -lmysqlclient -lpthread

MySocket.o : ../base/MySocket.cpp ../base/MySocket.h ../base/MyLock.h
	g++ $(CXXFLAGS) -c  ../base/MySocket.cpp 

MySockTaskManager.o : ../base/MySockTaskManager.h ../base/MySockTaskManager.cpp
	g++ $(CXXFLAGS) -c  ../base/MySockTaskManager.cpp

MySockTask.o : ../base/MySockTask.cpp ../base/MySockTask.h ../base/MyQueue.h ../base/MySocket.h
	g++ $(CXXFLAGS) -c  ../base/MySockTask.cpp 

MyThread.o : ../base/MyThread.cpp ../base/MyThread.h ../base/MyLock.h
	g++ $(CXXFLAGS) -c  ../base/MyThread.cpp -lpthread

MySockTaskPool.o : ../base/MySockTaskPool.cpp ../base/MySockTaskPool.h ../base/MySockTaskManager.h
	g++ $(CXXFLAGS) -c  ../base/MySockTaskPool.cpp

MyTcpServer.o : MyTcpServer.cpp MyTcpServer.h
	g++ $(CXXFLAGS) -c MyTcpServer.cpp

MyBaseServer.o : MyBaseServer.cpp MyBaseServer.h
	g++ $(CXXFLAGS) -c MyBaseServer.cpp

MyServer.o : MyServer.cpp MyServer.h ../base/CmdDispatcher.h
	g++ $(CXXFLAGS) -c MyServer.cpp

MyServerTask.o : ../base/MyTask.cpp ../base/MyTask.h MyServerTask.h MyServerTask.cpp
	g++ $(CXXFLAGS) -c MyServerTask.cpp
MyTask.o : ../base/MyTask.cpp ../base/MyTask.h
	g++ $(CXXFLAGS) -c ../base/MyTask.cpp
MyHashFunction.o : ../base/MyHashFunction.h ../base/MyHashFunction.cpp
	g++  $(CXXFLAGS) -c ../base/MyHashFunction.cpp
test.o : test.cpp
	g++  $(CXXFLAGS) -c test.cpp
MyServerMsgProcess.o : MyServerMsgProcess.cpp
	g++  $(CXXFLAGS) -c MyServerMsgProcess.cpp
ConnectionLoadBalance.o : ConnectionLoadBalance.h ConnectionLoadBalance.cpp 
	g++  $(CXXFLAGS) -c ConnectionLoadBalance.cpp ../base/MyConstHash.h
clean:
	rm -rf test test.o MySockTaskPool.o MySocket.o MySockTask.o MyThread.o MyTcpServer.o MyBaseServer.o MyServerTask.o MyServer.o MyTask.o MySockTaskManager.o MyServerMsgProcess.o libhyServer.so MyHashFunction.o ConnectionLoadBalance.o MyHashFunction.o
	rm -rf ../lib/libhyServer.so
install:
	cp libhyServer.so ../lib
