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

#include "toolbox_handlers.h"

/* this handler shows and hides frames associated with the radio widget which
 * emits the event.  the name of the frame associated is passed as frame_name
 */

const void
toggle_frame( GtkWidget *frame, gpointer *data)
{

  if(GTK_WIDGET_VISIBLE(frame))
    gtk_widget_hide(frame);
  else
    gtk_widget_show(frame);    
}


/* this handler toggles the sensitivity of the x, y, and z entry widgets
 * and the button widget for the group whose name is passed as data.
 * currently, rotate and scale use this function
 */

const void
toggle_user_point(GtkWidget *radio, gpointer data)
{
  char *widget_name;
  GtkWidget *widget;
  GladeXML *xml = glade_get_widget_tree (GTK_WIDGET (radio));
  char *widget_meta_name[4] =
    { "%s_x_entry", "%s_y_entry", "%s_z_entry", "%s_button" };
  char *p, *widget_prefix = glade_get_widget_name(radio);
  int i;

  //set widget_prefix to everything up to the first _
  for(p = widget_prefix; *p != '_'; p++);
  *p = '\0';

  for(i = 0; i < 4; i++){
    asprintf(&widget_name, widget_meta_name[i], widget_prefix);
    widget = glade_xml_get_widget(xml, (const char *) widget_name);
    gtk_widget_set_sensitive(widget,(gboolean)GTK_TOGGLE_BUTTON(radio)->active);
    free(widget_name);
  }
  free(widget_prefix);
}

/* set the 'select by vertex' checkbutton to insensitive when
 * the select tool is being used to select vertices
 */
const void
on_select_vertex_radio_toggled(GtkWidget *radio, gpointer data)
{
  GladeXML *xml = glade_get_widget_tree (GTK_WIDGET (radio));
  gtk_widget_set_sensitive( glade_xml_get_widget(xml, "select_by_vertex_checkbutton"),
			    (gboolean) ! GTK_TOGGLE_BUTTON (radio)->active
			    );
}

