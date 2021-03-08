typedef struct Setting{
	
	int miro_round;
	int sudoku_round;
	int basic_level;
	//난이도 당 곱하는 건 미로, 스도쿠에서 구현 완료
	int minus_score; //다인모드 감점 스코어

} Setting;

void sepOption(Setting *set);
