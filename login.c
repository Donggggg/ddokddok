#include <stdio.h>
#include <assert.h>
#include <form.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <err.h>
#include "login.h"

int check_in;
static FORM *form;
static FIELD *fields[5];
static WINDOW *win_body, *win_form;
static char ID[255];
static char PW[255];
static int flag;
static char* trim_whitespaces(char *str);
static void set_field(FIELD *field[]);
static void free_all(FORM *form,FIELD *fields[]);
static void print_logo(WINDOW *my_menu_win);

void login_UI(Info *player)
{
	//init window tab
	initscr();
	start_color();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);

	//create main window and form window
	win_body = newwin(30,70, 4, 4);
	win_form = derwin(win_body, 20, 60, 4,4 );//20, 60 ,4 ,4
	mvwprintw(win_body, 1, 15, "Welcome to ddok ddok games");
	//declare filed location
	fields[0] = new_field(1, 10, 0, 10, 0, 0);
	fields[1] = new_field(1, 40, 0, 15, 0, 0);
	fields[2] = new_field(1, 10, 2, 10, 0, 0);
	fields[3] = new_field(1, 40, 2, 15, 0, 0);
	fields[4] = NULL;

	set_field_buffer(fields[0], 0, "ID : ");
	set_field_buffer(fields[2], 0, "PW : ");

	//set field options
	set_field(fields);

	form = new_form(fields);

	set_form_win(form, win_form);
	set_form_sub(form, derwin(win_form, 18, 76, 1, 1));//18
	post_form(form);
	print_logo(win_body);
	box(win_body, 0, 0);

	//refresh all
	refresh();
	wrefresh(win_body);
	wrefresh(win_form);

	//input id and password
	int ch;
	while (((ch = getch())))
	{
		int i;

		switch (ch) {
			case 10://enter
				{
					// Or the current field buffer won't be sync with what is displayed
					form_driver(form, REQ_NEXT_FIELD);
					form_driver(form, REQ_PREV_FIELD);
					move(LINES-3, 5);
					strcpy(ID, trim_whitespaces(field_buffer(fields[1], 0)));
				//	mvprintw(30,20,"ID :%s", ID);
					strcpy(PW, trim_whitespaces(field_buffer(fields[3], 0)));
				//	mvprintw(30,30," PW: %s",PW);
					refresh();
					pos_form_cursor(form);
					
					check_in=login(ID,PW);

					//true file offset
					//false return -1
					if(check_in != -1){ 
						*player = login2(check_in);
						flag=1;
					}
					else{
						mvprintw(31,25,"Incorrect!!");
					}
					break;
				}
			case 0x09://tab
				form_driver(form,REQ_NEXT_FIELD);
				form_driver(form,REQ_END_LINE);
				break;
			case KEY_DOWN:
				form_driver(form, REQ_NEXT_FIELD);
				form_driver(form, REQ_END_LINE);
				break;

			case KEY_UP:
				form_driver(form, REQ_PREV_FIELD);
				form_driver(form, REQ_END_LINE);
				break;

			case KEY_LEFT:
				form_driver(form, REQ_PREV_CHAR);
				break;

			case KEY_RIGHT:
				form_driver(form, REQ_NEXT_CHAR);
				break;

				// Delete the char before cursor
			case KEY_BACKSPACE:
			case 127:
				form_driver(form, REQ_DEL_PREV);
				break;

				// Delete the char under the cursor
			case KEY_DC:
				form_driver(form, REQ_DEL_CHAR);
				break;

			default:
				form_driver(form, ch);
				break;
		}
		wrefresh(win_form);
		if(flag)break;
	}
	clear();
	//free all
	unpost_form(form);
	free_all(form,fields);
	delwin(win_form);
	delwin(win_body);
	endwin();

}
static char* trim_whitespaces(char *str)
{
	char *end;

	// trim leading space
	while(isspace(*str))
		str++;

	if(*str == 0) // all spaces?
		return str;

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while(end > str && isspace(*end))
		end--;

	// write new null terminator
	*(end+1) = '\0';

	return str;
}
static void set_field(FIELD *field[])
{

	set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
	set_field_opts(fields[1], O_VISIBLE |O_PUBLIC | O_EDIT | O_ACTIVE);
	set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
	set_field_opts(fields[3], O_VISIBLE | O_EDIT | O_ACTIVE);
	field_opts_off(fields[3],O_PUBLIC);
	set_field_back(fields[1], A_UNDERLINE);
	set_field_back(fields[3], A_UNDERLINE);
}
static void free_all(FORM *form,FIELD *fields[])
{
	free_form(form);
	free_field(fields[0]);
	free_field(fields[1]);
	free_field(fields[2]);
	free_field(fields[3]);
}



static void print_logo(WINDOW *my_menu_win)
{
	char line[255];
	FILE *fp;
	fp=fopen(".logo","r");
	if(fp==NULL)    err(EXIT_FAILURE,"NO logo file");
	int i=12;
	while(fgets(line,sizeof(line),fp)!=NULL)
	{
		mvwprintw(my_menu_win, i++, 4, "%s", line);
	}
}

//회원가입 함수
void inputInfo(){
	FILE *fp = fopen(".member", "ab+");
	Info input, person_list[100];
	int N = 0, noone_check = 0; //갖고 올 회원 수, 회원이 없을 때 회원번호 판별 변수

	rewind(fp); //회원 리스트에다가 기존 회원의 정보들 삽입
	while(fread(&person_list[N], sizeof(Info), 1, fp)){ 
		N += 1;
		noone_check = 1;
	}
	// person_list 0~N-1에는 회원 정보가 담겨있다!

	while(1){
		int isExist = 0;
		printf("사용하실 ID를 입력하세요 : ");
		scanf("%s", input.id);

		for(int i = 0; i < N; i++){
			if(strcmp(input.id, person_list[i].id) == 0){ //만약 닉네임이 겹치면
				printf("겹치는 ID가 존재합니다. 다시 입력해주세요\n\n");
				isExist = 1;
				break;
			}
		}
		if(isExist == 0)
			break;
	}
	printf("패스워드를 입력하세요(4-12자) : ");
	scanf("%s", input.password);
	printf("이름을 입력하세요 : ");
	scanf("%s", input.name);
	printf("이메일을 입력하세요 : ");
	scanf("%s", input.email);
	printf("전화번호를 (-)을 생략하고 입력하세요 : ");
	scanf("%s", input.phone);
	while(1){
		int isExist = 0;
		printf("사용하실 닉네임을 입력하세요 : ");
		scanf("%s", input.nickname);
		for(int i = 0; i < N; i++){
			if(strcmp(input.id, person_list[i].id) == 0){ //만약 닉네임이 겹치면
				printf("겹치는 ID가 존재합니다. 다시 입력해주세요\n\n");
				isExist = 1;
				break;
			}
		}
		if(isExist == 0)
			break;
	}


	input.manage = 0;


	if(noone_check == 0) //아무도 없는 경우 회원번호
		input.member_number = N + 1;
	else
		input.member_number = person_list[N-1].member_number + 1;


	fseek(fp,0,SEEK_END); //맨 끝으로 이동
	fwrite(&input, sizeof(Info), 1, fp); //이진 파일로 저장
	printf("\n회원가입이 완료되었습니다!\n\n");
	fclose(fp);

}

Info login2(int log_num){
	FILE *fp = fopen(".member", "ab+");
	Info person_list[100];
	int N = 0;
	rewind(fp);
	while(fread(&person_list[N], sizeof(Info), 1, fp)){
		N += 1;
	}
	fclose(fp);

	return person_list[log_num];

}


//로그인 함수
int login(char ID[255], char PW[255]){
	FILE *fp = fopen(".member", "ab+");
	Info person_list[100];
	char check_id[30], check_password[30];
	int isExist = 0, N = 0, me; //로그인 닉네임 체크, 갖고 올 회원 수, 몇 번째 회원

	rewind(fp); //회원 리스트에다가 기존 회원의 정보들 삽입
	while(fread(&person_list[N], sizeof(Info), 1, fp)){ 
		N += 1;
	}

	fclose(fp);

	int isokay = 0;
	strcpy(check_id, ID);

	for(int i = 0; i < N; i++){
		if(strcmp(check_id, person_list[i].id) == 0){ //만약 닉네임이 겹치면
			isExist = 1;
			me = i; // 해당 번째 인물 위치 저장
			isokay = 1; //while 탈출!
			break;
		}
	}

	if(strcmp("",check_id) == 0)
		return -1;

	if(isExist == 0){
		return -1;
	}

	strcpy(check_password, PW);
	if(strcmp(check_password, person_list[me].password) == 0){
		//printf("\n로그인이 성공했습니다.\n\n");
		return me;
	}
	else{
		//printf("패스워드가 틀립니다.\n\n");
		return -1;
	}

}

//ID랑 PW를 찾는 함수
void searchIdPass(int check){
	FILE *fp = fopen(".member","ab+");
	Info person_list[100];
	int N = 0, idExist = 0, M = 0;
	char input_info[5][30]; // 0:ID 1:이름 2:닉네임 3:이메일 4:번호
	rewind(fp);

	while(fread(&person_list[N], sizeof(Info), 1, fp)){ 
		N += 1;
	}

	if(check==1){ //ID 찾기
		printf("ID를 찾기 위한 정보를 입력하세요\n");
		printf("이름 : ");
		scanf("%s", input_info[1]);
		printf("E-mail : ");
		scanf("%s", input_info[3]);
		printf("번호(휴대폰 - 없이) : ");
		scanf("%s", input_info[4]);

		for(int i = 0; i < N; i++){
			if(strcmp(person_list[i].name, input_info[1]) == 0 && strcmp(person_list[i].email, input_info[3]) == 0 && strcmp(person_list[i].phone, input_info[4]) == 0){
				idExist = 1;
				printf("\n입력하신 정보와 일치하는 ID를 찾았습니다.\n");
				printf("ID : %s\n", person_list[i].id);
				break;
			}
		}
		if(idExist == 0)
			printf("\n입력하신 정보와 일치하는 ID가 존재하지 않습니다.\n");
	}

	else if(check == 2){ //비밀번호 찾기

		printf("ID를 입력하세요 : ");
		scanf("%s",input_info[0]);
		for(int i = 0; i < N; i++){
			if(strcmp(person_list[i].id, input_info[0]) == 0){
				idExist = 1;
				M = i;
				break;
			}
		}
		if(idExist == 0)
			printf("입력하신 ID가 존재하지 않습니다.\n");
		else{
			printf("비밀번호를 찾기 위한 정보를 입력하세요\n");
			printf("이름 : ");
			scanf("%s", input_info[1]);
			printf("E-mail : ");
			scanf("%s", input_info[3]);
			printf("번호(휴대폰 - 없이) : ");
			scanf("%s",input_info[4]);

			if(strcmp(person_list[M].name, input_info[1]) == 0 && strcmp(person_list[M].email, input_info[3]) == 0 && strcmp(person_list[M].phone, input_info[4]) == 0){
				printf("\n입력하신 정보와 일치하는 비밀번호를 찾았습니다.\n");
				printf("비밀번호 : %s", person_list[M].password);
			}
			else
				printf("\n입력하신 정보와 일치하는 비밀번호가 존재하지 않습니다.\n");
		}
	}

	fclose(fp);
}

//관리자 메뉴 함수
void manageMember(){	
	FILE *fp = fopen(".member","ab+");
	Info person_list[100];
	int N = 0, go, input_memnum, delete; //최대 인원, 이동, 입력할 회원번호, 삭제할 회원 순서
	rewind(fp);
	while(fread(&person_list[N], sizeof(Info),1,fp)){ 
		N+=1;
	}
	while(1){
		printf("\n====================관 리 자 메 뉴====================\n");
		printf("1. 회원 명부 출력 / 2. 회원 삭제 / 3. 관리자 모드 종료\n입력: ");
		scanf("%d",&go);

		if(go == 1){
			printf("\n=================================================\n");
			for(int i = 0; i < N; i++)
				printf("%02d| 회원번호: %d / 이름: %s / 닉네임: %s / ID: %s / PW: %s\n", i+1,person_list[i].member_number, person_list[i].name, person_list[i].nickname, person_list[i].id, person_list[i].password);
		}

		else if(go == 2){ // 
			printf("삭제하실 회원의 회원번호를 입력하세요 : ");
			scanf("%d", &input_memnum);
			for(int i = 0; i <= N; i++){
				if(person_list[i].member_number == input_memnum){
					delete = i;
					break;
				}
			}
			printf("회원번호 %d번 \"%s\" 고객님의 정보를 삭제하시겠습니까? (1/0) : ", person_list[delete].member_number, person_list[delete].name);
			scanf("%d", &go);
			if(go == 0)
				printf("삭제를 취소합니다.\n");
			else{
				for(int i = delete; i <= N; i++)
					person_list[i] = person_list[i+1]; //덮어쓰기
				N -= 1;
				fclose(fp);
				fp = fopen(".member", "wb");
				rewind(fp);
				for(int i = 0; i < N; i++)
					fwrite(&person_list[i], sizeof(Info), 1, fp);
				printf("삭제가 완료되었습니다.\n");
				fclose(fp);
			}


		}
		else if(go == 3){
			printf("관리자 모드를 종료합니다.\n");
			break;
		}

		else{
			printf("없는 메뉴입니다.\n");
			break;
		}
	}
	fclose(fp);
}

// 메뉴창을 프린트하는 함수
int printMenu(Info member){
	int go; 
	/*
	   if(member.manage == 1){ //관리자
	   printf("\n===============================================\n");
	   printf("1. ID 찾기 / 2. 비밀번호 찾기 / 3. 로그아웃 / 4. 종료\n");
	   printf("관리자 메뉴 [ 5. 회원 관리 ]\n");
	   printf("입력 : ");
	   scanf("%d", &go);

	   }
	 */

	printf("\n===============================================\n");
	printf("1. 로그인 / 2. 회원가입 / ");
	printf("3. ID 찾기 / 4. 비밀번호 찾기 / 5. 종료\n");
	printf("입력 : ");
	scanf("%d", &go);


	return go;
}


void startLogin(Info *player){
	int go, login_check = 0, log_num; //메뉴 이동(일반, 관리자 버전), 로그인 체크
	Info member = {0, }; //로그인한 멤버의 정보를 저장
	char ID[255], PW[255];

	while(login_check == 0){
		go = printMenu(member);
		if(go == 1){ //로그인
			printf("ID: ");
			scanf("%s", ID);
			printf("PW: ");
			scanf("%s", PW);

			log_num = login(ID, PW);
			if(log_num != -1){
				member = login2(log_num);
				login_check = 1;
				*player = member;
				break; // 로그인 시 종료
			}

		}
		else if(go == 2){ //회원가입
			inputInfo();
		}
		else if(go == 3){ //ID 찾기
			searchIdPass(1);
		}
		else if(go == 4){ //PW 찾기
			searchIdPass(2);
		}
		else if(go == 5){
			exit(0);
		}

		//manage 관련
		/*
		   else if(go == 6){
		   member.manage = 0;
		   login_check = 0;
		   printf("로그아웃이 완료되었습니다.\n");
		   }
		 */
		else 
			exit(0);

	}

	if(member.manage == 1 && login_check == 1){ //관리자 모드 로그인 시 (관리자 모드가 종료되면 프로그램 종료)
		manageMember();
		exit(0);
	}

}


void logout(Info *player){
	strcpy(player->id,"\0");
	strcpy(player->password,"\0");
	strcpy(player->name,"\0");
	strcpy(player->email,"\0");
	strcpy(player->phone,"\0");
	strcpy(player->nickname,"\0");
	player->member_number = 0;
	player->win = 0;
	player->lose = 0;
	player->manage = 0;

	return;
}
