#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

char *choices[] = {
                        "1. Play Solo",
                        "2. Play Multi",
                        "3. Show Rank",
                        "4. Options",
                        "5. Logout",
                        "6. Exit",
                        (char *)NULL,
                  };

void print_logo(WINDOW *my_menu_win)
{
        char line[255];
	FILE *fp;
        fp=fopen("test.txt","r");
        if(fp==NULL)    err(EXIT_FAILURE,"NO test.txt file");
        int i=12;
        while(fgets(line,sizeof(line),fp)!=NULL)
        {
	        //mvwprintw(my_menu_win, i, 30, "%s", line);
	        mvwprintw(my_menu_win, i++, 4, "%s", line);
        }
}

int main()
{	ITEM **my_items;
	int c;				
	MENU *my_menu;
        WINDOW *my_menu_win;
        int n_choices, i;
	
	/* Initialize curses */
	initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);

	/* Create items */
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], choices[i]);

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(30, 70, 4, 4);
        keypad(my_menu_win, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 6, 17, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, "--> ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	mvwprintw(my_menu_win, 1, 30, "%s", "Game Menu");
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 68);
	mvprintw(LINES - 2, 34, "ddok ddok");
        
        print_logo(my_menu_win);
        box(my_menu_win, 0, 0);
        refresh();
        
	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	while((c = wgetch(my_menu_win)) != KEY_F(2))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
		}
                wrefresh(my_menu_win);
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
	endwin();
}
