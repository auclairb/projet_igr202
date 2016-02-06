#include "Light.h"
#include "Vec3.h"

extern Light * lightTable;

using namespace std;

Light::Light(Vec3f _pos, float _intensity, Vec3f  _coneDir, Vec3f _coneAngle){
  this->pos = _pos;
  this->intensity = _intensity;
  this->dir = _coneDir;
  this->angle = _coneAngle;
}

Vec3f Light::getPos()const{
  return this->pos;
}
float Light::getIntensity() const{
  return this->intensity;
}

void Light::setIntensity(float _intensity){
  this->intensity = _intensity;
}

Vec3f Light::getDir() const{
  return this->dir;
}
Vec3f Light::getAngle() const{
  return this->angle;
}

bool Light::isEqual(Light light) const {
  if (this->getPos()==light.getPos()){
    if (this->getIntensity()==light.getIntensity()){
      if (this->getDir()==light.getDir()){
	if(this->getAngle()==light.getAngle()){
	  return true;
	}
      }
    }
  }
  return false;
}


