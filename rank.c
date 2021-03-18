#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
#include "member.h"
#include "score.h"

static char *choices[] = {
                        "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
			"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
			"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
			"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
                        "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
			"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
			"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
			"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
                        "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
			"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
			"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
			"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
                        "Choice 1", "Choice 2", "Choice 3", "Choice 4", "Choice 5",
			"Choice 6", "Choice 7", "Choice 8", "Choice 9", "Choice 10",
			"Choice 11", "Choice 12", "Choice 13", "Choice 14", "Choice 15",
			"Choice 16", "Choice 17", "Choice 18", "Choice 19", "Choice 20",
                        "Exit",
                        (char *)NULL,
                  };


void printScore(int ChooseLank){
	Score output[500]={0,}, temp={0, }; //1000명의 사람을 수용
	Score miro_out[500] = {0, }; //미로(1-10)
	Score sudo_out[500] = {0, }; //스도쿠(1-10)
	int miro = 0;
	int sudo = 0;
	int N = 0,  go; //갖고오는 사람 수
	FILE *fp = fopen("score.txt","rb+");
	if(fp==NULL){
		printf("파일 오류입니다.\n");
		return;
	}
	printf("\n\n");
	rewind(fp);
	while(fread(&output[N],sizeof(Score),1,fp)){
		N+=1;
	}
	fclose(fp);

	for(int i=0; i<N-1; i++){ //버블정렬로 파일에서 입력받은 정보들을 소트
		for(int j=0; j<N-i-1; j++){
			if(output[j].score>output[j+1].score){
				strcpy(temp.name,output[j].name);
				temp.score = output[j].score;
				temp.level = output[j].level;
				strcpy(output[j].name, output[j+1].name);
				output[j].score = output[j+1].score;
				output[j].level = output[j+1].level;
				strcpy(output[j+1].name, temp.name);
				output[j+1].score = temp.score;
				output[j+1].level = temp.level;


			}
		}
	}	

	
	for(int i=0; i<N; i++){
		if(output[i].miro_su == 1){ //미로
			miro_out[miro] = output[i];
	//		printf("%d미로이름: %s\n",miro, miro_out[miro].name);
			miro +=1;
		}
		else if(output[i].miro_su == 2){ //스도쿠
			sudo_out[sudo] = output[i];
	//		printf("%d스도쿠이름: %s\n",sudo, sudo_out[sudo].name);
			sudo +=1;
		}
	}
	// 
//	printf("확인하고 싶은 명예의 전당을 입력하세요 (1=미로, 2=스도쿠) : ");
//	scanf("%d",&go);
	go = ChooseLank;
	int choice_go = 0;
	char lank[20], strscore[20];


	//미로 랭킹
	if(go == 2){ //상위 30명만
		for(int i = 0; i < 30; i++){ //30명의
			if(strcmp(miro_out[i].name, "") == 0){
				choices[choice_go] = (char *)NULL;
				break;
			}
			for(int j = 0; j < 3; j++){ //rank, name, score
				if(choice_go % 3 == 0){ //rank
			//		printf("랭크IN\n");
					sprintf(lank, "%d Rank", i+1);
					choices[choice_go] = malloc(sizeof(char)*10);
					strcpy(choices[choice_go], lank);
			//		printf("%s||%s\n", lank, choices[choice_go]);
				}
				else if(choice_go % 3 == 1){ //name
			//		printf("이름IN\n");
					choices[choice_go] = malloc(sizeof(char)*30);
					strcpy(choices[choice_go], miro_out[i].name);
			//		printf("%s||%s\n", lank, choices[choice_go]);
			//		printf("%s||%s\n", miro_out[i].name, choices[choice_go]);
				}
				else if(choice_go % 3 == 2){ //score
			//		printf("점수IN\n");
					sprintf(strscore, "%d", miro_out[i].score);
					choices[choice_go] = malloc(sizeof(char)*10);
					strcpy(choices[choice_go], strscore);
			//		printf("%s||%s\n", lank, choices[choice_go]);
			//		printf("%s||%s\n", strscore, choices[choice_go]);
				}
				choice_go += 1;
			}
		}
	}

	//스도쿠 랭킹
	else if(go == 1){ //상위 30명만
		for(int i = 0; i < 30; i ++){ //30명의
			if(strcmp(sudo_out[i].name, "") == 0){
				choices[choice_go] = (char *)NULL;
				break;
			}
			for(int j = 0; j < 3; j ++){ //rank, name, score
				if(choice_go % 3 == 0){ //rank
					sprintf(lank, "%d Rank", i+1);
					choices[choice_go] = malloc(sizeof(char)*10);
					strcpy(choices[choice_go], lank);
			//		printf("%s||%s\n", lank, choices[choice_go]);
				}
				else if(choice_go % 3 == 1){ //name
					choices[choice_go] = malloc(sizeof(char)*30);
					strcpy(choices[choice_go], sudo_out[i].name);
			//		printf("%s||%s\n", lank, choices[choice_go]);
				}
				else if(choice_go % 3 == 2){ //score
					sprintf(strscore, "%d", sudo_out[i].score);
					choices[choice_go] = malloc(sizeof(char)*10);
					strcpy(choices[choice_go], strscore);
			//		printf("%s||%s\n", lank, choices[choice_go]);
				}
				choice_go += 1;
			}
		}


	}


	else{
		printf("잘못 입력하셨습니다.\n");
	}
/*
	for(int i = 0; i <90; i+=3)
		printf("%s %s %s\n", choices[i],choices[i+1],choices[i+2]);
*/
}

void show_rank(int game_selection)
{
		
		printScore(game_selection);

        ITEM **my_items;
	int c;				
	MENU *my_menu;
        WINDOW *win;
        int n_choices, i;
	
	//init ncurses window
        initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
        init_pair(1, COLOR_RED,COLOR_YELLOW);
	//create rank list
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)malloc(n_choices* sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], choices[i]);

	//create menu window
	my_menu = new_menu((ITEM **)my_items);
	menu_opts_off(my_menu, O_SHOWDESC);//only show 1 list  per line

        
        //create main window
        win = newwin(30, 70, 4, 4);
        keypad(win, TRUE);
        
        //###############should googling derwin(), set_menu_sub()###################
        set_menu_win(my_menu, win);
        set_menu_sub(my_menu, derwin(win, 25, 68, 3, 1));
	set_menu_format(my_menu, 25, 3);
	set_menu_mark(my_menu, "--> ");

        
        //print border line and title
        box(win, 0, 0);
        mvwprintw(win,0,8,"RANK");	
        mvwprintw(win,0,20,"ID");	
        mvwprintw(win,0,32,"SCORE");
	//print explation at the bottom
        attron(COLOR_PAIR(1));
	mvprintw(LINES - 3, 30, "<--- prev     next--->");
	mvprintw(LINES - 2, 34, "(F2 to Exit)");
	attroff(COLOR_PAIR(1));

        //refresh all
	refresh();
	post_menu(my_menu);
	wrefresh(win);

	while((c = wgetch(win)) != KEY_F(2))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(my_menu, REQ_SCR_UPAGE);
				break;
			case KEY_RIGHT:
				menu_driver(my_menu, REQ_SCR_DPAGE);
				break;
		}
                wrefresh(win);
	}	

        //free all
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
        endwin();
}
