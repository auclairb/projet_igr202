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
  void createLightTree(const vector<Light> &);
  list<Light> listCopy(const vector<Light> &);
  ftuplist createNeighboursTable(const list<Light> &);
  Light createCluster(ftuplist, ltuplist &);
  
};
#endif
