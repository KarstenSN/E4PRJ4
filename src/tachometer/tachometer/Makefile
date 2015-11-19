SRC=tachometer.cpp tachometerTest.cpp
OBJ=$(addprefix ${BUILD_DIR}/,$(SRC:.cpp=.o))
DEPS=$(addprefix ${BUILD_DIR}/,$(SRC:.cpp=.d))
EXE=prog
CXXFLAGS=-I.

ARCH ?= host

ifeq (${ARCH},host)
GXX=g++ -pthread
BUILD_DIR=build/host
BIN_DIR=bin/host
endif

ifeq (${ARCH},target)
GXX=arm-linux-gnueabihf-g++ -pthread
BUILD_DIR=build/target
BIN_DIR=bin/target
endif

$(EXE): $(OBJ) $(DEPS)
	mkdir -p ${BIN_DIR}
	$(GXX) $(CXXFLAGS) -o ${BIN_DIR}/$@ $(OBJ)

${BUILD_DIR}/%.o:%.cpp
	$(GXX) $(CXXFLAGS) -c -o $@ $<

${BUILD_DIR}/%.d:%.cpp
	mkdir -p ${BUILD_DIR}
	$(GXX) -MT $(@:.d=.o) -MM $(CXXFLAGS) $^ > $@
	@echo "Creating dependency: " $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

clean: 
	rm -rf build bin

help:
	@echo "Available targets:"
	@echo "default: ARCH=host"
	@echo "ARCH=target"
	@echo "clean"
	@echo "help"