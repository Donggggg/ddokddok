#include <gtk/gtk.h>

int main(int argc,char *argv[])
{
        GtkWidget *window;
        GtkWidget *button;
        GtkWidget *hbox;
        GtkWidget *vbox;
        GtkWidget *sep;

        gtk_init(&argc,&argv);

        window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
//        gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
        gtk_window_set_default_size(GTK_WINDOW(window),500,500);
        gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
        gtk_window_set_title(GTK_WINDOW(window),"Setting tab");

        g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
        gtk_container_set_border_width(GTK_CONTAINER(window),10);

        hbox=gtk_hbox_new(1,0);
        vbox=gtk_vbox_new(1,0);
        //create button 1
        button=gtk_button_new_with_label("button1");
        gtk_box_pack_start(GTK_BOX(hbox),button,1,1,0);
        gtk_widget_show(button);

        //create button 2
        button=gtk_button_new_with_label("button2");
        gtk_box_pack_start(GTK_BOX(hbox),button,1,1,0);
        gtk_widget_show(button);

        gtk_box_pack_start(GTK_BOX(vbox),hbox,1,1,0);
        //create seperator
        sep=gtk_hseparator_new();
        gtk_box_pack_start(GTK_BOX(vbox),sep,0,0,0);
        gtk_widget_show(sep);
        //create button 3
        button=gtk_button_new_with_label("button3");
        gtk_box_pack_start(GTK_BOX(vbox),button,1,1,0);
        gtk_widget_show(button);
        //create button 4
        button=gtk_button_new_with_label("button4");
        gtk_box_pack_start(GTK_BOX(vbox),button,1,1,0);
        gtk_widget_show(button);
        
        gtk_container_add(GTK_CONTAINER(window),vbox);

        gtk_widget_show(hbox);
        gtk_widget_show(vbox);
        gtk_widget_show(window);
        gtk_main();
        return 0;
}
