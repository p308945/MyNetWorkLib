CFLAGS=-Wall
test : test.o MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTcpServer.o MyBaseServer.o MyServer.o MyTask.o MyServerMsgProcess.o
	g++ -g -o test MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTcpServer.o MyBaseServer.o MyServer.o MyTask.o test.o MyServerMsgProcess.o -lmysqlclient -lpthread


MySocket.o : ../base/MySocket.cpp ../base/MySocket.h ../base/MyLock.h
	g++ -c -g ../base/MySocket.cpp 

MySockTaskManager.o : ../base/MySockTaskManager.h ../base/MySockTaskManager.cpp
	g++ -c -g ../base/MySockTaskManager.cpp

MySockTask.o : ../base/MySockTask.cpp ../base/MySockTask.h
	g++ -c -g ../base/MySockTask.cpp 

MyThread.o : ../base/MyThread.cpp ../base/MyThread.h ../base/MyLock.h
	g++ -c -g ../base/MyThread.cpp -lpthread

MySockTaskPool.o : ../base/MySockTaskPool.cpp ../base/MySockTaskPool.h ../base/MySockTaskManager.h
	g++ -c -g ../base/MySockTaskPool.cpp

MyTcpServer.o : MyTcpServer.cpp MyTcpServer.h
	g++ -c -g MyTcpServer.cpp

MyBaseServer.o : MyBaseServer.cpp MyBaseServer.h
	g++ -c -g MyBaseServer.cpp

MyServer.o : MyServer.cpp MyServer.h
	g++ -c -g MyServer.cpp

MyTask.o : ../base/MyTask.cpp ../base/MyTask.h
	g++ -c -g ../base/MyTask.cpp
test.o : test.cpp
	g++ -c -g test.cpp
MyServerMsgProcess.o : MyServerMsgProcess.cpp
	g++ -c -g MyServerMsgProcess.cpp
clean:
	rm test test.o MySockTaskPool.o MySocket.o MySockTask.o MyThread.o MyTcpServer.o MyBaseServer.o MyServer.o MyTask.o MySockTaskManager.o MyServerMsgProcess.o
