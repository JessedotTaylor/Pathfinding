#ifndef __LPASTAR_H__
#define __LPASTAR_H__

#include <vector> 
#include "globalVariables.h"

class GridWorld; //forward declare class GridWorld to be able to create the friend functions later

class LpaStar{

public:
    LpaStar(int rows, int cols); //constructor

    void initialise(int startX, int startY, int goalX, int goalY);
    bool computeShortestPath(void);
    bool computeShortestPathStep(int steps);
    void updateVertex(vertex * u, vertex * uPrime);

   
	
    int minValue(int v1, int v2);
    int maxValue(int v1, int v2);
	
    double calc_H(int x, int y);
    void updateHValues();

    void remove(vertex * v); // Could be updated to take int val
    bool inQueue(vertex * v);
    vertex * pop(void);
    void insert(vertex *v, double key0, double key1);
    void insert(vertex *v, Key inpKey);
    Key topKey(void);

    Key calcKey(int x, int y);
    Key calcKey(vertex *cell);

    bool lt(double key00, double key01, double key10, double key11);
    bool lt(Key k1, Key k2);

    void updateAllKeyValues();

    //void copyMazeToDisplayMap(GridWorld gWorld);
    friend void copyMazeToDisplayMap(GridWorld &gWorld, LpaStar* lpa);
    friend void copyDisplayMapToMaze(GridWorld &gWorld, LpaStar* lpa);

	
    vector<vector<vertex> > maze;   

    vector<vertex* > U; //Priority Queue
    int lenU = 0;

    int rows;
    int cols;

    int startI;
    int startJ;
    int goalI;
    int goalJ;

    vertex *start;
    vertex *goal;

    Key tempKey;

    char alg = 'L';
    int searchState = 0;

};

#endif
