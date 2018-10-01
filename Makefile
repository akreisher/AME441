CXX = g++
CPPFLAGS = -Wall -g -std=c++11 -pthread
BIN_DIR = bin

all: yawControl

yawControl: yawControl.cpp AHRS.o SerialPort.o SerialIO.o OffsetTracker.o InertialDataIntegrator.o ContinuousAngleTracker.o motorClass.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi
	
pitchControl: pitchControl.cpp AHRS.o SerialPort.o SerialIO.o OffsetTracker.o InertialDataIntegrator.o ContinuousAngleTracker.o motorClass.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi

yawSim: yawSimTest.cpp AHRS.o SerialPort.o SerialIO.o OffsetTracker.o InertialDataIntegrator.o ContinuousAngleTracker.o motorSim.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi

pitchSim: pitchSim.cpp AHRS.o SerialPort.o SerialIO.o OffsetTracker.o InertialDataIntegrator.o ContinuousAngleTracker.o motorSim.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi
SerialPort.o: SerialPort.cpp
	${CXX} ${CPPFLAGS} -c $< -o $@
	
motorClass.o: motorClass.cpp motorClass.h
	${CXX} ${CPPFLAGS} -c $< -o $@
	
motorSim.o: motorSim.cpp motorSim.h
	${CXX} ${CPPFLAGS} -c $< -o $@
	
SerialIO.o: SerialIO.cpp SerialIO.h AHRSProtocol.h IMUProtocol.h IIOCompleteNotification.h IBoardCapabilities.h
	${CXX} ${CPPFLAGS} -c $< -o $@

OffsetTracker.o: OffsetTracker.cpp OffsetTracker.h
	${CXX} ${CPPFLAGS} -c $< -o $@
InertialDataIntegrator.o: InertialDataIntegrator.cpp InertialDataIntegrator.h
	${CXX} ${CPPFLAGS} -c $< -o $@

ContinuousAngleTracker.o: ContinuousAngleTracker.cpp ContinuousAngleTracker.h
	${CXX} ${CPPFLAGS} -c $< -o $@

AHRS.o: AHRS.cpp AHRS.h AHRSProtocol.h IIOProvider.h IIOCompleteNotification.h IBoardCapabilities.h InertialDataIntegrator.h OffsetTracker.h ContinuousAngleTracker.h SerialIO.h
	${CXX} ${CPPFLAGS} -c $< -o $@
