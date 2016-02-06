// --------------------------------------------------------------------------
// Copyright(C) 2009-2015
// Tamy Boubekeur
//                                                                            
// All rights reserved.                                                       
//                                                                            
// This program is free software; you can redistribute it and/or modify       
// it under the terms of the GNU General Public License as published by       
// the Free Software Foundation; either version 2 of the License, or          
// (at your option) any later version.                                        
//                                                                            
// This program is distributed in the hope that it will be useful,            
// but WITHOUT ANY WARRANTY; without even the implied warranty of             
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              
// GNU General Public License (http://www.gnu.org/licenses/gpl.txt)           
// for more details.                                                          
// --------------------------------------------------------------------------

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

void Mesh::loadOFF (const std::string & filename) {
	ifstream in (filename.c_str ());
    if (!in) 
        exit (1);
	string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    V.resize (sizeV);
    T.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> V[i].p;
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> T[i].v[j];
    }
    in.close ();
    centerAndScaleToUnit ();
    recomputeNormals ();
}

void Mesh::loadOBJMesh (const std::vector<tinyobj::shape_t> & shapes, std::vector<unsigned int> & material_ids, std::vector<pairUV> & coord_uv) {
  int num_of_vertex = 0; //total number of vertices
  int num_of_faces = 0; //total number of faces
  
  for (unsigned int i = 0; i < shapes.size(); i++)
  {
    num_of_vertex += shapes[i].mesh.positions.size()/3;
    num_of_faces += shapes[i].mesh.indices.size()/3;
  }
  V.resize(num_of_vertex);
  T.resize(num_of_faces);
  material_ids.resize(num_of_faces);
  coord_uv.resize(num_of_faces*3);
  
  int total_vert = 0;
  int total_face = 0;
  for (unsigned int i = 0; i < shapes.size(); i++) //for each shape in the mesh
  {
    
    for (unsigned int j = 0; j < shapes[i].mesh.positions.size() / 3; j++) //get positions and separate into vertices
    {
      float x = shapes[i].mesh.positions[3*j+0];
      float y = shapes[i].mesh.positions[3*j+1];
      float z = shapes[i].mesh.positions[3*j+2];    
      Vec3f pos(x, y, z);

      V[j + total_vert].p = pos;
    }
    
    for (unsigned int t = 0; t < shapes[i].mesh.indices.size() / 3; t++) //get indexes and separate into faces
    {
      int v1 = shapes[i].mesh.indices[3*t+0] + total_vert;
      int v2 = shapes[i].mesh.indices[3*t+1] + total_vert;
      int v3 = shapes[i].mesh.indices[3*t+2] + total_vert;
      Triangle tri(v1, v2, v3);
      
      if (shapes[i].mesh.texcoords.size() > 0)
      {
	float u1 = shapes[i].mesh.texcoords[2*shapes[i].mesh.indices[3*t+0] + 0];
	float v1 = shapes[i].mesh.texcoords[2*shapes[i].mesh.indices[3*t+0] + 1];
	float u2 = shapes[i].mesh.texcoords[2*shapes[i].mesh.indices[3*t+1] + 0];
	float v2 = shapes[i].mesh.texcoords[2*shapes[i].mesh.indices[3*t+1] + 1];
	float u3 = shapes[i].mesh.texcoords[2*shapes[i].mesh.indices[3*t+2] + 0];
	float v3 = shapes[i].mesh.texcoords[2*shapes[i].mesh.indices[3*t+2] + 1];
	
	pairUV uv1, uv2, uv3;
	uv1.first = u1; uv1.second = v1;
	uv2.first = u2; uv2.second = v2;
	uv3.first = u3; uv3.second = v3;
	
	coord_uv[(t+total_face)*3 + 0] = uv1;
	coord_uv[(t+total_face)*3 + 1] = uv2;
	coord_uv[(t+total_face)*3 + 2] = uv3;
      }
      
      T[t + total_face] = tri;
      material_ids[t + total_face] = shapes[i].mesh.material_ids[t];
    }
    total_vert += shapes[i].mesh.positions.size() / 3;
    total_face += shapes[i].mesh.indices.size() / 3;
  }
  
  centerAndScaleToUnit();
  recomputeNormals();
  
}

void Mesh::recomputeNormals () {
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n = Vec3f (0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < T.size (); i++) {
        Vec3f e01 = V[T[i].v[1]].p -  V[T[i].v[0]].p;
        Vec3f e02 = V[T[i].v[2]].p -  V[T[i].v[0]].p;
        Vec3f n = cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            V[T[i].v[j]].n += n;
    }
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n.normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3f c;
    for  (unsigned int i = 0; i < V.size (); i++)
        c += V[i].p;
    c /= V.size ();
    float maxD = dist (V[0].p, c);
    for (unsigned int i = 0; i < V.size (); i++){
        float m = dist (V[i].p, c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < V.size (); i++)
        V[i].p = (V[i].p - c) / maxD;
}
