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

#include "paver_gui.h"
#include "gui.c"
#include "viewer.h"
#include "paver_ibex.h"
#include "viewer_gui.c"

static GtkWidget *pav_but;      // button for launching the paver
static GtkWidget *halt_but;     // button for interrupting the paver
static GtkWidget *step_but;     // check button to switch from solving step-by-step and solving in a raw
static GtkWidget *xentry;       // entry for the variable associated to the horizontal axis
static GtkWidget *yentry;       // entry for the variable associated to the vertical axis
static GtkWidget *rrprecentry;  // entry for the precision of the round-robin bisection axis

#define NB_DEF_BG_COLORS 12
int default_colors[NB_DEF_BG_COLORS][3] = 
    { {0xFFFF,0,0}, {0,0xFFFF,0}, {0,0,0xFFFF}, {0xFFFF,0xFFFF,0}, {0xFFFF,0,0xFFFF}, {0,0xFFFF,0xFFFF}, 
    {0xAAAA,0,0}, {0,0xAAAA,0}, {0,0,0xAAAA}, {0xAAAA,0xAAAA,0}, {0xAAAA,0,0xAAAA}, {0,0xAAAA,0xAAAA} }; 

/* Columns:
 0 - contractor number
 1 - contractor number (txt)
 2 - contractor name
 3 - background color
 4 - foreground color */
enum { BGCOLOR_CLM=3, FGCOLOR_CLM=4, NB_CLM=5 };

/* add a contractor in the "main list" */
static void add_to_list(GtkWidget *list, const gint number, const gchar* number_txt, const gchar *ctc_name, const GdkColor bgcolor, const GdkColor fgcolor)
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, NUM_CLM, number, NUMTXT_CLM, number_txt, CTC_CLM, ctc_name, BGCOLOR_CLM, &bgcolor, FGCOLOR_CLM, &fgcolor, -1);
}

/* callback for the "add" button */
static void add_ctc(GtkWidget *widget, gpointer ignore)
{
  GdkColor bgcolor;
  int i=nb_ctc_main_list<NB_DEF_BG_COLORS ? nb_ctc_main_list : 0;

  bgcolor.red=default_colors[i][0];
  bgcolor.blue=default_colors[i][1];
  bgcolor.green=default_colors[i][2];

  GdkColor fgcolor;
  fgcolor.red=0;  
  fgcolor.blue=0;
  fgcolor.green=0;

  char num_txt[10];
  snprintf(num_txt, 10, "%d",nb_ctc_main_list+1);

  if (current_ctc!=NULL) {
    add_to_list(main_list, nb_ctc_main_list+1, num_txt, current_ctc, bgcolor, fgcolor);
    nb_ctc_main_list++;
    gtk_widget_set_sensitive(pav_but, TRUE);
    gtk_widget_set_sensitive(step_but, TRUE);
  }
}

static void edit_ctc(GtkWidget *widget, gpointer ignore) {

  GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW (main_list));
  GtkListStore *store = GTK_LIST_STORE(model);
  GtkTreeSelection *select = gtk_tree_view_get_selection (GTK_TREE_VIEW (main_list));
  GtkTreeIter iter;
  GdkColor *bg_color, *fg_color;

  if (gtk_tree_selection_get_selected (select, &model, &iter)) { // should be true!
      gtk_tree_model_get(model, &iter, BGCOLOR_CLM, &bg_color, FGCOLOR_CLM, &fg_color, -1);
  } else return;

  GtkWidget *dialog = gtk_dialog_new_with_buttons ("Edit contractor",
  GTK_WINDOW(window), GTK_DIALOG_MODAL,
  GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, 
  GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT, NULL);

  GtkWidget* vbox = GTK_DIALOG(dialog)->vbox; //gtk_dialog_get_content_area(dialog); 

  GtkWidget* bg_label = gtk_label_new("set background color");
  gtk_box_pack_start(GTK_BOX(vbox), bg_label, FALSE, FALSE, 3);

  GtkWidget* bg_but = gtk_color_button_new_with_color(bg_color);
  gtk_box_pack_start(GTK_BOX(vbox), bg_but, FALSE, FALSE, 3);
 
  GtkWidget* fg_label = gtk_label_new("set border color");
  gtk_box_pack_start(GTK_BOX(vbox), fg_label, FALSE, FALSE, 3);

  GtkWidget* fg_but = gtk_color_button_new_with_color(fg_color);
  gtk_box_pack_start(GTK_BOX(vbox), fg_but, FALSE, FALSE, 3);
 
  gtk_widget_show_all (dialog);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
      gtk_color_button_get_color(GTK_COLOR_BUTTON(bg_but), bg_color);
      gtk_color_button_get_color(GTK_COLOR_BUTTON(fg_but), fg_color); 
      gtk_list_store_set(store, &iter, BGCOLOR_CLM, bg_color, FGCOLOR_CLM, fg_color, -1);
  }
  gtk_widget_destroy (dialog); 
}

static void update_gui_after_rem() {
  if (nb_ctc_main_list==0) {
    gtk_widget_set_sensitive(pav_but, FALSE);
    gtk_widget_set_sensitive(step_but, FALSE);
  }
}

/* called when a file is opened */
static void open_success() {
  gtk_entry_set_text(GTK_ENTRY(xentry),"");
  gtk_entry_set_text(GTK_ENTRY(yentry),"");
  gtk_entry_set_text(GTK_ENTRY(rrprecentry),"");    
  
  glClearScreen();
  gl_refresh();

  gtk_widget_set_sensitive(pav_but, FALSE);    
}

/* called when a file is parsed successfully */
static void parse_success() {
  /* update x and y entries with default variables */
  char* def_xvar = get_first_var_name();
  char* def_yvar = get_second_var_name();
  gtk_entry_set_text(GTK_ENTRY(xentry),def_xvar);
  gtk_entry_set_text(GTK_ENTRY(yentry),def_yvar);
  char def_rrprec[100];
  sprintf(def_rrprec, "%f", get_prec_epsilon());
  gtk_entry_set_text(GTK_ENTRY(rrprecentry),def_rrprec);
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
  store = gtk_list_store_new(NB_CLM, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_COLOR, GDK_TYPE_COLOR);

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
  renderer2 = gtk_cell_renderer_text_new ();
  GtkTreeViewColumn *column2;
  column2 = gtk_tree_view_column_new_with_attributes ("BG", renderer2, "background-gdk", BGCOLOR_CLM, NULL);
  gtk_tree_view_column_set_sizing (column2, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_expand (column2, TRUE);
  gtk_tree_view_column_set_fixed_width (column2, 5);

  GtkCellRenderer *renderer3;
  renderer3 = gtk_cell_renderer_text_new ();
  GtkTreeViewColumn *column3;
  column3 = gtk_tree_view_column_new_with_attributes ("FG", renderer3, "background-gdk",FGCOLOR_CLM, NULL);
  gtk_tree_view_column_set_sizing (column3, GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_expand (column3, TRUE);
  gtk_tree_view_column_set_fixed_width (column3, 5);


  gtk_box_pack_start(GTK_BOX(ctc_box), main_list, FALSE, FALSE, 0);

  gtk_tree_view_append_column (GTK_TREE_VIEW (main_list), column0);
  gtk_tree_view_append_column (GTK_TREE_VIEW (main_list), column);
  gtk_tree_view_append_column (GTK_TREE_VIEW (main_list), column2);
  gtk_tree_view_append_column (GTK_TREE_VIEW (main_list), column3);

  /* Setup the selection handler */
  GtkTreeSelection *select;
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (main_list));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed", G_CALLBACK (select_ctc_list), &main_list_id);
  g_signal_connect (G_OBJECT (main_list), "cursor-changed", G_CALLBACK (focus_ctc_list), &main_list_id);
  return ctc_box;
}


static void go_paver(GtkWidget *widget, gpointer data)
{
  //gtk_label_set_text(label, in_progress_msg);
  struct selected_ctc ctc[nb_ctc_main_list];
  gchar *x = strdup(gtk_entry_get_text(GTK_ENTRY(xentry))); 
  gchar *y = strdup(gtk_entry_get_text(GTK_ENTRY(yentry))); 
  gchar *rrprec_str = strdup(gtk_entry_get_text(GTK_ENTRY(rrprecentry))); 

  double rr_prec = atof(rrprec_str);
  if (rr_prec==0.0) {
    error_popup("invalid bisection precision");
    return;
  }

  // free(rrprec_str) ??

  GdkColor *bg_col, *fg_col;

  GtkTreeIter iter;
  GtkTreeModel *model;
  GtkListStore *store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(main_list)));
  model = gtk_tree_view_get_model (GTK_TREE_VIEW (main_list));

  int i=0;

  if (gtk_tree_model_get_iter_first(model, &iter))
    do {
      gtk_tree_model_get (model, &iter, CTC_CLM, &(ctc[i].name), 
                                        BGCOLOR_CLM, &bg_col, 
                                        FGCOLOR_CLM, &fg_col, -1);
      ctc[i].cl.red1 = ((double) bg_col->red)/65535.;
      ctc[i].cl.green1 = ((double) bg_col->green)/65535.;
      ctc[i].cl.blue1 = ((double) bg_col->blue)/65535.;      

      ctc[i].cl.red2 = ((double) fg_col->red)/65535.;
      ctc[i].cl.green2 = ((double) fg_col->green)/65535.;
      ctc[i].cl.blue2 = ((double) fg_col->blue)/65535.;

      i++;
//      free(bg_col);
//      free(fg_col);

    } while (gtk_tree_model_iter_next(model, &iter));  

  gtk_widget_set_sensitive(halt_but, TRUE);
  int ret_code = run_paver(ctc, nb_ctc_main_list, x, y, rr_prec);
  gtk_widget_set_sensitive(halt_but, FALSE);

  switch (ret_code) {
  case  1: error_popup("invalid variable name (horizontal axis)"); break;    
  case  2: error_popup("invalid variable name (vertical axis)"); break;    
  case -1: error_popup("cannot display domains with infinite bounds!"); break;
  }

  for (i=0; i<nb_ctc_main_list; i++) {
    free(ctc[i].name);
  }

  gl_refresh();
//  free(ctc);
}

static void switch_mode(GtkWidget* but, gpointer ignore) {
 gboolean state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(but));
 switch (state) {
   case TRUE : step_mode = 1; break;
   case FALSE : step_mode = 0; break;
  }
}

static gboolean interruption;


static void interrupt(GtkWidget* but, gpointer ignore) {
 interruption = TRUE;
}

int check_interrupt() {
  if (gtk_events_pending ()) {
/*    GdkEvent* event=gtk_get_current_event();
    if (gtk_get_event_widget(event)==halt_but) { */
      interruption = FALSE;
      gtk_main_iteration ();
      return (interruption==TRUE);        
/*      gdk_event_free(event); */
/*      return 1; */       
  }
  return 0;
}


static GtkWidget* init_gui(int argc, char *argv[]) {

  gtk_window_set_title(GTK_WINDOW(window), "QUIMPER - qPave2D");

  GtkWidget* bottomframe = gtk_hbox_new(FALSE, 20);  

  GtkWidget *paramframe = gtk_vbox_new(TRUE, 5);

  GtkWidget* param_align = gtk_alignment_new(0,0,0,0);
  gtk_widget_set_size_request(param_align, 150, 300);
  gtk_container_add(GTK_CONTAINER(param_align), paramframe);

  gtk_box_pack_start(GTK_BOX(bottomframe), param_align, FALSE, TRUE, 0);

  GtkWidget *xvar = gtk_label_new("Horizontal axis");
  gtk_box_pack_start(GTK_BOX(paramframe), xvar, TRUE, TRUE, 0);

  xentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(paramframe), xentry, TRUE, TRUE, 0);

  GtkWidget *yvar = gtk_label_new("Vertical axis");
  gtk_box_pack_start(GTK_BOX(paramframe), yvar, TRUE, TRUE, 0);

  yentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(paramframe), yentry, TRUE, TRUE, 0);
 
  GtkWidget *rrprec = gtk_label_new("Round-robin prec");
  gtk_box_pack_start(GTK_BOX(paramframe), rrprec, TRUE, TRUE, 0);

  rrprecentry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(paramframe), rrprecentry, TRUE, TRUE, 0);

  pav_but = gtk_button_new_with_label("go!");
  gtk_widget_set_size_request(pav_but, 80, 30);
  gtk_widget_set_sensitive(pav_but, FALSE);
  gtk_box_pack_start(GTK_BOX(paramframe), pav_but, FALSE, FALSE, 0);

  halt_but = gtk_button_new_with_label("interrupt");
  gtk_widget_set_size_request(halt_but, 80, 30);
  gtk_widget_set_sensitive(halt_but, FALSE);
  gtk_box_pack_start(GTK_BOX(paramframe), halt_but, FALSE, FALSE, 0);

  step_but = gtk_check_button_new_with_label("step-by-step");
  gtk_widget_set_size_request(step_but, 80, 30);
  gtk_widget_set_sensitive(step_but, FALSE);
  gtk_box_pack_start(GTK_BOX(paramframe), step_but, FALSE, FALSE, 0);

  /*=========== init GtkGLExt ===============*/
  gtk_gl_init (&argc, &argv);
  gl_area= gtk_drawing_area_new ();
  gtk_widget_set_events (gl_area, GDK_EXPOSURE_MASK);
//  gtk_widget_set_size_request (gl_area, 200, 200);

  gtk_box_pack_start(GTK_BOX(bottomframe), gl_area, TRUE, TRUE, 0);

  init_gl_area(gl_area);

  g_signal_connect(pav_but, "clicked", G_CALLBACK(go_paver), NULL);

  g_signal_connect(step_but, "clicked", G_CALLBACK(switch_mode), NULL);

  g_signal_connect(halt_but, "clicked", G_CALLBACK(interrupt), NULL);


  return bottomframe;

}
