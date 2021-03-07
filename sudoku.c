#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"
#include "main.h"

_Sudoku sudoku;
_Player* player;
 
int ran;
int blank = 0;

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

int box(int n, int x, int y, int arr[][9]){
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			if(arr[(y/3)*3+i][(x/3)*3+j] == n){
				return FALSE;
			}
		}
	}
	return TRUE;
}

int completeSudoku(int tmp){ return tmp == blank; }

int correctSudoku(_Player* pp){
	int k, tmp = 0;
	for(int i=0; i<9;i++)
		for(int j = 0; j<9;j++){
			if(!sudoku.problem[i][j]){
			//빈 칸인 경우
				k = pp->input[i][j];
				if(!vertical(k,j,pp->sol) || !horizontal(k,i,pp->sol) || !box(k,j,i,pp->sol))
					return FALSE; //수가 스도쿠 규칙에 위반할 경우 FALSE 리턴
				else {
					pp->sol[i][j] = k;
					tmp++;
				}
			}
			else {
				if(sudoku.problem[i][j] != pp->input[i][j]) 
					return FALSE;
			}
			//이미 주어진 숫자를 잘못 입력한 경우 오류 메시지 출력 및 -1리턴
		}
	return tmp; //모든 빈칸을 잘 채운 경우 TRUE 리턴
}

void printSudoku(int sudo_p[][9]){
    for(int i = 0; i<9; i++){
        //if(i%3==0)  printf("-------------------------\n");
        for(int j = 0; j<9; j++){
           // if(j%3==0)  printf("| ");
            if(sudo_p[i][j] == 0)
				printf("  ");
			else 
				printf("%d ", sudo_p[i][j]);
       }
        printf("\n");
    }
    //printf("-------------------------\n");
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
	FILE *fp = fopen("sudoku.txt","rt");
	if(fp==NULL) exit(-1);
	for(int i = 0; i<9; i++)
		for(int j = 0; j<9; j++){
			int tmp = fscanf(fp, "%d ", &sudoku.origin[i][j]);
			if(sudoku.origin[i][j] == 0 || tmp == EOF) printf("error:스도쿠가 제대로 입력되지 않았음\n");
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
	int er = 0;
	printf("수정할 스도쿠 입력:\n");
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++){
			scanf("%d", &sudoku.origin[i][j]);
			if(sudoku.origin[i][j] < 1 || sudoku.origin[i][j] > 9){
				printf("error: 1부터 9까지의 자연수를 입력하시오.\n");
				er = 1;
			}
		}
	if(er) editSudoku();
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

//////////////////////////////////////
//       입력 시 ui 수정 필요       //
//////////////////////////////////////
void IN_sudoku(_Player* pop){
	char ch;
	getchar();
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9;j++){
            scanf("%d", &pop->input[i][j]);
			pop->sol[i][j] = sudoku.problem[i][j];
		}
    }
}

int playSudoku(int player_num, int level){
	int input_num = 0, wrong = 0; 	//답 입력 개수
	int cor = FALSE; 	//정답 여부
//라운드 별 반복 구간
	//문제 생성
	makeSudokuOrigin();
	makeSudokuProblem(level);
	printSudoku(sudoku.problem);
	printSudoku(sudoku.origin);

	while(!cor){
		IN_sudoku(player); //[player_num]); //해당 플레이어의 구조체 주소 전송
		input_num = correctSudoku(player); //[player_num]);
		if(input_num == FALSE){		//답이 틀린 경우
			printf("오답\n");
			wrong++;
		}
		else if(input_num == -1){	//기존의 주어진 문제를 잘못 입력한 경우
			printf("오답\n");
			wrong++;
		}
		//위 두 케이스를 구분할지 말지 정하지 않음
		else{
			cor = completeSudoku(input_num);
			if(cor) {
				printf("정답\n");
				printSudoku(player->sol);
			}
		}
	}
	return wrong;
}

int startSudoku(int mode,int level){
	downloadSudoku();
	if(mode == SOLO){
		player = (_Player*)malloc(sizeof(_Player)*1);
		int w = playSudoku(1,level);
		free(player);
		player = NULL;
		return w;
	}
	else if(mode == MULTI){
	}
	uploadSudoku();
}
