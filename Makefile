OBJECTS=main.o UDPClient.o ATClient.o Thread.o NavDataObserver.o NavDataReceiver.o Logger.o Trajectory.o MotionController.o Camera.o
COMPILER=arm-none-eabi-g++

all: $(OBJECTS)
	$(COMPILER) -lpthread -lrt $(OBJECTS) -o Release/main.out
main.o:
	$(COMPILER) -c main.cpp
Logger.o: Common/Logger.h
	$(COMPILER) -c Common/Logger.cpp
UDPClient.o: Common/Logger.h Communication/UDPClient.h
	$(COMPILER) -lexception -c Communication/UDPClient.cpp
ATClient.o: Communication/ATClient.h Communication/UDPClient.h Common/Thread.h
	$(COMPILER) -c Communication/ATClient.cpp
Thread.o: Common/Thread.h
	$(COMPILER) -c Common/Thread.cpp
NavDataObserver.o: Communication/NavDataObserver.h
	$(COMPILER) -c Communication/NavDataObserver.cpp
NavDataReceiver.o: Communication/NavDataReceiver.h Communication/ATClient.h Communication/NavDataObserver.h
	$(COMPILER) -c Communication/NavDataReceiver.cpp
Trajectory.o: MotionLayer/Trajectory.h
	$(COMPILER) -c MotionLayer/Trajectory.cpp
MotionController.o: MotionLayer/MotionController.h MotionLayer/Trajectory.h Common/Thread.h Communication/NavDataReceiver.h Communication/ATClient.h Communication/NavDataObserver.h
	$(COMPILER) -c MotionLayer/MotionController.cpp
Camera.o: VisionLayer/Camera.h
	$(COMPILER) -c VisionLayer/Camera.cpp
clean:
	rm *.o Release/main.out