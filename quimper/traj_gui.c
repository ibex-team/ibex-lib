/*---------------------------------------------------------------------------------
 * 
 *  QUIMPER graphical user interface (GTK+OpenGL) [TRAJ]
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
#include "viewer.h"
#include "paver_ibex.h"
#include "viewer_gui.c"

static GtkWidget *contract_but;      // button for applying a contractor
static GtkWidget *plot_but;          // button for updating the plot
static GtkWidget *trajvar_entry;     // entry for the name of the trajectory variable (array of 2-3 coordinates)
static GtkWidget *export_file_entry; // output data
static GtkWidget* export_but; 

static gboolean export_mode = TRUE; // by default, do not export data to a file

/* Columns:
 0 - contractor number
 1 - contractor number (txt)
 2 - contractor name */
enum { NB_CLM=3 };

/* add a contractor in the "main list" */
static void add_to_list(GtkWidget *list, const gint number, const gchar* number_txt, const gchar *ctc_name)
{
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, NUM_CLM, number, NUMTXT_CLM, number_txt, CTC_CLM, ctc_name, -1);
}

/* callback for the "add" button */
static void add_ctc(GtkWidget *widget, gpointer ignore)
{

}

static void edit_ctc(GtkWidget *widget, gpointer ignore) { 

}

static void update_gui_after_rem() {

}

/* called when a file is opened */
static void open_success() {
  gtk_entry_set_text(GTK_ENTRY(trajvar_entry),"");    
}

/* called when a file is parsed successfully */
static void parse_success() { 

}

/* creates the "main list" */
static GtkWidget* create_main_ctc_list() {

  return NULL;

}

static void go_contract(GtkWidget *widget, gpointer data)
{
  if (current_ctc!=NULL) {
    contract(current_ctc);
  }
}

static void go_plot(GtkWidget *widget, gpointer data)
{
  int res= plot(gtk_entry_get_text(GTK_ENTRY(trajvar_entry)));
  switch (res) {
  case -1 : error_popup("cannot display domains with infinite bounds!"); break;
  case -2 : error_popup("invalid variable name"); break;
  case -3 : error_popup("invalid variable dimensions"); break;
    //  case -4 : error_popup("Warning: 3D plots not supported yet"); break;
  case -5 : error_popup("Nothing to plot"); break;
  }
}

static GtkWidget* init_gui(int argc, char *argv[]) {

  gtk_window_set_title(GTK_WINDOW(window), "QUIMPER - qTraj2D");

  GtkWidget* bottomframe = gtk_hbox_new(FALSE, 20);  

  GtkWidget *paramframe = gtk_vbox_new(TRUE, 5);

  GtkWidget* param_align = gtk_alignment_new(0,0,0,0);
  gtk_widget_set_size_request(param_align, 150, 300);
  gtk_container_add(GTK_CONTAINER(param_align), paramframe);

  gtk_box_pack_start(GTK_BOX(bottomframe), param_align, FALSE, TRUE, 0);

  GtkWidget *trajvar_label = gtk_label_new("Position variable");
  gtk_box_pack_start(GTK_BOX(paramframe), trajvar_label, TRUE, TRUE, 0);

  trajvar_entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(paramframe), trajvar_entry, TRUE, TRUE, 0);

/*  export_but = gtk_check_button_new_with_label("Export data");
  gtk_box_pack_start(GTK_BOX(paramframe), export_but, TRUE, TRUE, 0);

  GtkWidget *export_label = gtk_label_new("Output file name");
  gtk_box_pack_start(GTK_BOX(paramframe), export_label, TRUE, TRUE, 0); 

  export_file_entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(paramframe), export_file_entry, TRUE, TRUE, 0);

  switch_export_mode(export_but, NULL); */

  contract_but = gtk_button_new_with_label("contract");
  gtk_widget_set_size_request(contract_but, 80, 30);
  gtk_widget_set_sensitive(contract_but, TRUE);
  gtk_box_pack_start(GTK_BOX(paramframe), contract_but, FALSE, FALSE, 0);

  plot_but = gtk_button_new_with_label("plot");
  gtk_widget_set_size_request(plot_but, 80, 30);
  gtk_widget_set_sensitive(plot_but, TRUE);
  gtk_box_pack_start(GTK_BOX(paramframe), plot_but, FALSE, FALSE, 0);

/*  g_signal_connect(export_but, "toggled", G_CALLBACK(switch_export_mode), NULL); */

  g_signal_connect(contract_but, "clicked", G_CALLBACK(go_contract), NULL);
  g_signal_connect(plot_but, "clicked", G_CALLBACK(go_plot), NULL);

  /*=========== init GtkGLExt ===============*/
  gtk_gl_init (&argc, &argv);
  gl_area= gtk_drawing_area_new ();
  gtk_widget_set_events (gl_area, GDK_EXPOSURE_MASK);
//  gtk_widget_set_size_request (gl_area, 200, 200);

  gtk_box_pack_start(GTK_BOX(bottomframe), gl_area, TRUE, TRUE, 0);

  init_gl_area(gl_area);

  return bottomframe;

}
