#include "Lightcut.h"
#include "Ray.h"
#include "Vec3.h"

using namespace std;
extern int * * result;
extern int * test;
//static int N; used for AO
typedef list<tuple<Light,Light,Light>> ltuplist;

void Lightcut::allIntersects(Mesh& mesh, const vector<Light> & lightTable, int ** & result){
  result = new int*[mesh.V.size()];
  for (unsigned int i = 0; i < mesh.V.size (); i++){
    int index = 0;
    result[i]=new int[lightTable.size()];;
    cout << i << endl;        
    const Vertex & v = mesh.V[i];
    for(vector<Light>::const_iterator it = lightTable.begin(); it!=lightTable.end();it++,index++){
      Vec3f wi = (it->getPos()-v.p);
      wi.normalize();
      Ray ray = Ray(v.p,wi);

      if(ray.intersects(mesh)){
	(result[i])[index] = 1;
      }
      else {
	(result[i])[index] = 0;
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


void Lightcut::buildLightcut(ltuplist & clusterTable, Mesh& mesh, const vector<Light> & lightTable, float error){
  this->allIntersects(mesh, lightTable, result);
  ltuplist clusterTableCopy = clusterTable;
  int * radiance = new int[mesh.V.size()];
  float errorTest = 0;

  for(unsigned int k = 0; k<mesh.V.size();k++){
    radiance[k]=0;
    const Vertex & v = mesh.V[k];
    for(unsigned int j = 0; j<lightTable.size();j++){
      Light light = lightTable.at(j);
      Vec3f wi = (light.getPos()-v.p);
      float geometry = 1 / (wi.length())*(wi.length());
      float intensity = light.getIntensity();
      float visibility = (result[k])[j];
      radiance[k]+=geometry*intensity*visibility;
    }
  }

  vector<Light> cut;
  cut.push_back(get<0>(clusterTable.back()));

  for(unsigned int k = 0; k<mesh.V.size();k++){
    const Vertex & v = mesh.V[k];
    Light light = cut.back();
    Vec3f wi = (light.getPos()-v.p);
    int geometry = 1 / (wi.length())*(wi.length());
    int intensity = light.getIntensity();
    if(radiance[k]!=0){
      errorTest+=(radiance[k]-geometry*intensity)/radiance[k];
    }
  }
  
  while(errorTest>error){
    errorTest=0;
    //float errorBound = INFINITY;
    //float errorRec;
    Light son1;
    Light son2;
    
    for(vector<Light>::iterator it = cut.begin();it!=cut.end();it++){
      //errorRec=0;
      /*for(ltuplist::iterator jt = clusterTable.begin(); jt!=clusterTable.end();jt++){
	if((get<0>(jt)).isEqual(it)){

	  for(unsigned int k = 0; k<mesh.V.size();k++){
	    const Vertex & v = mesh.V[k];
	    Light light = cut.back();
	    Vec3f wi = (light.getPos()-v.p);
	    int geometry = 1 / (wi.length())*(wi.length());
	    int intensity = light.getIntensity();
	    if(radiance[k]!=0){
	      errorTest+=(radiance[k]-geometry*intensity)/radiance[k];
	    }

	  }
	}
	}*/
    }
  }
}
