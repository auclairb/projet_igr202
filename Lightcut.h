#ifndef LIGHCUT_H
#define LIGHTCUT_H
#include "LightTree.h"
#include <tuple>
#include <list>
using namespace std;
typedef list<tuple<Light,Light,Light>> ltuplist;

class Lightcut{

 public:
  Lightcut();
  virtual ~Lightcut(){};
  void buildLightcut(ltuplist & );
};
#endif
