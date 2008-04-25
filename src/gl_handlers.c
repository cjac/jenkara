/*
	Copyright (C) 2001 C.J. Collier (cjcollier@pobox.com)

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:
	
		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

#include "gl_handlers.h"

extern jk_scene_struct *scene;

GLfloat
scale_x(GtkGLArea *glarea, gdouble x);

GLfloat
scale_y(GtkGLArea *glarea, gdouble y);

const void
gl_configure_event_handler(GtkGLArea *glarea, GdkEventConfigure *event)
{
  gl_window_info *info;
  info = gtk_object_get_data(GTK_OBJECT(glarea), "info");

  info->aspect_ratio =
    (GLfloat) GTK_WIDGET (glarea)->allocation.width /
    (GLfloat) GTK_WIDGET (glarea)->allocation.height;
  info->x_median = info->zoom / 2.0 * info->aspect_ratio;
  info->y_median = info->zoom / 2.0;


  if (gtk_gl_area_make_current(GTK_GL_AREA(glarea)))
    {
      glViewport(0,0, GTK_WIDGET (glarea)->allocation.width, GTK_WIDGET (glarea)->allocation.height);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gl_window_orient_window(glarea);

      glMatrixMode(GL_MODELVIEW);
    }
  return;
}

const void
gl_expose_event_handler(GtkGLArea *glarea, GdkEventExpose *event )
{
  const gchar *widget_name;
  gl_window_info *info;
  int i;

  gboolean is_multi_frame;

  GList *model_list;
  jk_model_struct   *model;
  jk_pose_struct    *pose;
  jk_texture_struct *texture;

  widget_name = gtk_widget_get_name(GTK_WIDGET (glarea) );

  info = gtk_object_get_data(GTK_OBJECT(glarea), "info");

  /* Draw only last expose. */
  if (event->count > 0)
    return;
  
  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(glarea)))
    {
      //load the projection identity matrix
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      //load the modelview identity matrix
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //clear the color and depth buffers
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

      //orient the scene
      gl_window_orient_window(glarea);

      //if grid showing hasn't been turned off, show the grid
      if(info->show_grid)
	gl_window_draw_grid(glarea);

      //if axis showing hasn't been turned off, show the axis
      if(info->show_axis)
	gl_window_draw_axis(glarea);

      if(info->orientation == ORIENTATION_3D){
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, 1);
      }

      glColor3f(1.0, 0.0, 0.0);

      for(model_list = scene->model;
	  model_list != NULL;
	  model_list = model_list->next
	  ){

	model   = (jk_model_struct *)model_list->data;
	pose    = (jk_pose_struct *)model->cur_pose->data;
	texture = model->cur_texture;

	//assert that those guys are set
	g_return_if_fail(model   != NULL);
	g_return_if_fail(pose    != NULL);
	g_return_if_fail(texture != NULL);
	
	glVertexPointer(VERTEX_ORDINATE_COUNT, GL_FLOAT, 0, pose->vertex);
	glTexCoordPointer(TEXCOORD_ORDINATE_COUNT, GL_FLOAT, 0,
			  texture->texcoord);
	glIndexPointer(GL_UNSIGNED_INT, 0,
		       model->index);

	//specify the texture
	glBindTexture(GL_TEXTURE_2D, model->cur_texture->tex_name);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
			  texture->w, texture->h,
			  GL_RGBA, GL_BYTE, texture->pixel);
  
	//glDrawElements(GL_TRIANGLES, model->numtris, GL_UNSIGNED_INT, model->index);
	glBegin(GL_TRIANGLES);
	for(i = 0; i < model->numtris * 3; i++)
	  glArrayElement(model->index[i]);
	glEnd();
      }

      //if any of the models were multi-frame, show the animation tools
      if(is_multi_frame){
	//reset the boolean
	is_multi_frame = FALSE;
      }else{

      }

      glDisable(GL_TEXTURE_2D);

      //if this is a texture scene, just draw the texture sphere and return
      if(info->orientation == ORIENTATION_TEXTURE){
	gl_window_draw_texsphere(glarea);
	/* Swap backbuffer to front */
	gtk_gl_area_swapbuffers(GTK_GL_AREA(glarea));

	return;
      }

      //draw the mesh
      //glDrawArrays(GL_POINTS, 0, 3);

      //Swap backbuffer to front
      gtk_gl_area_swapbuffers(GTK_GL_AREA(glarea));
    }

  return;
}

const void
gl_map_handler(GtkGLArea *glarea, GdkEvent *event, gchar *o_string)
{
  o_enum orientation;
  gl_window_info *info;

  //convert the orientation string to an enum
  orientation = gl_orientation_string_to_enum(o_string);

  //change the scene's orientation to the passed orientation
  gl_window_change_orientation(glarea, orientation);

  //get the gl window's info
  info = gtk_object_get_data(GTK_OBJECT(glarea), "info");

  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(glarea)))
    {
      //resize the GL viewport to the size of the widget
      glViewport(0,0, GTK_WIDGET (glarea)->allocation.width, GTK_WIDGET (glarea)->allocation.height);

      //enable the depth buffer and select the GL_LEQUAL depth comparison function
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);

      //load the identity matrix
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      //orient the scene
      gl_window_orient_window(glarea);

      // enable vertex array.
      glEnableClientState(GL_VERTEX_ARRAY);

      //enable texcoord array
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      //enable the index pointer
      glEnableClientState(GL_INDEX_ARRAY);

      //set the size of points to 4 pixels
      glPointSize(4.0);
    }
  return;
}

gboolean
gl_button_release_handler(GtkGLArea *glarea, GdkEventButton *event)
{
  return FALSE;
}

gboolean
gl_button_press_handler(GtkGLArea *glarea, GdkEventButton *event)
{
  gl_window_info *info = (gl_window_info*)gtk_object_get_data(GTK_OBJECT(glarea), "info");
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET (glarea));
  GtkMenu *menu;
  GdkRectangle area;

  area.x = 0;
  area.y = 0;
  area.width  = GTK_WIDGET (glarea)->allocation.width;
  area.height = GTK_WIDGET (glarea)->allocation.height;
  
  if (event->button == 1) {
    /* beginning of drag, reset mouse position */
    info->beginx = event->x;
    info->beginy = event->y;
    return TRUE;
  }

  if (event->button == 2) {
    info->beginx = event->x;
    info->beginy = event->y;
    return TRUE;
  }

  if(event->button == 3 && info->orientation != ORIENTATION_TEXTURE ) {

    menu = GTK_MENU(glade_xml_get_widget(xml, "glm_orientation_menu"));
    gtk_object_set_data(GTK_OBJECT(menu), "glarea", glarea);

    menu = GTK_MENU(glade_xml_get_widget(xml, "gl_menu"));
    gtk_object_set_data(GTK_OBJECT(menu), "glarea", glarea);

    configure_gl_popup_menu(GTK_GL_AREA (glarea) );

    gtk_menu_popup(menu, NULL, NULL,
		   NULL, NULL, 3,
		   event->time
		   );
    return TRUE;
  }

  if(event->button == 4 || event->button == 5){
    if (event->button == 4) {
      /* mouse scroll up */
      info->zoom /= ZOOM_SCALE;
      if(info->zoom < MIN_ZOOM) info->zoom = MIN_ZOOM;
    }

    if (event->button == 5) {
      /* mouse scroll down */
      info->zoom *= ZOOM_SCALE;
      if(info->zoom > MAX_ZOOM) info->zoom = MAX_ZOOM;
    }

    info->x_median = info->zoom / 2.0 * info->aspect_ratio;
    info->y_median = info->zoom / 2.0;

    /* zoom has changed, redraw mesh */
    gtk_widget_draw(GTK_WIDGET (glarea), &area);
    return TRUE;
  }

  return FALSE;
}

gboolean
gl_motion_notify_handler(GtkGLArea *glarea, GdkEventMotion *event)
{
  int x, y;

  GLfloat display_x, display_y, display_z;
  gchar status[STATUSBAR_SIZE];

  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET (glarea));

  GdkRectangle area;
  GdkModifierType state;
  gl_window_info *info = (gl_window_info*)gtk_object_get_data(GTK_OBJECT(glarea), "info");

  if (event->is_hint) {
    /* fix this! */
#if !defined(WIN32)
    gdk_window_get_pointer(event->window, &x, &y, &state);
#endif
  } else {
    x = event->x;
    y = event->y;
    state = event->state;
  }
  
  area.x = 0;
  area.y = 0;
  area.width  = GTK_WIDGET (glarea)->allocation.width;
  area.height = GTK_WIDGET (glarea)->allocation.height;

  if(info->orientation != ORIENTATION_3D &&
     info->orientation != ORIENTATION_TEXTURE ){
    /* create the X, Y, and Z locations from..
     * - the zoom value
     * - the orientation
     * - the mouse position
     * - the width of the screen
     */
    
    display_x = 0;
    display_y = 0;
    display_z = 0;

    switch(info->orientation){
    case ORIENTATION_LEFT:
      display_y = scale_y(glarea, event->y);
      display_z = scale_x(glarea, event->x);

      break;
    case ORIENTATION_RIGHT:
      display_y = scale_y(glarea, event->y);
      display_z = scale_x(glarea, event->x) * -1;
      break;

    case ORIENTATION_TOP:
      display_x = scale_x(glarea, event->x);
      display_z = scale_y(glarea, event->y);

      break;
    case ORIENTATION_BOTTOM:
      display_x = scale_x(glarea, event->x);
      display_z = scale_y(glarea, event->y) * -1;
      break;

    case ORIENTATION_BACK:
      display_x = scale_x(glarea, event->x) * -1;
      display_y = scale_y(glarea, event->y);
      break;
    case ORIENTATION_FRONT:
      display_x = scale_x(glarea, event->x);
      display_y = scale_y(glarea, event->y);
      break;
    default: break;
    }
    
    snprintf(status, STATUSBAR_SIZE,
	     "X: %.2f, Y: %.2f, Z: %.2f",
	     display_x, display_y, display_z
	     );

    gtk_statusbar_push(GTK_STATUSBAR(glade_xml_get_widget(xml, "statusbar")),
		       1, status
		       );

    
  }else if( (state & GDK_BUTTON1_MASK) ) {
    /* drag in progress, simulate trackball */
    float spin_quat[4];
    trackball(spin_quat,
	      (2.0*info->beginx -       area.width) / area.width,
	      (     area.height - 2.0*info->beginy) / area.height,
	      (           2.0*x -       area.width) / area.width,
	      (     area.height -            2.0*y) / area.height);
    add_quats(spin_quat, info->quat, info->quat);

    /* orientation has changed, redraw mesh */
    gtk_widget_draw(GTK_WIDGET (glarea), &area);
  }

  if (state & GDK_BUTTON2_MASK) {
    /* zooming drag */

    if(info->beginy > event->y)
      info->zoom /= 1.025;
    else if(info->beginy < event->y)
      info->zoom *= 1.025;

    if (info->zoom < MIN_ZOOM) info->zoom = MIN_ZOOM;
    if (info->zoom > MAX_ZOOM) info->zoom = MAX_ZOOM;

    printf("zoom is %.2f\n", info->zoom);

    info->x_median = info->zoom / 2.0 * info->aspect_ratio;
    info->y_median = info->zoom / 2.0;

    /* zoom has changed, redraw mesh */
    gtk_widget_draw(GTK_WIDGET (glarea), &area);
  }
  info->beginx = x;
  info->beginy = y;

  return FALSE;
}


GLfloat
scale_y(GtkGLArea *glarea, gdouble y)
{
  GLfloat scaled_y;
  gl_window_info *info = (gl_window_info*)gtk_object_get_data(GTK_OBJECT(glarea), "info");

  /* generate a number in the range [0, 1] that represents the location of the mouse
   * on the x=0 y=0 line
   */
  scaled_y = y / (GLfloat) GTK_WIDGET (glarea)->allocation.height;
  
  /* scale that number to the range [0, info->zoom] */
  scaled_y *= info->zoom;

  /* subtract the median of the zoom from the scaled number to get a number
   * in the range [ -info->y_median, info->y_median ]
   */
  scaled_y -= info->y_median;

  /* round to the nearest 0.05 */
  scaled_y -= fmod(scaled_y,0.05);

  return ( scaled_y * -1);
}

GLfloat
scale_x(GtkGLArea *glarea, gdouble x)
{
  GLfloat scaled_x;
  gl_window_info *info = (gl_window_info*)gtk_object_get_data(GTK_OBJECT(glarea), "info");

  /* generate a number in the range [0, 1] that represents the location of the mouse
   * on the y=0 z=0 line
   */
  scaled_x = x / (GLfloat) GTK_WIDGET (glarea)->allocation.width;

  /* scale that number to the range [ 0, info->zoom ] */
  scaled_x *= info->zoom;

  /* multiply that number by the w / h aspect ratio */
  scaled_x *= info->aspect_ratio;
  
  /* subtract the median of the zoom from the scaled number to get a number
   * in the range [ -info->x_median, info->x_median ]
   */
  scaled_x -= info->x_median;

  /* round to the nearest 0.05 */
  scaled_x -= fmod(scaled_x,0.05);

  return( scaled_x );
}

