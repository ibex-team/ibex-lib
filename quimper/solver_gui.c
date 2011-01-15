/*---------------------------------------------------------------------------------
 * 
 *  QUIMPER graphical user interface (GTK+OpenGL) [PAVER]
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

#include "gui.c"
#include "solver_ibex.h"

static GtkWidget *sol_but;           // button for launching the solver
//static GtkWidget *halt_but;        // button for interrupting the paver
static GtkWidget *rrprecentry;       // entry for the precision of the round-robin bisection axis
static GtkWidget *export_file_entry; // output data
static GtkWidget* export_but; 

static gboolean export_mode = TRUE; // by default, do not export data to a file

/* Columns:
 0 - contractor number
 1 - contractor number (txt)
 2 - contractor name
 3 - selection */
enum { SEL_CLM=3, NB_CLM=4 };

/* add a contractor in the "main list" */
static void add_to_list(GtkWidget *list, const gint number, const gchar* number_txt, const gchar *ctc_name, const gboolean sel)
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, NUM_CLM, number, NUMTXT_CLM, number_txt, CTC_CLM, ctc_name, SEL_CLM, sel, -1);
}

/* callback for the "add" button */
static void add_ctc(GtkWidget *widget, gpointer ignore)
{
  char num_txt[10];
  snprintf(num_txt, 10, "%d",nb_ctc_main_list+1);

  if (current_ctc!=NULL) {
    add_to_list(main_list, nb_ctc_main_list+1, num_txt, current_ctc, TRUE);
    nb_ctc_main_list++;
    gtk_widget_set_sensitive(sol_but, TRUE);
  }
}

static void edit_ctc(GtkWidget *widget, gpointer ignore) { 

}

static void update_gui_after_rem() {
  if (nb_ctc_main_list==0) {
    gtk_widget_set_sensitive(sol_but, FALSE);
  }
}

/* called when a file is opened */
static void open_success() {
  gtk_entry_set_text(GTK_ENTRY(rrprecentry),"");    

  gtk_widget_set_sensitive(sol_but, FALSE);
}

/* called when a file is parsed successfully */
static void parse_success() {
  /* update x and y entries with default variables */
  char def_rrprec[100];
  sprintf(def_rrprec, "%f", get_prec_epsilon());
  gtk_entry_set_text(GTK_ENTRY(rrprecentry),def_rrprec);
}

static void toggle_ctc(GtkWidget *widget, gchar *str_path, gpointer ignore) { 

  GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(main_list)));
  GtkTreeIter iter;
  GtkTreePath *path = gtk_tree_path_new_from_string(str_path);
  gboolean res=gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);
  g_free(path);
  if (!res) return;

  gboolean sel;
  gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, SEL_CLM, &sel, -1);
  sel=(sel==TRUE? FALSE : TRUE);
  gtk_list_store_set(store, &iter, SEL_CLM, sel, -1);
}

/* creates the "main list" */
static GtkWidget* create_main_ctc_list() {

  GtkWidget *ctc_box;
  GtkWidget *title_align;
  GtkWidget *title;  

  int i;
  ctc_box = gtk_vbox_new(FALSE, 0);

  title = gtk_label_new("Main list");
  title_align = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(title_align), title);
  gtk_box_pack_start(GTK_BOX(ctc_box), title_align, FALSE, FALSE, 0);

  /* list view */
  main_list = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(main_list), TRUE);

  /* list model  */
  GtkListStore *store;
  store = gtk_list_store_new(NB_CLM, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN);

  GtkTreeIter iter;

  /* bind view to model */
  gtk_tree_view_set_model(GTK_TREE_VIEW(main_list), GTK_TREE_MODEL(store));

  /* cell renderer */
  GtkCellRenderer *renderer0;
  renderer0 = gtk_cell_renderer_text_new ();
  GtkTreeViewColumn *column0;
  column0 = gtk_tree_view_column_new_with_attributes ("Number", renderer0, "text", NUMTXT_CLM, NULL);
  gtk_tree_view_column_set_sizing (column0, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_expand (column0, TRUE);
  gtk_tree_view_column_set_fixed_width (column0, 5);
  gtk_tree_view_column_set_resizable (column0, TRUE);

  GtkCellRenderer *renderer;
  renderer = gtk_cell_renderer_text_new ();
  GtkTreeViewColumn *column;
  column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", CTC_CLM, NULL);
  gtk_tree_view_column_set_sizing (column, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_expand (column, TRUE);
  gtk_tree_view_column_set_fixed_width (column, 50);
  gtk_tree_view_column_set_resizable (column, TRUE);

  GtkCellRenderer *renderer2;
  renderer2 = gtk_cell_renderer_toggle_new();
  GtkTreeViewColumn *column2;
  column2 = gtk_tree_view_column_new_with_attributes ("sel?", renderer2, "active", SEL_CLM, NULL);
  gtk_tree_view_column_set_sizing (column2, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_expand (column2, TRUE);
  gtk_tree_view_column_set_fixed_width (column2, 5);

  g_signal_connect(renderer2, "toggled", G_CALLBACK(toggle_ctc), NULL);

  gtk_box_pack_start(GTK_BOX(ctc_box), main_list, FALSE, FALSE, 0);

  gtk_tree_view_append_column (GTK_TREE_VIEW (main_list), column0);
  gtk_tree_view_append_column (GTK_TREE_VIEW (main_list), column);
  gtk_tree_view_append_column (GTK_TREE_VIEW (main_list), column2);

  /* Setup the selection handler */
  GtkTreeSelection *select;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (main_list));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed", G_CALLBACK (select_ctc_list), &main_list_id);
  g_signal_connect (G_OBJECT (main_list), "cursor-changed", G_CALLBACK (focus_ctc_list), &main_list_id);
  return ctc_box;
}


static void switch_export_mode(GtkWidget *widget, gpointer data) {
  export_mode = export_mode ? FALSE : TRUE;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(export_but), export_mode);
  gtk_widget_set_sensitive(export_file_entry, export_mode);    
}

static void go_solver(GtkWidget *widget, gpointer data)
{
  //gtk_label_set_text(label, in_progress_msg);
  struct selected_ctc ctc[nb_ctc_main_list];
  gchar *rrprec_str = strdup(gtk_entry_get_text(GTK_ENTRY(rrprecentry))); 
  
  double rr_prec = atof(rrprec_str);
  if (rr_prec==0.0) {
    error_popup("invalid bisection precision");
    return;
  }

  // free(rrprec_str) ??

  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkListStore *store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(main_list)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (main_list));

  int i=0;
  gboolean sel;

  if (gtk_tree_model_get_iter_first(model, &iter))
    do {
      gtk_tree_model_get (model, &iter, CTC_CLM, &(ctc[i].name), 
                                        SEL_CLM, &sel, -1);

      ctc[i].selected = sel==TRUE? 1 : 0;

      i++;

    } while (gtk_tree_model_iter_next(model, &iter));  

  int res=run_solver(ctc, nb_ctc_main_list, rr_prec, export_mode? (char*) gtk_entry_get_text(GTK_ENTRY(export_file_entry)) : NULL);

  if (res==-1) {
    error_popup("Warning: could not export data (invalid file name)");
  }

  for (i=0; i<nb_ctc_main_list; i++) {
    free(ctc[i].name);
  }
}

static GtkWidget* init_gui(int argc, char *argv[]) {

  gtk_window_set_title(GTK_WINDOW(window), "QUIMPER - qSolve");

  GtkWidget* bottomframe = gtk_hbox_new(FALSE, 20);  

  GtkWidget *paramframe = gtk_vbox_new(TRUE, 5);

  GtkWidget* param_align = gtk_alignment_new(0,0,0,0);
  gtk_widget_set_size_request(param_align, 150, 300);
  gtk_container_add(GTK_CONTAINER(param_align), paramframe);

  gtk_box_pack_start(GTK_BOX(bottomframe), param_align, FALSE, TRUE, 0);

  GtkWidget *rrprec = gtk_label_new("Round-robin prec");
  gtk_box_pack_start(GTK_BOX(paramframe), rrprec, TRUE, TRUE, 0);

  rrprecentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(paramframe), rrprecentry, TRUE, TRUE, 0);

  export_but = gtk_check_button_new_with_label("Export data");
  gtk_box_pack_start(GTK_BOX(paramframe), export_but, TRUE, TRUE, 0);

  GtkWidget *export_label = gtk_label_new("Output file name");
  gtk_box_pack_start(GTK_BOX(paramframe), export_label, TRUE, TRUE, 0);

  export_file_entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(paramframe), export_file_entry, TRUE, TRUE, 0);

  switch_export_mode(export_but, NULL);

  sol_but = gtk_button_new_with_label("go!");
  gtk_widget_set_size_request(sol_but, 80, 30);
  gtk_widget_set_sensitive(sol_but, FALSE);
  gtk_box_pack_start(GTK_BOX(paramframe), sol_but, FALSE, FALSE, 0);

  g_signal_connect(export_but, "toggled", G_CALLBACK(switch_export_mode), NULL);

  g_signal_connect(sol_but, "clicked", G_CALLBACK(go_solver), NULL);

  return bottomframe;

}
