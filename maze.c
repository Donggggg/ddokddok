#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "back_g.h"
#include "maze.h"
#define SOLO 1
#define MULTI 2
#include <menu.h>

int N;
int now;
int front;
int rear;
int maze[51][51];
int wall[51][51];
int visit[51][51];

xy queue[MAX_QUEUE_SIZE];
static WINDOW *my_menu_win;
static void create_Win(int level)
{
        initscr();
        start_color();
        cbreak();
 //       noecho();

        //create main menu window
        my_menu_win = newwin(9+level, 7+level, 10,50 ); 
        init_pair(1,COLOR_RED,COLOR_YELLOW);

        //print title and ddok ddok
        attron(COLOR_PAIR(1));
        mvwprintw(my_menu_win, 1, 1, "%s", "Start");
        attroff(COLOR_PAIR(1));
        mvwhline(my_menu_win, 2, 1, ACS_HLINE, 68);
        //cero line



        box(my_menu_win, 0, 0);
        refresh();
        wrefresh(my_menu_win);
        int c;    
        int flag=0;
        //while((c = wgetch(my_menu_win)) != 'a');   
        //endwin();
}



//방향 (상 하 좌 우)
int dx[4] = {0, 0, -1, 1};
int dy[4] = {1, -1, 0, 0};

int isEmpty(){
	if (front == rear){
		return 1;
	}
	else
		return 0;
}

void enqueue(int x,int y){
	xy temp;
	temp.x = x;
	temp.y = y;
	rear = rear % MAX_QUEUE_SIZE;
	queue[rear++] = temp;
}

xy dequeue(){
	front = front % MAX_QUEUE_SIZE;
	return queue[front++];
}
 
int startMaze(int mode, int level, Game *game){

	create_Win(level);
        
        srand(time(NULL));
        	
	N = 5 + level;
	//wall배열 최댓값으로 초기화
	for (int x = 1; x <= N; x++){
		for (int y = 1; y <= N; y++){
			wall[x][y] = MAX;
		}
	}

	if(mode == SOLO){
		

		makeMaze();

		visit[1][1] = 0;
		wall[1][1] = 0;

		enqueue(1,1);
		shortDistance(1, 1);
		enqueue(1, 1);
		breakWall(1, 1);
		
		return 	checkAnswer(mode, game);
	}
	else if(mode == MULTI){

		makeMaze();

		visit[1][1] = 0;
		wall[1][1] = 0;

		enqueue(1,1);
		shortDistance(1, 1);
		enqueue(1, 1);
		breakWall(1, 1);

		return checkAnswer(mode, game);
	}
}

//미로 출력 함수
void makeMaze(){
	
	//미로 받는 부분
	for (int x = 1; x <= N; x++){
		for (int y = 1; y <= N; y++){
			if (x == 1 && y == 1){
				maze[x][y] = 1;
			}
			else if (x == N && y == N){
				maze[x][y] = 1;
			}
			else{
				maze[x][y] = rand() % 2;
			}
		}
	}

	//미로 출력
        //
        int garo=1,cero=3;
	for (int x = 1; x <= N; x++,garo=1,cero++){
		for (int y = 1; y <= N; y++){
                        if(maze[x][y]==1)
                                mvwprintw(my_menu_win,cero,garo++," ");
                        else
                                mvwprintw(my_menu_win,cero,garo++,"X");

                                wrefresh(my_menu_win);
                                refresh();

		}
	}
}

//미로 벽뚫기 함수
void breakWall(int x, int y){
	int nx;
	int ny;
	while(! isEmpty()){
		xy pop = dequeue();
		for (int i = 0; i < 4; i++){
			nx = pop.x + dx[i];
			ny = pop.y + dy[i];

			//미로판을 아예 나갈 때
			if (nx < 1 || nx > N || ny < 1 || ny > N){
				continue;
			}
			
			//벽인 경우
			if (maze[nx][ny] == 0){
				if (wall[nx][ny] > wall[pop.x][pop.y] + 1){
					wall[nx][ny] = wall[pop.x][pop.y] + 1;
					enqueue(nx, ny);
				}
			}

			//길인 경우
			else if(maze[nx][ny] == 1){
				if (wall[nx][ny] > wall[pop.x][pop.y]){
					wall[nx][ny] = wall[pop.x][pop.y];
					enqueue(nx, ny);
				}
			}
		}
	}
}

//미로 최단거리 찾는 함수
void shortDistance(int x, int y){
	int nx;
	int ny;
	while(! isEmpty()){
		xy pop = dequeue();
		for (int i = 0; i < 4; i++){
			nx = pop.x + dx[i];
			ny = pop.y + dy[i];

			//미로판을 아예 나갈 때
			if (nx < 1 || nx > N || ny <1 || ny > N)
				continue;
			//벽인 경우
			if (maze[nx][ny] == 0)
				continue;
			//
			if (visit[nx][ny] == 0){
				visit[nx][ny] = visit[pop.x][pop.y] + 1;
				enqueue(nx, ny);
			}
		}
	}
}

//정답 확인 함수
int checkAnswer(int mode, Game *game){
	Answer answer;
	int wrong = 0;
	int maze_check; //미로가 탈출 가능하면 1, 아닐 경우 0
	int count = 0; //정답일 경우 1, 아닐 경우 0

	//탈출 가능한 미로인지 확인 여부
	if (wall[N][N] == 0){
		maze_check = 1;
	}
	else{
		maze_check = 0;
	}

	int player, isGameover;
	//정답 확인
	while (count != 1){
		
		if (mode == MULTI){
			//플레이어 번호
			mvprintw(LINES-2,34,"플레이어 번호(-1을 입력 시 게임을 저장하고 종료합니다.) : ");
                        scanw("%d",&player);
			if (player == -1){
				saveGame(game);
				exit(1);
			}

			if (game->plus_score[player-1] <= 0){
				printf("기회가 없습니다.\n");
				continue;
			}
		}

                //##//
                //##//
		//답 입력
		mvprintw(LINES-4,34,"Can escape this maze? (O or X) : ");
		scanw(" %c", &answer.Yes_No);
                if(wrong)
                {
                        mvprintw(LINES-6,34,"                                       ");
                        mvprintw(LINES-5,34,"                                       ");
                }
                mvprintw(LINES-6,34,"[+]your answer is [%c] ",answer.Yes_No);
		if (answer.Yes_No == 'O' || answer.Yes_No == 'o'){
			mvprintw(LINES-3,34,"What is the shortest cost for escaping this maze? : ");
                        scanw("%d",&answer.num);
                        mvprintw(LINES-5,34,"[+]your cost is [%d]",answer.num);
		}
		else if (answer.Yes_No == 'X' || answer.Yes_No == 'x'){
			mvprintw(LINES-3,34,"What is the number of walls you need to break to break through to escape maze? : ");
                        scanw("%d",&answer.num);
                        mvprintw(LINES-5,34,"[+]your cost is [%d]",answer.num);
		}
		//정답인지 확인
		if (maze_check == 1){
			if (answer.Yes_No == 'O' || answer.Yes_No == 'o'){
				if (answer.num == visit[N][N]){
					count = 1;
				}
				else{
                                        mvprintw(LINES-2,3,"Your answer is wrong");
					wrong++;
					if (mode == MULTI)
						game->plus_score[player-1] -= 50;
					continue;
				}
			}
			else{
                                mvprintw(LINES-2,3,"Your answer is wrong");
				wrong++;
				if (mode == MULTI)
					game->plus_score[player-1] -= 50;
				continue;
			}
		}
		else if (maze_check == 0){
			if (answer.Yes_No == 'X' || answer.Yes_No == 'x'){
				if (answer.num == wall[N][N]){
					count = 1;
				}
				else{
                                        mvprintw(LINES-2,3,"Your answer is wrong");
					wrong++;
					if (mode == MULTI)
						game->plus_score[player-1] -= 50;
					continue;
				}
			}
			else{
                                mvprintw(LINES-2,3,"Your answer is wrong");
				wrong++;
				if (mode == MULTI)
					game->plus_score[player-1] -= 50;
				continue;
			}
		}
		
		//모든 플레이어 기회 x
		if(mode==MULTI){
			isGameover = 1;
			for(int k = 0; k<game->people; k++){
				if(game ->plus_score[k] > 0) isGameover = 0;
			}
			if(isGameover){
				printf("무승부");
				break;
			}
		}
	}
        mvprintw(LINES-2,3,"Your answer is correct");
	if (mode == MULTI)
		game->score[player-1] += game->plus_score[player-1];
	return wrong;
}
