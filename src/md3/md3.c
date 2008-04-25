#include "md3.h"

md3_struct *
load_md3(const char *filename)
{
  md3_struct *md3;
  FILE *f;
  size_t num_tags;
  int i;
  md3_mesh_struct *mesh;
  f = fopen(filename, "r");

  //load the header
  fread( &(md3->header), sizeof(md3->header), 1, f);

  //load the tags
  num_tags = md3->header.BoneFrame_num * md3->header.Tag_num;
  md3->tag = (md3_tag *) malloc(sizeof(md3_tag) * num_tags);
  fread( md3->tag, sizeof(md3_tag), num_tags, f);

  //load the bone frames
  md3->boneframe = (md3_boneframe *) malloc(sizeof(md3_boneframe) * md3->header.BoneFrame_num);
  fread( md3->tag, sizeof(md3_boneframe), md3->header.BoneFrame_num, f);

  md3->mesh = (md3_mesh_struct *) malloc(sizeof(md3_mesh_struct) * md3->header.Mesh_num);

  printf("number of meshes: %i\n", md3->header.Mesh_num);
  printf("md3 file name: '%s'\n", md3->header.FileName);
 

  for(i = 0; i < md3->header.Mesh_num; i++){
    mesh = &(md3->mesh[i]);

    //load mesh header
    fread( mesh, sizeof(md3_mesh_header), 1, f);

    //load mesh skin names
    mesh->skin_name = (char *) malloc(sizeof(char[68]) *
				      mesh->header.Skin_num
				      );
    fread(mesh->skin_name, sizeof(char[68]), mesh->header.Skin_num, f);

    //load mesh triangles
    mesh->triangle = (int *) malloc(sizeof(int[3]) * mesh->header.Triangle_num);

    fread(mesh->skin_name, sizeof(int[3]), mesh->header.Triangle_num, f);

    //load texture U/V Coordinates

    mesh->tex_vec = (float *) malloc(sizeof(float[2]) * mesh->header.Vertex_num);
    fread(mesh->tex_vec, sizeof(float[2]), mesh->header.Vertex_num, f);

    //load vertices
    mesh->vertex = (md3_mesh_vertex *) malloc(sizeof(md3_mesh_vertex) *
					      mesh->header.Vertex_num *
					      mesh->header.MeshFrame_num
					      );
    fread(mesh->vertex, sizeof(md3_mesh_vertex), mesh->header.Vertex_num * mesh->header.MeshFrame_num, f);
  }

  return md3;
}
