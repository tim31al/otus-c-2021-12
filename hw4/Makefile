TARGET = app
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

all: $(TARGET)

$(TARGET): main.o curl_query.o cjson.o metaweather.o
	gcc main.o $(CFLAGS) curl_query.o cjson.o metaweather.o -lcurl -o $(TARGET)


main.o: main.c
	gcc -c main.c -o main.o

curl_query.o: curl_query.h
	gcc -c curl_query.c -o curl_query.o

cjson.o: cjson/cJSON.c
	gcc -c cjson/cJSON.c -o cjson.o

metaweather.o: metaweather.c
		gcc -c metaweather.c -o metaweather.o


clean:
	rm -f *.o *.gch $(TARGET)
