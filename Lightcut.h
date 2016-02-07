#ifndef LIGHCUT_H
#define LIGHTCUT_H
#include "LightTree.h"
#include <tuple>
#include <list>
#include "Mesh.h"
using namespace std;
typedef list<tuple<Light,Light,Light>> ltuplist;

class Lightcut{

 public:
  Lightcut(){};
  virtual ~Lightcut(){};
  void allIntersects(Mesh& mesh,const vector<Light> &,int ** &);
  void buildLightcut(ltuplist &, Mesh & mesh, const vector<Light> &,float);
};
#endif
