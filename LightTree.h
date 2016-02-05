#ifndef LIGHTTREE_H
#define LIGHTTREE_H
#include <vector>
#include <tuple>
#include <list>
#include "Light.h"
using namespace std;

typedef tuple<float,float,float> ftup;
typedef list<tuple<float,float,float>> ftuplist;

class LightTree{
 public:
  void createLightTree(const vector<Light> & lightTable);
  ftuplist createNeighboursTable(const vector<Light> & lightTable);
  
};
#endif
