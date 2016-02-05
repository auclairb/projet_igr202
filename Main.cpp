// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Algorithmes de Rendu
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <GL/glut.h>

#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "Ray.h"

using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_FILE ("models/man.off");

static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Algorithmes de Rendu");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;

static Camera camera;
static Mesh mesh;
//Adding new static variables
static Vec3f lightPos(5.0f,0.0f,0.0f);
static Vec3f kd (0.9f,0.5f,0.1f);
//static float ks = 1.0f;
//static float s = 10.0f;
static float alpha = 0.1f;  //rugosité 0 =< alpha =< 1
static float F0 = 0.03f; //Terme de Fresnel [0.02, 0.05] plastique [0.91,0.92] alu
static	int* result;
static int N = 0;


void printUsage () {
	std::cerr << std::endl 
		 << appTitle << std::endl
         << "Author: Tamy Boubekeur" << std::endl << std::endl
         << "Usage: ./main [<file.off>]" << std::endl
         << "Commands:" << std::endl 
         << "------------------" << std::endl
         << " ?: Print help" << std::endl
		 << " w: Toggle wireframe mode" << std::endl
         << " <drag>+<left button>: rotate model" << std::endl 
         << " <drag>+<right button>: move model" << std::endl
         << " <drag>+<middle button>: zoom" << std::endl
         << " q, <esc>: Quit" << std::endl << std::endl; 
}

void allIntersects(Mesh& mesh){
  result = new int[mesh.V.size()];
  for (unsigned int i = 0; i < mesh.V.size (); i++)  {
    cout << i << endl;        
    const Vertex & v = mesh.V[i];
    Vec3f wi = (lightPos-v.p);
    wi.normalize();
    Ray ray = Ray(v.p,wi);

    if(ray.intersects(mesh)){
      result[i] = 1;
    }
    else {
      result[i] = 0;
       Vec3f result(0.0f,0.0f,0.0f) ;
      result = cartesianToPolar(v.n);
      for (int i=0; i<N; i++){
	float theta = result[1] + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/M_PI/2));
	float phi = result[2] + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(2*M_PI)));
	Vec3f randDir (1.0f,theta,phi);
	randDir = polarToCartesian(randDir);
	Ray randRay = Ray(v.p, randDir);
	if(randRay.intersects(mesh)){
	  result[i] += 1/N;
	}
      }
    }
  }
}

void init (const char * modelFilename) {
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
    glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    mesh.loadOFF(modelFilename);
    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);   
    allIntersects(mesh);
}
 



void drawScene () {
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < mesh.T.size (); i++) 
        for (unsigned int j = 0; j < 3; j++) {
            const Vertex & v = mesh.V[mesh.T[i].v[j]];
            
            // EXERCISE : the following color response shall be replaced with a proper reflectance evaluation/shadow test/etc.
            float w0f[3] = {0.0f,0.0f,0.0f};
            camera.getPos(w0f[0],w0f[1],w0f[2]);
            Vec3f w0(w0f[0],w0f[1],w0f[2]);
            w0 = w0 - v.p;
            w0.normalize();
            Ray ray = Ray(v.p,w0);
            
            if(result[mesh.T[i].v[j]] ){
                glColor3f (0.0f, 0.0f, 0.0f);
            }
            else {
                Vec3f wi = (lightPos-v.p);
                wi.normalize();
                Vec3f wH = w0 + wi;
                wH.normalize();  
                float fs = 0.0f;
                
                // Implementing Lambert
                //float color = max(0.0f,dot(wi,v.n))*(kd/M_PI);
                
                //Implementing Blinn-Phong
                //fs = pow( ks*dot(wH,v.n), s);
			
		//Implementing Cook-Torrance
		/*
		  float expTerm = (pow(dot(v.n,wH),2) - 1) / (pow(alpha,2)*pow(dot(v.n,wH),2));
		  float inv = M_PI*pow(alpha,2)*pow(dot(v.n,wH),4); 
		  float D = 1/(inv)*exp(expTerm); 
		  float F = F0 + (1 - F0) * pow(1 - max(0.0f,dot(wi,wH)),5);
		  float gShadow = (2*dot(v.n,wH)*dot(v.n,wi)) / dot(w0,wH);
		  float gMask =   (2*dot(v.n,wH)*dot(v.n,w0)) / dot(w0,wH);
		  float G = min(1.0f,gShadow);
		  G = min(G,gMask);
		  fs = (D*F*G)/ (4*dot(v.n,wi)*dot(v.n,w0));
		*/
			
		//Implementing GGX
		float Dx = pow(alpha,2) / (M_PI * pow(1+(pow(alpha,2)-1)*pow(dot(v.n,wH),2),2));
		float Fx = F0 + (1 - F0) * pow(1 - max(0.0f,dot(wi,wH)),5);
		float Gi = 2*dot(v.n,wi) / (  dot(v.n,wi) + sqrt( pow(alpha,2) + (1 - pow(alpha,2))*pow(dot(v.n,wi),2) )  );
		float G0 = 2*dot(v.n,w0) / (  dot(v.n,w0) + sqrt( pow(alpha,2) + (1 - pow(alpha,2))*pow(dot(v.n,w0),2) )  );
		float Gx = Gi*G0;
		fs = (Dx*Fx*Gx)/ (4*dot(v.n,wi)*dot(v.n,w0));
			
		//Resulting
		Vec3f fsVec(fs);
		Vec3f color = max(0.0f,dot(wi,v.n))*(kd/M_PI + fs );
		glColor3f (color[0], color[1], color[2]);
            }
                        
            glNormal3f (v.n[0], v.n[1], v.n[2]); // Specifies current normal vertex   
            glVertex3f (v.p[0], v.p[1], v.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
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
    if (argc > 2) {
        printUsage ();
        exit (1);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
    window = glutCreateWindow (appTitle.c_str ());
    init (argc == 2 ? argv[1] : DEFAULT_MESH_FILE.c_str ());
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

