#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <err.h>
#include <menu.h>
#include <form.h>
#include "save.h"
#include "sudoku.h"

#define SOLO 1
#define MULTI 2

int ran, blank = 0;
int sudoku_answer_int[81];
_Sudoku sudoku;
_Player* player;

static char sudoku_answer_psj[82];
int sudoku_answer_int[81];

static int check_in;
static FORM *form;
static FIELD *fields[11];
static WINDOW *win_body, *win_form;
static WINDOW *my_menu_win;

//static int flag;
static char* trim_whitespaces(char *str);
static void set_field(FIELD *field[]);
static void free_all(FORM *form,FIELD *fields[]);

static void create_Win()
{
	initscr();
	start_color();
	cbreak();
	noecho();

	//create main menu window
	my_menu_win = newwin(30, 64, 4, 4); 
	init_pair(1,COLOR_YELLOW,COLOR_GREEN);

	//print title and ddok ddok
	mvwprintw(my_menu_win, 1, 30, "%s", "Sudoku 9 by 9");
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 68);
	//cero line

	for(int k=3;k<20;k++)
	{
		mvwprintw(my_menu_win,k,14,".");
		mvwprintw(my_menu_win,k,7,".");
		mvwprintw(my_menu_win,k,28,".");
		mvwprintw(my_menu_win,k,35,".");
		mvwprintw(my_menu_win,k,49,".");
		mvwprintw(my_menu_win,k,56,".");
	}
	//garo line
	int j,i;
	for(i=1,j=4;i<=9;i++,j+=2)
	{
		if(i%3==0)
		{
			mvwhline(my_menu_win, j,1, ACS_HLINE, 68);
			continue;
		}

		for(int k=0;k<69;k++)
			mvwprintw(my_menu_win,j,k, ".");
	}
	//colum line
	mvwvline(my_menu_win, 3,21, ACS_VLINE,17);
	mvwvline(my_menu_win, 3,42, ACS_VLINE,17);


	box(my_menu_win, 0, 0); 
	refresh();
	wrefresh(my_menu_win);
	int c;    
	int flag=0;
	endwin();
}

int startSudoku(int mode,int level, Game* game){
	int w;
	downloadSudoku();

	player = (_Player*)malloc(sizeof(_Player)*1);
	w = playSudoku(mode,level,game);
	free(player);
	player = NULL;

	uploadSudoku();
	return w;
}

int vertical(int n, int x, int arr[][9]){
	for(int i=0; i<9; i++){
		if(arr[i][x] == n){
			return FALSE;
		}
	}
	return TRUE;
}

int horizontal(int n, int y, int arr[][9]){
	for(int i=0; i<9; i++){
		if(arr[y][i] == n){
			return FALSE;
		}
	}
	return TRUE;
}

int box_sudoku(int n, int x, int y, int arr[][9]){
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			if(arr[(y/3)*3+i][(x/3)*3+j] == n){
				return FALSE;
			}
		}
	}
	return TRUE;
}

int correctSudoku(_Player* pp){
	int k, tmp = 0;
	for(int i=0; i<9;i++)
		for(int j = 0; j<9;j++){
			if(!sudoku.problem[i][j]){
				//빈 칸인 경우
				k = pp->input[i][j];
				if(!vertical(k,j,pp->sol) || !horizontal(k,i,pp->sol) || !box_sudoku(k,j,i,pp->sol)){
					return FALSE; //수가 스도쿠 규칙에 위반할 경우 FALSE 리턴
				}
				else {
					pp->sol[i][j] = k;
					tmp++;
				}
			}
			else {
				if(sudoku.problem[i][j] != pp->input[i][j]) {
					return FALSE;
				}
			}
			//이미 주어진 숫자를 잘못 입력한 경우 오류 메시지 출력 및 -1리턴
		}
	return tmp; //모든 빈칸을 잘 채운 경우 TRUE 리턴
}

void printSudoku(int sudo_p[][9]){
	create_Win();
	int tmp[82];
	int idx=0;
	for(int i = 0; i<9; i++){
		for(int j = 0; j<9; j++){
			if(sudo_p[i][j] == 0)
			{
				tmp[idx++]=0; //##############
			}
			else 
			{
				tmp[idx++]=sudo_p[i][j];//########
			}
		}
		//	printf("\n");
	}

	int k=3;
	int j=0;
	for(int i=0;i<9;i++,k+=2)
	{
		for(int a=3;a<60;a+=7,j++)
		{
			if(tmp[j]!=0)
				mvwprintw(my_menu_win,k,a,"%d",tmp[j]);
		}

	}
	refresh();
	wrefresh(my_menu_win);

}

//지정된 두 가로 줄 교환// a,b:서로 다른 3*9집합의 가로 줄 번호(a<b)
void tradeRow(int a, int b){
	int tmp;
	for(int j = a*3; j<a*3+3;j++)
		for(int i = 0; i<9; i++){
			tmp = sudoku.origin[j][i];
			sudoku.origin[j][i] = sudoku.origin[j+3*(b-a)][i];
			sudoku.origin[j+3*(b-a)][i] = tmp;
		}
}
//지정된 두 세로 줄 교환// a,b:서로 다른 3*9집합의 세로 줄 번호(a<b)
void tradeColumn(int a, int b){
	int tmp;
	for(int j = a*3; j<a*3+3; j++)
		for(int i = 0; i<9; i++){
			tmp = sudoku.origin[i][j];
			sudoku.origin[i][j] = sudoku.origin[i][j+3*(b-a)];
			sudoku.origin[i][j+3*(b-a)] = tmp;
		}
}

//3*9칸 가로끼리 혹은 세로끼리 교환
void tradeLine(){
	switch(ran%2){
		case 0:
			//가로 줄끼리 교환
			if(ran%3==2)		tradeRow(0,1);
			else if(ran%3==1)	tradeRow(1,2);
			else				tradeRow(0,2);
			break;
		case 1:
			//세로 줄끼리 교환
			if(ran%3==2)		tradeColumn(0,1);
			else if(ran%3==1)	tradeColumn(1,2);
			else				tradeColumn(0,2);
			break;
		default: break;
	}
}
//3*9칸 가로끼리 혹은 세로끼리 모두 교환
void tradeAll(){
	switch(ran%3){
		case 1:
			//가로 전체 교환
			if(ran%2==1)	{tradeRow(0,1); tradeRow(0,2);}
			else			{tradeRow(0,1); tradeRow(1,2);}
			break;
		case 2:
			//세로 전체 교환
			if(ran%2==1)	{tradeColumn(0,1); tradeColumn(0,2);}
			else			{tradeColumn(0,1); tradeColumn(0,2);}
			break;
		case 3:
			//가로,세로 모두 교환
			break;
		default: break;
	}
}

void rotation(){
	int tmp;
	switch(ran%3){
		case 0:
			//90도 회전
			for(int a = 0; a<5; a++)
				for(int b = 0; b<4; b++){
					tmp = sudoku.origin[a][b];
					sudoku.origin[a][b] = sudoku.origin[b][8-a];
					sudoku.origin[b][8-a] = sudoku.origin[8-a][8-b];
					sudoku.origin[8-a][8-b] = sudoku.origin[8-b][a];
					sudoku.origin[8-b][a] = tmp;
				}
			break;
		case 1:
			//180도 회전
			for(int a = 0; a<5; a++)
				for(int b = 0; b<4; b++){
					tmp = sudoku.origin[a][b];
					sudoku.origin[a][b] = sudoku.origin[8-a][8-b];
					sudoku.origin[8-a][8-b] = tmp;
					tmp = sudoku.origin[8-b][a];
					sudoku.origin[8-b][a] = sudoku.origin[b][8-a];
					sudoku.origin[b][8-a] = tmp;
				}
			break;
		case 2:
			//270도 회전
			for(int a = 0; a<5; a++)
				for(int b = 0; b<4; b++){
					tmp = sudoku.origin[a][b];
					sudoku.origin[a][b] = sudoku.origin[8-b][a];
					sudoku.origin[8-b][a] = sudoku.origin[8-a][8-b];
					sudoku.origin[8-a][8-b] = sudoku.origin[b][8-a];
					sudoku.origin[b][8-a] = tmp;
				}
			break;
		default:break;
	}
}

void symmetry(){
	int tmp;
	switch(ran%4){
		case 0:	//x축 대칭
			for(int a = 0; a < 4; a++)
				for(int i = 0; i < 9; i++){
					tmp = sudoku.origin[a][i];
					sudoku.origin[a][i] = sudoku.origin[8-a][i];
					sudoku.origin[8-a][i] = tmp;
				}
			break;
		case 1:	//y축 대칭
			for(int a = 0; a < 4; a++)
				for(int i = 0; i < 9; i++){
					tmp = sudoku.origin[i][a];
					sudoku.origin[i][a] = sudoku.origin[i][8-a];
					sudoku.origin[i][8-a] = tmp;
				}
			break;
		case 2:	//y=x축 대칭
			for(int i = 0; i < 9; i++)
				for(int j = 0; j < 8-i; j++){
					tmp = sudoku.origin[i][j];
					sudoku.origin[i][j] = sudoku.origin[8-j][8-i];
					sudoku.origin[8-j][8-i] = tmp;
				}
			break;
		case 3:	//y=-x축 대칭
			for(int i = 0; i < 9; i++)
				for(int j = i+1; j < 9; j++){
					tmp = sudoku.origin[i][j];
					sudoku.origin[i][j] = sudoku.origin[j][i];
					sudoku.origin[j][i] = tmp;
				}
			break;
		default:break;
	}
}

void tradeNumber(){
	int num1, num2;
	num1 = rand()%9+1;
	//랜덤 숫자 다시 생성
	while(1){
		num2 = rand()%9+1;
		if(num1 != num2) break;
	}
	for(int i = 0; i<9; i++)
		for(int j = 0; j<9; j++){
			if(sudoku.origin[i][j] == num1) sudoku.origin[i][j] = num2;
			else if(sudoku.origin[i][j] == num2) sudoku.origin[i][j] = num1;
		}
}

//void removeNum(int level, int weight){


//파일 입출력을 통해 좋은 예시의 스도쿠 원형을 받음
void downloadSudoku(){
	FILE *fp = fopen(".sudoku","rt");
	if(fp==NULL) exit(-1);
	for(int i = 0; i<9; i++)
		for(int j = 0; j<9; j++){
			int tmp = fscanf(fp, "%d ", &sudoku.origin[i][j]);
			if(sudoku.origin[i][j] == 0 || tmp == EOF) {mvprintw(LINES-2,70,"error:스도쿠가 제대로 입력되지 않았음");}
		}
	fclose(fp);
}

//파일에 다음에 쓸 스도쿠 저장
void uploadSudoku(){
	FILE *fp = fopen(".sudoku","wt");
	if(fp==NULL) exit(-1);
	for(int i = 0; i<9; i++)
		for(int j = 0; j<9; j++){
			if(sudoku.origin[i][j] == 0) mvprintw(LINES-2,70,"error:스도쿠가 제대로 저장되지 않았음");
			fprintf(fp, "%d ", sudoku.origin[i][j]);
		}
	fclose(fp);
}

void makeSudokuOrigin(){
	srand(time(NULL));
	int	r = rand();
	ran = rand();
	//한 번 혹은 두 번만 섞어줌
	for(int i = 0; i < r%2+1; i++){
		switch(ran%5){
			case 0: 
				ran = rand();
				tradeLine();
				break;
			case 1: 
				ran = rand();
				tradeAll();
				break;
			case 2:	
				ran = rand();
				rotation();
				break;
			case 3:	
				ran = rand();
				symmetry();
				break;
			case 4: 
				ran = rand();
				tradeNumber();
				break;
			default: break;
		}
	}
	for(int i = 0; i < 81; i++)
		sudoku.problem[i/9][i%9] = sudoku.origin[i/9][i%9];
}

void makeSudokuProblem(int level){
	//37 - 55까지
	//레벨 당 구멍 개수: 35 + level*2
	int count = (35 + level*2), hole; 
	blank = count;
	srand(time(NULL));
	while(count){
		hole = rand()%81;
		if(sudoku.problem[hole/9][hole%9]){	//0이 아닐 경우에만 구멍을 내줌
			sudoku.problem[hole/9][hole%9] = 0;
			count--;
		}
	}
}

//저장된 스도쿠 불러오기
void IN_sudoku(_Player* pop){
	int k=0;
	for(int i = 0; i<9; i++)
	{
		for(int j = 0; j<9;j++)
		{
			pop->input[i][j]=sudoku_answer_int[k++];
			//pop->input[i][j]=sudoku.origin[i][j];
			pop->sol[i][j] = sudoku.problem[i][j];
		}
	}
}

int playSudoku(int mode,int level,Game* game){
	int input_num = 0, wrong = 0; 	//답 입력 개수
	int cor = FALSE; 	//정답 여부
	int playerNum, isGameover;

	//문제 생성
	makeSudokuOrigin();
	makeSudokuProblem(level);

	while(!cor)
	{
		printSudoku(sudoku.problem); // 문제 출력
		//printSudoku(sudoku.origin); // 정답 스도쿠 디버깅용

		if(mode == MULTI) {
			saveGame(game); // 현재 상태 세이브

			mvprintw(LINES-2,34,"Input Player Number : ");
			scanw("%d", &playerNum);
			mvprintw(LINES-2, 73,"%d", playerNum);

			if(playerNum>game->people||playerNum<0) {
				mvprintw(LINES-2,34,"Wrong INPUT.");
				continue;
			}

			if(game->plus_score[playerNum-1] <= 0) {
				mvprintw(LINES-3,34,"Player %d can't answer anymore", playerNum);
				continue;
			}
		}

		sudoku_answer(); // 정답 입력 UI 출력
		IN_sudoku(player); // 정답 검증 준비

		//스도쿠 검증 디버깅 코드
		for(int a=0;a<9;a++){
			for(int b=0;b<9;b++){
				printf("%d ", player->input[a][b]);
			}
			printf("\n");
		}
		printf("---------------------------\n");
		for(int a=0;a<9;a++){
			for(int b=0;b<9;b++){
				printf("%d ", player->sol[a][b]);
			}
			printf("\n");
		}

		input_num = correctSudoku(player); // 정답 검증

		if(input_num == FALSE){	//답이 틀린 경우
			mvprintw(LINES-2,34,"INCORRECT");
			refresh();
			sleep(1);

			if(mode==MULTI)
				game->plus_score[playerNum-1] -= 50;

			wrong++;
		}
		else if(input_num == -1) {	//기존의 주어진 문제를 잘못 입력한 경우
			mvprintw(LINES-2,34,"INCORRECT");
			game->plus_score[playerNum-1] -= 50;
			wrong++;
		}
		else {
			cor=1; 

			if(cor) {
				mvprintw(LINES-2,34,"CORRECT");
				refresh();
				sleep(1);

				if(mode==MULTI)
					game->score[playerNum-1] = game->plus_score[playerNum-1];//this point
			}
		}

		if(mode == MULTI) {	
			isGameover = 1;

			for(int k = 0; k < game->people; k++)
				if(game -> plus_score[k] > 0) isGameover = 0;

			if(isGameover) {
				mvprintw(LINES-2, 34, "DRAW!!!!");
				refresh();
				sleep(1);
				break;
			}
		}
	}

	return wrong;
}


void sudoku_answer()
{
	//init window tab
	initscr();
	start_color();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);

	//create main window and form window
	win_body = newwin(30,70, 4, 70);
	win_form = derwin(win_body, 20, 60, 4,4 );//20, 60 ,4 ,4
	mvwprintw(win_body, 1, 15, "Write your answers");
	//declare filed location
	fields[0] = new_field(1, 10, 0, 10, 0, 0);
	for(int i=1,k=2;i<10;i++,k+=2)
	{
		fields[i] = new_field(1, 40, k, 15, 0, 0);
	}
	fields[10] = NULL;

	set_field_buffer(fields[0], 0, "Input : ");

	//set field options
	set_field(fields);
	form = new_form(fields);

	set_form_win(form, win_form);
	set_form_sub(form, derwin(win_form, 18, 76, 1, 1));//18
	post_form(form);
	//print_logo(win_body);
	box(win_body, 0, 0);

	//refresh all
	refresh();
	wrefresh(win_body);
	wrefresh(win_form);

	//input id and password
	int ch;
	int flag=0;
	while (((ch = getch())))
	{
		int i;
		switch (ch) {
			case 10://enter
				{
					form_driver(form, REQ_NEXT_FIELD);
					form_driver(form, REQ_PREV_FIELD);
					move(LINES-3, 2);


					strcpy(sudoku_answer_psj, trim_whitespaces(field_buffer(fields[1], 0)));
					for(int i=2;i<10;i++)
					{
						strcat(sudoku_answer_psj, trim_whitespaces(field_buffer(fields[i], 0)));
					}
					refresh();
					pos_form_cursor(form);
					int k=1;
					int j=40;
					for(int i=0;i<81;i++,j++)
					{  
						sudoku_answer_int[i]=sudoku_answer_psj[i]-'0';
					}
					flag=1;
					break;
				}
			case 0x09://tab
				form_driver(form,REQ_NEXT_FIELD);
				form_driver(form,REQ_END_LINE);
				break;
			case KEY_DOWN:
				form_driver(form, REQ_NEXT_FIELD);
				form_driver(form, REQ_END_LINE);
				break;

			case KEY_UP:
				form_driver(form, REQ_PREV_FIELD);
				form_driver(form, REQ_END_LINE);
				break;

			case KEY_LEFT:
				form_driver(form, REQ_PREV_CHAR);
				break;

			case KEY_RIGHT:
				form_driver(form, REQ_NEXT_CHAR);
				break;

				// Delete the char before cursor
			case KEY_BACKSPACE:
			case 127:
				form_driver(form, REQ_DEL_PREV);
				break;

				// Delete the char under the cursor
			case KEY_DC:
				form_driver(form, REQ_DEL_CHAR);
				break;

			default:
				form_driver(form, ch);
				break;
		}
		wrefresh(win_form);
		if(flag)break;
	}
	clear();
	//free all
	unpost_form(form);
	free_all(form,fields);
	delwin(win_form);
	delwin(win_body);
	endwin();

}
static char* trim_whitespaces(char *str)
{
	char *end;

	// trim leading space
	while(isspace(*str))
		str++;

	if(*str == 0) // all spaces?
		return str;

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while(end > str && isspace(*end))
		end--;

	// write new null terminator
	*(end+1) = '\0';

	return str;
}
static void set_field(FIELD *field[])
{

	set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
	for(int i=1;i<10;i++)
	{
		set_field_opts(fields[i], O_VISIBLE |O_PUBLIC | O_EDIT | O_ACTIVE);
		set_field_back(fields[i], A_UNDERLINE);
	}
}
static void free_all(FORM *form,FIELD *fields[])
{
	free_form(form);
	for(int i=0;i<11;i++)
	{
		free_field(fields[i]);
	}
}
