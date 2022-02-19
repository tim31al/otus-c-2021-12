TARGET = app
KEYS = -Wall -Wextra -Wpedantic -std=c11


all: $(TARGET)

$(TARGET): main.o
	gcc main.o $(KEYS) -o $(TARGET)

main.o: main.c
		gcc -c main.c -o main.o

clean:
	rm -f *.o $(TARGET)
