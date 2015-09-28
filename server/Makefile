CFLAGS=-Wall -std=c++11 -g -gstabs -Wparentheses
.PHONY : test
test : test.o MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTcpServer.o MyBaseServer.o MySockTask.o MyServer.o  MyTask.o MyServerMsgProcess.o MyServerTask.o MyHashFunction.o ConnectionLoadBalance.o
	g++ $(CFLAGS)  -o  test MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTcpServer.o MyBaseServer.o MyServer.o MyTask.o test.o MyServerMsgProcess.o MyServerTask.o MyHashFunction.o ConnectionLoadBalance.o -lmysqlclient -lpthread


MySocket.o : ../base/MySocket.cpp ../base/MySocket.h ../base/MyLock.h
	g++ $(CFLAGS) -c  ../base/MySocket.cpp 

MySockTaskManager.o : ../base/MySockTaskManager.h ../base/MySockTaskManager.cpp
	g++ $(CFLAGS) -c  ../base/MySockTaskManager.cpp

MySockTask.o : ../base/MySockTask.cpp ../base/MySockTask.h ../base/MyQueue.h ../base/MySocket.h
	g++ $(CFLAGS) -c  ../base/MySockTask.cpp 

MyThread.o : ../base/MyThread.cpp ../base/MyThread.h ../base/MyLock.h
	g++ $(CFLAGS) -c  ../base/MyThread.cpp -lpthread

MySockTaskPool.o : ../base/MySockTaskPool.cpp ../base/MySockTaskPool.h ../base/MySockTaskManager.h
	g++ $(CFLAGS) -c  ../base/MySockTaskPool.cpp

MyTcpServer.o : MyTcpServer.cpp MyTcpServer.h
	g++ $(CFLAGS) -c MyTcpServer.cpp

MyBaseServer.o : MyBaseServer.cpp MyBaseServer.h
	g++ $(CFLAGS) -c MyBaseServer.cpp

MyServer.o : MyServer.cpp MyServer.h ../base/CmdDispatcher.h
	g++ $(CFLAGS) -c MyServer.cpp

MyServerTask.o : ../base/MyTask.cpp ../base/MyTask.h MyServerTask.h MyServerTask.cpp
	g++ $(CFLAGS) -c MyServerTask.cpp
MyTask.o : ../base/MyTask.cpp ../base/MyTask.h
	g++ $(CFLAGS) -c ../base/MyTask.cpp
MyHashFunction.o : ../base/MyHashFunction.h ../base/MyHashFunction.cpp
	g++  $(CFLAGS) -c ../base/MyHashFunction.cpp
test.o : test.cpp
	g++  $(CFLAGS) -c test.cpp
MyServerMsgProcess.o : MyServerMsgProcess.cpp
	g++  $(CFLAGS) -c MyServerMsgProcess.cpp
ConnectionLoadBalance.o : ConnectionLoadBalance.h ConnectionLoadBalance.cpp 
	g++  $(CFLAGS) -c ConnectionLoadBalance.cpp ../base/MyConstHash.h
clean:
	rm test test.o MySockTaskPool.o MySocket.o MySockTask.o MyThread.o MyTcpServer.o MyBaseServer.o MyServer.o MyTask.o MySockTaskManager.o MyServerMsgProcess.o
