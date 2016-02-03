#include "Light.h"
#include <tuple>

using namespace std;

Light::Light(Vec3f _pos, float _intensity, Vec3f  _coneDir, Vec3f _coneAngle){
  this.pos = _pos;
  this.intensity = _intensity;
  this.dir = _coneDir;
  this.angle = _coneAngle;
}

void Light::createLightTree(const vector<Light> & lightTable){

  tuple<float, float, float> distTable;

  for(int index=0, vector<Light>::iterator it = lightTable.begin(); it != lightTable.end()-1; it ++, index ++){

    float min = -1.0f; //Supposed to be INFINITY WIP !!!
    float lightIndex = index;
    float distance = 0.0f;
    for(index2 = index, vector<Light>::iterator jt = it+1; jt != lightTable.end(); jt ++, index2++){
      //calcule distance de it.pos à jt.pos
      distance = dist(it->pos, jt->pos);
      //if (distance < min) min = distance & lightIndex = actualIndex
      if(distance < min)
	{
	  min = distance;
	  lightIndex = lightIndex;
	}
      }
    }
    //store min and lightIndex with itIndex in distTable for couple closest neighbors
  }
  //Here the distTable is complete

  for(element in distTable){
    //prendre minimum
    //clusteriser les 2 lumieres en une seule. (complexe ie trouver la + importante et virer l'autre)

    //comment stocker?
    //reiterer.
    
  }
}
