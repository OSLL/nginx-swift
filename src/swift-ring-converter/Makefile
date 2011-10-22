vpath %.cpp ./src
vpath %.h   ./inc

TARGET=swift-ring-converter
INC=-I./inc 
CPPFLAG=-O2
LD_LIB=-lpython2.6 -lboost_serialization -lboost_regex -lboost_iostreams

all: main.o Ring.o PythonDeserialize.o
	g++ $(CPPFLAG) $(INC) main.o Ring.o PythonDeserialize.o $(LD_LIB) -o $(TARGET)
	
main.o: main.cpp 
	g++ $(CPPFLAG) $(INC) -Wno-deprecated -c ./src/main.cpp 

Ring.o: Ring.cpp Ring.h
	g++ $(CPPFLAG) $(INC) -c ./src/Ring.cpp
	
PythonDeserialize.o: PythonDeserialize.h PythonDeserialize.cpp
	g++ $(CPPFLAG) $(INC) -c ./src/PythonDeserialize.cpp
	
	
clean:
	rm -f *.o