main: calc.c calc.h main.c expr.o
	gcc -Wall -o calc calc.c expr.o numbers.o tools.o -lm
	gcc -Wall -o main main.c expr.o numbers.o tools.o -lm
	
expr.o: expr.c expr.h numbers.o
	gcc -Wall -c expr.c

numbers.o: numbers.c numbers.h tools.o
	gcc -Wall -c numbers.c

tools.o: tools.c tools.h
	gcc -Wall -c tools.c

clean:
	rm *.o
