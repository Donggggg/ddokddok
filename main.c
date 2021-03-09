#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "maze.h"
#include "member.h"
#include "sudoku.h"

#include "score.h"
#include "setting.h"
//#include "back_g.h"

void soloMode();
void multiMode();

int main()
{
	int input;

	Game *game = malloc(sizeof(Game)); //멀티모드 진행 게임 상황
	Setting *set = malloc(sizeof(Setting)); //설정 저장

	Info *player = malloc(sizeof(Info)); 
	startLogin(player); //로그인 함수
	
	printf("1. 1인모드 2. 다인모드 3. 명예의 전당 4. 설정 5. 로그아웃 6. 종료\n");
	printf("> ");
	scanf("%d", &input);

	switch(input) {
		case 1 : // 1인모드
			soloMode();
			break;
		case 2 : // 다인모드
			multiMode();
			break;
		case 3 : // 명예의 전당
			printScore();
			break;
		case 4 : // 설정
			setOption(&set); 		//인자 포인터?
			break;
		case 5 : // 로그아웃
			logout(player);
			break;
		case 6 : // 종료
			exit(0);
			break;
		default :
			printf("없는 메뉴입니다.\n");
			break;
	}
	return 0;
}


int level;

void soloMode()
{
	int input, wrong, regist;
	double times;
	time_t start, end;
	wrong = 0;
	printf("1인모드입니다.\n");
	printf("1. 미로 2. 스도쿠\n");
	printf("> ");
	scanf("%d", &input);
	switch(input) {
		case 1:
			printf("난이도(1 ~ 10)까지 입력 : ");
			scanf("%d", &level);
			printf("\n");
			start = time(NULL);
			wrong = startMaze(SOLO,level);
			end = time(NULL);
			times = (double)(end-start) + (double)(wrong*10);
			printf("점수 : %.2lf\n", times);
			printf("점수를 등록하시겠습니까? (1/0)");
			scanf("%d",&regist);
			if(regist == 1){
				inputScore(player, times, level, input);
			}

			break;
		case 2:
			printf("난이도(1 ~ 10)까지 입력 : ");
			scanf("%d", &level);
			printf("\n");
			start = time(NULL);
			wrong = startSudoku(SOLO,level);
			end = time(NULL);
			times = (double)(end-start) + (double)(wrong*10);
			printf("점수 : %.2lf\n", times);
			printf("점수를 등록하시겠습니까? (1/0)");
			scanf("%d",&regist);
			if(regist == 1){
				inputScore(player, times, level, input);
			}


			break;
		default :
			printf("없는 메뉴입니다.\n");
			break;
	}


}

void multiMode()
{
	int road_check = roadGame(&game);	//인자 포인터?
	if(road_check == 1){
		//세이브파일 존재 : game에는 이전 게임 정보가 삽입
	}
	else{
		setGame(&game, &set);	//인자 포인터?
	}

	startMaze(MULTI,level);
}
