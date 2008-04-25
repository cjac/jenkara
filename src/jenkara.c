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

#include "jenkara.h"

int main(int argc, char * argv[]) {
  GladeXML *xml;

  //initialize the vertex array structure
  gldata_init();

  //gtk, glade & gnome init
  gtk_init(&argc, &argv);
  //glade_gnome_init();
  //glade_init();
  gnome_program_init("jenkara", "0.1", LIBGNOMEUI_MODULE,
		     argc, argv,
		     NULL);

  /* Check if OpenGL is supported. */
  if (gdk_gl_query() == FALSE) {
    g_print("OpenGL not supported\n");
    return 0;
  }

  //FIXME: This should point to the dist directory
  xml = glade_xml_new("../ui/jenkara.glade2", NULL, NULL);
  glade_xml_signal_autoconnect(xml);

  gtk_main();
  return 0;
}
