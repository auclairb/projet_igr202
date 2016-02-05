#include "LightTree.h"
#include <stdlib.h>
#include <list>

using namespace std;
typedef list<tuple<Light,Light,Light>> ltuplist;
typedef tuple<float,Light,Light> ftup;
typedef list<tuple<float,Light,Light>> ftuplist;

///Function that turns a vector into a list
list<Light> LightTree::vecToList(const vector<Light> & vec){
  list<Light> list;
  for(vector<Light>::const_iterator it = vec.begin(); it != vec.end(); it++){
    list.push_back(*it);
  }
  return list;

}

///Function that turns a list into a vector
vector<Light> LightTree::listToVec(const list<Light> & liste){

  vector<Light> vec;
  for(list<Light>::const_iterator it = liste.begin(); it != liste.end(); it++){
    vec.push_back(*it);
  }
  return vec;

}


///Function that builds a light Tree
void LightTree::createLightTree(const list<Light> & lightTable){ 
  /*
  list<Light> lightTable1 = lightTable;
  //get Neighbours table
  ftuplist distTable = createNeighboursTable(lightTable1);
  ltuplist clusterTable;

  while (!distTable.empty()){
    Light cluster = createCluster(distTable, clusterTable);
    const Light lightOne = get<1>(distTable.front());
    const Light lightTwo = get<2>(distTable.front());
    lightTable1.remove(lightOne);
    lightTable1.remove(lightTwo);
    lightTable1.push_back(cluster);
    distTable = createNeighboursTable(lightTable1);
    }*/
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

Light LightTree::createCluster(ftuplist disTable, ltuplist & clusterTable){
  
  Light lightOne = get<1>(disTable.front());
  Light lightTwo = get<2>(disTable.front());
  Light cluster;

  float intensityOne = lightOne.getIntensity();
  float intensityTwo = lightTwo.getIntensity();
  float intensity = intensityOne+intensityTwo;

  float probOne = intensityOne / intensity;

  float prob = (rand() % 1000)/1000;

  if (prob <= probOne){
    cluster = lightOne;
  } else {
    cluster = lightTwo;
  }

  cluster.setIntensity(intensity);

  tuple<Light,Light,Light> clusterOne = make_tuple(cluster,lightOne,lightTwo) ;
  clusterTable.push_back(clusterOne);
  return cluster;
}
