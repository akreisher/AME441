CXX = g++
CPPFLAGS = -Wall -g -std=c++11 -pthread
BIN_DIR = bin
AHRS_DIR = navX
MOTOR_DIR = motor

all: $(BIN_DIR)/.dirstamp $(BIN_DIR)/pitchControl

$(BIN_DIR)/.dirstamp:
	mkdir -p $(BIN_DIR)
	touch $(BIN_DIR)/.dirstamp

$(BIN_DIR)/yawControl: yawControl.cpp $(BIN_DIR)/AHRS.o $(BIN_DIR)/SerialPort.o $(BIN_DIR)/SerialIO.o $(BIN_DIR)/OffsetTracker.o $(BIN_DIR)/InertialDataIntegrator.o $(BIN_DIR)/ContinuousAngleTracker.o $(BIN_DIR)/motorClass.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi
	
$(BIN_DIR)/pitchControl: pitchControl.cpp $(BIN_DIR)/AHRS.o $(BIN_DIR)/SerialPort.o $(BIN_DIR)/SerialIO.o $(BIN_DIR)/OffsetTracker.o $(BIN_DIR)/InertialDataIntegrator.o $(BIN_DIR)/ContinuousAngleTracker.o $(BIN_DIR)/motorClass.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi
	
$(BIN_DIR)/motorController: motorController.cpp $(BIN_DIR)/AHRS.o $(BIN_DIR)/SerialPort.o $(BIN_DIR)/SerialIO.o $(BIN_DIR)/OffsetTracker.o $(BIN_DIR)/InertialDataIntegrator.o $(BIN_DIR)/ContinuousAngleTracker.o $(BIN_DIR)/motorClass.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi
	
$(BIN_DIR)/pitchSim: pitchSim.cpp $(BIN_DIR)/AHRS.o $(BIN_DIR)/SerialPort.o $(BIN_DIR)/SerialIO.o $(BIN_DIR)/OffsetTracker.o $(BIN_DIR)/InertialDataIntegrator.o $(BIN_DIR)/ContinuousAngleTracker.o $(BIN_DIR)/motorSim.o
	${CXX} ${CPPFLAGS} $^ -o $@ -lwiringPi

$(BIN_DIR)/SerialPort.o: $(AHRS_DIR)/SerialPort.cpp
	${CXX} ${CPPFLAGS} -c $< -o $@
	
$(BIN_DIR)/motorClass.o: $(MOTOR_DIR)/motorClass.cpp $(MOTOR_DIR)/motorClass.h
	${CXX} ${CPPFLAGS} -c $< -o $@
	
$(BIN_DIR)/motorSim.o: $(MOTOR_DIR)/motorSim.cpp $(MOTOR_DIR)/motorSim.h
	${CXX} ${CPPFLAGS} -c $< -o $@
	
$(BIN_DIR)/SerialIO.o: $(AHRS_DIR)/SerialIO.cpp $(AHRS_DIR)/SerialIO.h $(AHRS_DIR)/AHRSProtocol.h $(AHRS_DIR)/IMUProtocol.h $(AHRS_DIR)/IIOCompleteNotification.h $(AHRS_DIR)/IBoardCapabilities.h
	${CXX} ${CPPFLAGS} -c $< -o $@

$(BIN_DIR)/OffsetTracker.o: $(AHRS_DIR)/OffsetTracker.cpp $(AHRS_DIR)/OffsetTracker.h
	${CXX} ${CPPFLAGS} -c $< -o $@
$(BIN_DIR)/InertialDataIntegrator.o: $(AHRS_DIR)/InertialDataIntegrator.cpp $(AHRS_DIR)/InertialDataIntegrator.h
	${CXX} ${CPPFLAGS} -c $< -o $@

$(BIN_DIR)/ContinuousAngleTracker.o: $(AHRS_DIR)/ContinuousAngleTracker.cpp $(AHRS_DIR)/ContinuousAngleTracker.h
	${CXX} ${CPPFLAGS} -c $< -o $@

$(BIN_DIR)/AHRS.o: $(AHRS_DIR)/AHRS.cpp $(AHRS_DIR)/AHRS.h $(AHRS_DIR)/AHRSProtocol.h $(AHRS_DIR)/IIOProvider.h $(AHRS_DIR)/IIOCompleteNotification.h $(AHRS_DIR)/IBoardCapabilities.h $(AHRS_DIR)/InertialDataIntegrator.h $(AHRS_DIR)/OffsetTracker.h $(AHRS_DIR)/ContinuousAngleTracker.h $(AHRS_DIR)/SerialIO.h
	${CXX} ${CPPFLAGS} -c $< -o $@
	
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
