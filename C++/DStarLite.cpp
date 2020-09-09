#include <stdio.h>
#include <vector>
#include <malloc.h>
#include <assert.h>
#include <algorithm>    
#include <math.h>

#include "DStarLite.h"
#include "globalVariables.h"



DStarLite::DStarLite(int _rows, int _cols){
    rows = _rows;
    cols = _cols;

    km = 0;

    maze.resize(rows);
    for(int i=0; i < rows; i++){
        maze[i].resize(cols);
    }
}

void DStarLite::initialise(int _startI, int _startJ, int _goalI, int _goalJ) {
    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            maze[i][j].g = INF;
            maze[i][j].rhs = INF;
            maze[i][j].h = calc_H(i, j);
            cout << calc_H(i, j) << '\n'
        }
    }

    startI = _startI;
    startJ = _startJ;
    goalI = _goalI;
    goalJ = _goalJ;

    sLast = &(maze[startI][startJ]);
    goal = &(maze[goalI][goalJ]);



}

double DStarLite::calc_H(int _i, int _j){
	
	int diffY = abs(startJ - _j);
	int diffX = abs(startI - _i);
	
	//maze[y][x].h = (double)maxValue(diffY, diffX);
	return (double)maxValue(diffY, diffX);
}

int DStarLite::maxValue(int v1, int v2){	
	if(v1 >= v2){
		return v1;
	} else {
		return v2;
	}	
}


void DStarLite::updateHValues(void){return;}
void DStarLite::updateAllKeyValues(void){return;}