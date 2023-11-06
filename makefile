CC = gcc
CFLAGS = -Wall -Werror -O2
LD = gcc
LDFLAGS =
all: main.o linear.o random.o random-source.h
	$(LD) -o printrandom $(LDFLAGS) -rdynamic main.o linear.o random.o random-source.h -ldl
main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c
linear.o: linear.c
	$(CC) $(CFLAGS) -fPIC -DPIC -shared -c -o linear.o linear.c
random.o: random.c
	$(CC) $(CFLAGS) -fPIC -DPIC -shared -c -o random.o random.c
clear:
	rm *.o printrandom