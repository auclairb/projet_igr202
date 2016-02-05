#ifndef RAY_H
#define RAY_H
#include "Vec3.h"
#include "Mesh.h"
class Ray {
 public:
  Vec3f origin;
  Vec3f direction;
  Ray(){};
  virtual ~Ray(){};
  Ray(Vec3f _origin,Vec3f _direction); 
  bool intersects(Vec3f,Vec3f,Vec3f);
  bool intersects(Mesh&);
};
#endif
