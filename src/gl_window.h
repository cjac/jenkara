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

/* This header defines the functions and data that will be used to
 * manipulate the GL window
 */

#ifndef _gl_window_h_
#define _gl_window_h_

#include <stdio.h>
#include <string.h>
#include <GL/gl.h>

#include "gldata.h"
#include "trackball.h"
#include "gl_handlers.h"
#include "jenkara.h"

/* mutually exclusive orientation modes */
typedef enum
  {
    ORIENTATION_NONE,
    ORIENTATION_3D,      // Virtual Trackball
    ORIENTATION_TEXTURE, // The texture sphere
    ORIENTATION_LEFT,    // X = 0, Left  = -Z, Down = -Y
    ORIENTATION_RIGHT,   // X = 0, Right = -Z, Down = -Y
    ORIENTATION_BOTTOM,  // Y = 0, Left  = -X, Down = -Z
    ORIENTATION_TOP,     // Y = 0, Left  = -X, Up   = -Z
    ORIENTATION_FRONT,   // Z = 0, Right = -X, Down = -Y
    ORIENTATION_BACK,    // Z = 0, Left  = -X, Down = -Y
  } o_enum;

typedef enum
  {
    MODE_NONE,
    MODE_WIREFRAME,
    MODE_FLAT,
    MODE_SMOOTH,
    MODE_TEXTURED
  } m_enum;

typedef struct gl_window_info {
  gboolean show_axis;
  gboolean show_grid;

  gfloat   aspect_ratio;
  gfloat   x_median;
  gfloat   y_median;

  float    beginx,beginy;  /* position of mouse */
  float    quat[4];        /* orientation of object */
  GLfloat  zoom;           /* field of view in degrees */

  o_enum   orientation;
  m_enum   polygon_mode;
} gl_window_info;

void
gl_window_change_orientation(GtkGLArea *glarea, o_enum orientation);

void
gl_window_init(GtkGLArea *glarea);

void
gl_window_orient_window(GtkGLArea *glarea);

gchar
gl_window_get_orientation(GtkGLArea *glarea);

void
gl_window_draw_grid(GtkGLArea *glarea);

void
gl_window_draw_axis(GtkGLArea *glarea);

void
gl_window_draw_texsphere(GtkGLArea *glarea);

o_enum
gl_orientation_string_to_enum(gchar *o_string);

void
configure_gl_popup_menu(GtkGLArea *glarea);

#endif /*_gl_window_h_*/
