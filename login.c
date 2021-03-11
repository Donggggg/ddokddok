#include <assert.h>
#include <form.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <err.h>
static FORM *form;
static FIELD *fields[5];
static WINDOW *win_body, *win_form;

static char* trim_whitespaces(char *str);
static void set_field(FIELD *field[]);
static void driver(int ch);
static void free_all(FORM *form,FIELD *fields[]);
static void print_logo(WINDOW *my_menu_win);
int main()
{
        //init window tab
	initscr();
        start_color();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);

        
	win_body = newwin(30,70, 4, 4);
	assert(win_body != NULL);
	box(win_body, 0, 0);

	win_form = derwin(win_body, 20, 60, 4,4 );//20, 60 ,4 ,4
	assert(win_form != NULL);
	//box(win_form, 0, 0);
	mvwprintw(win_body, 1, 2, "Press F3 to quit and F2 to print fields content");
	
	fields[0] = new_field(1, 10, 0, 0, 0, 0);
	fields[1] = new_field(1, 40, 0, 15, 0, 0);
	fields[2] = new_field(1, 10, 2, 0, 0, 0);
	fields[3] = new_field(1, 40, 2, 15, 0, 0);
	fields[4] = NULL;
	assert(fields[0] != NULL && fields[1] != NULL && fields[2] != NULL && fields[3] != NULL);

	set_field_buffer(fields[0], 0, "ID : ");
	set_field_buffer(fields[2], 0, "PW : ");

        //set field options
        set_field(fields);

	form = new_form(fields);
	assert(form != NULL);
	set_form_win(form, win_form);
	set_form_sub(form, derwin(win_form, 18, 76, 1, 1));//18
	post_form(form);
	print_logo(win_body);

        //refresh all
        refresh();
	wrefresh(win_body);
	wrefresh(win_form);
        
        //input id and password
	int ch;
	while ((ch = getch()) != KEY_F(3))
		driver(ch);

        //free all
	unpost_form(form);
        free_all(form,fields);
	delwin(win_form);
	delwin(win_body);
	endwin();

	return 0;
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
static void driver(int ch)
{
	int i;

	switch (ch) {
		case KEY_F(2):
			// Or the current field buffer won't be sync with what is displayed
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_PREV_FIELD);
			move(LINES-3, 2);

			for (i = 0; fields[i]; i++) {
				printw("%s", trim_whitespaces(field_buffer(fields[i], 0)));

				if (field_opts(fields[i]) & O_ACTIVE)
					printw("\"\t");
				else
					printw(": \"");
			}

			refresh();
			pos_form_cursor(form);
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
        fp=fopen("test.txt","r");
        if(fp==NULL)    err(EXIT_FAILURE,"NO test.txt file");
        int i=12;
        while(fgets(line,sizeof(line),fp)!=NULL)
        {
	        mvwprintw(my_menu_win, i++, 4, "%s", line);
        }
}
