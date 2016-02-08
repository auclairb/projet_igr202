#include "LightTree.h"
#include <stdlib.h>
#include <list>

using namespace std;
typedef list<tuple<Light,Light,Light>> ltuplist;
typedef tuple<float,float,float> ftup;
typedef list<tuple<float,float,float>> ftuplist;

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
ltuplist LightTree::createLightTree(const vector<Light> & lightTable){
  cout << "Entering with caution"<<endl;
  vector<Light> lightTable1 = lightTable;
  //get Neighbours table
  ftuplist distTable = createNeighboursTable(lightTable1);
  ltuplist clusterTable={};
  
  while (!distTable.empty()){
    cout << "ENTER WHILE" << endl;
    Light cluster = createCluster(distTable, clusterTable,lightTable1);
    cout << "Bah ou est l'erreur"<<endl;
    int lightOne = get<1>(distTable.front());
    cout << " EUH?"<<endl;
    
    int lightTwo = get<2>(distTable.front());
    cout << "???"<<endl;
    
    cout << "le cluster a le numero :"<<cluster.getIndex() <<endl;
    for(vector<Light>::iterator it = lightTable1.begin(); it != lightTable1.end(); it ++){
      if((*it).getIndex()==lightOne){
	cout << "LIgt ONE ERASING" << endl;
	lightTable1.erase(it);
	cout << "LIGHT ONE ERASED" << endl;
	// it-- in order to cancel the effect of erase auto increment
	it--;
      }
      if((*it).getIndex()==lightTwo){
	cout << "lightTwo erasing : "  << endl;
	lightTable1.erase(it);
	cout << "lightTwo erased" << endl;
	it--;
      }
    }
    lightTable1.push_back(cluster);
    cout << "Waiting for NE TABLE" << endl;
    distTable = createNeighboursTable(lightTable1);
    cout << " NE TABLE gotten" << endl;
    }
    cout << "Cluster table"<<endl;
    for(ltuplist::iterator it = clusterTable.begin(); it != clusterTable.end(); it++){
      cout << "*** Cluster n°" << (get<0>(*it)).getIndex() << " ***"<<endl;
      cout << "son 1     :" << (get<1>(*it)).getIndex() << endl;
      cout << "son 2     :" << (get<2>(*it)).getIndex() << endl;
      cout << "Posé      : " << (get<0>(*it)).getPos() << endl; 
      cout << "Intensity : " << (get<0>(*it)).getIntensity() << endl;
      cout << "Dir       : " << (get<0>(*it)).getDir() << endl;
      cout << "Angle     : " << (get<0>(*it)).getAngle() << endl;
     
    }
    cout <<endl;
  return clusterTable;
}

///Function that builds the closest neighbours list<tuple<float,float,float>> table
ftuplist LightTree::createNeighboursTable(const vector<Light> & lightTable){
  static int compteur =0;
  compteur ++;
  cout <<"oh c'est ma " << compteur << " fois !!" <<endl;
  //Output table
  ftuplist distTable = {};
  //Search the whole table
  for(vector<Light>::const_iterator it = lightTable.begin(); it != lightTable.end(); it ++){
    float min = INFINITY;
    int index = it->getIndex();
    int closestNeighbour = index;
    float distance = 0.0f;
    //Compare each Light to all other Lights
    for(vector<Light>::const_iterator jt = lightTable.begin(); jt != lightTable.end();jt++){

      //Compute distance
      distance = dist(it->getPos(), jt->getPos());
      
      //Store it as closest neighbour if conditions are met
      if(distance < min && distance >0)
	{
	  min = distance;
	  closestNeighbour = jt->getIndex();
	}
    }
    //Store (distanceValue, index1, index2) as closest neighbours
    if(closestNeighbour != index){
      distTable.push_back(make_tuple(min,index,closestNeighbour));
    }
  }
  distTable.sort([](const ftup & a, const ftup & b) { return get<0>(a) < get<0>(b); });
  for(ftuplist::iterator it = distTable.begin(); it != distTable.end(); it++){
    cout << "***dist min      " << (get<0>(*it)) << endl;
    cout << "neighbour 1     :" << (get<1>(*it)) << endl;
    cout << "neighbour 2     :" << (get<2>(*it)) << endl;   
  }
  return distTable;
}

Light LightTree::createCluster(ftuplist disTable, ltuplist & clusterTable, vector<Light> & lightTable){

  Light lightOne, lightTwo;
  for(vector<Light>::iterator it=lightTable.begin(); it!=lightTable.end(); it ++){
    if(it->getIndex()==get<1>(disTable.front()) ){
      lightOne = *it;
    }
    if(it->getIndex()==get<2>(disTable.front()) ){
      lightTwo = *it;
    }
  }
  cout << "Avec brio"<<endl;
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
  cout << "Avant bernardo"<<endl;
  clusterTable.push_back(clusterOne);
  cout << "Après l'avoir poussé" << endl;
  return cluster;
}
