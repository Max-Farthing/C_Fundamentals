CC = gcc
CFLAGS += -Wall -std=c99 -g
LDLIBS = -lgcov

all: driver stringTest mapTest

driver: driver.o map.o input.o value.o
	$(CC) $(CFLAGS) -o driver driver.o map.o input.o value.o $(LDLIBS)

stringTest: stringTest.o value.o
	$(CC) $(CFLAGS) -o stringTest stringTest.o value.o $(LDLIBS)

mapTest: mapTest.o map.o value.o
	$(CC) $(CFLAGS) -o mapTest mapTest.o map.o value.o $(LDLIBS)

stringTest.o: stringTest.c value.h
	$(CC) $(CFLAGS) -c stringTest.c

mapTest.o: mapTest.c map.h value.h
	$(CC) $(CFLAGS) -c mapTest.c

driver.o: driver.c input.h map.h value.h
	$(CC) $(CFLAGS) -c driver.c

map.o: map.c map.h value.h
	$(CC) $(CFLAGS) -c map.c

value.o: value.c value.h
	$(CC) $(CFLAGS) -c value.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

clean:
	rm -f driver stringTest mapTest
	rm -f driver.o map.o value.o input.o mapTest.o stringTest.o
	rm -f *.gcda *.gcno *.gcov
