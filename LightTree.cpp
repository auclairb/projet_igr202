#include "LightTree.h"
#include <list>

using namespace std;

void LightTree::createLightTree(const vector<Light> & lightTable){
  // (distance, closestNeighbour1Index, closestNeighbour2Index)
      
  ftuplist distTable = createNeighboursTable(lightTable);
  //Here the distTable is complete
  float min = INFINITY;
  for(ftuplist::iterator it = distTable.begin();it != distTable.end(); it++){
    //prendre minimum
    
    //clusteriser les 2 lumieres en une seule. (complexe ie trouver la + importante et virer l'autre)

    //comment stocker?
    //reiterer.
    
  }
}

ftuplist LightTree::createNeighboursTable(const vector<Light> & lightTable){
  ftuplist distTable;
  int index = 0;
  for(vector<Light>::const_iterator it = lightTable.begin(); it != lightTable.end()-1; it ++, index ++){

    //Supposed to be INFINITY WIP !!
    float min = INFINITY;
    float lightIndex = index;
    float distance = 0.0f;
    int index2 = 0;
    for(vector<Light>::const_iterator jt = lightTable.begin(); jt != lightTable.end(); jt ++, index2++){
      if(dist(it->getPos(), jt->getPos())==0){
	continue;
      }
      //calcule distance de it.pos à jt.pos
      distance = dist(it->getPos(), jt->getPos());
      //if (distance < min) min = distance & lightIndex = actualIndex
      if(distance < min && distance != 0)
	{
	  min = distance;
	  lightIndex = index2;
	}
      }
//store min and lightIndex with itIndex in distTable for couple closest neighbors
    distTable.push_back(make_tuple(min,index,lightIndex));
    }

  thelist.sort([](const ipair & a, const ipair & b) { return a.first < b.first; });
}
