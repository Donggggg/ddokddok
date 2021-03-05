#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>
#include "member.h"
#include "score.h"

/*
typedef struct Score{
	char name[30]; //한글 or 영어 최대 10자로 지정(영어만 or 한글만?)
	double score;
	int level; //난이도
	int miro_su; //미로, 스도쿠 판단
} Score;
*/


int getch(void){
    int ch;
    struct termios buf;
    struct termios save;
    tcgetattr(0, &save);
    buf = save;
    buf.c_lflag &= ~(ICANON | ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch=getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

void inputScore(Info *member, double score, int level, int miro_su){
	Score input = {"",0}; //라운드는 5로 고정(예)
	FILE *fp = fopen("score.txt","ab+");
	if(fp==NULL){
		printf("파일 오류입니다.\n");
		fclose(fp);
		return;
	}
	strcpy(input.name, member->nickname); //이름
	input.score = score; //점수(걸린 시간)
	input.level = level; //난이도
	input.miro_su = miro_su; //미로 스도쿠 (1 2)


	fseek(fp,0,SEEK_END); //맨 끝으로 이동
	fwrite(&input,sizeof(Score),1,fp); //이진 파일로 저장
	fclose(fp);
	printf("\n입력이 완료되었습니다.\n\n");
}


void printScore(){
	Score output[1000]={0,}, temp={0, }; //100명의 사람을 수용
	Score miro_out[10][100] = {0, }; //미로(1-10)
	Score sudo_out[10][100] = {0, }; //스도쿠(1-10)
	int miro[10] = {0, };
	int sudo[10] = {0, };
	int N = 0,  go; //갖고오는 사람 수
	FILE *fp = fopen("score.txt","rb+");
	if(fp==NULL){
		printf("파일 오류입니다.\n");
		return;
	}
	printf("\n\n");
	rewind(fp);
	while(fread(&output[N],sizeof(Score),1,fp)){
	//	printf("%-10s %-2d %-5f\n",output[N].name,output[N].level,output[N].score); //제대로 출력 확인 (주석)
		N+=1;
	}

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
			int check = output[i].level;
			miro_out[check-1][miro[check]] = output[i];
			miro[check] +=1;
		}
		else if(output[i].miro_su == 2){ //스도쿠
			int check = output[i].level;
			sudo_out[check-1][sudo[check]] = output[i];
			sudo[check] +=1;
		}
	}
	// miro_out[leve1-1] 이랑 sudo_out[level-1]에 각각 점수별로 저장된다.
	printf("확인하고 싶은 명예의 전당을 입력하세요 (1=미로, 2=스도쿠) : ");
	scanf("%d",&go);
	getchar(); //enter 입력 getch() 예방
	int level_leri = 0, rank_updo = 0, key_check;
	
	//미로 랭킹
	if(go == 1){
		while(1){
			system("clear");

			printf("%19s===================랭킹===================\n","");
			printf("%19s 순위          이름          난이도    점수(시간/초)\n","");
			if(rank_updo == 0){ //(1-10위)
				for(int i = 0; i < 10; i++)
					printf("%19s %02d위   %-16s       %-3d     %-5.1f초\n","", i+1, miro_out[level_leri][i].name, level_leri + 1, miro_out[level_leri][i].score);
				printf("\n랭킹 순위 이동 : 위 아래 방향키 | 난이도 이동 : 왼쪽 오른쪽 방향키 | 랭킹 그만보기 : Backspace\n");
			}
			else if(rank_updo == 1){ //(11-20위)
				for(int i = 10; i < 20; i++)
					printf("%19s %02d위   %-16s       %-3d     %-5.1f초\n","", i+1, miro_out[level_leri][i].name, level_leri + 1, miro_out[level_leri][i].score);	
				printf("\n랭킹 순위 이동 : 위 아래 방향키 | 난이도 이동 : 왼쪽 오른쪽 방향키 | 랭킹 그만보기 : Backspace\n");
			}
			else if(rank_updo == 2){ //(21-30위)
				for(int i = 20; i < 30; i++)
					printf("%19s %02d위   %-16s       %-3d     %-5.1f초\n","", i+1, miro_out[level_leri][i].name, level_leri + 1, miro_out[level_leri][i].score);
				printf("\n랭킹 순위 이동 : 위 아래 방향키 | 난이도 이동 : 왼쪽 오른쪽 방향키 | 랭킹 그만보기 : Backspace\n");
			}

			key_check = getch();
			if(key_check == 127) //backspace
				break;
			else if(key_check == 27){
				key_check = getch();
				if(key_check == 91){
					key_check = getch();
					if(key_check == 65){ //위
						rank_updo -=1;
					}
					else if(key_check == 66){ //아래
						rank_updo +=1;
					}
					else if(key_check == 68){ //왼쪽
						level_leri -=1;
					}
					else if(key_check == 67){ //오른쪽
						level_leri +=1;
					}
				}
			}

			else{ //다른 거 입력 시 아무 반응X
				continue;
			}

			if(level_leri>=9) //최대 난이도 10에서 -> 시 10
				level_leri = 9;
			if(level_leri<=0) //최소 난이도 0에서 <- 시 0
				level_leri = 0;
			if(rank_updo>=2) //최대 랭킹(21-30)에서 down 시 그대로
				rank_updo = 2;
			if(rank_updo<=0) //최소 랭킹(1-10)에서 up 시 그대로
				rank_updo = 0;

		}
	}

	//스도쿠 랭킹
	else if(go == 2){	
		while(1){
			system("clear");

			printf("%19s===================랭킹===================\n","");
			printf("%19s 순위          이름          난이도    점수(시간/초)\n","");
			if(rank_updo == 0){ //(1-10위)
				for(int i = 0; i < 10; i++)
					printf("%19s %02d위   %-16s       %-3d     %-5.1f초\n","", i+1, sudo_out[level_leri][i].name, level_leri + 1, sudo_out[level_leri][i].score);
				printf("\n랭킹 순위 이동 : 위 아래 방향키 | 난이도 이동 : 왼쪽 오른쪽 방향키 | 랭킹 그만보기 : Backspace\n");
			}
			else if(rank_updo == 1){ //(11-20위)
				for(int i = 10; i < 20; i++)
					printf("%19s %02d위   %-16s       %-3d     %-5.1f초\n","", i+1, sudo_out[level_leri][i].name, level_leri + 1, sudo_out[level_leri][i].score);	
				printf("\n랭킹 순위 이동 : 위 아래 방향키 | 난이도 이동 : 왼쪽 오른쪽 방향키 | 랭킹 그만보기 : Backspace\n");
			}
			else if(rank_updo == 2){ //(21-30위)
				for(int i = 20; i < 30; i++)
					printf("%19s %02d위   %-16s       %-3d     %-5.1f초\n","", i+1, sudo_out[level_leri][i].name, level_leri + 1, sudo_out[level_leri][i].score);
				printf("\n랭킹 순위 이동 : 위 아래 방향키 | 난이도 이동 : 왼쪽 오른쪽 방향키 | 랭킹 그만보기 : Backspace\n");
			}
			
			key_check = getch();
			if(key_check == 127) //backspace
				break;
			else if(key_check == 27){
				key_check = getch();
				if(key_check == 91){
					key_check = getch();
					if(key_check == 65){ //위
						rank_updo -=1;
					}
					else if(key_check == 66){ //아래
						rank_updo +=1;
					}
					else if(key_check == 68){ //왼쪽
						level_leri -=1;
					}
					else if(key_check == 67){ //오른쪽
						level_leri +=1;
					}
				}
			}
			else{ //다른 거 입력 시 아무 반응X
				continue;
			}

			if(level_leri>=9) //최대 난이도 10에서 -> 시 10
				level_leri = 9;
			if(level_leri<=0) //최소 난이도 0에서 <- 시 0
				level_leri = 0;
			if(rank_updo>=2) //최대 랭킹(21-30)에서 down 시 그대로
				rank_updo = 2;
			if(rank_updo<=0) //최소 랭킹(1-10)에서 up 시 그대로
				rank_updo = 0;

		}
	}

	else{
		printf("잘못 입력하셨습니다.\n");
	}


}


int main(){
	Info *member = malloc(sizeof(Info));
	strcpy(member->nickname,"Parkyusu");
	printScore();
}
