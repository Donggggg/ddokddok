maketest : main.o test1.o test2.o
	gcc main.o test1.o test2.o -o maketest

main.o : main.c main.h
	gcc -c main.c

test1.o : test1.c main.h
	gcc -c test1.c

test2.o : test2.c main.h
	gcc -c test2.c

clean :
		rm *.o
