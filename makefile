main: calc.c calc.h expr.o
	gcc -Wall -o calc calc.c expr.o numbers.o tools.o

expr.o: expr.c expr.h numbers.o
	gcc -Wall -c expr.c

numbers.o: numbers.c numbers.h tools.o
	gcc -Wall -c numbers.c

tools.o: tools.c tools.h
	gcc -Wall -c tools.c

clean:
	rm *.o
