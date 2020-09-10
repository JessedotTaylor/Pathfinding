#include <stdio.h>
#include <iostream>
#include <stdlib.h>     /* calloc, exit, free */
#include <math.h>  //sqrt, pow

#include "LPAstar.h"
//#include "gridworld.h"

// LpaStar * lpa_star;

// int main(int argc, char *argv[]) {
//     cout << "Started\n";
//     lpa_star = new LpaStar(8,6);
//     lpa_star->initialise(4,1,1,6);
//     lpa_star->computeShortestPathStep(2);
//     cout << "Finished\n";
//     return 0;
    
// }

 LpaStar::LpaStar(int rows_, int cols_){
		rows = rows_;
	    cols = cols_;
	 
		 //Allocate memory 
		 maze.resize(rows);
		 for(int i=0; i < rows; i++){
		   maze[i].resize(cols);
		 }
}

void LpaStar::initialise(int _startJ, int _startI, int _goalJ, int _goalI){
	startI = _startI;
    startJ = _startJ;
    goalI = _goalI;
    goalJ = _goalJ;
	
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   	maze[i][j].g = INF;
			maze[i][j].rhs = INF;
			maze[i][j].h = calc_H(i, j);
			maze[i][j].row = i;
            maze[i][j].x = i;
            maze[i][j].col = j;
            maze[i][j].y = j;
		}
	}

	start = &(maze[startI][startJ]);
    goal = &(maze[goalI][goalJ]);
	
	//START VERTEX
	start->rhs = 0.0;

	start->key[0] = maze[startI][startJ].h;
	start->key[1] = 0;

	insert(&maze[startI][startJ], maze[startI][startJ].key[0], maze[startI][startJ].key[1]);

	// for (int x=0; x < lenU; x++) {
    //     //cout << "(" << U[x]->row << ", " << U[x]->col << ")\t";
    //     cout << "(" << (char)((U[x]->row-1) + 'A') << ", " << (U[x]->col-1) << ")\t";
    // }
    // cout << '\n';
    // for (int x=0; x < lenU; x++) {
    //     cout << "[" << U[x]->key[0] << ", " << U[x]->key[1] << "]\t";
    // }
    // cout << '\n';
	
}
bool LpaStar::computeShortestPath(void) {
	while ((lt(topKey(), calcKey(goalI, goalJ)) || (maze[goalI][goalJ].rhs != maze[goalI][goalJ].g))){
		vertex * u = pop();

		if (u->g > u->rhs) {
			u->g = u->rhs;
			
			for (int m=0; m < DIRECTIONS; m++) {
				if (u->linkCost[m] != INF) {
					updateVertex(u->move[m], u);
				}
			}
		} else {
			u->g = INF;
			for(int m=0; m < DIRECTIONS; m++) {
				if (u->linkCost[m] != INF) {
					updateVertex(u->move[m], u);
				}
			}
			updateVertex(u, u);
		}
	}
	return 1;
}

bool LpaStar::computeShortestPathStep(int steps) {
	steps++;
	for (int z = 1; z < steps; z++) {
		if (lt(topKey(), calcKey(goalI, goalJ)) || (maze[goalI][goalJ].rhs != maze[goalI][goalJ].g)){
			vertex * u = pop();

			cout << "\nStep: " << z << '\n';
            //cout << "D-Q: ("<< u->row <<", " << u->col << ")\n";
            cout << "D-Q: \n(" << (char)((u->row-1) + 'A') << ", " << (u->col-1) << ")\n[" << u->key[0] << ", " << u->key[1] << "]\n";
            
			if (u->g > u->rhs) {
				u->g = u->rhs;

				for (int m=0; m < DIRECTIONS; m++) {
					if (u->linkCost[m] != INF) {
						updateVertex(u->move[m], u);
					}
				}
			} else {
				u->g = INF;
                for(int m=0; m < DIRECTIONS; m++) {
                    if (u->linkCost[m] != INF) {
                        updateVertex(u->move[m], u);
                    }
                }
                updateVertex(u, u);
			}
		} else {
			cout << "Loop Broke (Goal Found?)" << '\n';
            return 1;
		}
	cout << "\nEnd Step Q\n";
    for (int x=0; x < lenU; x++) {
        //cout << "(" << U[x]->row << ", " << U[x]->col << ")\t";
        cout << "(" << (char)((U[x]->row-1) + 'A') << ", " << (U[x]->col-1) << ")\t";
    }
    cout << '\n';
    for (int x=0; x < lenU; x++) {
        cout << "[" << U[x]->key[0] << ", " << U[x]->key[1] << "]\t";
    }
    cout << '\n';
	}
	return 0;
}

void LpaStar::updateVertex(vertex * u, vertex * uPrime) {
    bool debug =false;
    if (debug) {cout << "updateVertex: (" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\t";}
    
	if ((u->row != startI)|| (u->col != startJ)) {
		if (debug) {cout << "Entered cost eval\t";}

        int currMin = INF;
		for(int m=0; m < DIRECTIONS; m++) {
            if (u->linkCost[m] != INF) {
                int cost = u->move[m]->g + neighbours[m].cost;
                //cout << cost << '\n';
                if (cost < currMin) {currMin = cost;}
            } 
        }
        u->rhs = currMin;
    }

	if (inQueue(u)) {
        if (debug) {cout << "Entered remove\t";}
        remove(u);
    }

	if (u->g != u->rhs) {
        if (debug) {cout << "Entered insert\t";}
        insert(u, calcKey(u));
    }

	if (debug) {cout << '\n';}
}


void LpaStar::remove(vertex * v) {
    int i = 0;
    while (i < lenU) {
        if ((U[i]->row == v->row) && (U[i]->col == v->col)) {
            U.erase(U.begin() + i);
            lenU--;
            return;
        }
        i++;
    }
}

bool LpaStar::inQueue(vertex * v) {
    int i = 0;
    while (i < lenU) {
        if ((U[i]->row == v->row) && (U[i]->col == v->col)) {
            //cout << "True\n";
            return true;
        }
        i++;
    }
    //cout << "False\n";
    return false;
}

int LpaStar::minValue(int v1, int v2){
    if(v1 <= v2){
		return v1;
	} else {
		return v2;
	}	
}

int LpaStar::maxValue(int v1, int v2){
	if(v1 >= v2){
		return v1;
	} else {
		return v2;
	}	
}

double LpaStar::calc_H(int i, int j){
	
	int diffY = abs(goalI - i);
	int diffX = abs(goalJ - j);
	
	//maze[i][j].h = (double)maxValue(diffY, diffX);
	return (double)maxValue(diffY, diffX);
}

void LpaStar::updateHValues(){
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   maze[i][j].h = calc_H(j, i);
		}
	}
}

vertex * LpaStar::pop(void) {
    vertex * v = U[0];
    U.erase(U.begin());
    lenU--;
    return v;
}

void LpaStar::insert(vertex *v, double key0, double key1) {
    //cout << "Add to Q: (" << (char)((v->row-1) + 'A') << " " << (v->col-1) << ")\n";
    int i = 0;
    //cout << lenU << '\n';
    while ((i < lenU) && !(lt(key0, key1, U[i]->key[0], U[i]->key[1]))) {i++;}  
    U.insert(U.begin() + i, v);
    lenU++;
}

void LpaStar::insert(vertex *v, Key inpKey) {
    double key0 = inpKey.keyV[0];
    double key1 = inpKey.keyV[1];
    insert(v, key0, key1);
}

Key LpaStar::topKey(void) {
    if (lenU > 0) {
        tempKey.keyV[0] = U[0]->key[0];
        tempKey.keyV[1] = U[0]->key[1];
        return tempKey;
    }
}

Key LpaStar::calcKey(int i, int j){
	vertex *v = &(maze[i][j]);
    return calcKey(v);
}

Key LpaStar::calcKey(vertex *cell){
	double key2 = minValue(cell->g, cell->rhs);
	double key1 = key2 + cell->h;
	
	cell->key[0] = key1;
	cell->key[1] = key2;
	tempKey.keyV[0] = key1;
    tempKey.keyV[1] = key2;

	return tempKey;
}

bool LpaStar::lt(double key00, double key01, double key10, double key11) {
    if (key00 < key10) {return true;} 
    if (key00 == key10) {
        if (key01 < key11) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool LpaStar::lt(Key k1, Key k2) {
    double key00 = k1.keyV[0];
    double key01 = k1.keyV[1];
    double key10 = k2.keyV[0];
    double key11 = k2.keyV[1];
    return lt(key00, key01, key10, key11);
}

void LpaStar::updateAllKeyValues(){	
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   calcKey(&maze[i][j]);
		}
	}
}




