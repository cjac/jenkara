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
#include "import.h"

extern jk_scene_struct *scene;
extern int errno;

/*this is just for play*/
void export_targa(char *, char *);

//structure initializers
void init_model(jk_model_struct *model);
void init_phrase(jk_phrase_struct *phrase);
void init_pose(jk_pose_struct *pose);


void
import_mdl(char *mdl_filename)
{
  mdl_struct *mdl = load_mdl(mdl_filename);
  //used as a short name for mdl->frame[phrase_num].pose[pose_num].vertex[vert_num].v;
  Uint8 *vertex;
  //number of pixels in a texture
  int numpixels;
  //look at all the iterators!
  int pixel_num, group_num, tex_num, tri_num, tex_coord_num,
    tri_vert_num, phrase_num, pose_num, vert_num, ord_num;
  //the palette storage array
  guint8 palette[PALETTE_SIZE];
  //the filepointer
  FILE *fp;
  //palette filename :), short name for model & phrases, misc char pointer
  char *palette_filename, *name, *p;
  //phrase names to help sort the poses into phrases
  char *phrase_name, *last_phrase_name;
  //the model, phrase & pose temporary pointers
  jk_model_struct  *model;
  jk_phrase_struct *phrase;
  jk_pose_struct   *pose;
  //current and old indexes.  used to straighten out facesfront fiasco
  GLuint  cur_index, old_index;
  //texture coordinate organization pointer ;)
  GLfloat *texcoord;

  if(mdl == NULL){
    printf("Couldn't load mdl file %s\n", mdl_filename);
    return;
  }

  //FIXME: This should point to the dist directory
  palette_filename = "../data/palette.lmp";
  export_targa(mdl_filename, palette_filename);

  //get the name of the model from the filename
  name = basename(mdl_filename);
  p = strrchr(name, '.');
  *p = '\0';

  //initialize the model
  model = malloc(sizeof(jk_model_struct));

  model->name     = strdup(name);         //name of model
  model->numverts = 0;                    //setting to 0 until we decide the #
  model->numtris  = mdl->header.numtris;  //number of triangles
  model->phrase   = NULL;                 //NULL is an empty list

  model->num_texgroups = mdl->header.num_texgroups; //number of texture groups
  model->texgroup     = malloc(sizeof(jk_texgroup_struct) *
			       mdl->header.num_texgroups
			       ); 
  
  if( (fp = fopen(palette_filename, "r")) == NULL){
    printf("Open failure: %s\n", strerror(errno));
    return;
  }
  fread(palette, PALETTE_SIZE, 1, fp);
  fclose(fp);

  numpixels = mdl->header.texheight * mdl->header.texwidth;

  //initialize the last phrase name
  last_phrase_name = NULL;
  phrase = malloc(sizeof(jk_phrase_struct));
  init_phrase(phrase);

  phrase->pose = NULL; //NULL is an empty list

  for(phrase_num = 0; phrase_num < mdl->header.numframes; phrase_num++){
    for(pose_num = 0; pose_num < mdl->frame[phrase_num].numposes; pose_num++){
      pose = malloc(sizeof(jk_pose_struct));
      init_pose(pose);
      g_return_if_fail(pose != NULL);

      //grab name, pose number & vertex data
      pose->name    = strdup(mdl->frame[phrase_num].pose[pose_num].name);   
      
      //TODO: Implement vertex arrays with linked lists.
      //Cache to a contiguous array when changes are made
      pose->vertex = malloc(sizeof(GLfloat) *
			    mdl->header.numverts *
			    VERTEX_ORDINATE_COUNT *
			    5);
      
      //load the vertex array
      for(vert_num = 0; vert_num < mdl->header.numverts; vert_num++){
	vertex = mdl->frame[phrase_num].pose[pose_num].vertex[vert_num].v;
	for(ord_num = 0; ord_num < 3; ord_num++){
	  //convert packed byte ordinate to float
	  pose->vertex[vert_num * VERTEX_ORDINATE_COUNT + ord_num] = (GLfloat)
	    ( ( vertex[ord_num] * mdl->header.scale[ord_num] ) +
	      mdl->header.scale_origin[ord_num] );
	}
	model->numverts++;
      }
      
      //for calculating vertex normals, see darkplaces code:
      //gl_models.c: ZymoticCalcNormals()
    }

    //the phrase name is the pose name up to the first digit
    phrase_name = strdup(pose->name);
    for(p = phrase_name; !isdigit(*p); p++);
    *p = '\0';    

    //check the current phrase name against the last phrase name
    if(last_phrase_name == NULL ||
       strcmp(phrase_name, last_phrase_name) == 0){

      if(last_phrase_name == NULL){
	//if the last phrase name is not set, set it
	last_phrase_name = phrase_name;
      }else{
	//else free the phrase name string
	free(phrase_name);
      }

      //if it's the same, add the current pose to the phrase
      phrase->pose = g_list_prepend(phrase->pose, pose);
      
    }else{
      //if it's different, this phrase is completed
      phrase->name = strdup(last_phrase_name);
      phrase->pose  = g_list_reverse(phrase->pose);
      model->phrase = g_list_prepend(model->phrase, phrase);

      //create a new phrase
      phrase = malloc(sizeof(jk_phrase_struct));
      init_phrase(phrase);
      phrase->pose = NULL; //NULL is an empty list

      //FIXME
      //bah.  import intervals at another time.  I don't want to now ;)
      //phrase->interval = mdl->frame[phrase_num].interval;

      //and add this pose to it
      phrase->pose = g_list_prepend(phrase->pose, pose);
      //free the old phrase name and set the new one
      if(last_phrase_name != NULL)
	free(last_phrase_name);
      last_phrase_name = phrase_name;
    }
  }

  g_return_if_fail(phrase->pose != NULL);
  g_return_if_fail(last_phrase_name != NULL);

  phrase->name = strdup(last_phrase_name);
  //if(last_phrase_name != NULL)
  //free(last_phrase_name);

  phrase->pose  = g_list_reverse(phrase->pose);
  model->phrase = g_list_prepend(model->phrase, phrase);
  model->phrase = g_list_reverse(model->phrase);


  //FIXME: this is not exact.  we need to find out
  //the number of vertices by calculating in facesfront stuff

  texcoord = malloc(sizeof(GLfloat)         *
		    mdl->header.numverts    *
		    TEXCOORD_ORDINATE_COUNT *
		    5);

  //load the texture coords array from in-memory mdl struct
  for(tex_coord_num = 0; tex_coord_num < mdl->header.numverts; tex_coord_num++){
    texcoord[tex_coord_num*TEXCOORD_ORDINATE_COUNT + 0] = (float)
      mdl->stvert_data[tex_coord_num].s / mdl->header.texwidth;
    texcoord[tex_coord_num*TEXCOORD_ORDINATE_COUNT + 1] = (float)
      mdl->stvert_data[tex_coord_num].t / mdl->header.texheight;
  }

  numpixels = mdl->header.texwidth * mdl->header.texheight;
  model->texgroup =
    malloc(sizeof(jk_texgroup_struct) * model->num_texgroups);

  for(group_num = 0;
      group_num < mdl->header.num_texgroups;
      group_num++){

    model->texgroup[group_num].numtextures =
      mdl->texgroup[group_num].numtextures;

    model->texgroup[group_num].texture =
      malloc(sizeof(jk_texture_struct) * model->texgroup[group_num].numtextures);

    //allocate for interval data
    model->texgroup[group_num].interval =
      malloc(sizeof(GLfloat) * model->texgroup[group_num].numtextures);


    for(tex_num = 0;
	tex_num < mdl->texgroup[group_num].numtextures;
	tex_num++){

    glGenTextures(1, &model->texgroup[group_num].texture[tex_num].tex_name);

      model->texgroup[group_num].texture[tex_num].texcoord = texcoord;

      //fill in interval data
      model->texgroup[group_num].interval[tex_num] =
	mdl->texgroup[group_num].interval[tex_num];

      //set texture width & height
      model->texgroup[group_num].texture[tex_num].w =
	mdl->header.texwidth;
      
      model->texgroup[group_num].texture[tex_num].h =
	mdl->header.texheight;

      //allocate for the pixel data
      model->texgroup[group_num].texture[tex_num].pixel =
	malloc(sizeof(guint8) * numpixels * 4);

      //set the pixel data!
      for(pixel_num = 0; pixel_num < numpixels; pixel_num++){
	//RED
	model->texgroup[group_num].texture[tex_num].pixel[pixel_num*4 + 0] =
	  palette[(int) mdl->texgroup[group_num].texture[tex_num].pixel[pixel_num] * 3 + 0];
	//GREEN
	model->texgroup[group_num].texture[tex_num].pixel[pixel_num*4 + 1] =
	  palette[(int) mdl->texgroup[group_num].texture[tex_num].pixel[pixel_num] * 3 + 1];
	//BLUE
	model->texgroup[group_num].texture[tex_num].pixel[pixel_num*4 + 2] =
	  palette[(int) mdl->texgroup[group_num].texture[tex_num].pixel[pixel_num] * 3 + 2];
	//ALPHA
	model->texgroup[group_num].texture[tex_num].pixel[pixel_num * 4 + 3] = 255;
      }
    }
  }

  //assert the following...
  g_return_if_fail(texcoord == model->texgroup[0].texture[0].texcoord);

  model->index = malloc(sizeof(GLuint) * model->numverts * TRIANGLE_VERTEX_COUNT * 2);

  //the current index is the one after the last one, not including
  //the facesfront data
  cur_index = mdl->header.numverts;

  //here's our loop to create our index array & hack in support for
  //onseam/facesfront
  for(tri_num = 0; tri_num < mdl->header.numtris; tri_num++){
    if(mdl->triangle_data[tri_num].facesfront != 0){
      //If the model faces front, copy index straight across
      for(tri_vert_num = 0; tri_vert_num < TRIANGLE_VERTEX_COUNT; tri_vert_num++){

	model->index[ tri_num * TRIANGLE_VERTEX_COUNT + tri_vert_num ] = (GLuint)
	  mdl->triangle_data[tri_num].vertindex[tri_vert_num];
	
      }
    }else{
      for(tri_vert_num = 0;
	  tri_vert_num < TRIANGLE_VERTEX_COUNT;
	  tri_vert_num++){

	old_index = (GLuint)mdl->triangle_data[tri_num].vertindex[tri_vert_num];
	if(mdl->stvert_data[old_index].onseam){

	  //new data format

	  //for each pose of each phrase,
	  //copy the x,y,z ordinates to the end of the array
	  for(phrase_num = 0;
	      phrase_num < g_list_length(model->phrase);
	      phrase_num++
	      ){
	    phrase = (jk_phrase_struct *)
	      g_list_nth_data(model->phrase, phrase_num);

	    for(pose_num = 0;
		pose_num < g_list_length(phrase->pose);
		pose_num++
		){
	      pose = (jk_pose_struct *)
		g_list_nth_data(phrase->pose, pose_num);
	      
	      memcpy(&pose->vertex[cur_index * VERTEX_ORDINATE_COUNT],
		     &pose->vertex[old_index * VERTEX_ORDINATE_COUNT],
		     sizeof(GLfloat) * VERTEX_ORDINATE_COUNT
		     );
	      
	    }
	  }

	  //for each texcoord of each texture in each texgroup
	  //calculate the new ordinates and copy to the new index
	  //since we only have one real set of texcoords, we'll change those...

	  //s changes
 	  texcoord[cur_index * TEXCOORD_ORDINATE_COUNT + 0] = (GLfloat)
	    (mdl->stvert_data[old_index].s + (mdl->header.texwidth/2)) /
	    mdl->header.texwidth;

	  //t doesn't
 	  texcoord[cur_index * TEXCOORD_ORDINATE_COUNT + 1] = 
	    texcoord[old_index * TEXCOORD_ORDINATE_COUNT + 1];

	  //set the index to point to our new vertices
	  model->index[ tri_num * TRIANGLE_VERTEX_COUNT + tri_vert_num ] = cur_index;

	  //add 3 to the vert count so the new ones get displayed
	  model->numverts += TRIANGLE_VERTEX_COUNT;
	  cur_index++;
	}else{
	  model->index[ tri_num * TRIANGLE_VERTEX_COUNT + tri_vert_num ] = (GLuint)
	    mdl->triangle_data[tri_num].vertindex[tri_vert_num];

	}
      }
    }
  }

  model->cur_phrase  = model->phrase;
  model->cur_pose    = ((jk_phrase_struct *)model->cur_phrase->data)->pose;
  model->cur_texture = &model->texgroup[0].texture[0];
  
  scene->model          = g_list_prepend(scene->model, model);
  scene->selected_model = scene->model; 

  return;
}

/* this is mostly just for play ;) */
void export_targa(char *mdl_filename, char *palette_filename)
{
  char *name, *dot, *targa_name, *ext, *targa_path;
  char *texture_dir;
  mdl_struct *mdl = load_mdl(mdl_filename);

  //get the basename of the model file
  name = basename(mdl_filename);

  //set the targa output directory to the user's home dir. hah.
  texture_dir = getenv("HOME");

  targa_name = malloc(strlen(name) + 3);
  strcpy(targa_name, name);

  dot = strrchr(targa_name, '.');
  *dot = '\0';

  ext = "tga";

  targa_path = malloc(strlen(texture_dir) + 1 +
		      strlen("quake_texture-") + strlen(targa_name) +
		      strlen(ext) + 10); // this is 10 because it was segfaulting
  targa_path[0] = '\0';

  sprintf(targa_path, "%s/%s-%s.%s",
	  texture_dir,
	  "quake_texture", targa_name,
	  ext
	  );
	
  //write a targa of the image
  write_targa(mdl->header.texwidth,
	      mdl->header.texheight,
	      mdl->texgroup[0].texture[0].pixel,
	      palette_filename,
	      targa_path
	      );

  return;
}

void init_model(jk_model_struct *model)
{
  model->name          = NULL;
  model->phrase        = NULL;
  model->numverts      = 0;
  model->index         = NULL;
  model->texgroup      = NULL;
  model->num_texgroups = 0;
}

void init_phrase(jk_phrase_struct *phrase)
{
  phrase->name     = NULL;
  phrase->pose     = NULL;
  phrase->interval = 0.0;
}

void init_pose(jk_pose_struct *pose)
{
  pose->name   = NULL;
  pose->vertex = NULL;
  pose->normal = NULL;
}
