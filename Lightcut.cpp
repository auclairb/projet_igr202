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
    result[i]=new int[lightTable.size()];        
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
  static int lightCutCount = 1;
  cout << "Entering buildLightcut : " << lightCutCount << "th time" << endl;
  lightCutCount++;
  int * radiance = new int[mesh.V.size()];
  int * radianceTest = new int[mesh.V.size()];
  float errorTest = 0.0f;

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
    radianceTest[k]=0;
    const Vertex & v = mesh.V[k];
    Light light = cut.back();
    Vec3f wi = (light.getPos()-v.p);
    float geometry = 1 / (wi.length())*(wi.length());
    float intensity = light.getIntensity();
    float visibility = (result[k])[(cut.at(0)).getIndex()];
    radianceTest[k]+=geometry*intensity*visibility;
    if(radiance[k]!=0){
      errorTest+=abs(radiance[k]-radianceTest[k])/radiance[k];
    }
  }
  cout << "errorTest = " << errorTest << endl;
  cout << "Contenu de cut:" << (cut.front()).getIndex()<<endl;
  
  while(errorTest>error && cut.size()!= lightTable.size()){
    cout <<"Contenu de cut :"<<endl;
    for(unsigned int k =0;k<cut.size();k++){
      cout << (cut.at(k)).getIndex()<<endl;
    }
    
    float errorBound = INFINITY;
    float errorRec = 0.0f;
    Light root;
    Light son1;
    Light son2;
    for(vector<Light>::iterator it = cut.begin();it!=cut.end();it++){
      errorRec=0;
      cout <<"taille de la clusterTable :"<< clusterTable.size()<<endl;
      for(ltuplist::iterator jt = clusterTable.begin(); jt!=clusterTable.end();jt++){
	
	Light R, S1, S2;
	if((get<0>(*jt)).isEqual(*it) && (*it).getIntensity() == (get<1>(*jt)).getIntensity() +(get<2>(*jt)).getIntensity() ){
	  for(unsigned int k = 0; k<mesh.V.size();k++){
	    const Vertex & v = mesh.V[k];
	    R = *it;
	    S1 = get<1>(*jt);
	    S2 = get<2>(*jt);

	    Vec3f wiR = (R.getPos()-v.p);
	    Vec3f wiS1 = (S1.getPos()-v.p);
	    Vec3f wiS2 = (S1.getPos()-v.p);

	    float geometryR = 1 / (wiR.length())*(wiR.length());
	    float intensityR = R.getIntensity();
	    float visibilityR = (result[k])[R.getIndex()];

	    float geometryS1 = 1 / (wiS1.length())*(wiS1.length());
	    float intensityS1 = S1.getIntensity();
	    float visibilityS1 = (result[k])[S1.getIndex()];

	    float geometryS2 = 1 / (wiS2.length())*(wiS2.length());
	    float intensityS2 = S2.getIntensity();
	    float visibilityS2 = (result[k])[S2.getIndex()];

	    if(visibilityS2 == 0 && visibilityS1 != 0){
	      errorRec+=abs(geometryS1*intensityS1*visibilityS1-geometryR*intensityR*visibilityR)/(geometryS1*intensityS1*visibilityS1);
	    } else if (visibilityS1 == 0 && visibilityS2 != 0){
	      errorRec+=abs(geometryS2*intensityS2*visibilityS2-geometryR*intensityR*visibilityR)/(geometryS2*intensityS2*visibilityS2);
	    } else if (visibilityS1 != 0 && visibilityS2 != 0) {
	      errorRec+=abs(geometryS1*intensityS1*visibilityS1-geometryR*intensityR*visibilityR)/(geometryS1*intensityS1*visibilityS1) + abs(geometryS2*intensityS2*visibilityS2-geometryR*intensityR*visibilityR)/(geometryS2*intensityS2*visibilityS2);
	    }
	  }
	  cout <<"erreur entre fils et racine :"<<errorRec<<endl;

	  if (errorRec<errorBound){
	    root = R;
	    son1 = S1;
	    son2= S2;
	    errorBound = errorRec;
	  }
	  cout << "valeur errorBound :"<<errorBound<<endl;
	}
      }
    }
    for(vector<Light>::iterator it = cut.begin();it!=cut.end();it++){
      if(root.isEqual(*it)){
	cut.erase(it);
	it--;
      }
    }
    cut.push_back(son1);
    cut.push_back(son2);
    errorTest=0;
    
    for(unsigned int k = 0; k<mesh.V.size();k++){
      const Vertex & v = mesh.V[k];
      radianceTest[k]=0;
      for(vector<Light>::iterator it = cut.begin();it !=cut.end();it++){
	Light light = *it;
	Vec3f wi = (light.getPos()-v.p);
	float geometry = 1 / (wi.length())*(wi.length());
	float intensity = light.getIntensity();
	float visibility = (result[k])[light.getIndex()];
	radianceTest[k] += geometry*intensity*visibility; 
      }
      
      if(radiance[k]!=0){
	errorTest += abs(radiance[k]-radianceTest[k])/radiance[k];
      }
    }

    cout << "erreurTest :"<<errorTest<<endl;
    
  }

    
  for(vector<Light>::iterator it = cut.begin(); it != cut.end(); it++){
    cout << "*** Light " << (*it).getIndex() << " ***"<<endl;
    cout << "Pos       : " << (*it).getPos() << endl; 
    cout << "Intensity : " << (*it).getIntensity() << endl;
    cout << "Dir       : " << (*it).getDir() << endl;
    cout << "Angle     : " << (*it).getAngle() << endl;
  }
}
