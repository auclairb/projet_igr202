#ifndef LIGHTTREE_H
#define LIGHTTREE_H
#include <vector>
#include <tuple>
#include <list>
#include "Light.h"
using namespace std;

typedef tuple<float,Light,Light> ftup;
typedef list<tuple<float,Light,Light>> ftuplist;

class LightTree{
 public:
  LightTree();
  virtual ~LightTree(){};
  void createLightTree(const vector<Light> & lightTable);
  ftuplist createNeighboursTable(const vector<Light> & lightTable);
  
};
#endif
