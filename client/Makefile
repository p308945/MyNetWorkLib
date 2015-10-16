CXXFLAGS=-Wall  -std=c++11 -g -Wparentheses
LIBFLAGS=-fPIC -shared
.PHONY : all
all : test libhyClient.so
test : test.o MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTask.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o MyClientTaskManager.o MyClient.o
	g++ $(CXXFLAGS) -o test MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTask.o test.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o MyClientTaskManager.o -lmysqlclient -lpthread MyClient.o

libhyClient.so : MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTask.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o MyClientTaskManager.o
	g++ $(CXXFLAGS) $(LIBFLAGS) -o $@ MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTask.o test.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o MyClientTaskManager.o -lmysqlclient -lpthread MyClient.o
test.o : test.cpp
	g++ -c -g test.cpp
MySocket.o : ../base/MySocket.cpp ../base/MySocket.h ../base/MyLock.h
	g++ $(CXXFLAGS) -c ../base/MySocket.cpp 

MySockTaskManager.o : ../base/MySockTaskManager.h ../base/MySockTaskManager.cpp
	g++ $(CXXFLAGS) -c ../base/MySockTaskManager.cpp

MySockTask.o : ../base/MySockTask.cpp ../base/MySockTask.h ../base/MySocket.h
	g++ $(CXXFLAGS) -c ../base/MySockTask.cpp 

MyThread.o : ../base/MyThread.cpp ../base/MyThread.h ../base/MyLock.h
	g++ $(CXXFLAGS) -c ../base/MyThread.cpp -lpthread

MySockTaskPool.o : ../base/MySockTaskPool.cpp ../base/MySockTaskPool.h ../base/MySockTaskManager.h
	g++ $(CXXFLAGS) -c ../base/MySockTaskPool.cpp

MyTask.o : ../base/MyTask.cpp ../base/MyTask.h
	g++ $(CXXFLAGS) -c ../base/MyTask.cpp

MyTcpClient.o : MyTcpClient.cpp MyTcpClient.h
	g++ $(CXXFLAGS) -c MyTcpClient.cpp

MySockClientTask.o : MySockClientTask.cpp MySockClientTask.h
	g++ $(CXXFLAGS) -c MySockClientTask.cpp

MyClientTask.o : MyClientTask.cpp MyClientTask.h
	g++ $(CXXFLAGS) -c MyClientTask.cpp

MySockClientTaskPool.o : MySockClientTaskPool.cpp MySockClientTaskPool.h
	g++ $(CXXFLAGS) -c MySockClientTaskPool.cpp

MyClientTaskManager.o : MyClientTaskManager.cpp MyClientTaskManager.h
	g++ $(CXXFLAGS) -c MyClientTaskManager.cpp

MyClient.o : MyClient.cpp MyClient.h
	g++ $(CXXFLAGS) -c MyClient.cpp
clean:
	rm -rf test test.o MySockTaskPool.o MySocket.o MySockTask.o MyThread.o MyTask.o MySockTaskManager.o MySockClientTask.o MyTcpClient.o MySockClientTaskPool.o MyClientTaskManager.o MyClient.o MyClientTask.o libhyClient.so
	rm -rf ../lib/libhyClient.so
install:
	cp libhyClient.so ../lib
