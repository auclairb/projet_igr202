#include "LightTree.h"

using namespace std;

typedef tuple<float,Light,Light> ftup;
typedef list<tuple<float,Light,Light>> ftuplist;

///Function that builds a light Tree
void LightTree::createLightTree(const vector<Light> & lightTable){

  //get Neighbours table
  ftuplist distTable = createNeighboursTable(lightTable);

  for(ftuplist::iterator it = distTable.begin();it != distTable.end(); it++){
    //prendre minimum
    
    //clusteriser les 2 lumieres en une seule. (complexe ie trouver la + importante et virer l'autre)

    //comment stocker?
    //reiterer.
    
  }
}

///Function that builds the closest neighbours list<tuple<float,float,float>> table
ftuplist LightTree::createNeighboursTable(const vector<Light> & lightTable){
  //Output table
  ftuplist distTable;
  int index = 0;
  //Search the whole table
  for(vector<Light>::const_iterator it = lightTable.begin(); it != lightTable.end()-1; it ++, index ++){
    float min = INFINITY;
    int closestNeighbour = index;
    float distance = 0.0f;
    int index2 = 0;
    //Compare each Light to all other Lights
    for(vector<Light>::const_iterator jt = lightTable.begin(); jt != lightTable.end(); jt ++, index2++){
      //If itself, ignore
      if(dist(it->getPos(), jt->getPos())==0){
	continue;
      }
      //Compute distance
      distance = dist(it->getPos(), jt->getPos());
      //Store it as closest neighbour if conditions are met
      if(distance < min && distance >0)
	{
	  min = distance;
	  closestNeighbour = index2;
	}
      }
    //Store (distanceValue, Light1, Light2) as closest neighbours
    distTable.push_back(make_tuple(min,lightTable.at(index),lightTable.at(closestNeighbour)));
    }

  distTable.sort([](const ftup & a, const ftup & b) { return get<0>(a) < get<0>(b); });
  return distTable;
}
