#include "LightTree.h"

void createLightTree(const vector<Light> & lightTable){
  // (distance, closestNeighbour1Index, closestNeighbour2Index)
  vector<tuple<float,float,float>> distTable;
  int index = 0;
  for(vector<Light>::const_iterator it = lightTable.begin(); it != lightTable.end()-1; it ++, index ++){

    //Supposed to be INFINITY WIP !!
    float min = INFINITY;
    float lightIndex = index;
    float distance = 0.0f;
    int index2 = index;
    for(vector<Light>::const_iterator jt = it+1; jt != lightTable.end(); jt ++, index2++){
      //calcule distance de it.pos à jt.pos
      distance = dist(it->pos, jt->pos);
      //if (distance < min) min = distance & lightIndex = actualIndex
      if(distance < min)
	{
	  min = distance;
	  lightIndex = index2;
	}
      }
//store min and lightIndex with itIndex in distTable for couple closest neighbors
    distTable.push_back(make_tuple(min,index,lightIndex));
    }    

  //Here the distTable is complete
  int min = INFINITY;
  for(vector<tuple<float,float,float>>::iterator it = distTable.begin();it != distTable.end(); it++){
    //prendre minimum
    
    //clusteriser les 2 lumieres en une seule. (complexe ie trouver la + importante et virer l'autre)

    //comment stocker?
    //reiterer.
    
  }
}
