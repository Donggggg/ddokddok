#include <gtk/gtk.h>

static gboolean closeApp(GtkWidget *widget, gpointer data){
	gtk_main_quit();
	return FALSE;
}

int main(int argc, char *argv[]){

	GtkWidget *window;
	GtkWidget *table;
	GtkWidget *scroller;
	GtkWidget *text;
	
	gtk_init(&argc, &argv);
	
	// Create window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_border_width(GTK_CONTAINER(window), 10);
	
	// Set a default window size
	gtk_window_set_default_size(GTK_WINDOW(window), 640, 200);
	
	// Set the window title
	gtk_window_set_title(GTK_WINDOW(window), "Text Area");
	
	
	// Create a table
	table = gtk_table_new(2, 2, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);
	
	text = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_WORD);
	scroller = gtk_vscrollbar_new(gtk_text_view_get_vadjustment(GTK_TEXT_VIEW(text)));
	gtk_table_attach_defaults(GTK_TABLE (table), text, 0, 1, 0, 1);
	gtk_widget_show(text);
	
	
	gtk_table_attach_defaults(GTK_TABLE(table), scroller, 1, 2, 0, 1);
	gtk_widget_show(scroller);	
	
	
	g_signal_connect(window, "delete-event", G_CALLBACK(closeApp), NULL);
	
	// Display widgets
	gtk_widget_show(table);
	gtk_widget_show(window);
	
	// Start the GTK event loop
	gtk_main();
	
	return 0;
	
}
