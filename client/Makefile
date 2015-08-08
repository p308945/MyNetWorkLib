CFLAGS=-Wall
test : test.o MySocket.o MySockTaskManager.o MySockTask.o MyThread.o MySockTaskPool.o MyTask.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o MyClientTaskManager.o MyClient.o
	g++ -g -o test MySocket.o MySockTask.o MyThread.o MySockTaskManager.o MySockTaskPool.o  MyTask.o test.o MyTcpClient.o MySockClientTask.o MyClientTask.o MySockClientTaskPool.o MyClientTaskManager.o -lmysqlclient -lpthread MyClient.o

test.o : test.cpp
	g++ -c -g test.cpp
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

MyTask.o : ../base/MyTask.cpp ../base/MyTask.h
	g++ -c -g ../base/MyTask.cpp

MyTcpClient.o : MyTcpClient.cpp MyTcpClient.h
	g++ -c -g MyTcpClient.cpp

MySockClientTask.o : MySockClientTask.cpp MySockClientTask.h
	g++ -c -g MySockClientTask.cpp

MyClientTask.o : MyClientTask.cpp MyClientTask.h
	g++ -c -g MyClientTask.cpp

MySockClientTaskPool.o : MySockClientTaskPool.cpp MySockClientTaskPool.h
	g++ -c -g MySockClientTaskPool.cpp

MyClientTaskManager.o : MyClientTaskManager.cpp MyClientTaskManager.h
	g++ -c -g MyClientTaskManager.cpp

MyClient.o : MyClient.cpp MyClient.h
	g++ -c -g MyClient.cpp
clean:
	rm test test.o MySockTaskPool.o MySocket.o MySockTask.o MyThread.o MyTask.o MySockTaskManager.o MySockClientTask.o MyTcpClient.o MySockClientTaskPool.o MyClientTaskManager.o MyClient.o MyClientTask.o
