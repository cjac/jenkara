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

#include "gl_menu_handlers.h"

void
toggle_axis(GtkCheckMenuItem *menu_item)
{
  GtkMenu *menu;
  GtkGLArea *glarea;
  gl_window_info *info;
  GdkRectangle area;

  //get the menu that holds menu_item
  menu = GTK_MENU (GTK_WIDGET (menu_item)->parent);

  //get the glarea that was set in gl_button_press_handler
  glarea = gtk_object_get_data(GTK_OBJECT (menu), "glarea");
  if(glarea == NULL)
    g_print("Error.  No glarea data in menu\n");

  //set the portion of glarea to draw
  area.x = 0;
  area.y = 0;
  area.width  = GTK_WIDGET (glarea)->allocation.width;
  area.height = GTK_WIDGET (glarea)->allocation.height;

  //get the glarea info
  info = gtk_object_get_data(GTK_OBJECT (glarea), "info");

  //change our axis drawing boolean based on current state of the menu item
  info->show_axis = menu_item->active;

  //scene has changed, redraw
  gtk_widget_draw(GTK_WIDGET (glarea), &area);

  return;
}

void
toggle_grid(GtkCheckMenuItem *menu_item)
{
  GtkMenu *menu;
  GtkGLArea *glarea;
  gl_window_info *info;
  GdkRectangle area;

  //get the menu that holds menu_item
  menu = GTK_MENU (GTK_WIDGET (menu_item)->parent);

  //get the glarea that was set in gl_button_press_handler
  glarea = gtk_object_get_data(GTK_OBJECT (menu), "glarea");
  if(glarea == NULL)
    g_print("Error.  No glarea data in menu\n");

  //set the portion of glarea to draw
  area.x = 0;
  area.y = 0;
  area.width  = GTK_WIDGET (glarea)->allocation.width;
  area.height = GTK_WIDGET (glarea)->allocation.height;

  //get the glarea info
  info = gtk_object_get_data(GTK_OBJECT (glarea), "info");

  //change our grid drawing boolean based on current state of the menu item
  info->show_grid = menu_item->active;

  //scene has changed, redraw
  gtk_widget_draw(GTK_WIDGET (glarea), &area);

  return;
}

void
on_glm_change_orientation(GtkCheckMenuItem *menu_item, gchar *o_string)
{
  o_enum orientation = gl_orientation_string_to_enum(o_string);
  GtkMenu *menu;
  GtkGLArea *glarea;
  gl_window_info *info;
  GdkRectangle area;

  //get the menu_item's parent
  menu = GTK_MENU(GTK_WIDGET (menu_item)->parent);

  //get the glarea that was set in gl_button_press_handler
  glarea = GTK_GL_AREA (gtk_object_get_data(GTK_OBJECT (menu), "glarea"));
  if(glarea == NULL){
    g_print("Error:  No glarea data in orientation menu\n");
    return;
  }

  //set the portion of glarea to draw
  area.x = 0;
  area.y = 0;
  area.width  = GTK_WIDGET (glarea)->allocation.width;
  area.height = GTK_WIDGET (glarea)->allocation.height;

  //get the glarea info
  info = gtk_object_get_data(GTK_OBJECT (glarea), "info");

  gl_window_change_orientation(glarea, orientation);

  //orientation has changed, redraw
  gtk_widget_draw(GTK_WIDGET (glarea), &area);


  return;
}
