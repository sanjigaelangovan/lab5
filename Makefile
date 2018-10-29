CC=gcc
CFLAGS=-Wall -pedantic
LDFLAGS=-lwiringPi -lmyifttt -lcurl

alarm: alarm.c libmyifttt.a ifttt.o libmyifttt.a
	$(CC) $(CFLAGS) -L. alarm.c $(LDFLAGS) -o alarm

tester: tester.o libmyifttt.a
	$(CC) tester.o -L. -lmyifttt -lcurl -o tester

libmyifttt.a:	ifttt.o
	ar -rcs libmyifttt.a ifttt.o

ifttt.o: 	ifttt.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

tester.o:	tester.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

clean:
	rm tester *.o
