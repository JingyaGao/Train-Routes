#ifndef TRAIN_H
#define	TRAIN_H

#include "TrainRunner.h"
#include "BinaryHeap.h"
#include "LinkedList.h"
//#define MAX_D 100000000

class HeapNode
{
public:
  //Station station;
  //int adjacent[10];
  //int distances[10];
  int *adjacent;
  int *distances;
  int stationID, adjCount, numCars, *carID;
  //Car *cars;
  bool hasCar;
  
  unsigned int dv;
  int pv;
  bool known;
  
  HeapNode();
  HeapNode(int id, int *adj, int *distances, int adjCount); //, 
  //int d = MAX_D, int pv = -1, bool known = false);
};

/*class TrainCar
{
public:
  short ID;
};*/

class Train
{
public:

  int totalStations, totalCars;
  Station *myStations;
  // Car *myCars;
  List<int> *myTrain;
  
  
  Train(Station *stations, int numStations, int numCars);
  void run(Car *cars, Operation *operations, int *numOperations);
}; // class train 

#endif	// TRAIN_H

