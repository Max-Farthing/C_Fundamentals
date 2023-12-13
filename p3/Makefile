trader: trader.o transaction.o account.o util.o
	gcc trader.o transaction.o account.o util.o -o trader

trader.o: trader.c transaction.h account.h
	gcc -Wall -std=c99 -g -c trader.c

transaction.o: transaction.c transaction.h account.h util.h
	gcc -Wall -std=c99 -g -c transaction.c

account.o: account.c account.h util.h
	gcc -Wall -std=c99 -g -c account.c

util.o: util.c util.h
	gcc -Wall -std=c99 -g -c util.c

clean:
	rm -f trader.o transaction.o account.o util.o
	rm -f trader