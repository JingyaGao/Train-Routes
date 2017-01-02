// Author: Sean Davis

#include "TrainRunner.h"
#include "train.h"
#include "StackAr.h"
#include <cstring>
#include <iostream>

#define MAX_LOAD 50

HeapNode::HeapNode()
{
    //station = NULL;
}



HeapNode::HeapNode(int id, int *adj, int *distance, int adjCount) : //, int d, int pv, bool known) : 
stationID(id), adjCount(adjCount) //, dv(d), pv(pv), known(known)
{
    //station = st;
    adjacent = new int[10];
    distances = new int[10];
    memcpy(adjacent, adj, adjCount*sizeof(int));
    memcpy(distances, distance, adjCount*sizeof(int));
    //cars = new Car[10];
    carID = new int[10];
    numCars = 0;
    dv = -1;
    pv = -1;
    known = false;
    hasCar = false;
}



Train::Train(Station *stations, int numStations, int numCars) : 
totalStations(numStations), totalCars(numCars)
{
  myStations = new Station[numStations];
  memcpy(myStations, stations, numStations*sizeof(Station));
  
  myTrain = new List<int>[numStations];
} // Train()




void Train::run(Car *cars, Operation *operations, int *numOperations)
{
  *numOperations = 0;
  HeapNode *array = new HeapNode[totalStations]; 
     // is this ok? why not seg fault like operations when inserting???
  int i, change, currentIndex, currentStation = 0, totalTime = 0, currentCar = 0;
  
  
  
  //cout << "inserting HeapNodes" << endl;
  for (i = 0; i < totalStations; i++)
  {
      array[i] = HeapNode(i, myStations[i].adjacent, myStations[i].distances, myStations[i].adjCount);
      
      //cout << "\ncurrentStation: " << array[i].stationID << endl;
      /*for(currentIndex = 0; currentIndex < myStations[i].adjCount; currentIndex++)
      {
        cout << "neighbor# " << currentIndex << ": " << array[i].adjacent[currentIndex] << endl;
        cout << "  distance should be: " << myStations[i].distances[currentIndex] << endl;
        cout << "  distance: " << array[i].distances[currentIndex] << endl;
        
      }
      */
  } //correspond each station to each HeapNode

  //cout << "\n inserting cars into HeapNodes" << endl;
  for (i = 0; i < totalCars; i++)
  {
      // find where the car currently is and put it into its station
      currentIndex = cars[i].origin;
      array[currentIndex].hasCar = true;
      // array of cars
      //array[currentIndex].cars[array[currentIndex].numCars] = cars[i];
      // array of carIDs
      array[currentIndex].carID[(array[currentIndex].numCars)++] = i;
      
      //cout << "currentStation " << currentIndex << endl;//array[currentIndex].stationID << " carID: " << i << endl;
      //cout << "  carID at the station: " << array[currentIndex].carID[0] << endl;
      //cout << "  what the fuck is i: " << i << endl;
      //cout << "  num of cars: " << array[currentIndex].numCars << " carID at 0: " << array[currentIndex].carID[0] << endl;
  } // correspond cars to stations for HeapNodes
  
  
  

  //create Heap; used to find correct station
  BinaryHeap<HeapNode> heap = BinaryHeap<HeapNode>(500); //totalStations); // capacity defaults to 100
  // array of index changed for each dijkstra, reset the index after operation
  int *indexChanged = new int[500];
  
  //create stack; used to carry out operations once the station is found
  StackAr<int> stack(500);
  //int numToDeliver = 0;
  
  // lots and lots of dijkstras -- FUCK THIS SHIT
  while(totalCars > 0)
  {
    HeapNode v, *pickup = NULL, *delivery = NULL;
    // insert into heap??? insert the currentStation???
    array[currentStation].dv = 0;
    heap.insert(array[currentStation]);
    change = 0;
    indexChanged[change++] = currentStation;
    //cout << "\n*********************************************************"<< endl;
    //cout << "while totalCars is not 0" <<  endl;
    //cout << "totalCars: " << totalCars << endl;
    //cout << "currentStation: " << currentStation << endl;
    //cout << "indexChanged[0]: " << indexChanged[0] << endl;
    
  
    // dijstras
    // while( min doesn't have a pick up)
    //    pop min, insert its neighbors into the heap
    while(!heap.isEmpty()) //&& ((!heap.findMin().hasCar
    //&& myTrain[heap.findMin().stationID].isEmpty()) 
    //|| (heap.findMin().hasCar && heap.findMin().numCars + currentCar >= 50) 
    //|| currentCar >= 44)) // && there cant be cars delivered???
    {
        //if(currentCar >= 44 && !myTrain[heap.findMin().stationID].isEmpty())
        //  break;
        v = heap.findMin();
        /*if(v.known)
        {
          heap.deleteMin();
          continue;
        }
        */
        if(pickup == NULL && v.hasCar)
          pickup = &v;
        if(delivery == NULL && !myTrain[v.stationID].isEmpty())
          delivery = &v;
       
        v.known = true;
        heap.deleteMin();
        
        /*if(pickup)
        {
          numToDeliver = sizeof(myTrain[pickup->stationID])/sizeof(ListNode<int>) - sizeof(ListNode<int>);
          if(currentCar - numToDeliver + pickup->numCars <= MAX_LOAD || delivery)
            break; // use pickup in stack
        }*/
        if(pickup)
        //numToDeliver = sizeof(myTrain[pickup->stationID])/sizeof(ListNode<int>) - sizeof(ListNode<int>);
        if(pickup && currentCar == 0)
          break;
        
        //if(pickup && (currentCar - numToDeliver + pickup->numCars <= MAX_LOAD))// || delivery))
        //if(pickup && (currentCar - (sizeof(myTrain[pickup->stationID])/sizeof(ListNode<int>) - sizeof(ListNode<int>)) + pickup->numCars <= MAX_LOAD))
        if(pickup && (currentCar + pickup->numCars <= MAX_LOAD))// || delivery))
          break;
        else if((delivery && pickup && (currentCar + pickup->numCars > MAX_LOAD)) || (delivery && (currentCar == totalCars)))// && (totalCars <= MAX_LOAD)))
          {
            cout << "delivery" << endl;
            break;
          }
        
          
        //cout << "\nin the dijkstras loop" << endl;
        //cout << "station ID: " << v.stationID << endl;
        //cout << "station's dv: " << v.dv << " pv: " << v.pv <<  endl;
        //checks for overflow?????
        
        for (int count = 0; v.adjCount > count; count++)
        {
          //cout << "\n    updating neighbors: " << count << endl;
          //cout << "    count: " <<  count << endl;
          int neighbor = v.adjacent[count];
          if(!array[neighbor].known && (array[neighbor].dv > v.dv + v.distances[count]))
          {
            array[neighbor].dv = v.dv + v.distances[count];
            array[neighbor].pv = v.stationID;
            heap.insert(array[neighbor]);
            indexChanged[change++] = neighbor; 
            
            //cout << "\n    neighbor's stationID: " << neighbor  << endl;
            //cout << "    distance from station: " << v.distances[count] << endl;
            //cout << "    dv: " << array[neighbor].dv << " pv: " << array[neighbor].pv << endl;
          }
        }
        
    }
    
    
    
    
    // found the next station with pick up/delivery
    // carry out pick up/deievery operations
    //if (!heap.isEmpty())
    //  v = heap.findMin();
    
    //move the below to where the stack is implemented
    //if (pickup && (currentCar - numToDeliver + pickup->numCars <= MAX_LOAD))
    if(pickup && (currentCar + pickup->numCars <= MAX_LOAD))
      v = *pickup; // use pickup in stack
    else if((delivery && pickup && (currentCar + pickup->numCars > MAX_LOAD)) || (delivery && (currentCar == totalCars)))// && (totalCars <= MAX_LOAD)))
      v = *delivery; // use delivery in stack
    //cout << "\nMin (destination node): " << v.stationID << " dv: " << v.dv << " pv: " << v.pv << endl;
    
    //check for myTrain overflow; if overflow, deliver to closest?
    
    //cout << "\npv to be pushed: ";
    //1. move to correct station (recursive using pv???? NOPE TIME) (or while loop?)
    
    while(v.pv != -1) 
    {
      //store in a stackAr?? or just array? then after the while loop, pop in right order
      //cout << v.pv << " ";
      stack.push(v.stationID);
      v = array[v.pv];
    }
    
    //currentStation and next are the same thing. next is really not needed, delete later?
    int next = currentStation;
    while(!stack.isEmpty()) //move to next station to pick up, deliver along the way at intermediate nodes/stations
    {
      next = stack.topAndPop();
      //cout << "\ntrain is heading to station: " << next << endl;
      //operations[(*numOperations)++] = Operation(totalTime + array[next].dv, 'M', next);
      operations[(*numOperations)].time = totalTime + array[next].dv;
      operations[(*numOperations)].operation = 'M';
      operations[(*numOperations)++].ID = next;
      //check intermediate nodes for possible delivery
      if (!myTrain[next].isEmpty())
      {
        //cout << "delivery while on the way to pick up" << endl;
        for (ListNode<int> *itr = myTrain[next].header->next; itr; itr = itr->next)
        {
          //operations[(*numOperations)++] = Operation(totalTime + array[next].dv, 'D', itr->element);
          operations[(*numOperations)].time = totalTime + array[next].dv;
          operations[(*numOperations)].operation = 'D';
          operations[(*numOperations)++].ID = itr->element;
          myTrain[next].remove(itr->element);
          currentCar--;
          totalCars--;
        }
      }
    }
    //if(array[next].pv != -1)
      totalTime += array[next].dv;
    currentStation = next;
    //cout << "train is at station: " << next << endl;
    //cout << "  car at the current station: " << array[currentStation].carID[0] << endl;
    
    //2. pick up car(s) and/or deliver car(s) at that station if applies
    //if station has car, pick it up, else deliver
    
    //deliver at same station, if have cars; before pickup?
    
    
    if(array[currentStation].hasCar)
    {
      //cout << "pick up" << endl;
    for (i = 0; i < array[currentStation].numCars; i++)
    {
      myTrain[cars[array[currentStation].carID[i]].destination].insert(array[currentStation].carID[i],
        myTrain[cars[array[currentStation].carID[i]].destination].zeroth());
        // ^what a clusterfuck <--indeed
      //operations[(*numOperations)++] = Operation(totalTime, 'P', array[currentStation].carID[i]);
      operations[(*numOperations)].time = totalTime;
      operations[(*numOperations)].operation = 'P';
      operations[(*numOperations)++].ID = array[currentStation].carID[i];
      //cout << " pick up time: " << totalTime << endl;
      //cout << " pickup carID: " <<  array[currentStation].carID[i] << endl;
      currentCar++;
    }
    array[currentStation].numCars = 0;
    array[currentStation].hasCar = false;
    } // pick up 
    /*else if(!myTrain[currentStation].isEmpty())
    {
      cout << "delivery: " << endl;
      for (ListNode<int> *itr = myTrain[currentStation].header->next; itr; itr = itr->next)
      {
        //operations[(*numOperations)++] = Operation(totalTime + array[next].dv, 'D', itr->element);
        operations[(*numOperations)].time = totalTime; // + array[next].dv;
        operations[(*numOperations)].operation = 'D';
        operations[(*numOperations)++].ID = itr->element;          
        myTrain[currentStation].remove(itr->element);
        currentCar--;
        totalCars--;
      }
    } // delivery
    */
    //3. decrement totalCars when delivering
    
    
    
    
    
    
    
    // reset the HeapNodes using indexChanged
    for(i = 0; i < change; i++)
    {
        array[indexChanged[i]].dv = -1;
        array[indexChanged[i]].pv = -1;
        array[indexChanged[i]].known = false;
    }
    
    heap.makeEmpty();
    //break; // for now so no infinite loop
  }
  
  

  // Students must fill the operations array, and set numOperations.
} // run()
