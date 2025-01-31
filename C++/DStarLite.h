#ifndef __DSTARLITE_H__
#define __DSTARLITE_H__


#include <vector>
#include "globalVariables.h"

extern int goal_x, goal_y;
extern int start_x, start_y;

//~ extern int numberOfExpandedStates;
extern int numberOfVertexAccesses;

class GridWorld;

class DStarLite {
    public:
        DStarLite(int _rows, int _cols);
        void initialise(int _startI, int _startJ, int _goalI, int _goalJ);
        bool computeShortestPath(void);
        bool computeShortestPathStep(int steps);
        void updateVertex(vertex * u);

        void remove(vertex * v); // Could be updated to take int val
        bool inQueue(vertex * v);
        vertex * pop(void);
        void insert(vertex *v, double key0, double key1);
        void insert(vertex *v, Key inpKey);
        Key topKey(void);

        Key calcKeys(vertex * v);
        Key calcKeys(int i, int j);

        double calc_H(int x, int y);
        double calc_H(int i1, int j1, int i2, int j2);

        bool lt(double key00, double key01, double key10, double key11);
        bool lt(Key k1, Key k2);

        void updateHValues(void);
        void updateAllKeyValues(void);


        friend void copyMazeToDisplayMap(GridWorld &gWorld, DStarLite* DStar);
        friend void copyDisplayMapToMaze(GridWorld &gWorld, DStarLite* DStar);

        vector<vector<vertex> > maze;   
        vector<vertex *> U; //Priority Queue

        int rows;
        int cols;

        int startI;
        int startJ;
        int goalI;
        int goalJ;

        int km;

        int lenU = 0;
        int maxU = 0;

        Key tempKey;

        vertex *sLast;
        vertex *start;
        vertex *goal;

        char alg = 'D';
        int searchState = 0;
};

#endif
