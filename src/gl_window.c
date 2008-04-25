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

#include "gl_window.h"

void
gl_window_init(GtkGLArea *glarea)
{
  gl_window_info *info;

  info = (gl_window_info *)malloc(sizeof(gl_window_info));

  info->orientation    = ORIENTATION_NONE;
  info->polygon_mode   = MODE_WIREFRAME;

  info->show_axis      = TRUE;
  info->show_grid      = TRUE;

  info->beginx         = 0;
  info->beginy         = 0;
  info->zoom           = 40;
  info->aspect_ratio =
    (GLfloat) GTK_WIDGET (glarea)->allocation.width /
    (GLfloat) GTK_WIDGET (glarea)->allocation.height;
  info->x_median = info->zoom / 2.0 * info->aspect_ratio;
  info->y_median = info->zoom / 2.0;

  trackball(info->quat , 0.0, 0.0, 0.0, 0.0);

  gtk_object_set_data(GTK_OBJECT(glarea), "info", info);

  return;
}

void
gl_window_orient_window(GtkGLArea *glarea)
{
  gl_window_info *info = gtk_object_get_data(GTK_OBJECT(glarea), "info");
  GLfloat median;
  float m[4][4];

  if (gtk_gl_area_make_current(GTK_GL_AREA(glarea))){

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);

    if(info->orientation == ORIENTATION_3D ||
       info->orientation == ORIENTATION_TEXTURE ){

      glMatrixMode(GL_PROJECTION);
      glFrustum( -0.0005 * info->aspect_ratio, 0.0005 * info->aspect_ratio,
		 -0.0005 , 0.0005,
		 0.001, 1000
		 );

    }else{
      if(info->x_median >= info->y_median)
	median = info->x_median;
      else
	median = info->y_median;


      glOrtho(-1 * info->x_median, info->x_median,      // left, right
	      -1 * info->y_median, info->y_median,      // bottom, top
	      median * -3, median * 3                   // near, far
	      );
    }

    switch(info->orientation)
      {

      case ORIENTATION_FRONT:	break;
      case ORIENTATION_3D:
      case ORIENTATION_TEXTURE:
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0,0,-1 * info->zoom);

	build_rotmatrix(m, info->quat);
	glMultMatrixf(&m[0][0]);
	break;
      case ORIENTATION_TOP:	glRotatef(-90, 1,0,0);	break;
      case ORIENTATION_BOTTOM:	glRotatef( 90, 1,0,0);	break;
      case ORIENTATION_LEFT:	glRotatef(90, 0,1,0);	break;
      case ORIENTATION_RIGHT:	glRotatef(-90, 0,1,0);	break;
      case ORIENTATION_BACK:	glRotatef(180, 0,1,0);	break;
      default: break;
      }

  }/*if(gtk_gl_area_make_current(glarea))*/
}

void
gl_window_change_orientation(GtkGLArea *glarea, o_enum orientation)
{
  gl_window_info *info;
  GdkRectangle area;

  info = gtk_object_get_data(GTK_OBJECT(glarea), "info");

  if (gtk_gl_area_make_current(GTK_GL_AREA(glarea))){
    //if changing to 3d or texture orientation
    if(orientation == ORIENTATION_3D ||
       orientation == ORIENTATION_TEXTURE ){

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      glShadeModel(GL_SMOOTH);
      glClearColor(0.2, 0.5, 0.7, 1.0);
      glClearDepth(1);
    }else{
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glClearColor(0.6, 0.6, 0.6, 1);
    }
  }

  info->orientation = orientation;

  area.x = 0;
  area.y = 0;
  area.width  = GTK_WIDGET (glarea)->allocation.width;
  area.height = GTK_WIDGET (glarea)->allocation.height;

  //orientation changed, redraw
  gtk_widget_draw(GTK_WIDGET (glarea), &area);
}

void
gl_window_draw_grid(GtkGLArea *glarea)
{
  gl_window_info *info = gtk_object_get_data(GTK_OBJECT(glarea), "info");
  GLfloat top, left, right, bottom, x, y;
  static GLfloat line_spacing = 5;
  gint line_count, i;

  // don't draw grid for texture ball
  if(info->orientation == ORIENTATION_TEXTURE)
    return;

  line_count = (int) rintf(info->x_median / line_spacing) + 1;

  if(line_count > 10){
    line_spacing *= 2.0;
    line_count = (int) rintf(info->x_median / line_spacing) + 1;
  }else if(line_count < 6){
    line_spacing /= 2.0;
    line_count = (int) rintf(info->x_median / line_spacing) + 1;
  }

  /* set top, bottom, left and right ordinates */
  if(info->orientation == ORIENTATION_3D){

    /* in 3d mode, draw a grid on the Y = 0 plane */

    top = 0.90 * info->y_median;

    if(top < 10) top = 10;

    top -= fmod(top, line_spacing);
    bottom = -top;

    right = top;
    left  = bottom;

    line_count = rintf(right / line_spacing) + 1;
  }else{
    top    = info->y_median * -1;
    bottom = info->y_median;

    left   = info->x_median * -1;
    right  = info->x_median;
  }

  if (gtk_gl_area_make_current(GTK_GL_AREA(glarea))){
    glMatrixMode(GL_MODELVIEW);
    switch(info->orientation)
      {
      case ORIENTATION_FRONT:	break;
      case ORIENTATION_3D:	glRotatef(-90, 1,0,0);  break;
      case ORIENTATION_TOP:	glRotatef(-90, 1,0,0);	break;
      case ORIENTATION_BOTTOM:	glRotatef( 90, 1,0,0);	break;
      case ORIENTATION_LEFT:	glRotatef(90, 0,1,0);	break;
      case ORIENTATION_RIGHT:	glRotatef(-90, 0,1,0);	break;
      case ORIENTATION_BACK:	glRotatef(180, 0,1,0);	break;
      default: break;
      }

    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINES);

    /* draw x=0 z=0 line */
    glVertex3f( 0, top,    0 );
    glVertex3f( 0, bottom, 0 );

    /* draw y=0 z=0 line */
    glVertex3f( left,  0, 0 );
    glVertex3f( right, 0, 0 );

    for(i = 0; i < line_count; i++){

      y = line_spacing * i;
      x = y;

      glVertex3f( -x, top,    0 );
      glVertex3f( -x, bottom, 0 );

      glVertex3f( x, bottom, 0 );
      glVertex3f( x, top,    0 );

      glVertex3f( left,  y, 0 );
      glVertex3f( right, y, 0 );

      glVertex3f( right, -y, 0 );
      glVertex3f( left,  -y, 0 );
      
    }
    glEnd();

    switch(info->orientation)
      {
      case ORIENTATION_FRONT:	break;
      case ORIENTATION_3D:	glRotatef(90, 1,0,0);  break;
      case ORIENTATION_TOP:	glRotatef(90, 1,0,0);	break;
      case ORIENTATION_BOTTOM:	glRotatef(-90, 1,0,0);	break;
      case ORIENTATION_LEFT:	glRotatef(-90, 0,1,0);	break;
      case ORIENTATION_RIGHT:	glRotatef(90, 0,1,0);	break;
      case ORIENTATION_BACK:	glRotatef(180, 0,1,0);	break;
      default: break;
      }
  }


  return;
}

void
gl_window_draw_axis(GtkGLArea *glarea)
{
  if (gtk_gl_area_make_current(GTK_GL_AREA(glarea))){
    glBegin(GL_LINES);
      glColor3f(1.0, 1.0, 0.0);
      glVertex3f(0,0,0);
      glVertex3f(10,0,0);

      glColor3f(0.0, 1.0, 1.0);
      glVertex3f(0,0,0);
      glVertex3f(0,10,0);

      glColor3f(1.0, 0.0, 1.0);
      glVertex3f(0,0,0);
      glVertex3f(0,0,10);
    glEnd();
  }
}

void
gl_window_draw_texsphere(GtkGLArea *glarea)
{
  return;
}

// convert the string version of one of our constants to an int
o_enum
gl_orientation_string_to_enum(gchar *o_string)
{
  o_enum orientation;

  if(strncmp("ORIENTATION_NONE", o_string, strlen("ORIENTATION_NONE")) == 0)
    orientation = ORIENTATION_NONE;
  if(strncmp("ORIENTATION_TOP", o_string, strlen("ORIENTATION_TOP")) == 0)
    orientation = ORIENTATION_TOP;
  else if(strncmp("ORIENTATION_BOTTOM", o_string, strlen("ORIENTATION_BOTTOM")) == 0)
    orientation = ORIENTATION_BOTTOM;
  else if(strncmp("ORIENTATION_FRONT", o_string, strlen("ORIENTATION_FRONT")) == 0)
    orientation = ORIENTATION_FRONT;
  else if(strncmp("ORIENTATION_BACK", o_string, strlen("ORIENTATION_BACK")) == 0)
    orientation = ORIENTATION_BACK;
  else if(strncmp("ORIENTATION_LEFT", o_string, strlen("ORIENTATION_LEFT")) == 0)
    orientation = ORIENTATION_LEFT;
  else if(strncmp("ORIENTATION_RIGHT", o_string, strlen("ORIENTATION_RIGHT")) == 0)
    orientation = ORIENTATION_RIGHT;
  else if(strncmp("ORIENTATION_3D", o_string, strlen("ORIENTATION_3D")) == 0)
    orientation = ORIENTATION_3D;
  else if(strncmp("ORIENTATION_TEXTURE", o_string, strlen("ORIENTATION_TEXTURE")) == 0)
    orientation = ORIENTATION_TEXTURE;

  return orientation;
}


// set up the popup menu for the context of the passed glarea
void
configure_gl_popup_menu(GtkGLArea *glarea)
{
  gchar menu_item_name[64];
  gchar *o_string;
  gboolean sensitivity = FALSE;
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET (glarea));
  gl_window_info *info = (gl_window_info*)gtk_object_get_data(GTK_OBJECT(glarea), "info");
  GtkCheckMenuItem *menu_item;

  //make sure the booleans and radios are set correctly before displaying the menu...
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (glade_xml_get_widget(xml, "glm_show_axis")),
				 info->show_axis
				 );

  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM (glade_xml_get_widget(xml, "glm_show_grid")),
				 info->show_grid
				 );

  switch(info->orientation)
    {
    case ORIENTATION_3D:     o_string = "3d"; sensitivity = TRUE; break;
    case ORIENTATION_FRONT:  o_string = "front";  break;
    case ORIENTATION_BACK:   o_string = "back";   break;
    case ORIENTATION_LEFT:   o_string = "left";   break;
    case ORIENTATION_RIGHT:  o_string = "right";  break;
    case ORIENTATION_TOP:    o_string = "top";    break;
    case ORIENTATION_BOTTOM: o_string = "bottom"; break;

    default: break;
    }

  sprintf(menu_item_name, "glm_orientation_radio_%s", o_string);

  menu_item = GTK_CHECK_MENU_ITEM (glade_xml_get_widget(xml, (const char *) menu_item_name));

  if(menu_item == NULL)
    g_print("orientation menu item is null\n");
  else
    gtk_check_menu_item_set_active(menu_item, 1);

  switch(info->polygon_mode)
    {
    case MODE_WIREFRAME: strcpy(menu_item_name, "glm_wireframe");     break;
    case MODE_FLAT:      strcpy(menu_item_name, "glm_flat_shaded");   break;
    case MODE_SMOOTH:    strcpy(menu_item_name, "glm_smooth_shaded"); break;
    case MODE_TEXTURED:  strcpy(menu_item_name, "glm_textured");      break;

    default: break;
    }

  menu_item = GTK_CHECK_MENU_ITEM (glade_xml_get_widget(xml, (const char *) menu_item_name));
  if(menu_item == NULL)
    g_print("polygon mode menu item is null\n");
  else
    gtk_check_menu_item_set_active(menu_item, 1);

  return;
}
