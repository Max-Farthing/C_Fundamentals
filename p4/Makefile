parks: parks.o catalog.o input.o
	gcc parks.o catalog.o input.o -o parks -lm

parks.o: parks.c catalog.h input.h
	gcc -D_GNU_SOURCE -Wall -std=c99 -g -c parks.c

catalog.o: catalog.c catalog.h input.h
	gcc -D_GNU_SOURCE -Wall -std=c99 -g -c catalog.c

input.o: input.c input.h
	gcc -D_GNU_SOURCE -Wall -std=c99 -g -c input.c

clean: 
	rm -f parks.o catalog.o input.o
	rm -f parks