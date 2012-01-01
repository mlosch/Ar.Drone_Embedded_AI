OBJECTS=main.o UDPClient.o ATClient.o Thread.o NavDataObserver.o NavDataReceiver.o Logger.o Trajectory.o MotionController.o
all: $(OBJECTS)
	arm-none-eabi-g++ -lpthread -lrt $(OBJECTS) -o Release/main.out
main.o:
	arm-none-eabi-g++ -c main.cpp
Logger.o: Common/Logger.h
	arm-none-eabi-g++ -c Common/Logger.cpp
UDPClient.o: Common/Logger.h Communication/UDPClient.h
	arm-none-eabi-g++ -lexception -c Communication/UDPClient.cpp
ATClient.o: Communication/ATClient.h Communication/UDPClient.h Common/Thread.h
	arm-none-eabi-g++ -c Communication/ATClient.cpp
Thread.o: Common/Thread.h
	arm-none-eabi-g++ -c Common/Thread.cpp
NavDataObserver.o: Communication/NavDataObserver.h
	arm-none-eabi-g++ -c Communication/NavDataObserver.cpp
NavDataReceiver.o: Communication/NavDataReceiver.h Communication/ATClient.h Communication/NavDataObserver.h
	arm-none-eabi-g++ -c Communication/NavDataReceiver.cpp
Trajectory.o: MotionLayer/Trajectory.h
	arm-none-eabi-g++ -c MotionLayer/Trajectory.cpp
MotionController.o: MotionLayer/MotionController.h MotionLayer/Trajectory.h Common/Thread.h Communication/NavDataReceiver.h Communication/ATClient.h Communication/NavDataObserver.h
	arm-none-eabi-g++ -c MotionLayer/MotionController.cpp
clean:
	rm *.o Release/main.out