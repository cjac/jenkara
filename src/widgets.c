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

#include "widgets.h"

GtkGLArea *
create_gtkglarea ( char *widget_name, char *o_string,
		   int orientation, int int2
		   )
{
  GtkGLArea *glarea;

  static int attrlist[] = {
    GDK_GL_RGBA,
    GDK_GL_RED_SIZE,1,
    GDK_GL_GREEN_SIZE,1,
    GDK_GL_BLUE_SIZE,1,
    GDK_GL_DEPTH_SIZE,1,
    GDK_GL_DOUBLEBUFFER,
    GDK_GL_NONE
  };

  orientation = gl_orientation_string_to_enum(o_string);

  glarea = GTK_GL_AREA (gtk_gl_area_new(attrlist));
  gtk_widget_set_name(GTK_WIDGET(glarea), widget_name);
  gl_window_init(glarea);
  gtk_widget_set_events(GTK_WIDGET(glarea),
			GDK_EXPOSURE_MASK|
			GDK_BUTTON_PRESS_MASK|
			GDK_BUTTON_RELEASE_MASK|
			GDK_POINTER_MOTION_MASK|
			GDK_POINTER_MOTION_HINT_MASK);

  return glarea;
}
