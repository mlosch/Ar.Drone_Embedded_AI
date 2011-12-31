OBJECTS = main.o UDPClient.o ATClient.o Thread.o NavDataObserver.o NavDataReceiver.o Logger.o Trajectory.o MotionController.o
SOURCES = ./src/
Ardrone: $(OBJECTS)
	arm-none-eabi-g++ -lpthread -lrt $(OBJECTS) -o Release/main.out
main.o:
	arm-none-eabi-g++ -c $(SOURCES)main.cpp
Logger.o: $(SOURCES)Common/Logger.h
	arm-none-eabi-g++ -c $(SOURCES)Common/Logger.cpp
UDPClient.o: $(SOURCES)Common/Logger.h $(SOURCES)Communication/UDPClient.h
	arm-none-eabi-g++ -lexception -c $(SOURCES)Communication/UDPClient.cpp
ATClient.o: $(SOURCES)Communication/ATClient.h $(SOURCES)Communication/UDPClient.h $(SOURCES)Common/Thread.h
	arm-none-eabi-g++ -c $(SOURCES)Communication/ATClient.cpp
Thread.o: $(SOURCES)Common/Thread.h
	arm-none-eabi-g++ -c $(SOURCES)Common/Thread.cpp
NavDataObserver.o: $(SOURCES)Communication/NavDataObserver.h
	arm-none-eabi-g++ -c $(SOURCES)Communication/NavDataObserver.cpp
NavDataReceiver.o: $(SOURCES)Communication/NavDataReceiver.h $(SOURCES)Communication/ATClient.h $(SOURCES)Communication/NavDataObserver.h
	arm-none-eabi-g++ -c $(SOURCES)Communication/NavDataReceiver.cpp
Trajectory.o: $(SOURCES)MotionLayer/Trajectory.h
	arm-none-eabi-g++ -c $(SOURCES)MotionLayer/Trajectory.cpp
MotionController.o: $(SOURCES)MotionLayer/MotionController.h $(SOURCES)MotionLayer/Trajectory.h $(SOURCES)Common/Thread.h $(SOURCES)Communication/NavDataReceiver.h $(SOURCES)Communication/ATClient.h $(SOURCES)Communication/NavDataObserver.h
	arm-none-eabi-g++ -c $(SOURCES)MotionLayer/MotionController.cpp
clean:
	rm *.o Release/main.out
