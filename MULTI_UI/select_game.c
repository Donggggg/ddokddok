#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <err.h>
#include "back_g.h"
#include "member.h"
#include "maze.h"
#include "sudoku.h"
#include "score.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
#define SOLO 1
#define MULTI 2

Game *game;
Setting *set;
static char *choices[] = {
	"1. SUDOKU",
	"2. MAZE",
	"3. BACK",
	(char *)NULL,
};

static void print_logo(WINDOW *my_menu_win);

void soloMode(Info *player1)
{	
	int i, c, n_choices;
	// UI 변수 선언
	WINDOW *my_menu_win;
	MENU *my_menu;
	ITEM **my_items;

	//Init curses
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	//create menu seletions
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)malloc(n_choices* sizeof(ITEM *));

	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i],"");

	//create menu window
	my_menu = new_menu((ITEM **)my_items);
	my_menu_win = newwin(30, 70, 4, 4);
	keypad(my_menu_win, TRUE);
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 6, 17, 3, 1));
	set_menu_mark(my_menu, "--> ");

	box(my_menu_win, 0, 0);
	mvwprintw(my_menu_win, 1, 30, "%s", "Game Menu");
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 68);
	print_logo(my_menu_win);
	box(my_menu_win, 0, 0);

	refresh();

	//post menu in window
	post_menu(my_menu);
	wrefresh(my_menu_win);

	int flag=0,level=1;
	enum{SUDOKU=1,MAZE,BACK};

	while(c = wgetch(my_menu_win))
	{       
		switch(c) {	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				if(level>1) level-=1;
				mvprintw(LINES - 2, 34, "LEVEL : %d              ",level);
				break;
			case KEY_RIGHT:
				if(level<10) level+=1;
				mvprintw(LINES-2,34,"LEVEL : %d"                 ,level);
				break; 
			case 10: //enter
				{ 
					char selection=item_name(current_item(my_menu))[0];
					int input=atoi(&selection), wrong;
					double times;
					time_t start,end;

					if(atoi(&selection) == 3) // BACK 입력한 경우 
						return ;

					if(input==1) input+=1;
					else		input-=1;

					flag=1;
					clear();

					start=time(NULL);

					if(atoi(&selection)==1)
						startSudoku(1,level,NULL);
					else
						startMaze(1,level,NULL);

					end=time(NULL); 

					times=(double)(end-start)+(double)(wrong*10);
					inputScore(player1,times,level,input);

					break;
				}
		}

		if(flag) break;

		box(my_menu_win, 0, 0);
		refresh(); 
		post_menu(my_menu);
		wrefresh(my_menu_win);
	}

	//free all
	unpost_menu(my_menu);
	free_menu(my_menu);
	for(i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	endwin();
	clear();
}

void multiMode()
{
	Game *game = malloc(sizeof(Game));
	int road_check = roadGame(game);
	int rank[5] = {0,1,2,3,4};

	if(road_check != 1) // 세이브 파일이 없으면 새 게임 생성
		setGame(game);

	while (game->round[0] <= game->round[1]) 
	{
		printw(">>%d >>> %d\n", game->round[0], game->round[1]);
		mvprintw(1, 1, "Round %d\n", game->round[0]);
		mvprintw(5, 180, "SCORE");
		mvprintw(6, 180, "--------------------");

		for(int i = 0; i < game->people; i++) 
			mvprintw(i+7, 180, "Player %d [%d]", i+1, game->score[i]); 

		refresh();

		for (int j = 0; j < game -> people; j++)
			game->plus_score[j] = 100 * game->round[0];

		if (game->game_select[game->round[0]-1] == 1){
			startMaze(MULTI,game->round[0],game);
			game->round[0] += 1;
		}
		else if (game->game_select[game->round[0]-1] == 2){
			startSudoku(MULTI, game->round[0],game);
			game->round[0] += 1;
		}
		else // ERROR
			fprintf(stderr, "something wrong...\n");
		//	break;
	}

	clear();

	// 등수 정렬
	for(int i = 0; i < game->people; i++)
		for(int j = i; j < game->people; j++) {
			if(game->score[i] < game->score[j]) {
				int tmp = game->score[i], t_rank = rank[i];
				game->score[i] = game->score[j];
				game->score[j] = tmp;
				rank[i] = rank[j];
				rank[j] = t_rank;
			}
		}

	// 최종 결과 출력
	mvprintw(25, 110, "Final Result !!!");
	for(int i = 0; i < game->people; i++) {
		switch (i+1) {
			case 1 :
				mvprintw(i+26,110, "<1st> Player %d (%d)", rank[i]+1, game->score[i]); 
				break;
			case 2 :
				mvprintw(i+26, 110, "<2nd> Player %d (%d)", rank[i]+1, game->score[i]); 
				break;
			case 3 :
				mvprintw(i+26, 110, "<3rd> Player %d (%d)", rank[i]+1, game->score[i]); 
				break;
			default :
				mvprintw(i+26, 110, "<%dth> Player %d (%d)", i+1, rank[i]+1, game->score[i]); 
				break;
		}
	}

	refresh();
	mvprintw(25, 110, "Press Enter to Continue...");
	getchar();
}

static void print_logo(WINDOW *my_menu_win)
{
	char line[255];
	FILE *fp;
	fp=fopen("test.txt","r");
	if(fp==NULL)    err(EXIT_FAILURE,"NO test.txt file");
	int i=12;
	while(fgets(line,sizeof(line),fp)!=NULL)
		mvwprintw(my_menu_win, i++, 4, "%s", line);
}
