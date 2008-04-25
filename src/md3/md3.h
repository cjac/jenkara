/*
	Copyright (C) 2001 C.J. Collier (cjcollier@sinclair.net)

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

#ifndef _LOAD_MD3_H_
#define _LOAD_MD3_H_

#include <stdio.h>
#include <GL/gl.h>
#include <stdlib.h>

typedef struct md3_header {

  char ID[4];         //id of file, always "IDP3"
  int  Version;       //i suspect this is a version number, always 15 
  char FileName[68];  //sometimes left Blank... 65 chars, 32bit aligned == 68 chars 
  int  BoneFrame_num; //number of BoneFrames
  int  Tag_num;       //number of 'tags' per BoneFrame
  int  Mesh_num;      //number of meshes/skins 
  int  MaxSkin_num;   //maximum number of unique skins used in md3 file 
  int  HeaderLength;  //always equal to the length of this header 
  int  Tag_Start;     //starting position of tag-structures 
  int  Tag_End;       //ending position of tag-structures/starting position of mesh-structures 
  int  FileSize;      //size of file 

} md3_header;

typedef struct md3_tag {

  char Name[64];          //name of 'tag' as it's usually called in the
                          //md3 files try to see it as a sub-mesh/seperate
                          //mesh-part. sometimes this 64 string may
                          //contain some garbage, but i've been told this
                          //is because some tools leave garbage in those
                          //strings, but they ARE strings...
  GLfloat Postition[3];   //relative position of tag 
  GLfloat Rotation[3][3]; //the direction the tag is facing relative to the rest of the model

} md3_tag;

typedef struct md3_boneframe {

  //unverified: 
  float  Mins[3];
  float  Maxs[3];
  float  Position[3];
  float  scale;
  char   Creator[16]; //i think this is the 
                      //"creator" name.. 
                      //but i'm only guessing.
} md3_boneframe;

typedef struct md3_mesh_header {

  char ID[4];          //id, must be IDP3 
  char Name[68];       //name of mesh 
                       //65 chars, 32 bit aligned == 68 chars
  int  MeshFrame_num;  //number of meshframes 
                       //in mesh 
  int  Skin_num;       //number of skins in mesh 
  int  Vertex_num;     //number of vertices 
  int  Triangle_num;   //number of Triangles 
  int  Triangle_Start; //starting position of 
                       //Triangle data, relative 
                       //to start of Mesh_Header 
  int  HeaderSize;     //size of header 
  int  TexVec_Start;   //starting position of 
                       //texvector data, relative 
                       //to start of Mesh_Header 
  int  Vertex_Start;   //starting position of 
                       //vertex data,relative 
                       //to start of Mesh_Header 
  int  MeshSize;       //size of mesh 
             

} md3_mesh_header;

typedef struct md3_mesh_vertex {
  signed short Vec[3];       //vertex X/Y/Z coordinate
  unsigned char EnvTex[2];   //enviromental mapping texture coordinates
} md3_mesh_vertex;

typedef struct md3_mesh_struct{
  md3_mesh_header header;
  char            *skin_name;
  int             *triangle;
  float           *tex_vec;
  md3_mesh_vertex *vertex;
} md3_mesh_struct;

typedef struct md3_struct {
  md3_header      header;
  md3_tag         *tag;
  md3_boneframe   *boneframe;
  md3_mesh_struct *mesh;
} md3_struct;


md3_struct *
load_md3(const char *filename);

#endif /* _LOAD_MD3_H_ */
