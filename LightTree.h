#ifndef LIGHTTREE_H
#define LIGHTTREE_H
#include <vector>
#include <tuple>
#include "Light.h"
using namespace std;

typedef list<tuple<float,float,float>> ftuplist;

class LightTree{
 public:
  void createLightTree(const vector<Light> & lightTable);
  ftuplist createNeighboursTable(const vector<Light> & lightTable);
  
};
#endif
