#include "Light.h"

Light::Light(Vec3f _pos, float _intensity, Vec3f  _coneDir, Vec3f _coneAngle){
  this.pos = _pos;
  this.intensity = _intensity;
  this.dir = _coneDir;
  this.angle = _coneAngle;
}
