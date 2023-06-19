#
# makefile for image and color utilities C library
#
#

CC = gcc -std=gnu99
FLAGS = -Wall
CURL_LIB = -lcurl
INCLUDES =

simulate: simulation.c producer.o consumer.o request.o queue.o curl_utils.o utils.o
	$(CC) $(FLAGS) $(INCLUDES) simulation.c producer.o consumer.o request.o queue.o curl_utils.o utils.o -o simulate -lpthread $(CURL_LIB)

producer.o: producer.c producer.h
	$(CC) $(FLAGS) $(INCLUDES) -c producer.c -o producer.o

consumer.o: consumer.c consumer.h
	$(CC) $(FLAGS) $(INCLUDES) -c consumer.c -o consumer.o

request.o: request.c request.h
	$(CC) $(FLAGS) $(INCLUDES) -c request.c -o request.o

queue.o: queue.c queue.h
	$(CC) $(FLAGS) $(INCLUDES) -c queue.c -o queue.o

curl_utils.o: curl_utils.c curl_utils.h
	$(CC) $(FLAGS) $(INCLUDES) -c curl_utils.c -o curl_utils.o

utils.o: utils.c utils.h
	$(CC) $(FLAGS) $(INCLUDES) -c utils.c -o utils.o

clean:
	rm -f *~ *.o
