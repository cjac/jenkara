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

#ifndef _toolbox_handlers_h_
#define _toolbox_handlers_h_
#include <glib.h>
#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>
#include <glade/glade.h>
#include <stdio.h>
#include "gldata.h"
#include "gl_window.h"

const void
on_scrolledwindow2_realize(GtkWidget *glarea);

const void toggle_frame( GtkWidget *frame, gpointer *data);
const void toggle_user_point(GtkWidget *radio, gpointer data);
const void on_select_vertex_radio_toggled(GtkWidget *radio, gpointer data);

#endif /*_toolbox_handlers_h_*/
