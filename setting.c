#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Setting{
	
	int miro_round;
	int sudoku_round;
	int basic_level;
	//난이도 당 곱하는 건 미로, 스도쿠에서 구현 완료
	int minus_score; //다인모드 감점 스코어

} Setting;


void setOption(Setting *set){
	printf("다인 모드의 미로 라운드 수 : ");
	scanf("%d",&set->miro_round);
	
	printf("다인 모드의 스도쿠 라운드 수 : ");
	scanf("%d",&set->sudoku_round);
	
	printf("다인 모드의 시작 시 기본 레벨 : ");
	scanf("%d",&set->basic_level);

	printf("각 라운드 당 오답 시 감점 점수 : ");
	scanf("%d",&set->minus_score);

	return;
}
