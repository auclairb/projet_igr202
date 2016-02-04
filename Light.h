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
  Light(Vec3f,float,Vec3f,Vec3f);
  virtual ~Light();
  Vec3f getPos();
  float getIntensity();
  Vec3f getDir();
  Vec3f getAngle();
  //void createLightTree(const  vector<Light>  & lightTable);
  
};
#endif
