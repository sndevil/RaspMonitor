C=gcc
CC=gcc
CFLAGS= -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
DEPS = source/main.h source/init.h source/functions.h
OBJ = source/main.o source/init.o source/functions.o
OUT = bin/Finder.out

$(OUT) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

source/main.o: source/main.c source/main.h source/init.o source/functions.o
	$(CC) -c -o $@ $< $(CFLAGS)

source/functions.o: source/functions.c source/functions.h
	$(CC) -c -o $@ $< $(CFLAGS)

source/init.o: source/init.c source/init.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm $(OBJ)
	rm $(OUT)
