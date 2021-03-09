#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "setting.h"
#include "back_g.h"
#include "maze.h"
#include "member.h"
#include "sudoku.h"

#include "score.h"


void soloMode();
void multiMode();

Game *game; //멀티모드 진행 게임 상황
Setting *set; //설정 저장
Info *player;

int main()
{
	int input;
	game = malloc(sizeof(Game)); //멀티모드 진행 게임 상황
	set = malloc(sizeof(Setting)); //설정 저장
	player = malloc(sizeof(Info)); 

	startLogin(player); //로그인 함수

	do{	
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
				setOption(set);
				break;
			case 5 : // 로그아웃
				logout(player);
				break;
			case 6 : // 종료
				break;
			default :
				printf("없는 메뉴입니다.\n");
				break;
		}
	} while (input != 6);
	
	return 0;
}

void soloMode()
{
	int level;
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
			wrong = startMaze(SOLO,level, game);
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
	int road_check = roadGame(game);	//인자 포인터?
	if(road_check == 1){
		//세이브파일 존재 : game에는 이전 게임 정보가 삽입
	}
	else{
		setGame(game, set);	//인자 포인터?
	
	}
	int i = 0;
	while (game->round[0] <= game->round[1]){
		printf("\n%d라운드입니다\n", game->round[0]);
		for (int j = 0; j < game -> people; j++){
			game->plus_score[j] = 100 * game->round[0];
		}

		if (game->game_select[i] == 1){
			startMaze(MULTI,game->round[0],game);
			game->round[0] += 1;
		}
		else if (game->game_select[i] == 0){
			startSudoku(MULTI, game->round[0]);
			game->round[0] += 1;
		}
		else{
			printf("오류입니다");
			break;
		}
	}
}
