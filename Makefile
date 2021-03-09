ddokddok : main.o member.o maze.o sudoku.o
	gcc main.o member.o maze.o sudoku.o setting.o score.o -o ddokddok

main.o : main.c main.h
	gcc -c main.c

member.o : member.c member.h
	gcc -c member.c

maze.o : maze.c maze.h
	gcc -c maze.c

sudoku.o: sudoku.c sudoku.h
	gcc -c sudoku.c

setting.o : setting.c setting.h
	gcc -c setting.c

score.o : score.c score.h
	gcc -c score.c

clean :
		rm *.o
		rm ddokddok
