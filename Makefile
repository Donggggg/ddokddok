ddokddok : main.o member.o maze.o sudoku.o
	gcc main.o member.o maze.o sudoku.o -o ddokddok

main.o : main.c main.h
	gcc -c main.c

member.o : member.c member.h
	gcc -c member.c

maze.o : maze.c maze.h
	gcc -c maze.c

sudoku.o: sudoku.c sudoku.h
	gcc -c sudoku.c

clean :
		rm *.o
		rm ddokddok
