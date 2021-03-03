#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _sudoku{
	int origin[9][9];
	int problem[9][9];
}_Sudoku;

typedef struct _player{
	int sol[9][9];
	//+점수 등
}_Player;

_Sudoku sudoku;
_Player* player;

//int sudoku[9][9] = {0};
int ran;

void printSudoku(int sudo_p[][9]){
    for(int i = 0; i<9; i++){
        if(i%3==0)  printf("-------------------------\n");
        for(int j = 0; j<9; j++){
            if(j%3==0)  printf("| ");
            printf("%d ", sudo_p[i][j]);
       }
        printf("|\n");
    }
    printf("-------------------------\n");
}

//지정된 두 가로 줄 교환// a,b:서로 다른 3*9집합의 가로 줄 번호(a<b)
void tradeRow(int a, int b){
	int tmp;
	for(int j = a*3; j<a+3;j++)
		for(int i = 0; i<9; i++){
			tmp = sudoku.origin[j][i];
			sudoku.origin[j][i] = sudoku.origin[j+3*(b-a)][i];
			sudoku.origin[j+3*(b-a)][i] = tmp;
		}
}
//지정된 두 세로 줄 교환// a,b:서로 다른 3*9집합의 세로 줄 번호(a<b)
void tradeColumn(int a, int b){
	int tmp;
	for(int j = a*3; j<a+3; j++)
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
	num2 = rand()%9+1;
	for(int i = 0; i<9; i++)
		for(int j = 0; j<9; j++){
			if(sudoku.origin[i][j] == num1) sudoku.origin[i][j] = num2;
			if(sudoku.origin[i][j] == num2) sudoku.origin[i][j] = num1;
		}
}

//void removeNum(int level, int weight){
	

//파일 입출력을 통해 좋은 예시의 스도쿠 원형을 받음
void downloadSudoku(){
	FILE *fp = fopen("sudoku.txt","rt");
	if(fp==NULL) exit(-1);
	for(int i = 0; i<9; i++)
		for(int j = 0; j<9; j++){
			fscanf(fp, "%d ", &sudoku.origin[i][j]);
			if(sudoku.origin[i][j] == 0) printf("error:스도쿠가 제대로 입력되지 않았음\n");
		}
	fclose(fp);
}

//파일에 다음에 쓸 스도쿠 저장
void uploadSudoku(){
	FILE *fp = fopen("sudoku.txt","wt");
	if(fp==NULL) exit(-1);
	for(int i = 0; i<9; i++)
		for(int j = 0; j<9; j++){
			if(sudoku.origin[i][j] == 0) printf("error:스도쿠가 제대로 저장되지 않았음\n");
			fprintf(fp, "%d ", sudoku.origin[i][j]);
		}
	fclose(fp);
}

//스도쿠 오류 발생 시 수정
void editSudoku(){
	printf("수정할 스도쿠 입력:\n");
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++){
			scanf("%d", &sudoku.origin[i][j]);
			if(sudoku.origin[i][j] < 1 || sudoku.origin[i][j] > 9){
				printf("error: 1부터 9까지의 자연수를 입력하시오.\n");
				editSudoku();
				return ;
			}
		}
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
}
void makuSudokuProblem(){

}

void IN_sudoku(){
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9;j++){
            scanf("%d", &sudoku.problem[i][j]);
            sudoku.origin[i][j] = sudoku.problem[i][j];
            //if(!sudoku.problem[i][j]) blank++;
		}
    }
}

int main(){
	IN_sudoku();
	makeSudokuOrigin();
	printSudoku(sudoku.origin);
	return 0;
}
