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

#ifndef _GLDATA_H_
#define _GLDATA_H_

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>
#include <GL/gl.h>
#include <stdlib.h>

#define VERTEX_ORDINATE_COUNT	3
#define COLOR_INDEX_COUNT	3
#define INDEX_COUNT		1
#define NORMAL_ORDINATE_COUNT   3
#define TEXCOORD_ORDINATE_COUNT	2
#define EDGEFLAG_COUNT		1
#define TRIANGLE_VERTEX_COUNT   3

typedef struct scene {
  GList *model;          //linked list of models
  GList *selected_model; //the currently selected model
} jk_scene_struct;

typedef struct texture {
  guint8   *pixel;    //RGBA values for each pixel
  GLfloat  *texcoord; //s & t coordinates for each vertex
  guint    w;         //width of texture
  guint    h;         //height of texture
  GLuint   tex_name;  //GL Texture name (used for binding textures)
} jk_texture_struct;

typedef struct texgroup {
  gint32            numtextures;  //the number of textures
  jk_texture_struct *texture;     //an array of textures (for multitexturing?)
  GLfloat           *interval;    //the interval between each texture displayed
} jk_texgroup_struct;

typedef struct model {
  gchar              *name;         //Name of the model
  GList              *phrase;       //list of phrases of movements
  guint              numverts;      //number of vertices
  guint              numtris;       //number of triangles
  GLuint             *index;        //array of indicies into the arrays.
                                    //numtris * 3 elements.
  guint              num_texgroups; //number of textures
  jk_texgroup_struct *texgroup;     //our texture group data
  
  GList              *cur_phrase;
  GList              *cur_pose;
  jk_texture_struct  *cur_texture;
} jk_model_struct;

typedef struct phrase {
  gchar  *name;    //name of the phrase, like "run" "jump" "deatha", etc
  GList  *pose;    //list of poses in the phrase
  gfloat interval; //time to wait between displaying each pose
} jk_phrase_struct;

typedef struct pose {
  gchar   *name;         //name of the pose 
  GLfloat *vertex;       //the array of vertices for this pose
  GLfloat *normal;       //the array of vertex normals for this pose
} jk_pose_struct;

void
gldata_init();

#endif /* _GLDATA_H_ */
