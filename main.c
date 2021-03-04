#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "maze.h"
#include "member.h"

void soloMode();
void multiMode();

int main()
{
	int input;
	Info *player = malloc(sizeof(Info)); 
	startLogin(player); //로그인 함수
	
	printf("1. 1인모드 2. 다인모드 3. 명예의 전당 4. 설정 5. 로그아웃 6. 종료\n");
	printf("> ");
	scanf("%d", &input);

	switch(input) {
		case 1 : // 1인모드
			soloMode();
			break;
		case 2 : // 다인모드
			multiMode();
			break;
		case 3 : // 명예의 전당
			break;
		case 4 : // 설정
			break;
		case 5 : // 로그아웃
			logout(player);
			break;
		case 6 : // 종료
			exit(0);
			break;
		default :
			printf("없는 메뉴입니다.\n");
			break;
	}
	return 0;
}

void soloMode()
{
	int input;

	printf("1. 미로 2. 스도쿠\n");
	printf("> ");
	scanf("%d", &input);
	switch(input) {
		case 1:
			startMaze(SOLO);
			break;
		case 2:
			break;
		default :
			printf("없는 메뉴입니다.\n");
			break;
	}


}

void multiMode()
{
	startMaze(MULTI);
}
