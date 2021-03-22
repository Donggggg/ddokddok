#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "back_g.h"
#include <ncurses.h>

void setOption(Setting *set){
	mvprintw(11,34,"INPUT MIRO ROUND : ");
	scanw(" %d",&set->miro_round);
	mvprintw(11, 53, "%d", set->miro_round);
	//	set->miro_round = Miro_round;

	mvprintw(12,34,"INPUT SUDOKU ROUND : ");
	scanw(" %d",&set->sudoku_round);
	mvprintw(12, 55, "%d", set->sudoku_round);
	//	set->sudoku_round = Sudoku_round;
	return;
}


void deleteFile(){
	int dResult = remove("back_up.txt");
}

void setGame(Game *game){ // 새로운 게임 만들기
	mvprintw(10,34, "HOW MANY PLAYER(MAXIMUN 5)  : ");
	scanw(" %d",&game->people);
	mvprintw(10, 63, "%d", game->people);

	Setting *set = malloc(sizeof(Setting));
	setOption(set);

	game->minus_score = set->minus_score;
	mvprintw(13,34,"MINUS SCORE : %d", game->minus_score);


	mvprintw(14,34,"MIRO ROUND : %d round  |  SUDOKU ROUND : %d round  |  ALL %d ROUND",set->miro_round, set->sudoku_round, (set->miro_round) + (set->sudoku_round));
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
	mvprintw(16,34,"ORDER IS....");
	for(int i = 0; i < game->round[1]; i++){
		if(game->game_select[i] == 1) // 17 18 19 20 21
			mvprintw(i+17,34,"%d round : MIRO ROUND",i+1);
		else if(game->game_select[i] == 2)
			mvprintw(i+17,34,"%d round : SUDOKU ROUND",i+1);
	}

	mvprintw(25,34,"Let's start Game!!");
	mvprintw(26,34,"PRESS enter....");
	deleteFile(); //기존의 세이브파일을 삭제
	free(set);
	mvgetch(1,2);	
	clear();

}

void saveGame(Game *game){ //세이브파일 만들기
	FILE *fp = fopen("back_up.txt","wb");
	if(fp==NULL)
		mvprintw(LINES-6,34,"ERROR\n");
	else{
		fwrite(game,sizeof(Game),1,fp);
	}
	fclose(fp);

}


int roadGame(Game *game){ //리턴값이 0이면 세이브파일X | 리턴값이 1이면 세이브파일O + 저장
	FILE *fp1 = fopen("back_up.txt","rb+");
	int check;
	if(fp1==NULL){
		//		mvprintw(LINES-6,34,"세이브파일에 오류가 있습니다. 죄송합니다\n");
		return 0;
	}
	else{
		mvprintw(2,34,"We have a Savefile. Road it? (1|0)\n");
		scanw("%d", &check);
		//	check = Loadok;
		if(check == 1){
			rewind(fp1);
			mvprintw(4,34,"Road savefile....\n");
			fread(game, sizeof(Game), 1, fp1);
			fclose(fp1);
			mvprintw(6,34,"Road Success.\n");
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
