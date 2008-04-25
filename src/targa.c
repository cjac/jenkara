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

#include "targa.h"

void write_targa(int width, int height, char *image,
		 const char *palette_file, const char *targa_file
		 )
{
  FILE *fp;
  char rgb_palette[PALETTE_SIZE], bgr_palette[PALETTE_SIZE];
  targa_header tga;
  int i;
  

  //copy the palette to memory
  fp = fopen(palette_file, "r");
  fread(rgb_palette, PALETTE_SIZE, 1, fp);
  fclose(fp);

  //convert the palette from RGB to BGR
  for(i = 0; i < PALETTE_SIZE / 3; i++){
    bgr_palette[i*3 + 0] = rgb_palette[i*3 + 2];
    bgr_palette[i*3 + 1] = rgb_palette[i*3 + 1];
    bgr_palette[i*3 + 2] = rgb_palette[i*3 + 0];
  }


  printf("targa filename = %s\n", targa_file);
  fp = fopen(targa_file, "w+");

  memset(&tga, 0, sizeof(targa_header));

  tga.id_length = 0;
  tga.palette_exists = 1;
  tga.image_type = 1;

  tga.first_palette_index = 0;
  tga.num_palette_indices = 255;
  tga.bits_per_color = 24;

  tga.x_origin = 0;
  tga.y_origin = 0;
  tga.image_width = width;
  tga.image_height = height;
  tga.bits_per_pixel = 8;
  tga.descriptor_flags = 0x30;

  //write the header
  fwrite(&tga.id_length, sizeof(char), 1, fp);
  fwrite(&tga.palette_exists, sizeof(char), 1, fp);
  fwrite(&tga.image_type, sizeof(char), 1, fp);
  fwrite(&tga.first_palette_index, sizeof(short), 1, fp);
  fwrite(&tga.num_palette_indices, sizeof(short), 1, fp);
  fwrite(&tga.bits_per_color, sizeof(char), 1, fp);
  fwrite(&tga.x_origin, sizeof(short), 1, fp);
  fwrite(&tga.y_origin, sizeof(short), 1, fp);
  fwrite(&tga.image_width, sizeof(short), 1, fp);
  fwrite(&tga.image_height, sizeof(short), 1, fp);
  fwrite(&tga.bits_per_pixel, sizeof(char), 1, fp);
  fwrite(&tga.descriptor_flags, sizeof(char), 1, fp);

  //write the color palette
  fwrite(bgr_palette, PALETTE_SIZE, 1, fp);

  //write the image data
  fwrite(image, width * height, 1, fp);

  fclose(fp);

  return;
}
