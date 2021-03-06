// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Algorithmes de Rendu
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <GL/glut.h>

#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "Ray.h"
#include "Lightcut.h"
#include "Light.h"
#include "LightTree.h"
#include "tiny_obj_loader.h"

using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_TYPE ("off");
static const string DEFAULT_MESH_FILE ("models/monkey.off");

static string appTitle ("IGR202 Project : implementing Lightcuts");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;

static Camera camera;
static Mesh mesh;
//Adding new static variables
static Vec3f lightV0  (-10.0f, 0.0f, 0.0f);
static Vec3f lightV1  ( -5.0f, 0.0f, 0.0f);
static Vec3f lightV2  (  0.0f, 0.0f, 0.0f);
static Vec3f lightV3  (  9.0f, 2.0f, 0.0f);
static Vec3f nul      (  0.0f, 0.0f, 0.0f);

static Light light0 (lightV0, 20.0f, nul, nul, 0);
static Light light1 (lightV1, 1.0f, nul, nul, 1);
static Light light2 (lightV2, 10.0f, nul, nul, 2);
static Light light3 (lightV3, 12.0f, nul, nul, 3);

static const vector<Light> lightTable{light0,light1,light2,light3};

static Vec3f kd (0.9f,0.5f,0.1f);
//static float ks = 1.0f;
//static float s = 10.0f;
static float alpha = 0.1f;  //rugosité 0 =< alpha =< 1
static float F0 = 0.03f; //Terme de Fresnel [0.02, 0.05] plastique [0.91,0.92] alu
ltuplist * clusterTable = new ltuplist();
static LightTree * lightTree = new LightTree();
static Lightcut * lightcut = new Lightcut();
static vector< vector<Light> > allLightCuts;
static float cutsError = 0.05f;
static int** result;
static int ** cutPath;

//tinyobj variables
static std::string inputfile = "sibenik.obj";
static std::string texturefile = "kamen.bmp";
static std::vector<tinyobj::shape_t> shapes;
static std::vector<tinyobj::material_t> materials;
static std::vector<unsigned int> material_ids;
static std::vector<pairUV> cuv; //UV coordinates of each vertex of each triangle, separated as t1v1, t1v2, t1v3, t2v1, etc

GLuint loadImageCustom(const char * imagepath)
{
  // Data read from the header of the BMP file
  unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  unsigned int width, height;
  unsigned int imageSize;   // = width*height*3
  // Actual RGB data
  unsigned char * data;
  
  FILE * file = fopen(imagepath, "rb");
  if (!file){printf("Image could not be opened\n"); return 0;}
  
  if ( fread(header, 1, 54, file)!=54 ){
    printf("Not a correct BMP file\n");
    return false;
  }
  if ( header[0]!='B' || header[1]!='M' ){
    printf("Not a correct BMP file\n");
    return 0;
  }
  dataPos    = *(int*)&(header[0x0A]);
  imageSize  = *(int*)&(header[0x22]);
  width      = *(int*)&(header[0x12]);
  height     = *(int*)&(header[0x16]);
  
  if (imageSize==0)    imageSize=width*height*3;
  if (dataPos==0)      dataPos=54;
  
  data = new unsigned char [imageSize];
  fread(data,1,imageSize,file);
  fclose(file);
  
  GLuint textureID;
  glGenTextures(1, &textureID);
  
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  return textureID;
}

void printUsage () {
  std::cerr << std::endl
	    << "*******************************************************************************" << endl
	    << "**************** " << appTitle << " ****************" << std::endl
	    << "Authors: Tamy Boubekeur, Benjamin Auclair, Martin Chochod and Bernardo Cordeiro" << std::endl << std::endl
	    << "Usage: ./main [<file type = obj or off] [<file.obj> or <file.off>]" << std::endl
	    << "Commands:" << std::endl 
	    << "------------------" << std::endl
	    << " ?: Print help" << std::endl
	    << " w: Toggle wireframe mode" << std::endl
	    << " <drag>+<left button>: rotate model" << std::endl 
	    << " <drag>+<right button>: move model" << std::endl
	    << " <drag>+<middle button>: zoom" << std::endl
	    << " q, <esc>: Quit" << std::endl << std::endl; 
}



void init (const char * fileType, const char * modelFilename) {
  glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
  glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
  glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
  glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    
  glEnable(GL_COLOR_MATERIAL);
  glEnable( GL_TEXTURE_2D );
    
  if(strcmp(fileType,"obj")==0){
    cout << endl << "--- OBJ Mesh Type selected --- " << endl << endl;
    //charge l'objet
    std::string err; 
    tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());
    
    if (!err.empty()) {
      std::cerr << err << std::endl;
      exit(1);
    }

    //OLD = mesh.loadOFF(modelFilename)
    mesh.loadOBJMesh (shapes, material_ids, cuv);
    //charge l'objet
    
    //loads texture
    loadImageCustom(texturefile.c_str());
    //loads texture
  }
  else if(strcmp(fileType,"off")==0){
    cout<< endl << "--- OFF Mesh Type selected ---" << endl << endl;
    mesh.loadOFF(modelFilename);
  }
  else{
    cerr << endl << "--- WARNING : neither OBJ nor OFF type triggered ! ---" << endl << endl;
  }
  camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
  //Build light Tree/cluster table
  result = new int*[mesh.V.size()];
  cutPath = new int*[mesh.V.size()];
  
  *clusterTable = lightTree->createLightTree(lightTable);
  lightcut->allIntersects(mesh,lightTable,result);
  
  for(unsigned int k = 0; k<mesh.V.size();k++){
    cutPath[k] = new int[lightTable.size()];
    cutPath[k] = lightcut->buildLightcut(*clusterTable,mesh,lightTable,cutsError,k,result);
  }
}
 



void drawScene () {
  glBegin (GL_TRIANGLES);
  for (unsigned int i = 0; i < mesh.T.size (); i++) 
    {
      //material configurations
      //Check if OBJ mode. If empty, it is due to OFF mode
      if (materials.size() > 0)
	{
	  int mat_id = material_ids[i];
	  GLfloat amb[4] = {materials[mat_id].ambient[0], materials[mat_id].ambient[1], materials[mat_id].ambient[2], 1.f };
	  GLfloat diff[4] = {materials[mat_id].diffuse[0], materials[mat_id].diffuse[1], materials[mat_id].diffuse[2], 1.f };
	  GLfloat spec[4] = {materials[mat_id].specular[0], materials[mat_id].specular[1], materials[mat_id].specular[2], 1.f };
	  GLfloat emi[4] = {materials[mat_id].emission[0], materials[mat_id].emission[1], materials[mat_id].emission[2], 1.f };
	  GLfloat shin[1] = {materials[mat_id].shininess};
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emi);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin);
	}
      for (unsigned int j = 0; j < 3; j++) {
	const Vertex & v = mesh.V[mesh.T[i].v[j]];
            
	// EXERCISE : the following color response shall be replaced with a proper reflectance evaluation/shadow test/etc.
	Vec3f total_radiance = Vec3f(0.0f);
	float w0f[3] = {0.0f,0.0f,0.0f};
	camera.getPos(w0f[0],w0f[1],w0f[2]);
	Vec3f w0(w0f[0],w0f[1],w0f[2]);
	w0 = w0 - v.p;
	w0.normalize();
	Ray ray = Ray(v.p,w0);

	for (unsigned int lightInd = 0; lightInd < lightTable.size(); lightInd++)
	  {
	    if(result[mesh.T[i].v[j]][lightInd] ){ //Uses a int** where the index in the second array corresponds to the index of the light in lightTable
	      glColor3f (0.0f, 0.0f, 0.0f);
	    }
	    else {
	      Vec3f wi = (lightTable[lightInd].getPos()-v.p);
	      wi.normalize();
	      Vec3f wH = w0 + wi;
	      wH.normalize();  
	      float fs = 0.0f;
			  
	      //Implementing GGX
	      float Dx = pow(alpha,2) / (M_PI * pow(1+(pow(alpha,2)-1)*pow(dot(v.n,wH),2),2));
	      float Fx = F0 + (1 - F0) * pow(1 - max(0.0f,dot(wi,wH)),5);
	      float Gi = 2*dot(v.n,wi) / (  dot(v.n,wi) + sqrt( pow(alpha,2) + (1 - pow(alpha,2))*pow(dot(v.n,wi),2) )  );
	      float G0 = 2*dot(v.n,w0) / (  dot(v.n,w0) + sqrt( pow(alpha,2) + (1 - pow(alpha,2))*pow(dot(v.n,w0),2) )  );
	      float Gx = Gi*G0;
	      fs = (Dx*Fx*Gx)/ (4*dot(v.n,wi)*dot(v.n,w0));
			  
	      //Resulting
	      total_radiance +=  max(0.0f,dot(wi,v.n))*(kd/M_PI + fs );
	    }
	  }
	glColor3f (total_radiance[0], total_radiance[1], total_radiance[2]);
	//Check if OBJ mode. If cuv is empty, it is due to OFF mode
	if(cuv.size()>0){
	  glTexCoord2d(cuv[3*i + j].first,cuv[3*i + j].second); //Specifies texture coordinates to be used
	}
	glNormal3f (v.n[0], v.n[1], v.n[2]); // Specifies current normal vertex   
	glVertex3f (v.p[0], v.p[1], v.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
      }
    }
  glEnd (); 
}

void reshape(int w, int h) {
  camera.resize (w, h);
}

void display () {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera.apply (); 
  drawScene ();
  glFlush ();
  glutSwapBuffers (); 
}

void key (unsigned char keyPressed, int x, int y) {
  switch (keyPressed) {
  case 'f':
    if (fullScreen) {
      glutReshapeWindow (camera.getScreenWidth (), camera.getScreenHeight ());
      fullScreen = false;
    } else {
      glutFullScreen ();
      fullScreen = true;
    }      
    break;
  case 'q':
  case 27:
    exit (0);
    break;
  case 'w':
    GLint mode[2];
    glGetIntegerv (GL_POLYGON_MODE, mode);
    glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
    break;
    break;
  default:
    printUsage ();
    break;
  }
}

void mouse (int button, int state, int x, int y) {
  camera.handleMouseClickEvent (button, state, x, y);
}

void motion (int x, int y) {
  camera.handleMouseMoveEvent (x, y);
}

void idle () {
  static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
  static unsigned int counter = 0;
  counter++;
  float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
  if (currentTime - lastTime >= 1000.0f) {
    FPS = counter;
    counter = 0;
    static char winTitle [128];
    unsigned int numOfTriangles = mesh.T.size ();
    sprintf (winTitle, "Number Of Triangles: %d - FPS: %d", numOfTriangles, FPS);
    glutSetWindowTitle (winTitle);
    lastTime = currentTime;
  }
  glutPostRedisplay (); 
}

int main (int argc, char ** argv) {
  if (argc > 3) {
    printUsage ();
    exit (1);
  }
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
  window = glutCreateWindow (appTitle.c_str ());
  //Changing init input to enable both OFF and TINYOBJ renderings

  if(argc !=3){
    cerr << "WARNING : call structure has changed as follow :" << endl;
    cerr << "main [mesh_type] [filename]" << endl;
    init(DEFAULT_MESH_TYPE.c_str(), DEFAULT_MESH_FILE.c_str ());
  }
  else{
    init(argv[1],argv[2]);
  }
  glutIdleFunc (idle);
  glutReshapeFunc (reshape);
  glutDisplayFunc (display);
  glutKeyboardFunc (key);
  glutMotionFunc (motion);
  glutMouseFunc (mouse);
  printUsage ();  
  glutMainLoop ();
  return 0;
}

