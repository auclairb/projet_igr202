#ifndef LIGHTTREE_H
#define LIGHTTREE_H
#include <vector>
#include <tuple>
#include <list>
#include "Light.h"
using namespace std;

typedef tuple<float,float,float> ftup;
typedef list<tuple<float,float,float>> ftuplist;
typedef list<tuple<Light, Light,Light>> ltuplist;

class LightTree{
 public:
  LightTree(){};
  virtual ~LightTree(){};
  ltuplist createLightTree(const vector<Light> &);
  list<Light> vecToList(const vector<Light> &);
  vector<Light> listToVec(const list<Light> &);
  ftuplist createNeighboursTable(const vector<Light> &);
  Light createCluster(ftuplist, ltuplist &, vector<Light> &);
  
};
#endif
