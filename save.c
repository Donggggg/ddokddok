#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "save.h"
#include <ncurses.h>

void setOption(Setting *set){
	mvprintw(11,34,"INPUT MIRO ROUND : ");
	scanw(" %d",&set->miro_round);
	mvprintw(11, 53, "%d", set->miro_round);
	mvprintw(12,34,"INPUT SUDOKU ROUND : ");
	scanw(" %d",&set->sudoku_round);
	mvprintw(12, 55, "%d", set->sudoku_round);
	set->minus_score = -50;
	return;
}

void deleteFile(){
	int dResult = remove(".save");
}

void setGame(Game *game){ // 새로운 게임 만들기
	mvprintw(10,34, "HOW MANY PLAYER(MAXIMUN 5)  : ");
	scanw(" %d",&game->people);
	mvprintw(10, 63, "%d", game->people);

	for(int i = 0; i < game->people; i++)
		game->score[i] = 0;

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
	FILE *fp = fopen(".save","wb");
	if(fp==NULL)
		mvprintw(LINES-6,34,"ERROR\n");
	else{
		fwrite(game,sizeof(Game),1,fp);
	}
	fclose(fp);

}

int roadGame(Game *game){ //리턴값이 0이면 세이브파일X | 리턴값이 1이면 세이브파일O + 저장
	FILE *fp1 = fopen(".save","rb+");
	char check;
	if(fp1==NULL)
		return 0;
	else{
		mvprintw(2, 34,"We have a Savefile. Do you want to load it? (Y,N)\n");
		scanw("%c", &check);
		mvprintw(2, 84,"%c", check);

		if(check == 'Y' || check == 'y'){
			rewind(fp1);

			mvprintw(4,34,"Load savefile.");
			refresh();
			usleep(500000);
			mvprintw(4,34,"Load savefile..");
			refresh();
			usleep(500000);
			mvprintw(4,34,"Load savefile...");
			refresh();
			usleep(500000);

			fread(game, sizeof(Game), 1, fp1);
			fclose(fp1);
			mvprintw(6,34,"Load Success!!");
			refresh();
			usleep(1000000);
			clear();
			return 1;
		}
		else
			return 0;
	}

}	
