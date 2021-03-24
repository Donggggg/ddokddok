OPT1 = -I/usr/local/opt/ncurses/include
OPT2 = -L/usr/local/opt/ncurses/lib
COM = -lncurses -lform -lmenu -lcurses
ASR = -W -Wall -Wextra
OPT3 = $(COM) $(ASR)

ddokddok : login.o main.o maze.o sudoku.o save.o rank.o mode.o
	gcc $(OPT1) $(OPT2) login.o main.o maze.o save.o rank.o mode.o sudoku.o -o ddokddok $(OPT3)

main.o : main.c
	gcc -c main.c

login.o : login.c login.h
	gcc -c login.c

mode.o : mode.c mode.h
	gcc -c mode.c

maze.o : maze.c maze.h
	gcc -c maze.c

sudoku.o: sudoku.c sudoku.h
	gcc -c sudoku.c

rank.o : rank.c rank.h
	gcc -c rank.c

save.o : save.c save.h
	gcc -c save.c

clean :
		rm *.o
		rm ddokddok
