ddokddok : main.o maze.o member.o
	gcc main.o maze.o member.o -o ddokddok

main.o : main.c main.h
	gcc -c main.c

maze.o : maze.c maze.h
	gcc -c maze.c

member.o : member.c member.h
	gcc -c member.c

clean :
		rm *.o
		rm ddokddok
