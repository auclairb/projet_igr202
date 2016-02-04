#include "Light.h"
#include "Vec3.h"
#include <tuple>

extern Light * lightTable;

using namespace std;

Light::Light(Vec3f _pos, float _intensity, Vec3f  _coneDir, Vec3f _coneAngle){
  this->pos = _pos;
  this->intensity = _intensity;
  this->dir = _coneDir;
  this->angle = _coneAngle;
}

Vec3f getPos(){
  return this->pos;
}
float getIntensity(){
  return this->intensity;
}
Vec3f getDir(){
  return this->dir;
}
Vec3f getAngle(){
  return this->angle;
}


