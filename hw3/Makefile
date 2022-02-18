TARGET = app

all: $(TARGET)

$(TARGET): main.o hash_table.o
	gcc main.o hash_table.o -Wall -Wextra -Wpedantic -std=c11 -lm -o $(TARGET)

main.o: main.c
	gcc -c main.c -o main.o


hash.o: hash_table.c
	gcc -c hash_table.c -o hash_table.o

clean:
	rm -f *.o *.gch $(TARGET)
