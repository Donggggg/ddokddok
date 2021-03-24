typedef struct Score{
	char name[30]; //한글 or 영어 최대 10자로 지정(영어만 or 한글만?)
	int score;
	int level; //난이도
	int miro_su; //미로, 스도쿠 판단
} Score;

void inputScore(char*, double, int, int);
void select_game_show();
void show_rank(int);
void printScore(int ChooseLank);
