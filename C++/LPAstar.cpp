#include <stdio.h>
#include <iostream>
#include <stdlib.h>     /* calloc, exit, free */
#include <math.h>  //sqrt, pow
#include <cmath>

#include "LPAstar.h"


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
		   	maze[i][j].setG(INF);
			maze[i][j].setRHS(INF);
            maze[i][j].setRow(i);
            maze[i][j].setCol(j);
			maze[i][j].setH(calc_H(i, j));
			
		}
	}

	start = &(maze[startI][startJ]);
    goal = &(maze[goalI][goalJ]);
	
	//START VERTEX
	start->setRHS(0.0);

    insert(&maze[startI][startJ], calcKey(start));

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
	while ((lt(topKey(), calcKey(goalI, goalJ)) || (maze[goalI][goalJ].getRHS() != maze[goalI][goalJ].getG()))){
		vertex * u = pop();
        u->status = '2';

		if (u->getG() > u->getRHS()) {
			u->setG(u->getRHS());
			
			for (int m=0; m < DIRECTIONS; m++) {
				if (u->getLinkCost(m) != INF) {
					updateVertex(u->getMove(m));
				}
			}
		} else {
			u->setG(INF);
			for(int m=0; m < DIRECTIONS; m++) {
				if (u->getLinkCost(m) != INF) {
					updateVertex(u->getMove(m));
				}
			}
			updateVertex(u);
		}
	}
	return 1;
}

bool LpaStar::computeShortestPathStep(int steps) {
	steps++;
	for (int z = 1; z < steps; z++) {
		if (lt(topKey(), calcKey(goalI, goalJ)) || (maze[goalI][goalJ].getRHS() != maze[goalI][goalJ].getG())){
			vertex * u = pop();
            u->status = '2';

			cout << "\nStep: " << z << '\n';
            //cout << "D-Q: ("<< u->row <<", " << u->col << ")\n";
            cout << "D-Q: \n(" << (char)((u->row-1) + 'A') << ", " << (u->col-1) << ")\n[" << u->key[0] << ", " << u->key[1] << "]\n";
            
			if (u->getG() > u->getRHS()) {
				u->setG(u->getRHS());

				for (int m=0; m < DIRECTIONS; m++) {
					if (u->getLinkCost(m) != INF) {
						updateVertex(u->getMove(m));
					}
				}
			} else {
				u->setG(INF);
                for(int m=0; m < DIRECTIONS; m++) {
                    if (u->getLinkCost(m) != INF) {
                        updateVertex(u->getMove(m));
                    }
                }
                updateVertex(u);
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

void LpaStar::updateVertex(vertex * u) {
    bool debug =false;
    if (debug) {cout << "updateVertex: (" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\t";}

    if (u->status == '0') {u->status = '1';}
    
	if ((u->getRow() != startI)|| (u->getCol() != startJ)) {
		if (debug) {cout << "Entered cost eval\t";}

        int currMin = INF;
		for(int m=0; m < DIRECTIONS; m++) {
            if (u->getLinkCost(m) != INF) {
                int cost = u->getMove(m)->getG() + neighbours[m].cost;
                //cout << cost << '\n';
                if (cost < currMin) {currMin = cost;}
            } 
        }
        u->setRHS(currMin);
    }

	if (inQueue(u)) {
        if (debug) {cout << "Entered remove\t";}
        remove(u);
    }

	if (u->getG() != u->getRHS()) {
        if (debug) {cout << "Entered insert\t";}
        insert(u, calcKey(u));
    }

	if (debug) {cout << '\n';}
}


void LpaStar::remove(vertex * v) {
    int i = 0;
    while (i < lenU) {
        if ((U[i]->getRow() == v->getRow()) && (U[i]->getCol() == v->getCol())) {
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
        if ((U[i]->getRow() == v->getRow()) && (U[i]->getCol() == v->getCol())) {
            //cout << "True\n";
            return true;
        }
        i++;
    }
    //cout << "False\n";
    return false;
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
    while ((i < lenU) && !(lt(key0, key1, U[i]->getKey(0), U[i]->getKey(1)))) {i++;}  
    U.insert(U.begin() + i, v);
    lenU++;
    if (lenU > maxU) {maxU = lenU;}
}

void LpaStar::insert(vertex *v, Key inpKey) {
    double key0 = inpKey.keyV[0];
    double key1 = inpKey.keyV[1];
    insert(v, key0, key1);
}

Key LpaStar::topKey(void) {
    if (lenU > 0) {
        tempKey.keyV[0] = U[0]->getKey(0);
        tempKey.keyV[1] = U[0]->getKey(1);
    } else {
        tempKey.keyV[0] = INF;
        tempKey.keyV[1] = INF;
    }
    return tempKey;
}

Key LpaStar::calcKey(int i, int j){
	vertex *v = &(maze[i][j]);
    return calcKey(v);
}

Key LpaStar::calcKey(vertex *cell){
	double key2 = min(cell->getG(), cell->getRHS());
	double key1 = key2 + cell->getH();
	
	cell->setKey(0, key1);
	cell->setKey(1, key2);
	tempKey.keyV[0] = key1;
    tempKey.keyV[1] = key2;

	return tempKey;
}
double LpaStar::calc_H(int i, int j){
	if (HEURISTIC == MANHATTAN) {
        int diffX = abs(goalI - i);
        int diffY = abs(goalJ - j);
        
        return (double)max(diffY, diffX);
    } else if (HEURISTIC == EUCLIDEAN) {
        int diffX = pow((goalI - i), 2);
        int diffY = pow((goalJ - j), 2);

        return (double)sqrt(diffX + diffY);
    }
    return INF;
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

void LpaStar::updateHValues(){
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   maze[i][j].setH(calc_H(i, j));
		}
	}
}

void LpaStar::updateAllKeyValues(){	
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
		   calcKey(&maze[i][j]);
		}
	}
}




