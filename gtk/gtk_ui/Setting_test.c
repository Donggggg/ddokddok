#include <stdio.h>
#include <gtk/gtk.h>

static void get_value( GtkWidget *widget,
                       GtkWidget *spinner[])
{
  GtkSpinButton *spin[6];
  int k;
  for(int i=0;i<6;i++)
  {
          if(i%3==0)fprintf(stderr,"\n");
          spin[i]=GTK_SPIN_BUTTON(spinner[i]);
          k=gtk_spin_button_get_value_as_int(spin[i]);
          fprintf(stderr,"value : %d\n",k);
  }
}



int main( int   argc,
          char *argv[] )
{
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *hbox;
  GtkWidget *main_vbox;
  GtkWidget *vbox;
  GtkWidget *vbox2;
static GtkWidget *spinner[6];
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *val_label;
  GtkAdjustment *adj;

  /* Initialise GTK */
  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window),500,500);
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
  g_signal_connect (window, "destroy",
		    G_CALLBACK (gtk_main_quit),
		    NULL);
  gtk_window_set_title (GTK_WINDOW (window), "Spin Button");
  main_vbox = gtk_vbox_new (FALSE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 10);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
  
  frame = gtk_frame_new ("Setting tab");
  gtk_box_pack_start (GTK_BOX (main_vbox), frame, TRUE, TRUE, 0);
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
  gtk_container_add (GTK_CONTAINER (frame), vbox);
  
  //Maze Setting Options
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 5);
  //Maze Round
  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 5);
  label = gtk_label_new ("Maze round :");
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), label, FALSE, TRUE, 0);
  adj = (GtkAdjustment *) gtk_adjustment_new (1.0, 1.0, 10.0, 1.0,
					      5.0, 0.0);
  spinner[0] = gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner[0]), TRUE);
  gtk_box_pack_start (GTK_BOX (vbox2), spinner[0], FALSE, TRUE, 0);
  //Maze Difficulty
  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 5);
  label = gtk_label_new ("Difficulty :");
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), label, FALSE, TRUE, 0);
  adj = (GtkAdjustment *) gtk_adjustment_new (1.0, 1.0, 10.0, 1.0,
					      5.0, 0.0);
  spinner[1] = gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner[1]), TRUE);
  gtk_box_pack_start (GTK_BOX (vbox2), spinner[1], FALSE, TRUE, 0);
  //Maze Penaly
  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 5);
  label = gtk_label_new ("Penalty :");
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), label, FALSE, TRUE, 0); 
  adj = (GtkAdjustment *) gtk_adjustment_new (50.0, 0.0, 100.0,
					      1.0, 100.0, 0.0);
  spinner[2] = gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner[2]), FALSE);
  gtk_widget_set_size_request (spinner[2], 55, -1);
  gtk_box_pack_start (GTK_BOX (vbox2), spinner[2], FALSE, TRUE, 0);
  //untill this point
  //
  //start Sudoku setting
  //
  //Sudoku Round
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 5);
  
  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 5);

  label = gtk_label_new ("Sudoku round :");
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), label, FALSE, TRUE, 0);
  
  adj = (GtkAdjustment *) gtk_adjustment_new (1.0, 1.0, 10.0, 1.0,
					      5.0, 0.0);
  spinner[3] = gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner[3]), TRUE);
  gtk_box_pack_start (GTK_BOX (vbox2), spinner[3], FALSE, TRUE, 0);
  
  //Sudoku Difficulty
  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 5);
  
  label = gtk_label_new ("Difficulty :");
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), label, FALSE, TRUE, 0);
  
  adj = (GtkAdjustment *) gtk_adjustment_new (1.0, 1.0, 10.0, 1.0,
					      5.0, 0.0);
  spinner[4] = gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner[4]), TRUE);
  gtk_box_pack_start (GTK_BOX (vbox2), spinner[4], FALSE, TRUE, 0);

  //Sudoku Penalty
  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 5);
  label = gtk_label_new ("Penalty :");
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), label, FALSE, TRUE, 0);
  adj = (GtkAdjustment *) gtk_adjustment_new (50.0, 0.0, 100.0,
					      1.0, 100.0, 0.0);
  spinner[5] = gtk_spin_button_new (adj, 0, 0);
  gtk_spin_button_set_wrap (GTK_SPIN_BUTTON (spinner[5]), FALSE);
  gtk_widget_set_size_request (spinner[5], 55, -1);
  gtk_box_pack_start (GTK_BOX (vbox2), spinner[5], FALSE, TRUE, 0);
  

  val_label=gtk_label_new("");
  //save box
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, TRUE, 5);
  button = gtk_button_new_with_label ("Save");
  g_object_set_data (G_OBJECT (button), "user_data", val_label);
  g_signal_connect (button, "clicked",
		    G_CALLBACK (get_value),
		    spinner);
  gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 5);
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), hbox, FALSE, TRUE, 0);
  
  //Should Make Save Options
  button = gtk_button_new_with_label ("Close");
  g_signal_connect_swapped (button, "clicked",
                            G_CALLBACK (gtk_widget_destroy),
			    window);
  //SHOW ALL OBJECT
  gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 5);
  gtk_widget_show_all (window);
  gtk_main ();
  return 0;
}
