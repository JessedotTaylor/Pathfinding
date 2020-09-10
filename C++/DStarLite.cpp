#include <stdio.h>
#include <vector>
#include <malloc.h>
#include <assert.h>
#include <algorithm>    
#include <math.h>
#include <iostream>
#include <stdlib.h>     /* calloc, exit, free */

#include "DStarLite.h"
#include "globalVariables.h"

//DStarLite * D_Star_Lite;

// int main(int argc, char *argv[]) {
//     cout << "Started\n";
//     D_Star_Lite = new DStarLite(7,5);
//     D_Star_Lite->initialise(1,2,3,5);
//     D_Star_Lite->computeShortestPathStep(1);
//     cout << "Finished\n";
//     return 0;
    
// }


DStarLite::DStarLite(int _rows, int _cols){
    cout << "DStar Called\n";
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

    //cout << "Start Maze\n";
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
    //cout << "End Maze\n";


    sLast = &(maze[startI][startJ]);
    start = &(maze[startI][startJ]);
    goal = &(maze[goalI][goalJ]);

    maze[goalI][goalJ].rhs = 0;
    goal->key[0] = maze[goalI][goalJ].h;
    goal->key[1] = 0;

    //cout << maze[goalI][goalJ].key[0] <<  maze[goalI][goalJ].key[1] << '\n';
    //cout << "Start Insert\n";
    insert(&maze[goalI][goalJ], maze[goalI][goalJ].key[0],  maze[goalI][goalJ].key[1]);
    //cout << "End Insert\n";
    //cout << U[0]->key[0] << U[0]->key[1] << '\n';
}

void DStarLite::computeShortestPath(void) {
    while (lt(topKey(), calcKeys(startI, startJ)) || (maze[startI][startJ].rhs != maze[startI][startJ].g)) {
         Key kOld = topKey();

        vertex *v = pop();
        vertex * u = &maze[v->row][v->col];
        
        // cout << "\nStep: " << z << '\n';
        // //cout << "D-Q: ("<< u->row <<", " << u->col << ")\n";
        // cout << "D-Q: (" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\n";
        
        
        if (lt(kOld, calcKeys(u->row, u->col))) {
            insert(u, u->key[0], u->key[1]);
        } else if (u->g > u->rhs) {
            u->g = u->rhs;

            for(int m=0; m < DIRECTIONS; m++) {
                if (u->neighbourData.cost[m] != INF) {
                    updateVertex(&(maze[u->neighbourData.realIJ[m].i][u->neighbourData.realIJ[m].j]), u);
                }
            }
        } else {
            u->g = INF;
            for(int m=0; m < DIRECTIONS; m++) {
                if (u->neighbourData.cost[m] != INF) {
                    updateVertex(&(maze[u->neighbourData.realIJ[m].i][u->neighbourData.realIJ[m].j]), u);
                }
            }
            updateVertex(u, u);
        }
    }
}



void DStarLite::computeShortestPathStep(int steps) {
    for (int z = 1; z < steps; z++) {
        if (lt(topKey(), calcKeys(startI, startJ)) || (maze[startI][startJ].rhs != maze[startI][startJ].g)) {
            Key kOld = topKey();

            vertex *v = pop();
            vertex * u = &maze[v->row][v->col];
            
            cout << "\nStep: " << z << '\n';
            //cout << "D-Q: ("<< u->row <<", " << u->col << ")\n";
            cout << "D-Q: (" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\n";
            
            
            if (lt(kOld, calcKeys(u->row, u->col))) {
                insert(u, u->key[0], u->key[1]);
            } else if (u->g > u->rhs) {
                u->g = u->rhs;
                //cout << u->g << '\n';

                // cout << "(" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\n";
                // for (int m =0; m < DIRECTIONS; m++) {
                //     cout << "(" << (char)((u->neighbourData.realIJ[m].i-1) + 'A') << " " << (u->neighbourData.realIJ[m].j-1) << ") Cost: " 
                //     << u->neighbourData.cost[m] << "\t";
                // }
                // cout << '\n';

                for(int m=0; m < DIRECTIONS; m++) {
                    if (u->neighbourData.cost[m] != INF) {
                        updateVertex(&(maze[u->neighbourData.realIJ[m].i][u->neighbourData.realIJ[m].j]), u);
                    }
                }
            } else {
                u->g = INF;
                for(int m=0; m < DIRECTIONS; m++) {
                    if (u->neighbourData.cost[m] != INF) {
                        updateVertex(&(maze[u->neighbourData.realIJ[m].i][u->neighbourData.realIJ[m].j]), u);
                    }
                }
                updateVertex(u, u);
            }

        } else {
            cout << "Loop Broke" << '\n';
            return;
        }
    cout << "\nEnd Step Q\n";
    for (int x=0; x < lenU; x++) {
        //cout << "(" << U[x]->row << ", " << U[x]->col << ")\t";
        cout << "(" << (char)((U[x]->row-1) + 'A') << " " << (U[x]->col-1) << ")\t";
    }
    cout << '\n';
    for (int x=0; x < lenU; x++) {
        cout << "[" << U[x]->key[0] << ", " << U[x]->key[1] << "]\t";
    }
    cout << '\n';
    }
}

void DStarLite::updateVertex(vertex * u, vertex * uPrime) {
    //cout << "updateVertex: (" << (char)((u->row-1) + 'A') << " " << (u->col-1) << ")\t";
    
    //") GoalI: " << goalI << " GoalJ: " << goalJ;
    //cout << u->row << u->col << '\n';
    if ((u->row != goalI) || (u->col != goalJ)) {
        //cout << "Entered cost eval\t";
        int currMin = INF;
        for(int m=0; m < DIRECTIONS; m++) {
            if (u->neighbourData.cost[m] != INF) {
                int cost = maze[u->neighbourData.realIJ[m].i][u->neighbourData.realIJ[m].j].g + neighbours[m].cost;
                if (cost < currMin) {currMin = cost;}
            }
            
            //cout << cost << '\n';
            
        }
        u->rhs = currMin;
    }

    

    if (inQueue(u)) {
        //cout << "Entered remove\t";
        remove(u);
    }



    if (u->g != u->rhs) {
        //cout << "Entered insert\t";
        insert(u, calcKeys(u));
    }

    //cout << '\n';
}

void DStarLite::remove(vertex * v) {
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

bool DStarLite::inQueue(vertex * v) {
    // cout << "Q remove: (" << (char)((v->row-1) + 'A') << " " << (v->col-1) << ")\n";
    // for (int x=0; x < lenU; x++) {
    //     //cout << "(" << U[x]->row << ", " << U[x]->col << ")\t";
    //     cout << "(" << (char)((U[x]->row-1) + 'A') << " " << (U[x]->col-1) << ")\t";
    // }
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

vertex * DStarLite::pop(void) {
    vertex * v = U[0];
    U.erase(U.begin());
    lenU--;
    return v;
}

void DStarLite::insert(vertex *v, double key0, double key1) {
    //cout << "Add to Q: (" << (char)((v->row-1) + 'A') << " " << (v->col-1) << ")\n";
    int i = 0;
    //cout << lenU << '\n';
    while ((i < lenU) && !(lt(key0, key1, U[i]->key[0], U[i]->key[1]))) {i++;}  
    U.insert(U.begin() + i, v);
    lenU++;
}

void DStarLite::insert(vertex *v, Key inpKey) {
    double key0 = inpKey.keyV[0];
    double key1 = inpKey.keyV[1];
    insert(v, key0, key1);
}


Key DStarLite::topKey(void) {
    if (lenU > 0) {
        tempKey.keyV[0] = U[0]->key[0];
        tempKey.keyV[1] = U[0]->key[1];
        return tempKey;
    }
}

Key DStarLite::calcKeys(vertex * v) {
    double key2 = minVal(v->g, v->rhs);
    double key1 = key2 + v->h + km;
    v->key[0] = key1;
    tempKey.keyV[0] = key1;
    v->key[1] = key2;
    tempKey.keyV[1] = key2;
    
    return tempKey;
}

Key DStarLite::calcKeys(int i, int j) {
    vertex *v = &(maze[i][j]);
    return calcKeys(v);
}

double DStarLite::calc_H(int _i, int _j){
	
	int diffY = abs(startJ - _j);
	int diffX = abs(startI - _i);
	
	//maze[y][x].h = (double)maxValue(diffY, diffX);
	return (double)maxValue(diffY, diffX);
}

int DStarLite::minVal(int v1, int v2){
    if(v1 <= v2){
		return v1;
	} else {
		return v2;
	}
}

int DStarLite::maxValue(int v1, int v2){	
	if(v1 >= v2){
		return v1;
	} else {
		return v2;
	}	
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


void DStarLite::updateHValues(void){return;}
void DStarLite::updateAllKeyValues(void){return;}