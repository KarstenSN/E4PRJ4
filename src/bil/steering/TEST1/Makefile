SOURCES=main.cpp Steering.cpp Data.cpp Log.cpp
OBJECTS=$(addprefix ${BUILD_DIR}/,$(SOURCES:.cpp=.o))
DEPS=$(addprefix ${BUILD_DIR}/,$(SOURCES:.cpp=.d))
EXE=steering

#-I.
# Making for host
# > make ARCH=host
ifeq (${ARCH},host)
CXX=g++
BUILD_DIR=build/host
BIN_DIR=bin/host
INCLUDE	= -I. -I/usr/local/include
LDFLAGS	= -L/usr/local/lib
endif

# Making for target
# > make ARCH=target
ifeq (${ARCH},target)
CXX=arm-linux-gnueabihf-g++
BUILD_DIR=build/targetpi
BIN_DIR=bin/targetpi
INCLUDE	= -I. -I/home/stud/Documents/for_PI/include
LDFLAGS	= -L/home/stud/Documents/for_PI/lib
endif

CXXFLAGS= $(INCLUDE) -lwiringPi -pthread -Wall -std=c++0x
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

$(EXE): ${DEPS} ${OBJECTS} # << Check the $(DEPS) new dependency
	@mkdir -p ${BIN_DIR}
	@echo "Linking '$<'"
	@$(CXX) $(CXXFLAGS) -o ${BIN_DIR}/$@ $(OBJECTS) $(LDFLAGS) $(LDLIBS)

# Rule that describes how a .d ( dependency ) file is created from a .cpp file
# Similar to the assigment that you just completed %.cpp -> %.o
${BUILD_DIR}/%.d: %.cpp
	@echo "Generating dependency file for '$<'"
	@mkdir -p ${BUILD_DIR}
	@$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@ $(LDFLAGS) $(LDLIBS)

${BUILD_DIR}/%.o: %.cpp
	@echo "Compiling '$<'"
	@$(CXX) -c -o $@ $< $(CXXFLAGS) 

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),help)
-include ${DEPS}
endif
endif

clean:
	rm -rf build bin

help:
	@echo Possible targets are:
	@echo help
	@echo clean
	@echo ARCH=[target/host]
