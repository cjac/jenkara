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

#include <stdio.h>
#include <string.h>

#define PALETTE_SIZE 768

typedef struct{

  unsigned char    id_length;
  unsigned char    palette_exists;
  unsigned char    image_type;

  signed short int first_palette_index;
  signed short int num_palette_indices;
  unsigned char    bits_per_color;

  signed short int x_origin;
  signed short int y_origin;
  signed short int image_width;
  signed short int image_height;
  unsigned char    bits_per_pixel;
  unsigned char    descriptor_flags;

}targa_header;

typedef struct{


}targa_footer;

void write_targa(int width, int height, char *image,
		 const char *palette_file, const char *targa_file
		 );

