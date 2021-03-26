#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <err.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
#include "save.h"
#include "login.h"
#include "maze.h"
#include "sudoku.h"
#include "rank.h"

//menu select
static char *choices[] = {
	"1. SUDOKU",
	"2. MAZE",
	(char *)NULL,
};

static char *ranking[] = {
	"Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
	"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
	"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
	"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
	"Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
	"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
	"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
	"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
	"Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
	"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
	"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
	"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
	"Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
	"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
	"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
	"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
	"Exit",
	(char *)NULL,
};

//print ddok ddok logo
static void print_logo(WINDOW *my_menu_win);

char* intToTime(int time)
{
	int sec, min;
	char *format, buf[2];
	format = malloc(sizeof(char) * 5);
	memset(format, '\0', 5);

	min = time / 60;
	sec = time % 60;

	buf[0] = (min / 10) + '0';
	buf[1] = (min % 10) + '0';
	strcat(format, buf);
	format[2] = ':';
	buf[0] = (sec / 10) + '0';
	buf[1] = (sec % 10) + '0';
	strcat(format, buf);

	return format;
}

void select_game_show()
{	
	//menu seletions in MENU
	ITEM **my_items;
	int c;			
	//MENU WINDOW
	MENU *my_menu;
	//MAIN WINDOW
	WINDOW *my_menu_win;
	int n_choices, i;

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

	//create main menu window
	my_menu_win = newwin(30, 70, 4, 4);
	keypad(my_menu_win, TRUE);

	/* Set main window and sub window */
	//?? should to googling
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 6, 17, 3, 1));

	//point user selection
	set_menu_mark(my_menu, "--> ");

	//print title and ddok ddok
	box(my_menu_win, 0, 0);
	mvwprintw(my_menu_win, 1, 30, "%s", "Game Menu");
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 68);
	mvprintw(LINES - 2, 34, "ddok ddok");
	print_logo(my_menu_win);
	box(my_menu_win, 0, 0);
	refresh();

	//post menu in window
	post_menu(my_menu);
	wrefresh(my_menu_win);
	int flag=0,level=1;
	enum{SUDOKU=1,MAZE};
	while(1)
	{       
		c = wgetch(my_menu_win);
		switch(c)
		{	case KEY_DOWN:
			menu_driver(my_menu, REQ_DOWN_ITEM);
			break;
			case KEY_UP:
			menu_driver(my_menu, REQ_UP_ITEM);
			break;
			case 10: //enter
			{ 
				char selection=item_name(current_item(my_menu))[0];
				flag=1;
				clear();
				if(atoi(&selection)==1)
					show_rank(1);
				else
					show_rank(2);
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


void printScore(int ChooseLank){
	Score output[500]={0,}, temp={0, }; //1000명의 사람을 수용
	Score miro_out[500] = {0, }; //미로(1-10)
	Score sudo_out[500] = {0, }; //스도쿠(1-10)
	int miro = 0;
	int sudo = 0;
	int N = 0,  go; //갖고오는 사람 수
	FILE *fp = fopen(".score","rb+");
	if(fp==NULL){
		printf("파일 오류입니다.\n");
		return;
	}
	printf("\n\n");
	rewind(fp);
	while(fread(&output[N],sizeof(Score),1,fp)){
		N+=1;
	}
	fclose(fp);

	for(int i=0; i<N-1; i++){ //버블정렬로 파일에서 입력받은 정보들을 소트
		for(int j=0; j<N-i-1; j++){
			if(output[j].score>output[j+1].score){
				strcpy(temp.name,output[j].name);
				temp.score = output[j].score;
				temp.level = output[j].level;
				strcpy(output[j].name, output[j+1].name);
				output[j].score = output[j+1].score;
				output[j].level = output[j+1].level;
				strcpy(output[j+1].name, temp.name);
				output[j+1].score = temp.score;
				output[j+1].level = temp.level;
			}
		}
	}	


	for(int i=0; i<N; i++){
		if(output[i].miro_su == 1){ //미로
			miro_out[miro] = output[i];
			miro +=1;
		}
		else if(output[i].miro_su == 2){ //스도쿠
			sudo_out[sudo] = output[i];
			sudo +=1;
		}
	}

	go = ChooseLank;
	int choice_go = 0;
	char lank[20], strscore[20];

	if(go == 2){ //상위 30명만
		for(int i = 0; i < 30; i++){ //30명의
			if(strcmp(miro_out[i].name, "") == 0){
				ranking[choice_go] = (char *)NULL;
				break;
			}
			for(int j = 0; j < 3; j++){ //rank, name, score
				if(choice_go % 3 == 0){ //rank
					sprintf(lank, "%d Rank", i+1);
					ranking[choice_go] = malloc(sizeof(char)*10);
					strcpy(ranking[choice_go], lank);
				}
				else if(choice_go % 3 == 1){ //name
					ranking[choice_go] = malloc(sizeof(char)*30);
					strcpy(ranking[choice_go], miro_out[i].name);
				}
				else if(choice_go % 3 == 2){ //score
					sprintf(strscore, "%s", intToTime(miro_out[i].score));
					ranking[choice_go] = malloc(sizeof(char)*20);
					strcpy(ranking[choice_go], strscore);
				}
				choice_go += 1;
			}
		}
	}
	else if(go == 1){ //상위 30명만
		for(int i = 0; i < 30; i++){ //30명의
			if(strcmp(sudo_out[i].name, "") == 0){
				ranking[choice_go] = (char *)NULL;
				break;
			}
			for(int j = 0; j < 3; j++){ //rank, name, score
				if(choice_go % 3 == 0){ //rank
					sprintf(lank, "%d Rank", i+1);
					ranking[choice_go] = malloc(sizeof(char)*10);
					strcpy(ranking[choice_go], lank);
				}
				else if(choice_go % 3 == 1){ //name
					ranking[choice_go] = malloc(sizeof(char)*30);
					strcpy(ranking[choice_go], sudo_out[i].name);
				}
				else if(choice_go % 3 == 2){ //score
					sprintf(strscore, "%s", intToTime(sudo_out[i].score));
					ranking[choice_go] = malloc(sizeof(char)*20);
					strcpy(ranking[choice_go], strscore);
				}
				choice_go += 1;
			}
		}
	}
	else{
		printf("잘못 입력하셨습니다.\n");
	}
}

void show_rank(int game_selection)
{

	printScore(game_selection);

	ITEM **my_items;
	int c;				
	MENU *my_menu;
	WINDOW *win;
	int n_choices, i;

	//init ncurses window
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED,COLOR_YELLOW);
	//create rank list
	n_choices = ARRAY_SIZE(ranking);
	my_items = (ITEM **)malloc(n_choices* sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(ranking[i], ranking[i]);

	//create menu window
	my_menu = new_menu((ITEM **)my_items);
	menu_opts_off(my_menu, O_SHOWDESC);//only show 1 list  per line


	//create main window
	win = newwin(30, 70, 4, 4);
	keypad(win, TRUE);

	//###############should googling derwin(), set_menu_sub()###################
	set_menu_win(my_menu, win);
	set_menu_sub(my_menu, derwin(win, 25, 68, 3, 1));
	set_menu_format(my_menu, 25, 3);
	set_menu_mark(my_menu, "--> ");


	//print border line and title
	box(win, 0, 0);
	mvwprintw(win,0,8,"RANK");	
	mvwprintw(win,0,20,"NICKNAME");	
	mvwprintw(win,0,32,"SCORE");
	//print explation at the bottom
	attron(COLOR_PAIR(1));
	mvprintw(LINES - 3, 30, "<--- prev     next--->");
	mvprintw(LINES - 2, 34, "(F2 to Exit)");
	attroff(COLOR_PAIR(1));

	//refresh all
	refresh();
	post_menu(my_menu);
	wrefresh(win);

	while((c = wgetch(win)) != KEY_F(2))
	{       
		switch(c) {	
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(my_menu, REQ_SCR_UPAGE);
				break;
			case KEY_RIGHT:
				menu_driver(my_menu, REQ_SCR_DPAGE);
				break;
		}
		wrefresh(win);
	}	

	//free all
	unpost_menu(my_menu);
	free_menu(my_menu);
	for(i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	endwin();
}

void inputScore(char *nickname, double score, int level, int miro_su){
	Score input = {"",0};
	FILE *fp = fopen(".score","ab+");
	if(fp==NULL){
		printf("파일 오류입니다.\n");
		fclose(fp);
		return;
	}
	strcpy(input.name, nickname); //이름
	input.score = score; //점수(걸린 시간)
	input.level = level; //난이도
	input.miro_su = miro_su; //미로 스도쿠 (1 2)


	fseek(fp,0,SEEK_END); //맨 끝으로 이동
	fwrite(&input,sizeof(Score),1,fp); //이진 파일로 저장
	fclose(fp);
	printf("\n입력이 완료되었습니다.\n\n");
}
static void print_logo(WINDOW *my_menu_win)
{
	char line[255];
	FILE *fp;
	fp=fopen(".logo","r");
	if(fp==NULL)    err(EXIT_FAILURE,"NO logo file");
	int i=12;
	while(fgets(line,sizeof(line),fp)!=NULL)
		mvwprintw(my_menu_win, i++, 4, "%s", line);
}
