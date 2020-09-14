#include <stdio.h>
#include <vector>
#include <malloc.h>
#include <assert.h>
#include <algorithm>    
#include <math.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>     /* calloc, exit, free */

#include "DStarLite.h"
#include "globalVariables.h"

DStarLite::DStarLite(int _rows, int _cols){
    //cout << "DStar Called\n";
    rows = _rows;
    cols = _cols;

    km = 0;

    maze.resize(rows);
    for(int i=0; i < rows; i++){
        maze[i].resize(cols);
    }
}

void DStarLite::initialise(int _startJ, int _startI, int _goalJ, int _goalI) {
    startI = _startI;
    startJ = _startJ;
    goalI = _goalI;
    goalJ = _goalJ;

    km = 0;

    sLast = &(maze[startI][startJ]);
    start = &(maze[startI][startJ]);
    goal = &(maze[goalI][goalJ]);

    sLast->setG(INF);
    sLast->setRHS(INF);
    sLast->setRow(startI);
    sLast->setCol(startJ);
    sLast->setH(calc_H(startI, startJ));

    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            maze[i][j].setG(INF);
            maze[i][j].setRHS(INF);
            maze[i][j].setRow(i);
            maze[i][j].setCol(j);
            maze[i][j].setH(calc_H(i, j));
            
        }
    }

    goal->setRHS(0);

    insert(goal, calcKeys(goal));
}

bool DStarLite::computeShortestPath(void) {
    while (lt(topKey(), calcKeys(startI, startJ)) || (maze[startI][startJ].getRHS() != maze[startI][startJ].getG())) {
         Key kOld = topKey();

        vertex *v = pop();
        vertex * u = &maze[v->getRow()][v->getCol()];
        u->status = '2';

        
        
        if (lt(kOld, calcKeys(u))) {
            insert(u, u->getKey(0), u->getKey(1));
        } else if (u->getG() > u->getRHS()) {
            u->setG(u->getRHS());

            for(int m=0; m < DIRECTIONS; m++) {
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



bool DStarLite::computeShortestPathStep(int steps) {
    for (int z = 1; z < steps; z++) {
        if (lt(topKey(), calcKeys(startI, startJ)) || (maze[startI][startJ].getRHS() != maze[startI][startJ].getG())) {
            Key kOld = topKey();

            vertex *v = pop();
            vertex * u = &maze[v->row][v->col];
            u->status = '2';
            
            cout << "\nStep: " << z << '\n';
            //cout << "D-Q: ("<< u->row <<", " << u->col << ")\n";
            cout << "D-Q: \n(" << (char)((u->row-1) + 'A') << ", " << (u->col-1) << ")\n[" << u->key[0] << ", " << u->key[1] << "]\n";
            
            
            if (lt(kOld, calcKeys(u->getRow(), u->getCol()))) {
                insert(u, u->getKey(0), u->getKey(1));
            } else if (u->getG() > u->getRHS()) {
                u->setG(u->getRHS());
                //cout << u->g << '\n';

                // cout << "(" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\n";
                // for (int m =0; m < DIRECTIONS; m++) {
                //     cout << "(" << (char)((u->neighbourData.realIJ[m].i-1) + 'A') << " " << (u->neighbourData.realIJ[m].j-1) << ") Cost: " 
                //     << u->neighbourData.cost[m] << "\t";
                // }
                // cout << '\n';

                for(int m=0; m < DIRECTIONS; m++) {
                    //cout << u->linkCost[m];
                    if (u->getLinkCost(m) != INF) {
                        //cout << u->move[m]->h << maze[u->move[m]->row][u->move[m]->col].h  <<  u->h << '\n';
                        //updateVertex(&maze[u->move[m]->row][u->move[m]->col], u);
                        updateVertex(u->getMove(m));
                    }
                }
            } else {
                u->g = INF;
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

void DStarLite::updateVertex(vertex * u) {
    bool debug =false;
    if (debug) {cout << "updateVertex: (" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\t";}
    
    if (u->status == '0') {u->status = '1';}
    
    //") GoalI: " << goalI << " GoalJ: " << goalJ;
    //cout << u->row << u->col << '\n';
    if ((u->getRow() != goalI) || (u->getCol() != goalJ)) {

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
        //cout << '\n' << u->g <<  u->rhs << u->h << '\n';
        insert(u, calcKeys(u));
    }

    if (debug) {cout << '\n';}
}

void DStarLite::remove(vertex * v) {
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

bool DStarLite::inQueue(vertex * v) {
    int i = 0;
    while (i < lenU) {
        if ((U[i]->getRow() == v->getRow()) && (U[i]->getCol() == v->getCol())) {
            return true;
        }
        i++;
    }
    return false;
}

vertex * DStarLite::pop(void) {
    vertex * v = U[0];
    U.erase(U.begin());
    lenU--;
    return v;
}

void DStarLite::insert(vertex *v, double key0, double key1) {
    int i = 0;
    while ((i < lenU) && !(lt(key0, key1, U[i]->getKey(0), U[i]->getKey(1)))) {i++;}  
    U.insert(U.begin() + i, v);
    lenU++;
    if (lenU > maxU) {maxU = lenU;}
}

void DStarLite::insert(vertex *v, Key inpKey) {
    double key0 = inpKey.keyV[0];
    double key1 = inpKey.keyV[1];
    insert(v, key0, key1);
}

Key DStarLite::topKey(void) {
    if (lenU > 0) {
        tempKey.keyV[0] = U[0]->getKey(0);
        tempKey.keyV[1] = U[0]->getKey(1);
    } else {
        tempKey.keyV[0] = INF;
        tempKey.keyV[1] = INF;
    }
    return tempKey;
}

Key DStarLite::calcKeys(vertex * v) {
    //cout << v->g <<  v->rhs << v->h << '\n';
   
    double key2 = min(v->getG(), v->getRHS());
    double key1 = key2 + v->getH() + km;
    v->setKey(0, key1);
    tempKey.keyV[0] = key1;
    v->setKey(1, key2);
    tempKey.keyV[1] = key2;
    
    return tempKey;
}

Key DStarLite::calcKeys(int i, int j) {
    vertex *v = &(maze[i][j]);
    return calcKeys(v);
}

double DStarLite::calc_H(int _i, int _j){
	if (HEURISTIC == MANHATTAN) {
        int diffY = abs(sLast->getRow() - _i);
        int diffX = abs(sLast->getCol() - _j);
        
        return (double)max(diffX, diffY);
    } else if (HEURISTIC == EUCLIDEAN) {
        int diffX = pow((sLast->getRow() - _i), 2);
        int diffY = pow((sLast->getCol() - _j), 2);

        return (double)sqrt(diffX + diffY);
    }
    return INF;
}

bool DStarLite::lt(double key00, double key01, double key10, double key11) {
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

bool DStarLite::lt(Key k1, Key k2) {
    double key00 = k1.keyV[0];
    double key01 = k1.keyV[1];
    double key10 = k2.keyV[0];
    double key11 = k2.keyV[1];
    return lt(key00, key01, key10, key11);
}


void DStarLite::updateHValues(void){
    for(int i=0; i < rows; i++){
        for(int j=0; j < cols; j++){
            maze[i][j].setH(calc_H(i, j));
        }
    }
}
void DStarLite::updateAllKeyValues(void){cout << "This function is not implemented\n"; return;}