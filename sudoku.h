#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

int sudoku[9][9] = {0};

//스도쿠 랜덤 생성 "randomsudoku.c"
int ran;
void tradeRow(int,int);		//가로줄 1:1 교환 //첫 번째 매개변수<두 번째 매개변수
void tradeColumn(int,int);	//가로줄 1:1 교환 //위와 동일 
void tradeLine();			//줄 1:1 교환
void tradeAll();			//전체 교환
void rotation();			//회전
void symmetry();			//대칭
void tradeNumber();			//숫자끼리 교환

void downloadSudoku();		//스도쿠 "sudoku.txt"에서 입력받음
void uploadSudoku();		//스도쿠 "sudoku.txt"에 출력함
void editSudoku();			//스도쿠 수정

//스도쿠 관리 "sudoku.c"
void printSudoku();			//스도쿠 출력=====> 가독성 위해 출력 방식 수정 필요
int vertical(int,int);		//1*9집합 세로 탐색 > 중복 없으면 TURE
int horizontal(int,int);	//9*1집합 가로 탐색 > 위와 동일
int box(int,int,int);		//3*3집합 박스 탐색 > 위와 동일
int complete();				//스도쿠 완성 시 TRUE 리턴
int correctSudoku(int,int);	//스도쿠 정답 검증
