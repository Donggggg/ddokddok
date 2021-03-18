ddokddok : login_test.o member.o select_game_show.o menu.o maze.o sudoku_answer.o sudoku.o setting.o score.o back_g.o rank.o select_game.o
	gcc login_test.o member.o select_game_show.o menu.o maze.o setting.o score.o back_g.o rank.o select_game.o sudoku_answer.o sudoku.o -o ddokddok -lncurses -lform -lmenu

login_test.o : login_test.c login_test.h
	gcc -c login_test.c
menu.o : menu.c
	gcc -c menu.c

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

back_g.o : back_g.c back_g.h
	gcc -c back_g.c
rank.o : rank.c rank.h
	gcc -c rank.c
select_game.o : select_game.c select_game.h
	gcc -c select_game.c

sudoku_answer.o : sudoku_answer.c
	gcc -c sudoku_answer.c

select_game_show.o : select_game_show.c select_game_show.h
	gcc -c select_game_show.c

clean :
		rm *.o
		rm ddokddok
