#include <stdio.h>
#include <iostream>
#include <stdlib.h>     /* calloc, exit, free */
#include <math.h>  //sqrt, pow

#include "LPAstar.h"
#include "gridworld.h"


 LpaStar::LpaStar(int rows_, int cols_){
		 rows = rows_;
	    cols = cols_;
	 
		 //Allocate memory 
		 maze.resize(rows);
		 for(int i=0; i < rows; i++){
		   maze[i].resize(cols);
		 }
}

void LpaStar::initialise(int startX, int startY, int goalX, int goalY){
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		    maze[i][j].g = INF;
			maze[i][j].rhs = INF;
			maze[i][j].h = calc_H(i, j);
		}
	}
	start = new LpaStarCell;
	goal = new LpaStarCell;
	
	//START VERTEX
	start->g = INF;
	start->rhs = 0.0;
	start->x = startX;
	start->y = startY;
	
	//GOAL VERTEX
	goal->g = INF;
	goal->rhs = INF;
	goal->x = goalX;
	goal->y = goalY;
	//---------------------
	maze[start->y][start->x].g = start->g;
	maze[start->y][start->x].rhs = start->rhs;
	
	maze[goal->y][goal->x].g = goal->g;
	maze[goal->y][goal->x].rhs = goal->rhs;

	//maze[start->y][start->x].key = ([maze[start->y][start->x].h, 0]);
	//maze[start->y][start->x].calcKey()
	calcKey(start);
	this->insert(start, start->key);


	//---------------------
	
	//for debugging only
	//~ for(int i=0; i < rows; i++){
	   //~ for(int j=0; j < cols; j++){
		   //~ //cout << maze[i][j].g << ", ";
			//~ cout << maze[i][j].rhs << ", ";
			
		//~ }
		//~ cout << endl;
	//~ }
	
}

double LpaStar::minValue(double g_, double rhs_){
	if(g_ <= rhs_){
		return g_;
	} else {
		return rhs_;
	}	
}

int LpaStar::maxValue(int v1, int v2){
	
	if(v1 >= v2){
		return v1;
	} else {
		return v2;
	}	
}

double LpaStar::calc_H(int x, int y){
	
	int diffY = abs(goal->y - y);
	int diffX = abs(goal->x - x);
	
	//maze[y][x].h = (double)maxValue(diffY, diffX);
	return (double)maxValue(diffY, diffX);
}

void LpaStar::updateHValues(){
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   maze[i][j].h = calc_H(j, i);
		}
	}
	
	start->h = calc_H(start->x, start->y);
	goal->h = calc_H(goal->x, goal->y);
}

void LpaStar::calcKey(int x, int y){
	double key1, key2;
	
	key2 = minValue(maze[y][x].g, maze[y][x].rhs);
	key1 = key2 + maze[y][x].h;
}


void LpaStar::calcKey(LpaStarCell *cell){
	double key1, key2;
	
	key2 = minValue(cell->g, cell->rhs);
	key1 = key2 + cell->h;
	
	cell->key[0] = key1;
	cell->key[1] = key2;
}

void LpaStar::updateAllKeyValues(){	
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   calcKey(&maze[i][j]);
		}
	}
	
	calcKey(start);
	calcKey(goal);
}

double* LpaStar::smallestKey(double s[2], double sPrime[2]) {
	if (s[0] < sPrime[0]) {return s;}
	else if (s[0] == sPrime[0]) {
		if (s[1] < sPrime[1]) {return s;}
	} else {return sPrime;}
}

void LpaStar::plusLen(void) {
	lenU++;
	if (this->lenU > this->maxU) {maxU = lenU;}
}

void LpaStar::plusLen(int val) {
	lenU = lenU + val;
	if (this->lenU > this->maxU) {maxU = lenU;}
}

void LpaStar::insert(LpaStarCell * s, double key[2]) {
	int i = 0;
	while ((i < lenU) && (lt(U[i], key))) {
		i++;
	}

	U.insert(U.begin() + i, s);
	this->plusLen();
}

double * LpaStar::topKey(void) {
	if (this->lenU > 0) {return this->U[0]->key;}
}

LpaStarCell * LpaStar::pop(void) {
	//s = new LpaStarCell;
	LpaStarCell * s = U[0];
	U.erase(U.begin());
	lenU--;
	return s;
}

void LpaStar::remove(LpaStarCell s) {
	if (lenU > 0) {
		int i = 0;
		while (ne(U[i], s)) {
			i++;
		}
		U.erase(U.begin() + i);
		lenU--;
	}
}

bool LpaStar::et(LpaStarCell s, LpaStarCell * other) {
	if (s.key[0] == other->key[0]){
            if (s.key[1] == other->key[1]){
                return true;
            }
        }
    return false;
}

bool LpaStar::lt(LpaStarCell * s, double other[2]) {
	if (s->key[0] < other[0]) {return true;}
        else if (s->key[0] == other[0]) {
            if (s->key[1] < other[1]) {return true;} 
            else {return false;}
    } else {return false;}
}

bool LpaStar::ne(LpaStarCell * s, LpaStarCell * other) {
	if (s->key[0] == other->key[0]){
            if (s->key[1] == other->key[1]){
                return false;
            }
        }
    return true;
}

bool LpaStar::ne(LpaStarCell * s, LpaStarCell other) {
	if (s->key[0] == other.key[0]){
            if (s->key[1] == other.key[1]){
                return false;
            }
        }
    return true;
}
