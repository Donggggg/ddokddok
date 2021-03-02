ddokddok : main.o maze.o
	gcc main.o maze.o -o ddokddok

main.o : main.c main.h
	gcc -c main.c

maze.o : maze.c maze.h
	gcc -c maze.c


clean :
		rm *.o
		rm ddokddok
