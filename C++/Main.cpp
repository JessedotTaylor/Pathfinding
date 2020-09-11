///////////////////////////////////////////////////////////////////////////////////////////
//
//
//  
//                        
//
//	 	      Program Name: Incremental Search 
//	 	       Description: start-up code for simulating LPA* and D*Lite
//                        - implements a gridworld that can be loaded from file, and 
//                          modified through a user-interface 
//
//        Run Parameters: 
//
//    Keys for Operation: 
//
//	 		        History:  date of revision
//                         09/Aug/2020  
//                         28/July/2015  
//                         03/Aug/2014  
//
//      Start-up code by:    n.h.reyes@massey.ac.nz
//
///////////////////////////////////////////////////////////////////////////////////////////


#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <deque>
#include <set>
#include <vector>

//-------------------------
#include "globalVariables.h"
#include "transform.h"
#include "graphics.h"
#include "DStarLite.h"
#include "LPAstar.h"
#include "gridworld.h"



// colour constants
int BACKGROUND_COLOUR;
int LINE_COLOUR;

int robotWidth;
int GRIDWORLD_ROWS; //duplicated in GridWorld
int GRIDWORLD_COLS; //duplicated in GridWorld


//----------------------------
unsigned int HEURISTIC;
//~ bool USE_EUCLIDEAN_DISTANCE_HEURISTIC;
int numberOfExpandedStates;
int MAX_MOVES;
int maxQLength;
int qLengthAfterSearch;

///////////////////////////////////////////////////////////////////////////////
DStarLite* D_Star_Lite;
LpaStar * lpa_star;
char alg;
bool pathFound = false;

GridWorld grid_world;

bool SHOW_MAP_DETAILS;

///////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------
//copy maze (from LPA*) to map (of GridWorld)
void copyMazeToDisplayMap(GridWorld &gWorld, DStarLite* DStar){
	for(int i=0; i < gWorld.getGridWorldRows(); i++){
	   for(int j=0; j < gWorld.getGridWorldCols(); j++){
			gWorld.map[i][j].type = DStar->maze[i][j].type;
		   	gWorld.map[i][j].h = DStar->maze[i][j].h;
			gWorld.map[i][j].g = DStar->maze[i][j].g;
			gWorld.map[i][j].rhs = DStar->maze[i][j].rhs;
			gWorld.map[i][j].reads = DStar->maze[i][j].reads;
			gWorld.map[i][j].writes = DStar->maze[i][j].writes;
			gWorld.map[i][j].status = DStar->maze[i][j].status;
			
			for(int k=0; k < 2; k++){
			  gWorld.map[i][j].key[k] = DStar->maze[i][j].key[k];			  
			}

			for(int m=0; m < DIRECTIONS; m++) {
				if (DStar->maze[i][j].move[m] != NULL) {
					gWorld.map[i][j].move[m] = &(gWorld.map[DStar->maze[i][j].move[m]->row][DStar->maze[i][j].move[m]->col]);
				}
				gWorld.map[i][j].linkCost[m] = DStar->maze[i][j].linkCost[m];
			}
		}
	}
}

void copyMazeToDisplayMap(GridWorld &gWorld, LpaStar* LPA){
	for(int i=0; i < gWorld.getGridWorldRows(); i++){
	   for(int j=0; j < gWorld.getGridWorldCols(); j++){
			gWorld.map[i][j].type = LPA->maze[i][j].type;
		   	gWorld.map[i][j].h = LPA->maze[i][j].h;
			gWorld.map[i][j].g = LPA->maze[i][j].g;
			gWorld.map[i][j].rhs = LPA->maze[i][j].rhs;
			gWorld.map[i][j].reads = LPA->maze[i][j].reads;
			gWorld.map[i][j].writes = LPA->maze[i][j].writes;
			gWorld.map[i][j].status = LPA->maze[i][j].status;
			// gWorld.map[i][j].row = LPA->maze[i][j].y;
			// gWorld.map[i][j].col = LPA->maze[i][j].x;
			
			for(int k=0; k < 2; k++){
			  gWorld.map[i][j].key[k] = LPA->maze[i][j].key[k];			  
			}

			for(int m=0; m < DIRECTIONS; m++) {
				if (LPA->maze[i][j].move[m] != NULL) {
					gWorld.map[i][j].move[m] = &(gWorld.map[LPA->maze[i][j].move[m]->row][LPA->maze[i][j].move[m]->col]);
				}
				gWorld.map[i][j].linkCost[m] = LPA->maze[i][j].linkCost[m];
			}


		}
	}
}

//--------------------------------------------------------------
//copy map (of GridWorld)to maze (of LPA*)
void copyDisplayMapToMaze(GridWorld &gWorld, DStarLite* DStar){
	int rows = gWorld.getGridWorldRows();
	int cols = gWorld.getGridWorldCols();
	for(int i=0; i < rows; i++){
	   for(int j=0; j < cols; j++){
			DStar->maze[i][j].type = gWorld.map[i][j].type;

			DStar->maze[i][j].col = gWorld.map[i][j].col;
			DStar->maze[i][j].row = gWorld.map[i][j].row;

			for(int m=0; m < DIRECTIONS; m++) {
				if (gWorld.map[i][j].move[m] != NULL) {
					DStar->maze[i][j].move[m] = &(DStar->maze[gWorld.map[i][j].move[m]->row][gWorld.map[i][j].move[m]->col]);
				}

				DStar->maze[i][j].linkCost[m] = gWorld.map[i][j].linkCost[m];
			}
		}
	}

}

void copyDisplayMapToMaze(GridWorld &gWorld, LpaStar* LPA){
	for(int i=0; i < gWorld.getGridWorldRows(); i++){
	   for(int j=0; j < gWorld.getGridWorldCols(); j++){
			LPA->maze[i][j].type = gWorld.map[i][j].type;

			LPA->maze[i][j].col = gWorld.map[i][j].col;
			LPA->maze[i][j].row = gWorld.map[i][j].row;

			for(int m=0; m < DIRECTIONS; m++) {
				if (gWorld.map[i][j].move[m] != NULL) {
					LPA->maze[i][j].move[m] = &(LPA->maze[gWorld.map[i][j].move[m]->row][gWorld.map[i][j].move[m]->col]);
				}

				LPA->maze[i][j].linkCost[m] = gWorld.map[i][j].linkCost[m];
			}
		}
	}

}


///////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES


///////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////
void updateMap(GridWorld &gWorld, LpaStar* LPA) {
	for(int i=0; i < gWorld.getGridWorldRows(); i++){
	   for(int j=0; j < gWorld.getGridWorldCols(); j++){
		   switch (gWorld.map[i][j].type)
		   {
			case '8':
			{
				gWorld.map[i][j].type = '0';
				
				break;
			}
			case '9':
			{
				gWorld.map[i][j].type= '1';
				
				LPA->maze[i][j].g = INF;
				gWorld.map[i][j]. g = INF;
				
				LPA->maze[i][j].rhs = INF;
				gWorld.map[i][j]. rhs = INF;
				for(int m=0; m < DIRECTIONS; m++) {
					gWorld.map[i][j].linkCost[m] = INF;
					LPA->maze[i][j].linkCost[m] = INF;
					if (LPA->maze[i][j].move[m]->type != '1'){
						LPA->updateVertex(LPA->maze[i][j].move[m], &(LPA->maze[i][j]));
					}
				}
				break;
			}
			default:
			   break;
		   }
	   }
	}
 }

bool updateMap(GridWorld &gWorld, DStarLite * DStar, int i, int j) {
	vertex * neighbour;
	bool changes = 0;
	for (int m= 0; m < DIRECTIONS; m++) {
		neighbour = DStar->maze[i][j].move[m];
		if (neighbour != NULL) {
			int mapI = neighbour->row;
			int mapJ = neighbour->col;
		
			switch (neighbour->type)
			{
				case '8':
				{
					gWorld.map[mapI][mapJ].type = '0';
					break;
				}
				case '9':
				{
					gWorld.map[mapI][mapJ].type= '1';
					
					neighbour->g = INF;
					gWorld.map[mapI][mapJ].g = INF;
					
					neighbour->rhs = INF;
					gWorld.map[mapI][mapJ].rhs = INF;

					

					for(int m=0; m < DIRECTIONS; m++) {
						gWorld.map[mapI][mapJ].linkCost[m] = INF;
						neighbour->linkCost[m] = INF;
						if (neighbour->move[m]->type != '1'){
							DStar->updateVertex(neighbour->move[m], neighbour);
						}
					}
					changes = 1;
				}
				default:
				{
					break;
				}
			}
		}
	}
	return changes;
}



void drawInformationPanel(int x, int y, char* info){
   ///////////////////////////////////////////////////////////////////////////////////////////
	settextstyle(SMALL_FONT, HORIZ_DIR, 4);
	settextjustify(LEFT_TEXT,CENTER_TEXT);
	setcolor(YELLOW);
	outtextxy(x ,y, info);
	///////////////////////////////////////////////////////////////////////////////////////////
}

int getKey(){
	
	 if(GetAsyncKeyState(VK_UP) < 0) { //UP ARROW
       return 200;
    } 
	 
	 if(GetAsyncKeyState(VK_DOWN) < 0) { //DOWN ARROW
       return 201;
    }
	
    if(GetAsyncKeyState(VK_F4) < 0) { 
       SHOW_MAP_DETAILS=false;
		 return 104;
    } 
  
    if(GetAsyncKeyState(VK_F5) < 0) {
        SHOW_MAP_DETAILS=true;
		  return 105;
    }
	 
	 if(GetAsyncKeyState(VK_F6) < 0) {
        //execute A* with strict expanded list
		  return 106;
    }
	 if(GetAsyncKeyState(VK_F7) < 0) {
        //execute LPA*
		  return 107;
    }
	 if(GetAsyncKeyState(VK_F8) < 0) {
        //execute D*Lite
		  return 108;
    }
	 
	 //copy display map to algorithim's maze
	 if(GetAsyncKeyState(VK_F9) < 0) {
		  return 109;
    }
	 
	 //copy algorithim's maze to display map
	 if(GetAsyncKeyState(VK_F10) < 0) {
		  return 110;
    }
	 	 
	 if(GetAsyncKeyState(0x53) < 0) { //S-key (start cell)
		  return 6;
    }
	 
	 if(GetAsyncKeyState(0x58) < 0) { //X-key (goal cell)
		  return 7;
    }
	 
	 if(GetAsyncKeyState(0x42) < 0) { //B-key (block cell)
		  return 1;
    }
	 
	 if(GetAsyncKeyState(0x47) < 0) {  //G-key
		  return 9;
    }
	 
	 if(GetAsyncKeyState(0x48) < 0) {  //H-key
		  return 10;
    }
	 
	 if(GetAsyncKeyState(0x4B) < 0) {  //K-key
		  return 11;
    }
	 
	 if(GetAsyncKeyState(0x55) < 0) { //U-key (Unblock cell)
		  return 12;
    }

	if(GetAsyncKeyState(0x49) < 0) { //I-key (Algorithim Info)?
		  return 13;
    }
	 
	 if(GetAsyncKeyState(0x50) < 0) { //P-key (position of cells)
		  return 14;
    }
	 
	 if(GetAsyncKeyState(0x43) < 0) { //C-key (connections of cells)
		  return 15;
    }
	 
	 if(GetAsyncKeyState(0x4D) < 0) { //M-key (entire map connections)
		  return 16;
    }
	 
	 if(GetAsyncKeyState(0x52) < 0) { //R-key (REINFORCEMENT LEARNING - reward values)
		  return 17;
    }
	 
	 if(GetAsyncKeyState(0x51) < 0) { //Q-key (REINFORCEMENT LEARNING - maxQ-values)
		  return 18;
    }
	 
 }
 
void runSimulation(char *fileName){
	WorldBoundaryType worldBoundary; //duplicated in GridWorld
    DevBoundaryType deviceBoundary; //duplicated in GridWorld
	bool ANIMATE_MOUSE_FLAG=false;
	bool validCellSelected=false;
	static BOOL page=false;
	int mX, mY;
	float worldX, worldY;
	worldX=0.0f;
	worldY=0.0f;
	
	int action=-1;
	//-----------------------
	CellPosition p;
	int rowSelected, colSelected;
	//-----------------------
    rowSelected=-1;
	colSelected=-1;
	
	int mouseRadius=1;

	vertex * currPos;
	char oldType;
	long startTime;
	long endTime;
	long firstSearchTime = INF;
	long secondSearchTime = INF;
	double firstMaxU = INF;
	double secondMaxU = INF;
	double pathL = INF;
	double acc = 0;
	double ex = 0;
	double read = 0;
	double write = 0;
		
	srand(time(NULL));  // Seed the random number generator
			
	//Initialise the world boundaries
    grid_world.initSystemOfCoordinates();
	//cout << "Starting Map Load\n";
	grid_world.loadMapAndDisplay(fileName);
	//cout << "Finished Loading Map\n";
	grid_world.initialiseMapConnections();
	
	//----------------------------------------------------------------
	//D*Lite
	D_Star_Lite = new DStarLite(grid_world.getGridWorldRows(), grid_world.getGridWorldCols());

	//LPA*
	lpa_star = new LpaStar(grid_world.getGridWorldRows(), grid_world.getGridWorldCols());
	
	vertex start = grid_world.getStartVertex();
	currPos = &(grid_world.map[start.row][start.col]);
	oldType = '6';
	vertex goal = grid_world.getGoalVertex();
	
	cout << "(start.col = " << start.col << ", start.row = " << start.row << ")" << endl;
	//cout << "(currPos.col = " << currPos->col << ", currPos.row = " << currPos->row << ")" << endl;
	cout << "(goal.col = " << goal.col << ", goal.row = " << goal.row << ")" << endl;
	
	//cout << "Start D* init\n";
	D_Star_Lite->initialise(start.col, start.row, goal.col, goal.row);
	//cout << "End D* init, Start lPA* init\n";
	lpa_star->initialise(start.col, start.row, goal.col, goal.row);
	//cout << "End init\n";
	
	//cout << "Start Copy\n";
	copyDisplayMapToMaze(grid_world, D_Star_Lite);
	copyDisplayMapToMaze(grid_world, lpa_star);
	//cout << "End Copy\n";
	//----------------------------------------------------------------
	//cout << "Pre World Boundary\n";
	worldBoundary = grid_world.getWorldBoundary();
	//cout << "World Boundary Found\n";
	deviceBoundary = grid_world.getDeviceBoundary();
	//cout << "Device Boundary Found\n";
	GRIDWORLD_ROWS = grid_world.getGridWorldRows();
	//cout << "Rows Found\n";
	GRIDWORLD_COLS = grid_world.getGridWorldCols();
	//cout << "Cols Found\n";
	
	//setvisualpage(page);
	
	// keep running the program until the ESC key is pressed   
	//cout << "At While\n";
	while((GetAsyncKeyState(VK_ESCAPE)) == 0 ) {
			 setactivepage(page);
			 cleardevice();
	
		    action = getKey(); 
		
		    if(SHOW_MAP_DETAILS) 
				 grid_world.displayMapWithDetails();
			 else
			    grid_world.displayMap();
			// getch();
			 
			 switch(action){
				case 1: //Block selected cell
				{
					if( rowSelected != -1 && colSelected != -1){
						grid_world.setMapTypeValue(rowSelected-1, colSelected-1, '1');
						grid_world.initialiseMapConnections(); 
							grid_world.initialiseMapConnections(); 
						grid_world.initialiseMapConnections(); 
						
						rowSelected=-1;
						colSelected=-1;
					}
					action = -1;
					break;
				}
				
				case 105: 
					grid_world.displayMapWithKeyDetails();
					break;
				
				case 106: 
					break;
				
				case 13: //I (Info) key pressed
				{	
					//cout << "I Key\n";
					if (alg == 'L') {
						if (lpa_star->searchState > 0) {
							cout << "\n----------------------------\nLPA* Data:\n\n";
						}
					} else if (alg == 'D') {
						cout << "\n----------------------------\nD*Lite Data:\n\n";
					}

					if (firstMaxU < INF) {cout << "First Search - Max Q Length: " << firstMaxU << '\n';}
					if (secondMaxU < INF) {cout << "Second Search - Max Q Length: " << secondMaxU << '\n';}

					if (pathL < INF) {cout << "Max Path Length: " << pathL << '\n';}

					int rows = grid_world.getGridWorldRows();
					int cols = grid_world.getGridWorldCols();

					acc = 0;
					ex = 0;
					read = 0;
					write = 0;
					for (int i = 0; i < rows; i++) {
						for (int j = 0; j < cols; j++) {
							read = read + grid_world.map[i][j].reads;
							write = write + grid_world.map[i][j].writes;
							if (grid_world.map[i][j].status == '1') {
								acc++;
							} else if (grid_world.map[i][j].status == '2') {
								acc++;
								ex++;
							}
						}
					}
					if (acc > 0) {cout << "Vertex Accesses: " << acc << '\n';}
					if (ex > 0) {cout << "Vertex Expansions: " << ex << '\n';}

					if (read > 0) {cout << "Vertex Reads: " << read << '\n';}
					if (write > 0) {cout << "Vertex Writes: " << write << '\n';}

					if (firstSearchTime < INF) {cout << "First Search - Run Time: " << (double)(firstSearchTime) / CLOCKS_PER_SEC * 1000 << "ms\n";}
					if (secondSearchTime < INF) {cout << "Second Search - Run Time: " << (double)(secondSearchTime) / CLOCKS_PER_SEC * 1000 << "ms\n";}
					Sleep(200);
					break;
				}
				
				case 107: // F7 - Run LPA*
				{
					switch (lpa_star->searchState){
						case 0:
						{	
							startTime = clock();
							pathFound = lpa_star->computeShortestPath();
							endTime = clock();
							firstSearchTime = endTime - startTime;
							firstMaxU = lpa_star->maxU;							
							lpa_star->searchState = 1;
							break;
						}
						case 1:
						{
							updateMap(grid_world, lpa_star);
							copyDisplayMapToMaze(grid_world, lpa_star);
							startTime = clock();
							pathFound = lpa_star->computeShortestPath();
							endTime = clock();
							secondSearchTime = endTime - startTime;
							secondMaxU = lpa_star->maxU;	
							//cout << "LPA* Second Search Run time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000 << "ms\n";
							lpa_star->searchState = 2;
							break;
						}
						default:
							break;
					}
					//lpa_star->computeShortestPathStep(15);
					///cout << "Compute Done\n";
					if (pathFound) {
						copyMazeToDisplayMap(grid_world, lpa_star);
						cout << "copied lpa_star's 'maze' to display 'map'" << endl;
						alg = lpa_star->alg;
					}
					
					Sleep(200);
					break;
				}
				
				case 201: //Down Arrow
				{
					if (alg == 'D') {
						vertex * robV = grid_world.findMinNeighbour(currPos);

						bool changes = updateMap(grid_world, D_Star_Lite, robV->row,  robV->col);
						if (changes) {
							D_Star_Lite->km = D_Star_Lite->km + D_Star_Lite->calc_H(currPos->row, currPos->col);
							D_Star_Lite->sLast = &(D_Star_Lite->maze[currPos->row][currPos->col]);
							D_Star_Lite->updateHValues();

							startTime = clock();
							D_Star_Lite->computeShortestPath();
							endTime = clock();
							
							secondSearchTime = (endTime - startTime);
							secondMaxU = D_Star_Lite->maxU;

							//cout << "D* Second Search Run time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000 << "ms\n";
						} 
						if ((currPos->row != start.row) || (currPos->col != start.col)) {
							grid_world.setMapTypeValue(currPos->row, currPos->col, oldType);
						}
						
						//cout << "\nrobV: (" << (char)((robV->row-1) + 'A') << " " << (robV->col-1) << ")\n";

						oldType = grid_world.getMapTypeValue(robV->row, robV->col);
						grid_world.setMapTypeValue(robV->row, robV->col, '6');

						currPos = robV;			

						copyDisplayMapToMaze(grid_world, D_Star_Lite);

						Sleep(200);
					}
					break;
				}
				
				case 108: //F8 - Run D*Lite
				{
					//D_Star_Lite->computeShortestPathStep(10);
					startTime = clock();
					D_Star_Lite->computeShortestPath();
					endTime = clock();
					firstSearchTime = endTime - startTime;
					firstMaxU = D_Star_Lite->maxU;
					//cout << "D* First Search Run time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000 << "ms\n";
					///cout << "Compute Done\n";
					copyMazeToDisplayMap(grid_world, D_Star_Lite);
					cout << "copied D_Star_Lite's 'maze' to display 'map'" << endl;
					alg = D_Star_Lite->alg;
					lpa_star->searchState = 0;
					Sleep(200);
					break;
				}
				
				case 15:
				{
					if( rowSelected != -1 && colSelected != -1){
						grid_world.displayVertexConnections(rowSelected-1, colSelected-1);
						//cout << "display connections" << endl;
						rowSelected=-1;
						colSelected=-1;
					} else {
						cout << "invalid selected vertex, please select a vertex first." << endl;
						break;
					}
					//--------------------------------------------
					//Sleep(200);
					action = -1;
					break;
				}		
				
				case 16:
				{
					if(grid_world.isGridMapInitialised()){
						grid_world.displayMapConnections();
						//cout << "display connections" << endl;
						//~ rowSelected=-1;
						//~ colSelected=-1;
					} else {
						cout << "map has not been initialised yet." << endl;
						break;
					}
					//--------------------------------------------
					action = -1;
					break;		
			 		}
				
				case 6: //set cell as new START vertex 
				{
					//--------------------------------------------
					// retrieve current START vertex
					vertex s = 	grid_world.getStartVertex();
					if( (s.row != -1) && (s.col != -1) ){
						//set current START VERTEX to an ordinary TRAVERSABLE CELL
						grid_world.setMapTypeValue(s.row, s.col, '0'); 
						grid_world.initialiseMapConnections(); 
						//ok, proceed
					} else {
						cout << "invalid START vertex" << endl;
						break;
					}
					//--------------------------------------------
					//set selected cell as the NEW START VERTEX
					if( rowSelected != -1 && colSelected != -1){
						grid_world.setMapTypeValue(rowSelected-1, colSelected-1, '6');
						s.row = rowSelected-1;
						s.col = colSelected-1;
						grid_world.setStartVertex(s);
						
						rowSelected=-1;
						colSelected=-1;
					} else {
						cout << "invalid new START vertex, please select a new START vertex first." << endl;
						break;
					}
					//--------------------------------------------
					action = -1;
					break;
					}
				
				case 7: //set cell as new GOAL vertex 
				{
					//--------------------------------------------
					// retrieve current GOAL vertex
					vertex s = 	grid_world.getGoalVertex();
					if( (s.row != -1) && (s.col != -1) ){
						//set current GOAL VERTEX to an ordinary TRAVERSABLE CELL
						grid_world.setMapTypeValue(s.row, s.col, '0'); 
						
						//ok, proceed
					} else {
						cout << "invalid GOAL vertex" << endl;
						action = -1;
						break;
					}
					//--------------------------------------------
					//set selected cell as the NEW GOAL VERTEX
					if( rowSelected != -1 && colSelected != -1){
						grid_world.setMapTypeValue(rowSelected-1, colSelected-1, '7');
						s.row = rowSelected-1;
						s.col = colSelected-1;
						grid_world.setGoalVertex(s);
						grid_world.initialiseMapConnections(); 
						
						rowSelected=-1;
						colSelected=-1;
					} else {
						cout << "invalid new GOAL vertex, please select a new GOAL vertex first." << endl;
						action = -1;
						break;
					}
					//--------------------------------------------
					action = -1;
					break;
					}
							
				case 109: //Copy 'map' to 'maze'
				{					
					copyDisplayMapToMaze(grid_world, D_Star_Lite);
					cout << "copied display map to D_Star_Lite's maze" << endl;
					copyDisplayMapToMaze(grid_world, lpa_star);
					cout << "copied display map to lpa_star's maze" << endl;
					action = -1;
					Sleep(200);
					break;
				}	
				
				case 110:	//F10 - Copy 'Maze' to 'map' + Display Path
				{				
					if (alg == 'D') { //Last used algorithim D*
						copyMazeToDisplayMap(grid_world, D_Star_Lite);
						cout << "copied D_Star_Lite's maze to display map" << endl;
						grid_world.displayPath(D_Star_Lite->sLast, D_Star_Lite->goal);
						pathL = grid_world.pathLength;

					} else if (alg == 'L') { ////Last used algorithim LPA*
						copyMazeToDisplayMap(grid_world, lpa_star);
						cout << "copied lpa_star's maze to display map" << endl;
						grid_world.displayPath(lpa_star->goal, lpa_star->start);
						pathL = grid_world.pathLength;
					}
					
					action = -1;					
					//Sleep(200);
					break;
				}
				
				case 9: //display g-values only
					grid_world.displayMapWithSelectedDetails(true, false, false, false);  //(bool display_g, bool display_rhs, bool display_h, bool display_key) 
					action = -1;
					break;
				case 10: //display h-values only
					grid_world.displayMapWithSelectedDetails(false, false, true, false);  //(bool display_g, bool display_rhs, bool display_h, bool display_key) 
					action = -1;
					break;
				case 11: //display key-values only
					//D_Star_Lite->updateAllKeyValues();
					//copyMazeToDisplayMap(grid_world, D_Star_Lite);
					grid_world.displayMapWithSelectedDetails(false, false, false, true);  //(bool display_g, bool display_rhs, bool display_h, bool display_key) 
					action = -1;
					break;
				
				case 12: //make cell Traversable
			 	{
					 if( rowSelected != -1 && colSelected != -1){
						 grid_world.setMapTypeValue(rowSelected-1, colSelected-1, '0');
						 
						 rowSelected=-1;
						 colSelected=-1;
					 }
					 action = -1;
					 break; 
				 }	 
				
				case 14: 
					   grid_world.displayMapWithPositionDetails();
						action = -1;
				      break;	 
					 
				 //~ default: //Display grid without details
					   //~ grid_world.displayMap();
						  
				  
				 
		    };
		
	   //----------------------------------------------------------------------------------------------------------------	  
		// Mouse handling
		//
			 if(mousedown()){
						 				
				ANIMATE_MOUSE_FLAG=true;
				 			 
				mX = mousecurrentx();
				mY = mousecurrenty();
				 
				//if the goal selected is within the playing field boundaries
				if(mX >= grid_world.getFieldX1() && mX <= grid_world.getGridMaxX() && mY >= grid_world.getFieldY1() && mY <= grid_world.getGridMaxY()){
					
					    circle(mX, mY, 3);
					    validCellSelected = true;
  	            
				} else {
					validCellSelected = false;
				}
			 } //end of mousedown()
			 //------------------------------------------------------------------------------------------------------------------
			 /////////////////////////////////////////////////////////////////////////////
			 						 
			 if(ANIMATE_MOUSE_FLAG){
					
				  //draw Cross-hair to mark Goal	    
				  setcolor(RED);
				  circle(mX, mY, 20);
				  line(mX,mY-20,mX,mY+20);
				  line(mX-20,mY,mX+20,mY);
				  //end of draw Cross-hair 
			 
				  // special effect to display concentric circles locating the target
					setcolor(YELLOW);
					
					if(mouseRadius < 40) {
						mouseRadius += 1;
					}
					circle(mX, mY, mouseRadius);
					//Sleep(50);
									
					if(mouseRadius >= 40) {
						ANIMATE_MOUSE_FLAG=false;
						mouseRadius=0;
					}
					//end of special effect
			  }

			 
			 /////////////////////////////////////////////////////////////////////////////
			  char info[80]; 
			  float wX, wY;
			  
			  wX = xWorld(worldBoundary,deviceBoundary,mX);
			  wY = yWorld(worldBoundary,deviceBoundary,mY);
			  sprintf(info,"x: %d, y: %d",mX, mY); 
			  drawInformationPanel(grid_world.getFieldX2(), grid_world.getFieldY1() + textheight("H")*2, info);
			  
			 
			  sprintf(info,"wX: %3.0f, wY: %3.0f",wX, wY); 
			  drawInformationPanel(grid_world.getFieldX2(),grid_world.getFieldY1() + textheight("H")*5, info);
			 ///////////////////////////////////////////////////////////////////////////// 
			 
			  //~ CellPosition p;
			  //~ int rowSelected, colSelected;
			  
			  if(validCellSelected) {
				  p=grid_world.getCellPosition_markCell(mX, mY);
				  rowSelected = p.row;
				  colSelected = p.col;
				  
				  sprintf(info,"row: %d, col: %d",rowSelected, colSelected); 
			      drawInformationPanel(grid_world.getFieldX2(),grid_world.getFieldY1() + textheight("H")*6, info);
				  
			  }
			  setvisualpage(page);
			  page = !page;  //switch to another page
	}
}



//main.exe grid_lpa_journal.map m

int main(int argc, char *argv[]) {	
	char gridFileName[80];
	if (argc == 3){
        

	    string heuristic(argv[2]);
	    std::transform(heuristic.begin(), heuristic.end(), heuristic.begin(),::tolower);
     
		strcpy(gridFileName, argv[1]);
        
		//heuristic function selection
		if((heuristic.compare("euclidean")==0) || (heuristic.compare("e")==0)){
			HEURISTIC = EUCLIDEAN;
			cout << "Heuristic function = EUCLIDEAN" << endl;
		}
		if((heuristic.compare("manhattan")==0) || (heuristic.compare("m")==0)){
			HEURISTIC = MANHATTAN;
			cout << "Heuristic function = MANHATTAN" << endl;
		}	

	} else {
		cout << "missing parameters:  gridworld heuristic" << endl;
		cout << "Example: ./main .\\grids\\grid_Dstar_journal.map m" << endl;
	}
	
	int graphDriver = 0,graphMode = 0;
 	
	//initgraph(&graphDriver, &graphMode, "", 1440, 900); // Start Window
 	//initgraph(&graphDriver, &graphMode, "", 1280, 1024); // Start Window
	
    initgraph(&graphDriver, &graphMode, "", 1360, 768); // Start Window - LAPTOP SCREEN
	//initgraph(&graphDriver, &graphMode, "", 1920, 1080); // Start Window - Full-HD
	
	BACKGROUND_COLOUR = WHITE;
	LINE_COLOUR = GREEN;
	
	GRIDWORLD_ROWS = 0; //7; //6; //duplicated in GridWorld
    GRIDWORLD_COLS = 0; //15;//13; //duplicated in GridWorld
	SHOW_MAP_DETAILS=false;

    try{
		runSimulation(gridFileName);
    }
	
    catch(...){
    	cout << "Exception caught!\n";
    }
	
	cout << "----<< The End.>>----" << endl;
	
	return 0;
} 

