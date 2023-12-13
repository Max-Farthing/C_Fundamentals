all: encrypt decrypt DESTest

encrypt: encrypt.o DES.o io.o DESMagic.o
	gcc encrypt.o DES.o io.o DESMagic.o -o encrypt

decrypt: decrypt.o DES.o io.o DESMagic.o
	gcc decrypt.o DES.o io.o DESMagic.o -o decrypt

DESTest: DESTest.o DESMagic.o DES.o
	gcc DESTest.o DESMagic.o DES.o -o DESTest

DESTest.o: DESTest.c DESMagic.h DES.h
	gcc -Wall -std=c99 -g -c DESTest.c

encrypt.o: encrypt.c io.h DES.h DESMagic.h
	gcc -Wall -std=c99 -g -c encrypt.c

decrypt.o: decrypt.c io.h DES.h DESMagic.h
	gcc -Wall -std=c99 -g -c decrypt.c

io.o: io.c io.h DES.h
	gcc -Wall -std=c99 -g -c io.c

DES.o: DES.c DES.h DESMagic.h
	gcc -Wall -std=c99 -g -c DES.c

DESMagic.o: DESMagic.c DESMagic.h
	gcc -Wall -std=c99 -g -c DESMagic.c

clean: 
	rm -f io.o DES.o DESMagic.o encrypt.o decrypt.o DESTest.o
	rm -f encrypt decrypt DESTest