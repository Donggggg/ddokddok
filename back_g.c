#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Miro{
	//미로 크기 받아서 저장함
} Miro;
typedef struct Sudoku{
	//스도쿠 관련 저장
} Sudoku;

typedef struct Game{ 
	int people; //최대 인원을 5명으로 가정
	int level;
	int game_select; //1은 스도쿠, 2는 미로
	int round[2]; // 현재 라운드/총라운드
	int score[5];
	int plus_score[5];
	int sudoku[9][9];
	int miro[10][10]; //일단 10*10
	//미로, 스도쿠 정답

} Game;



void game_set(Game *game, int number){ // 새로운 게임 만들기
	printf("\n인원 수는 : ");
	scanf("%d",&game->people);
	printf("현재 레벨은 : ");
	scanf("%d",&game->level);
	game->game_select = number;
	printf("현재, 총 라운드는(ex.1 5) : ");
	scanf("%d %d",&game->round[0],&game->round[1]);

	printf("각 인원별 점수\n");
	for(int i=0; i<game->people; i++){
		printf("%d번째 : ",i+1);
		scanf("%d",&game->score[i]);
	}

	printf("각 인원별 라운드 점수\n");
	for(int i=0; i<game->people; i++){
		printf("%d번째 : ",i+1);
		scanf("%d",&game->plus_score[i]);
	}

	if(number==1){
		for(int i=0; i<9; i++)
			for(int j=0; j<9; j++)
				game->sudoku[i][j] = rand()%9+1; //그냥 랜덤 수 넣기
	}
	else if(number==2){
		for(int i=0; i<10; i++)
			for(int j=0; j<10; j++)
				game->miro[i][j] = rand()%2; //랜덤 0 1
	}


}

void game_save(Game game){ //세이브파일 만들기
	FILE *fp = fopen("back_up.txt","wb");
	if(fp==NULL)
		printf("저장하려는 파일에 오류가 있습니다.\n");
	else{
		fwrite(&game,sizeof(Game),1,fp);
	}
	fclose(fp);

}
/*
   void game_road(Game *game, FILE *fp1){
   if(fp1==NULL){
   printf("세이브파일에 오류가 있습니다. 죄송합니다\n");
   return;
   }
   fread(game, sizeof(Game),1,fp1);

   }	
*/ 

void game_print(Game game){ //구조체에 저장된 게임 출력
	// 새 게임은 바로 구조체에 저장, 로드 게임은 파일에서 구조체에 입력(이진)
	printf("\n======================================\n");
	printf("인원 수는 : %d\n",game.people);
	printf("현재 레벨은 : %d\n",game.level);
	if(game.game_select==1)
		printf("현재 게임은 : 스도쿠\n");
	else
		printf("현재 게임은 : 미로 찾기\n");
	printf("현재, 총 라운드는 : %d / %d\n",game.round[0],game.round[1]);
	printf("각 인원별 점수\n");
	for(int i=0; i<game.people; i++){
		printf("%d번째 : %d\n",i+1,game.score[i]);
	}

	printf("각 인원별 라운드 점수\n");
	for(int i=0; i<game.people; i++){
		printf("%d번째 : %d\n",i+1,game.plus_score[i]);
	}

	if(game.game_select==1){
		printf("스도쿠 출력\n");
		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				printf("%d ",game.sudoku[i][j]);
			}
			printf("\n");
		}
	}
	else if(game.game_select==2){
		printf("미로 출력\n");
		for(int i=0; i<10; i++){
			for(int j=0; j<10; j++){
				printf("%d ",game.miro[i][j]);
			}
			printf("\n");
		}
	}
}

void delete_file(){
	int dResult = remove("back_up.txt");
	if(dResult == -1){
		printf("파일 삭제가 실패했습니다.\n파일이 존재하는지 확인하고 다시 시도해 주세요.\n");
	}
}


int main(){ //새 게임을 시작하거나 기존 게임을 불러오는 경우 세이브파일 삭제
	Game game = {0};
	FILE *fp1 = fopen("back_up.txt","rb"); //무조건 읽는 걸로
	int go,re;
	srand(time(NULL));
	printf("\n===============================================\n");
	printf("1. 스도쿠 / 2. 미로 게임 / 3. 게임 출력 / 4. 저장 / 5. 저장 불러오기 / 6. 종료\n");
	printf("입력 : ");
	scanf("%d",&go);
	while(go!=6){

		if(go==1){
			game_set(&game,1);
			delete_file();
		}
		else if(go==2){
			game_set(&game,2);
			delete_file();
		}
		else if(go==3){
			game_print(game);
		}

		else if(go==4){
			printf("현재 상황을 저장합니다. 과거 내용은 삭제됩니다.\n");
			game_save(game);
		}
		else if(go==5){
			if(fp1==NULL)
				printf("세이브파일이 존재하지 않거나  오류가 생겼습니다.\n");
			else{
				fread(&game,sizeof(Game),1,fp1);
				delete_file();
			}
		}

		else
			break;



		printf("\n===============================================\n");
		printf("1. 스도쿠 / 2. 미로 게임 / 3. 게임 출력 / 4. 저장 / 5. 저장 불러오기 / 6. 종료\n");
		printf("입력 : ");
		scanf("%d",&go);

	}

}
