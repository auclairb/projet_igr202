#ifndef LIGHT_H
#define LIGHT_H
#include "Vec3.h"
#include <vector>

using namespace std;


class Light{
 private:
  Vec3f pos;
  float intensity;
  Vec3f dir;
  Vec3f angle;

 public:
  Light();
  Light(Vec3f,float,Vec3f,Vec3f);
  virtual ~Light(){};
  Vec3f getPos()const;
  float getIntensity()const;
  void setIntensity(float);
  Vec3f getDir()const;
  Vec3f getAngle()const;
};
#endif
