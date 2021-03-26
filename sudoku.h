#define TRUE 1
#define FALSE 0

typedef struct _sudoku{
	int origin[9][9];
	int problem[9][9];
}_Sudoku;

typedef struct _player{
	int sol[9][9];
	int input[9][9];
}_Player;

void tradeRow(int,int);		//가로줄 1:1 교환 //첫 번째 매개변수<두 번째 매개변수
void tradeColumn(int,int);	//가로줄 1:1 교환 //위와 동일 
void tradeLine();			//줄 1:1 교환
void tradeAll();			//전체 교환
void rotation();			//회전
void symmetry();			//대칭
void tradeNumber();			//숫자끼리 교환

void downloadSudoku();		//스도쿠 "sudoku.txt"에서 입력받음
void uploadSudoku();		//스도쿠 "sudoku.txt"에 출력함
void makeSudokuOrigin();	//스도쿠 원형 랜덤 생성
void makeSudokuProblem(int);//입력-레벨, 원형에서 랜덤으로 레벨에 맞게  문제 생성

int vertical(int,int,int[][9]);		//1*9집합 세로 탐색 > 중복 없으면 TURE
int horizontal(int,int,int[][9]);	//9*1집합 가로 탐색 > 위와 동일
int box_sudoku(int,int,int,int[][9]);		//3*3집합 박스 탐색 > 위와 동일

void printSudoku(int[][9]);	//입력 받은 스도쿠 판 출력
int correctSudoku(_Player*);	//스도쿠 정답 검증

int startSudoku(int,int,Game*);		//입력-모드,레벨
int playSudoku(int,int,Game*);	//입력-모드,레벨, 전체적인 기능 불러오기

void sudoku_answer();
