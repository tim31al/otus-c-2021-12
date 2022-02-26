TARGET = app
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

all: $(TARGET)

$(TARGET): main.o cp1251.o koi8_r.o iso_8859_5.o
	gcc main.o cp1251.o koi8_r.o iso_8859_5.o $(CFLAGS) -o $(TARGET)

main.o: main.c
	gcc -c main.c -o main.o

cp1251.o: cp1251.c
	gcc -c cp1251.c -o cp1251.o

koi8_r.o: koi8_r.c
		gcc -c koi8_r.c -o koi8_r.o

iso_8859_5.o: iso_8859_5.c
		gcc -c iso_8859_5.c -o iso_8859_5.o

clean:
	rm -f *.o *.gch $(TARGET)
