#include <assert.h>
#include <form.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <err.h>
#include <stdio.h>//debug
static char sudoku_answer_psj[82];
int sudoku_answer_int[81];

static int check_in;
static FORM *form;
static FIELD *fields[11];
static WINDOW *win_body, *win_form;
//static int flag;
static char* trim_whitespaces(char *str);
static void set_field(FIELD *field[]);
static void free_all(FORM *form,FIELD *fields[]);
void sudoku_answer()
{
	//init window tab
	initscr();
	start_color();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);

	//create main window and form window
	win_body = newwin(30,70, 4, 70);
	win_form = derwin(win_body, 20, 60, 4,4 );//20, 60 ,4 ,4
	mvwprintw(win_body, 1, 15, "Write your answers");
	//declare filed location
	fields[0] = new_field(1, 10, 0, 10, 0, 0);
	for(int i=1,k=2;i<10;i++,k+=2)
	{
		fields[i] = new_field(1, 40, k, 15, 0, 0);
	}
	fields[10] = NULL;

	set_field_buffer(fields[0], 0, "Input : ");

	//set field options
	set_field(fields);
	form = new_form(fields);

	set_form_win(form, win_form);
	set_form_sub(form, derwin(win_form, 18, 76, 1, 1));//18
	post_form(form);
	//print_logo(win_body);
	box(win_body, 0, 0);

	//refresh all
	refresh();
	wrefresh(win_body);
	wrefresh(win_form);

	//input id and password
	int ch;
	int flag=0;
	while (((ch = getch())))
	{
		int i;
		switch (ch) {
			case 10://enter
				{
					form_driver(form, REQ_NEXT_FIELD);
					form_driver(form, REQ_PREV_FIELD);
					move(LINES-3, 2);


					strcpy(sudoku_answer_psj, trim_whitespaces(field_buffer(fields[1], 0)));
					for(int i=2;i<10;i++)
					{
						strcat(sudoku_answer_psj, trim_whitespaces(field_buffer(fields[i], 0)));
					}
					refresh();
					pos_form_cursor(form);
					int k=1;
					int j=40;
					for(int i=0;i<81;i++,j++)
					{  
						sudoku_answer_int[i]=sudoku_answer_psj[i]-'0';
					}
					flag=1;
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
	for(int i=1;i<10;i++)
	{
		set_field_opts(fields[i], O_VISIBLE |O_PUBLIC | O_EDIT | O_ACTIVE);
		set_field_back(fields[i], A_UNDERLINE);
	}
}
static void free_all(FORM *form,FIELD *fields[])
{
	free_form(form);
	for(int i=0;i<11;i++)
	{
		free_field(fields[i]);
	}
}

