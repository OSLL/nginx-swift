vpath %.c ./src
vpath %.h   ./inc

TARGET=swift-ring-converter
INC=-I./inc 
CFLAG=-O2 -std=gnu99
LD_LIB=-lpython2.6 -ltpl -lpcreposix -lpcre
CC=gcc

all: main.o ring.o PythonDeserialize.o
	$(CC) $(CFLAG) $(INC) main.o ring.o PythonDeserialize.o $(LD_LIB) -o $(TARGET)
	
main.o: main.c
	$(CC) $(CFLAG) $(INC) -c ./src/main.c 

ring.o: ring.c ring.h
	$(CC) $(CFLAG) $(INC) -c ./src/ring.c
	
PythonDeserialize.o: PythonDeserialize.h PythonDeserialize.c
	$(CC) $(CFLAG) $(INC) -c ./src/PythonDeserialize.c
	
	
clean:
	rm -f *.o