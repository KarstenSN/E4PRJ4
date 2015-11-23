SOURCES=main.cpp Data.cpp
OBJECTS=$(addprefix ${BUILD_DIR}/,$(SOURCES:.cpp=.o))
DEPS=$(addprefix ${BUILD_DIR}/,$(SOURCES:.cpp=.d))
EXE=data
CXXFLAGS=-I. -lpthread

# Making for host
# > make ARCH=host
ifeq (${ARCH},host)
CXX=g++
BUILD_DIR=build/host
BIN_DIR=bin/host
endif

# Making for target
# > make ARCH=target
ifeq (${ARCH},target)
CXX=/usr/local/linaro/arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++
BUILD_DIR=build/target
BIN_DIR=bin/target
endif

$(EXE): ${DEPS} ${OBJECTS} # << Check the $(DEPS) new dependency
	@mkdir -p ${BIN_DIR}
	$(CXX) $(CXXFLAGS) -o ${BIN_DIR}/$@ $(OBJECTS)

# Rule that describes how a .d ( dependency ) file is created from a .cpp file
# Similar to the assigment that you just completed %.cpp -> %.o
${BUILD_DIR}/%.d: %.cpp
	@mkdir -p ${BUILD_DIR}
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

${BUILD_DIR}/%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

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
