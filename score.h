
typedef struct Score{
	char name[30]; //한글 or 영어 최대 10자로 지정(영어만 or 한글만?)
	double score;
	int level; //난이도
	int miro_su; //미로, 스도쿠 판단
} Score;


void inputScore(Info *member, double score, int level, int miro_su);
void printScore();
