#include "Lightcut.h"
#include "Ray.h"
#include "Vec3.h"

using namespace std;
extern int * result;
//static int N; used for AO
typedef list<tuple<Light,Light,Light>> ltuplist;

void Lightcut::allIntersects(Mesh& mesh, const vector<Light> & lightTable){
  result = new int[mesh.V.size()];
  for (unsigned int i = 0; i < mesh.V.size (); i++){
    result[i]=0;
    cout << i << endl;        
    const Vertex & v = mesh.V[i];
    for(vector<Light>::const_iterator it = lightTable.begin(); it!=lightTable.end();it++){
      Vec3f wi = (it->getPos()-v.p);
      float d = wi.length();
      wi.normalize();
      Ray ray = Ray(v.p,wi);

      if(ray.intersects(mesh)){
	//because we don't know what are the coefficients we put 0,01
	result[i] += it->getIntensity()/(0.01+0.01*d+0.01*d*d);
      }
      else {
	result[i] = 0;
	//What is following was for AO
	/*Vec3f result(0.0f,0.0f,0.0f) ;
	  result = cartesianToPolar(v.n);
	  for (int i=0; i<N; i++){
	  float theta = result[1] + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/M_PI/2));
	  float phi = result[2] + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(2*M_PI)));
	  Vec3f randDir (1.0f,theta,phi);
	  randDir = polarToCartesian(randDir);
	  Ray randRay = Ray(v.p, randDir);
	  if(randRay.intersects(mesh)){
	  result[i] += 1/N;
	  }*/
      }
    }
  }
}


void Lightcut::buildLightcut(ltuplist & clusterTable){
  
}
