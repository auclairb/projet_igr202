#ifndef LIGHTTREE_H
#define LIGHTTREE_H
#include <vector>
#include <tuple>
#include <list>
#include "Light.h"
using namespace std;

typedef tuple<float,Light,Light> ftup;
typedef list<tuple<float,Light,Light>> ftuplist;
typedef list<tuple<Light, Light,Light>> ltuplist;

class LightTree{
 public:
  LightTree();
  virtual ~LightTree(){};
  void createLightTree(const list<Light> &);
  list<Light> vecToList(const vector<Light> &);
  vector<Light> listToVec(const list<Light> &);
  ftuplist createNeighboursTable(const vector<Light> &);
  Light createCluster(ftuplist, ltuplist &);
  
};
#endif
