static GtkWidget* gl_area;

/*---------------------------------------------------------------------------------------------------*/
/*                                        OPENGL  DRAWABLE                                           */
/*---------------------------------------------------------------------------------------------------*/

gboolean expose (GtkWidget *da, GdkEventExpose *event, gpointer user_data) {
  GdkGLContext *glcontext = gtk_widget_get_gl_context (da);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (da);

  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)) {
    g_assert_not_reached ();
  }

  glDraw();

  if (gdk_gl_drawable_is_double_buffered (gldrawable))
    gdk_gl_drawable_swap_buffers (gldrawable);
  else
    fprintf(stderr, "glFlush not implemented?\n");
    //    glFlush ();

  gdk_gl_drawable_gl_end (gldrawable);

  return TRUE;
}

void gl_refresh() {
  expose(gl_area, NULL, NULL);
}

gboolean configure (GtkWidget *da, GdkEventConfigure *event, gpointer user_data) {
  GdkGLContext *glcontext = gtk_widget_get_gl_context (da);
  GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable (da);

  if (!gdk_gl_drawable_gl_begin (gldrawable, glcontext)) {
    g_assert_not_reached ();
  }

  glReshape(da->allocation.width, da->allocation.height);

  gdk_gl_drawable_gl_end (gldrawable);

  return TRUE;
}

void init_gl_area(GtkWidget* gl_area) {

  GdkGLConfig * glconfig;

  /*============= Configure OpenGL-capable visual.==================== /

  /* Try double-buffered visual */
  glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE);

  if (glconfig == NULL) {
     g_print ("*** Cannot find the double-buffered visual.\n");
     g_print ("*** Trying single-buffered visual.\n");

     /* Try single-buffered visual */
     glconfig = gdk_gl_config_new_by_mode (GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH);
     if (glconfig == NULL) {
        g_print ("Error: no appropriate OpenGL-capable visual found.\n");
        exit (1);
       }
   }

  /* Set OpenGL-capability to the widget. */ 
  if (!gtk_widget_set_gl_capability(gl_area, glconfig, NULL, FALSE, GDK_GL_RGBA_TYPE)) {
    fprintf(stderr, "Error: unable to set OpenGL capability.\n");
  }

  /* Connect callbacks */
  g_signal_connect (G_OBJECT (gl_area), "configure_event", G_CALLBACK (configure), NULL);
  g_signal_connect (G_OBJECT (gl_area), "expose_event", G_CALLBACK (expose), NULL);
}


