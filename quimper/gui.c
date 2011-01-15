/*---------------------------------------------------------------------------------
 * 
 *  QUIMPER graphical user interface (GTK+OpenGL)
 * -------------------------------------------------------
 *
 * Copyright (C) 2008 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *
 --------------------------------------------------------------------------------*/

#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <string.h>
#include <stdlib.h>
#include "ibex.h"
#include "gui.h"

static GtkWidget *window;       // top-level application window
static GtkWidget *all_list;     // list of contractors loaded from a QUIMPER input file
static GtkWidget *main_list;    // list of contractors given to the paver
static GtkWidget *add_but;      // button for adding a contractor in the main list
static GtkWidget *add_all_but;  // button for adding all the contractors in the main list
static GtkWidget *rem_but;      // button for removing a contractor from the main list
static GtkWidget *edit_but;     // button for editing colors associated to a contractor of the main list
static GtkWidget* import_but;   // button for loading the domain(s) of a(n array of) variable(s)

int step_mode;                  // 1 if step-by-step mode is activated, 0 otherwise.
 
typedef enum { ALL_LIST, MAIN_LIST } list_id_type;

enum { NUM_CLM=0, NUMTXT_CLM=1, CTC_CLM=2};

static list_id_type all_list_id = ALL_LIST;
static list_id_type main_list_id = MAIN_LIST;

/* number of contractors in the "main list" */
static int nb_ctc_main_list = 0;

/*------------------------------ actions depending on the mode (PAVER/SOLVER) ----------------------------*/
static GtkWidget* create_main_ctc_list();
static void add_ctc(GtkWidget *widget, gpointer ignore);           // when a file is opened 
static void open_success();                                        // when a file has been opened
static void parse_success();                                       // when parsing is successful
static void add_ctc(GtkWidget *widget, gpointer ignore);           // when the user clicks on "add"
static void edit_ctc(GtkWidget *widget, gpointer ignore);          // when the user clicks on "edit"
static void update_gui_after_rem();                                // after a user has clicked on "remove"
static GtkWidget* init_gui(int argc, char *argv[]);                // add specific widgets 
/*--------------------------------------------------------------------------------------------------------*/

static int solver_mode; // 0 = PAVER, 1 = SOLVER

static void error_popup(const char* message) {
  GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE, message);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

/*---------------------------------------------------------------------------------------------------*/
/*                                                MENU                                               */
/*---------------------------------------------------------------------------------------------------*/
static GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

static void open_file(GtkWidget* open_but) {
  GtkWidget *dialog;

  dialog = gtk_file_chooser_dialog_new ("Open File",
				      GTK_WINDOW(window),
				      GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {
    char **ctc_names;
    char *filename, *error_msg, *error_token;
    int ctc_size, error_line;

    /* ----------- Reset the GUI --------------*/
    /* clears main list */
    if (main_list) {
      GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(main_list)));
      gtk_list_store_clear(store);
      nb_ctc_main_list=0;

      /* clears "all list" */
      store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(all_list)));
      gtk_list_store_clear(store);
    }

    open_success();

    if (main_list) {
      gtk_widget_set_sensitive(add_but, FALSE);    
      gtk_widget_set_sensitive(add_all_but, FALSE);    
      gtk_widget_set_sensitive(rem_but, FALSE);    
      gtk_widget_set_sensitive(edit_but, FALSE);    
    }

    gtk_widget_set_sensitive(import_but, TRUE);

    /*----------------------------------------*/

    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    if (!parse(filename, &ctc_names, &ctc_size, &error_msg, &error_token, &error_line)) {
      GtkWidget *dialog;
      if (error_token)
	dialog = gtk_message_dialog_new (GTK_WINDOW(window), 
					 GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,
					 GTK_BUTTONS_CLOSE, 
					 "%s near '%s' line %d",
					 error_msg, error_token, error_line);
      else
	dialog = gtk_message_dialog_new (GTK_WINDOW(window), 
					 GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,
					 GTK_BUTTONS_CLOSE, 
					 "%s line %d",
					 error_msg, error_line);	
      free(error_msg);
      free(error_token);
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);

    } else {
      GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(all_list)));
      GtkTreeIter iter;
      int i;

      for (i=0; i<ctc_size; i++) {
        gtk_list_store_append (store, &iter);  /* Acquire an iterator */
        gtk_list_store_set (store, &iter, 0, ctc_names[i], -1);
        free(ctc_names[i]);
      }
      free(ctc_names);     

      if (main_list && ctc_size>1) gtk_widget_set_sensitive(add_all_but, TRUE);

      parse_success();
      /* test */
      import_data("gps_init.dat","p[1]");
      import_data("Quimper_v.dat","v");
      import_data("Quimper_phi.dat","phi");
      import_data("Quimper_theta.dat","theta");
      import_data("Quimper_psi.dat","psi"); 
      import_data("gps_end.dat","p[59996]"); 
    }
    g_free (filename); 
  }

  gtk_widget_destroy (dialog);
}

static GtkWidget* create_menu() {
  /* ---------------------- MENU --------------------------- */
  GtkWidget *vbox;  
  GtkWidget *menubar;
  GtkWidget *filemenu;
  GtkWidget *file;
  GtkWidget *new;
  GtkWidget *open; 
  GtkWidget *quit;

  menubar = gtk_menu_bar_new();
  filemenu = gtk_menu_new();

  file = gtk_menu_item_new_with_label("File");
  new  = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
  open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL); 
  quit = gtk_menu_item_new_with_label("Quit");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

  g_signal_connect(G_OBJECT(open), "activate", G_CALLBACK(open_file), NULL);
  g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(gtk_main_quit), NULL);

  return menubar;
}

/*---------------------------------------------------------------------------------------------------*/
/*                                        CONTRACTOR LISTS                                           */
/*---------------------------------------------------------------------------------------------------*/

/* name of the contractor currently selected in the "all list" */
static gchar* current_ctc = NULL;

/* remove a contractor from the "main list" */
static void rem_from_list(GtkWidget* list, GtkTreeSelection *selection) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkListStore *store;
  int num;
  char numtxt[10];
  gboolean not_last;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (list));

  if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) return;

  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    not_last = gtk_list_store_remove(store, &iter);
    nb_ctc_main_list--;
    while (not_last) {
      gtk_tree_model_get (model, &iter, NUM_CLM, &num, -1);
      num--;
      gtk_list_store_set(store, &iter, NUM_CLM, num, -1);
      snprintf(numtxt, 10, "%d",num);
      gtk_list_store_set(store, &iter, NUMTXT_CLM, numtxt, -1);

      not_last = gtk_tree_model_iter_next(model, &iter);
    }
  }
}

/* select a contractor (toggle buttons and update current_ctc) */
static void select_ctc_list(GtkTreeSelection *selection, gpointer listid)
{
  GtkTreeIter iter;
  GtkTreeModel *model;

 // warning: this function is also called when one cell is removed.
  if (current_ctc) { 
    g_free (current_ctc);
    current_ctc = NULL; 
  }

  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    switch (*((list_id_type*) listid)) {
	case ALL_LIST  : 
	  if (main_list) {
	    gtk_widget_set_sensitive(add_but, TRUE);
	    gtk_widget_set_sensitive(rem_but, FALSE);
	    gtk_widget_set_sensitive(edit_but, FALSE);
	  }
        gtk_tree_model_get (model, &iter, 0, &current_ctc, -1);
        break;
	case MAIN_LIST : 
        gtk_widget_set_sensitive(add_but, FALSE);
        gtk_widget_set_sensitive(rem_but, TRUE);
        gtk_widget_set_sensitive(edit_but, TRUE);
	break;
    }
  }
}

/* change list focus */
static void focus_ctc_list(GtkTreeView *list, gpointer listid) {
  GtkTreeSelection *select;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);  
  select_ctc_list(select, listid);
}

/* callback for the "add" button */
static void add_all_ctc(GtkWidget *widget, gpointer ignore)
{
  GtkTreeIter iter;
  GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW (all_list));

  if (gtk_tree_model_get_iter_first(model, &iter)) // should be true (otherwise: button unactivable)
    do {
      gtk_tree_model_get (model, &iter, 0, &current_ctc, -1);
      add_ctc(add_but, NULL);
   } while (gtk_tree_model_iter_next(model, &iter));  
}

/* callback for the "rem" button */
static void rem_ctc(GtkWidget *widget, gpointer ignore) {
  GtkTreeSelection *select;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (main_list));
//  if (current_ctc!=NULL) 
  if (select) rem_from_list(main_list, select);

  update_gui_after_rem();
}

/* creates and initializes the "all list" */
static GtkWidget* create_all_ctc_list() {

  GtkWidget *ctc_box;
  GtkWidget *title_align;
  GtkWidget *title;  

  int i;

  ctc_box = gtk_vbox_new(FALSE, 0);

  title = gtk_label_new("Contractors");
  title_align = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(title_align), title);
  gtk_box_pack_start(GTK_BOX(ctc_box), title_align, FALSE, FALSE, 0);

  /* list view */
  all_list = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(all_list), FALSE);

  /* list model  */
  GtkListStore *store;
  store = gtk_list_store_new(1, G_TYPE_STRING);

  /* bind view to model */
  gtk_tree_view_set_model(GTK_TREE_VIEW(all_list), GTK_TREE_MODEL(store));

  /* cell renderer */
  GtkCellRenderer *renderer;
  renderer = gtk_cell_renderer_text_new ();
  GtkTreeViewColumn *column;
  column = gtk_tree_view_column_new_with_attributes ("Contrator name", renderer, "text", 0, NULL);

  gtk_box_pack_start(GTK_BOX(ctc_box), all_list, FALSE, FALSE, 0);

  gtk_tree_view_append_column (GTK_TREE_VIEW (all_list), column);

  /* Setup the selection handler */
  GtkTreeSelection *select;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (all_list));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed", G_CALLBACK (select_ctc_list), &all_list_id);
  g_signal_connect (G_OBJECT (all_list), "cursor-changed", G_CALLBACK (focus_ctc_list), &all_list_id);

  return ctc_box;
}


/* creates buttons */
static GtkWidget* create_buttons() { 
 /* ---------------------- Buttons --------------------------- */
  add_all_but = gtk_button_new_with_label("add all");
  add_but = gtk_button_new_with_label("add");
  rem_but = gtk_button_new_with_label("remove");
  edit_but = gtk_button_new_with_label("edit");

  GtkWidget *buttons_box = gtk_vbox_new(TRUE, 3);

  gtk_widget_set_size_request(add_all_but, 80, 30);
  gtk_box_pack_start(GTK_BOX(buttons_box), add_all_but, FALSE, FALSE, 0);
  gtk_widget_set_size_request(add_but, 80, 30);
  gtk_box_pack_start(GTK_BOX(buttons_box), add_but, FALSE, FALSE, 0);
  gtk_widget_set_size_request(rem_but, 80, 30);
  gtk_box_pack_start(GTK_BOX(buttons_box), rem_but, FALSE, FALSE, 0);
  gtk_widget_set_size_request(rem_but, 80, 30);
  gtk_box_pack_start(GTK_BOX(buttons_box), edit_but, FALSE, FALSE, 0);

  g_signal_connect(add_all_but, "clicked", G_CALLBACK(add_all_ctc), main_list);
  g_signal_connect(add_but, "clicked", G_CALLBACK(add_ctc), main_list);
  g_signal_connect(rem_but, "clicked", G_CALLBACK(rem_ctc), main_list);
  g_signal_connect(edit_but, "clicked", G_CALLBACK(edit_ctc), main_list);

  gtk_widget_set_sensitive(add_all_but, FALSE);
  gtk_widget_set_sensitive(add_but, FALSE);
  gtk_widget_set_sensitive(rem_but, FALSE);
  gtk_widget_set_sensitive(edit_but, FALSE);

  return buttons_box;
}


static void import(GtkWidget *widget, gpointer ignore) {
  
  GtkWidget *dialog = gtk_file_chooser_dialog_new ("Import from file",
				      GTK_WINDOW(window),
				      GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK) {

    GtkWidget *dialog2 = gtk_dialog_new_with_buttons ("Variable name",
						     GTK_WINDOW(window), GTK_DIALOG_MODAL,
						     GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 
						     GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT, NULL);
    
    GtkWidget* vbox = GTK_DIALOG(dialog2)->vbox; //gtk_dialog_get_content_area(dialog); 
    
    GtkWidget* bg_label = gtk_label_new("Variable name");
    gtk_box_pack_start(GTK_BOX(vbox), bg_label, FALSE, FALSE, 3);
    GtkWidget* varname_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), varname_entry, TRUE, TRUE, 0);

    gtk_widget_show_all (dialog2);

    if (gtk_dialog_run (GTK_DIALOG (dialog2)) == GTK_RESPONSE_ACCEPT) {
      printf("import data...\n");
      int res=import_data(gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog)),
			  gtk_entry_get_text(GTK_ENTRY(varname_entry)));
      if (res==-1) error_popup("Invalid file name");
      else if (res==-2) error_popup("Invalid variable name or bad file format");
    }
    gtk_widget_destroy (dialog2);
    gtk_widget_destroy (dialog);
    
  } else 
    gtk_widget_destroy (dialog);
}

static GtkWidget* create_import_button() {
  import_but = gtk_button_new_with_label("import data");
  gtk_widget_set_size_request(import_but, 100, 30);
  gtk_widget_set_sensitive(import_but, FALSE);
  GtkWidget* import_align = gtk_alignment_new(0.5, 0.5, 0, 0);
  gtk_container_add(GTK_CONTAINER(import_align), import_but);
  g_signal_connect(import_but, "clicked", G_CALLBACK(import), NULL);
  return import_align;
}

void run_gui(int argc, char *argv[]) {

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
//  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("level0.jpg"));
  /* Get automatically redrawn if any of their children changed allocation. */
//  gtk_container_set_reallocate_redraws (GTK_CONTAINER (window), TRUE);


  GtkWidget* globalframe = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), globalframe);

  GtkWidget* bottomframe=init_gui(argc, argv);

  gtk_box_pack_end(GTK_BOX(globalframe), bottomframe, TRUE, TRUE, 10);

  GtkWidget* mainframe = gtk_hbox_new(FALSE, 20);

  gtk_box_pack_start(GTK_BOX(mainframe), create_all_ctc_list(), TRUE, TRUE, 0);

  GtkWidget* mainlist=create_main_ctc_list();

  if (main_list) {
    gtk_box_pack_start(GTK_BOX(mainframe), mainlist, TRUE, TRUE, 3);    
    gtk_box_pack_start(GTK_BOX(mainframe), create_buttons(), FALSE, FALSE, 0);
  }


  gtk_box_pack_start(GTK_BOX(mainframe), create_import_button(), FALSE, FALSE, 0);

  gtk_box_pack_end(GTK_BOX(globalframe), mainframe, FALSE, FALSE, 3); 

  gtk_box_pack_end(GTK_BOX(globalframe), create_menu(), FALSE, FALSE, 3);


  gtk_widget_show (window);

  gtk_widget_show_all(window);

  g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();

}

