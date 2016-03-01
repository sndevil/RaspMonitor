C=gcc
CFLAGS= -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
DEPS = 
OBJ = source/main.o 
OUT = bin/Finder.out

source/%.o: source/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm $(OUT) 
	rm $(OBJ)
