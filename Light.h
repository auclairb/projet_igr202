#ifndef LIGHT_H
#define LIGHT_H
#include "Vec3.h"


class Light{
 public:
  Light();
  virtual ~Light();
  void createLightTree(const vector<Light> & lightTable);
 
 private:
  Vec3f pos;
  float intensity;
  Vec3f dir;
  float angle;
};
#endif
