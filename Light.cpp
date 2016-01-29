#include "Light.h"

Light::Light(Vec3f _pos, float _intensity, Vec3f  _coneDir, Vec3f _coneAngle){
  this.pos = _pos;
  this.intensity = _intensity;
  this.dir = _coneDir;
  this.angle = _coneAngle;
}

void createLightTree(const vector<Light> & lightTable){
  {(float, float, float)...}  distTable;
  for(vector<Light>::iterator it = lightTable.begin(); it != lightTable.end()-1; it ++){
    float min = infinity;
    float lightIndex = lui_meme;
    for(vector<Light>::iterator jt = it+1; jt != lightTable.end(); jt ++){
      //calcule distance de it.pos à jt.pos

      //if (distance < min) min = distance & lightIndex = actualIndex
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
