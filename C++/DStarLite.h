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
        double calc_H(int x, int y);
        int maxValue(int v1, int v2);

        void updateHValues(void);
        void updateAllKeyValues(void);



        friend void copyMazeToDisplayMap(GridWorld &gWorld, DStarLite* DStar);
        friend void copyDisplayMapToMaze(GridWorld &gWorld, DStarLite* DStar);

        vector<vector<vector> > maze;   
        vector<vector* > U; //Priority Queue

        int rows;
        int cols;

        int startI;
        int startJ;
        int goalI;
        int goalJ;

        int km;

        vector* sLast;
        vector* start;
        vector* goal;
};

#endif
