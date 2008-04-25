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

#include "menu_handlers.h"

extern jk_scene_struct *scene;

const void
on_about_activate(GtkWidget *menu_item, gpointer data)
{
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET (menu_item));
  //gnome_dialog_run_and_close(GNOME_DIALOG(glade_xml_get_widget(xml, "about_window")));
}


/* finish up the program when the exit1 menu item is clicked */
const void
on_exit_activate(GtkWidget *menu_item, gpointer data)
{
  gtk_main_quit();
}

/* finish up the program when the window is closed (e.g. the X button) */
const void
on_root_window_destroy(GtkWidget *window, gpointer data)
{  
  gtk_main_quit();
}

const void
on_import_model_activate(GtkWidget *menu_item, gpointer data)
{
  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET (menu_item));
  GtkWidget *filesel = glade_xml_get_widget(xml, "model_filesel");
  GtkWidget *root_window = glade_xml_get_widget(xml, "root_window");

  gtk_widget_set_sensitive(GTK_WIDGET (root_window), FALSE);
  gtk_widget_show(GTK_WIDGET (filesel) );
  return;
}


const void
on_model_filesel_ok_clicked(GtkFileSelection *button, gpointer data)
{
  void (*import_func)();
  gchar *model_filename;

  GladeXML *xml = glade_get_widget_tree(GTK_WIDGET (button));
  GtkWidget *filesel = glade_xml_get_widget(xml, "model_filesel");
  GtkWidget *root_window = glade_xml_get_widget(xml, "root_window");

  gtk_object_set_data(GTK_OBJECT (filesel), "import_func", import_mdl);

  import_func =
    gtk_object_get_data(GTK_OBJECT (filesel), "import_func");

  model_filename =
    gtk_file_selection_get_filename(GTK_FILE_SELECTION (filesel));

  printf("Model filename: %s\n", model_filename);

  //import model (into the scene->model list)
  import_func(model_filename);

  //do some checks on the data (TODO), and if they pass, hide the file selector
  gtk_widget_hide(GTK_WIDGET (filesel) );

  //then set the root window sensitivity back to TRUE
  gtk_widget_set_sensitive(GTK_WIDGET (root_window), TRUE);

  //assert that a model was imported
  g_return_if_fail(scene->model != NULL);

  return;
}


