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

void LpaStar::initialise(int startCol, int startRow, int goalCol, int goalRow){
	//cout << "Start init\n";
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
			maze[i][j].g = INF;
			maze[i][j].rhs = INF;
			//maze[i][j].h = calc_H(i, j);
			maze[i][j].row = i;
			maze[i][j].col = j;
		}
	}
	//cout << "End maze gen\n";
	start = new LpaStarCell;
	goal = new LpaStarCell;
	
	//START VERTEX
	start->g = INF;
	start->rhs = 0.0;
	start->row = startRow;
	start->col = startCol;

	
	//GOAL VERTEX
	goal->g = INF;
	goal->rhs = INF;
	goal->row = goalRow;
	goal->col = goalCol;
	//---------------------
	//cout << "End vertex setup\n";
	// maze[start->y][start->x].g = start->g;
	// maze[start->y][start->x].rhs = start->rhs;
	
	// maze[goal->y][goal->x].g = goal->g;
	// maze[goal->y][goal->x].rhs = goal->rhs;

	//maze[start->y][start->x].key = ([maze[start->y][start->x].h, 0]);
	//maze[start->y][start->x].calcKey()
	//updateHValues();
	//cout << "End H val gen\n";

	calcKey(start);
	insert(start, start->key);
	//cout << "Queue: " << U << '\n';
	// cout << "Start Queue len: " << lenU << '\n';
	// cout << "Starting Queue: ";

	// for (int x = 0; x < lenU; x++) {
	// 	cout << U[x]->row << " " << U[x]->col << '\n';
	// }


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
	//cout << goal->y << goal->x << '\n';
	int diffY = abs(goal->y - y);
	int diffX = abs(goal->x - x);
	
	//maze[y][x].h = (double)maxValue(diffY, diffX);
	//cout << "End calc_H\n";
	return (double)maxValue(diffY, diffX);
}

void LpaStar::updateHValues(){
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   maze[i][j].h = calc_H(j, i);
		
		}
	}
	//cout <<"End H loop\n"; 
	
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

void LpaStar::computeShortestPath(void) {
	calcKey(start);
	cout << "topKey: " << *(topKey()) << " Goal Key: " << *(goal->key) << " Result: " << (topKey() < goal->key) << '\n';
	cout << "Goal RHS: " << goal->rhs << " Goal G: " << goal->g << " Result: " << (goal->rhs != goal->g) << '\n';;


	while((topKey() < goal->key) || (goal->rhs != goal->g)) {
		//cout << "Made it inside while\n";
		LpaStarCell u = *(pop());
		LpaStarCell* neighbour;
		cout << (char)((u.row-1) + 'A') << " " << (u.col-1) << '\n';
		
		if (u.g > u.rhs) {
			u.g = u.rhs;
			
			for(int m=0; m < DIRECTIONS; m++){
				neighbour = u.move[m];

				// cout << "Neighbour: " << m <<'\n';
				
				// cout << neighbour->type + '0' << '\n';
				// if(neighbour != NULL && neighbour->type != 1){
				// 	updateVertex(neighbour);
				// }  	
				cout << "Neighbour: " << m << " Position: (" << (char)(((neighbour->row)-1) + 'A') << " " << ((neighbour->col)-1) << ") Cost: " << u.linkCost[m] << '\n';

				if(u.linkCost[m] != INF){
					neighbour = u.move[m];
					updateVertex(neighbour);
				}
			}
		} else {
			u.g = INF;
			for(int m=0; m < DIRECTIONS; m++){
				neighbour = u.move[m];
				if(neighbour != NULL && neighbour->type != 1){
					updateVertex(neighbour);
				}  	
			}
			LpaStarCell * uP = &u;
			updateVertex(uP);

		}
	}
}

void LpaStar::updateVertex(LpaStarCell * u) {
	//Status check here
	cout << "Update Vertex: (" << (char)(((u->row)-1) + 'A') << " " << ((u->col)-1) << ")\n";
	LpaStarCell* neighbour;
	if (ne(u, this->start)) {
		int currMin = INF;
		//cout << DIR
		for(int m=0; m < DIRECTIONS; m++){
			//cout << u->move[m];

			neighbour = u->move[m];
			cout << "Neighbour: " << m << " Position: (" << (char)(((u->row)-1) + 'A') << " " << ((u->col)-1) << ")\n";
			int cost = neighbour->g + u->linkCost[m];
			if (cost < currMin) {currMin = cost;}
		} 
		//cout << currMin << '\n';
		u->rhs = currMin; 	
	}

	if (inQueue(u)) {remove(u);}

	if (u->g != u-> rhs) {insert(u, u->key);}
}


double* LpaStar::smallestKey(double s[2], double sPrime[2]) {
	if (s[0] < sPrime[0]) {return s;}
	else if (s[0] == sPrime[0]) {
		if (s[1] < sPrime[1]) {return s;}
	} 
	return sPrime;
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
	//cout << "Insert Called\n";
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

void LpaStar::remove(LpaStarCell * s) {
	if (lenU > 0) {
		int i = 0;
		while (ne(U[i], s)) {
			i++;
		}
		U.erase(U.begin() + i);
		lenU--;
	}
}

bool LpaStar::inQueue(LpaStarCell * u) {
	for (int i = 0; i < this->lenU; i++) {
		if ((u->row == U[i]->row) && (u->col == U[i]->col)) {return true;}
	}
	return false;
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

bool LpaStar::lt(double s[2], double other[2]) {
	if (s[0] < other[0]) {return true;}
        else if (s[0] == other[0]) {
            if (s[1] < other[1]) {return true;} 
            else {return false;}
    } else {return false;}
}

bool LpaStar::ne(LpaStarCell * s, LpaStarCell * other) {
	//cout << "ne Called\n";
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
