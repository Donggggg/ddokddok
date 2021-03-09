#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
//#include "maze.h"
//#include "sudoku.h"
#include "setting.h"
#include "back_g.h"

void deleteFile(){
	int dResult = remove("back_up.txt");
/*	if(dResult == -1){
		printf("파일 삭제가 실패했습니다.\n파일이 존재하는지 확인하고 다시 시도해 주세요.\n");
		} */
}

void setGame(Game *game, Setting *set){ // 새로운 게임 만들기
	printf("\n플레이할 인원 수를 입력하세요(최대 5인) : ");
	scanf("%d",&game->people);


	game->minus_score = set->minus_score;
	printf("\n각 라운드 당 감점 점수는 %d 입니다.\n", game->minus_score);


	printf("\n총 라운드 수는 미로 %d라운드 | 스도쿠 %d라운드 | 총 %d 라운드 입니다.\n",set->miro_round, set->sudoku_round, (set->miro_round) + (set->sudoku_round));
	game->round[1] = (set->miro_round) + (set->sudoku_round);
	game->round[0] = 1; //처음은 1라운드

	int check_miro = set->miro_round, check_sudoku = set->sudoku_round;
	for(int i = 0; i < game->round[1]; i++){ //1은 미로, 2는 스도쿠
		int check = rand()%2 + 1; //랜덤값 1 or 2
		if(check == 1 && check_miro>0){
			game->game_select[i] = check;
			check_miro -= 1;
		}
		else if(check == 2 && check_sudoku>0){
			game->game_select[i] = check;
			check_sudoku -= 1;
		}
		else
			i-=1; //다시 반복
	}
	printf("\n라운드 진행은\n\n");
	for(int i = 0; i < game->round[1]; i++){
		if(game->game_select[i] == 1)
			printf("%d라운드 : 미로 게임\n",i+1);
		else if(game->game_select[i] == 2)
			printf("%d라운드 : 스도쿠 게임\n",i+1);
	}
	printf("\n입니다.\n");

	printf("\n\n게임을 시작하겠습니다.\n\nenter를 눌러주세요....\n");
	deleteFile(); //기존의 세이브파일을 삭제
	getchar();
	getchar();

}

void saveGame(Game game){ //세이브파일 만들기
	FILE *fp = fopen("back_up.txt","wb");
	if(fp==NULL)
		printf("저장하려는 파일에 오류가 있습니다.\n");
	else{
		fwrite(&game,sizeof(Game),1,fp);
	}
	fclose(fp);

}


int roadGame(Game *game){ //리턴값이 0이면 세이브파일X | 리턴값이 1이면 세이브파일O + 저장
	FILE *fp1 = fopen("back_up.txt","rb");
	int check;
	if(fp1==NULL){
//		printf("세이브파일에 오류가 있습니다. 죄송합니다\n");
		return 0;
	}
	else{
		printf("세이브파일이 존재합니다. 불러오시겠습니까? (1|0)\n");
		scanf("%d", &check);
		if(check == 1){
			printf("세이브파일 불러오는 중....\n");
			fread(&game, sizeof(Game), 1, fp1);
			fclose(fp1);
			printf("세이브파일 불러오기가 성공했습니다.\n");
			return 1;
		}
		else
			return 0;
	}

}	





/*
int main(){ //새 게임을 시작하거나 기존 게임을 불러오는 경우 세이브파일 삭제

	srand(time(NULL));

	Game game = {0, };
	Setting set = {3, 2, 1, 10}; //미로 3라운드 스도쿠 2라운드 기본 레벨 1 감점 10
	setGame(&game, &set);

}
*/