vpath %.c ./src ./tst
vpath %.h ./inc

TARGET=swift-ring-converter
TEST_NAME=runtest
INC=-I./inc 
CFLAG=-O2 -std=gnu99
LD_LIB=-lpython2.6 -ltpl -lpcreposix -lpcre
TEST_LD_LIB=-lpython2.6 -ltpl -lcunit
CC=gcc

all: main.o ring.o PythonDeserialize.o
	$(CC) $(CFLAG) $(INC) main.o ring.o PythonDeserialize.o $(LD_LIB) -o $(TARGET)
	
main.o: main.c
	$(CC) $(CFLAG) $(INC) -c ./src/main.c 

ring.o: ring.c ring.h
	$(CC) $(CFLAG) $(INC) -c ./src/ring.c
	
PythonDeserialize.o: PythonDeserialize.h PythonDeserialize.c
	$(CC) $(CFLAG) $(INC) -c ./src/PythonDeserialize.c
	
test: PythonDeserialize.o ring.o
	$(CC) $(CFLAG) $(INC) -c ./tst/main.c -o tst_main.o
	$(CC) $(CFLAG) $(INC) tst_main.o ring.o PythonDeserialize.o $(TEST_LD_LIB) -o $(TEST_NAME)
	./runtest
	rm -f *.tpl
	
clean:
	rm -f *.o
distclean: clean
	rm -f $(TARGET) $(TEST_NAME)
	