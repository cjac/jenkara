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

#ifndef _gl_window_handlers_h_
#define _gl_window_handlers_h_

#include "gl_window.h"
#include <GL/glut.h>

#define STATUSBAR_SIZE  40

#define ZOOM_SCALE      1.5
#define ZOOM_SCALE_RATE 30
#define MIN_ZOOM        0.5
#define MAX_ZOOM        600

gboolean
gl_button_press_handler(GtkGLArea *glarea, GdkEventButton *event);

gboolean
gl_motion_notify_handler(GtkGLArea *glarea, GdkEventMotion *event);

const void
gl_expose_event_handler( GtkGLArea *glarea, GdkEventExpose *event );

const void
gl_configure_event_handler(GtkGLArea *widget, GdkEventConfigure *event);

const void
gl_show_handler(GtkGLArea *glarea, const gchar *o_string);
/*
const void gl_realize_handler(GtkWidget *widget);
*/

#endif /* _gl_window_handlers_h_ */
