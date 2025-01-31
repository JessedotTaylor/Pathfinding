#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__


#include <vector>

/*******************************************************************************************************************/
//------------------------------------------
//Flags - enable or disable features


#define INCREMENTAL_SEARCH_ALGORITHM
#define RL_ALGORITHM

#define EIGHT_CONNECTED_GRIDWORLD
//#define FOUR_CONNECTED_GRIDWORLD

//#define SHOW_DEBUG_INFO false
//#define SHOW_DEBUG_INFO true

#define MANHATTAN	1
#define EUCLIDEAN	2

//------------------------------------------

#define INF  1000000

//-------------------------------------------------------------------------------
#ifdef FOUR_CONNECTED_GRIDWORLD
 
	//4-connected gridworld
	#define DIRECTIONS 4
	const struct {
	  int x;
	  int y;
	} neighbours[4]={{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

	/////////////////////////////////////////////////////

#endif

const double SQRT_2 =  1.4142135623731;

//-------------------------------------------------------------------------------
#ifdef EIGHT_CONNECTED_GRIDWORLD

	//8-connected gridworld
	#define DIRECTIONS 8
	#define CORNER_COST SQRT_2
	
	//movement sequence, used in the journal
	const struct {
	  int i;
	  int j;
	  double cost;
	} neighbours[8]={ {-1,-1, CORNER_COST}, {0, -1, 1}, {1, -1, CORNER_COST}, 
					{-1, 0, 1}, {1, 0, 1}, 
					{-1, 1, CORNER_COST}, {0, 1, 1}, {1, 1, CORNER_COST} };
			
	//clockwise, starting at 3 o'clock			
	//~ const struct {
	  //~ int x;
	  //~ int y;
	//~ } succ[8]={ {1,0}, {1, 1}, {0,1}, {-1, 1}, {-1, 0}, {-1,-1}, {0, -1}, {1, -1} };
	
#endif
//-------------------------------------------------------------------------------

//------------------------------------------
	
extern int numberOfExpandedStates;
extern int numberOfVertexAccesses;
extern int maxQLength;	
extern int qLengthAfterSearch;
	
extern bool MAP_INITIALISED;
extern bool PRECALCULATED_GRIDWORLD_READY;

//~ extern bool USE_EUCLIDEAN_DISTANCE_HEURISTIC;
extern unsigned int HEURISTIC;

//Robot soccer dimensions	
extern int GRIDWORLD_ROWS; 
extern int GRIDWORLD_COLS;

//////////////////////////////////////////////////////////////////////////////
//REINFORCEMENT LEARNING

//extern int MAX_ACTIONS;

//end_REINFORCEMENT LEARNING
//////////////////////////////////////////////////////////////////////////////
using namespace std;

enum cellType{TRAVERSABLE=0, BLOCKED=1, UNKNOWN=9};
enum vertexStatus{UNEXPLORED=0, EXPANDED=1, ACCESSED=2};

 struct CellPosition
{
	int row;
	int col;
};

 struct Coordinates
{
	int x, y;
};


struct Key
{
	double keyV[2];
};


typedef struct {
  int y;
  int x;
} loc_t;




struct vertex
{
	
#ifdef INCREMENTAL_SEARCH_ALGORITHM	
	double getRHS(void) {reads++; return rhs;}
	void setRHS(double x) {writes++; rhs = x;}

	double getG(void) {reads++; return g;}
	void setG(double x) {writes++; g = x;}

	double getH(void) {reads++; return h;}
	void setH(double x) {writes++; h = x;}

	double getKey(int val) {reads++; return key[val];}
	void setKey(int val, double x) {writes++; key[val] = x;}

	vertex * getMove(int val) {reads++; return move[val];}
	void setMove(int val, vertex * x) {writes++; move[val] = x;}

	double getLinkCost(int val) {reads++; return linkCost[val];}
	void setLinkCost(int val, double x) {writes++; linkCost[val] = x;}




    double rhs;
    double g;
	 int c;
	 double h;
	 double f;
	 double key[2];
	 vertex* move[DIRECTIONS];
    double linkCost[DIRECTIONS];
	//Neighbours neighbourData;
#endif	
	
	
	
	
#ifdef RL_ALGORITHM
	
	#ifdef EIGHT_CONNECTED_GRIDWORLD
    double Q[8];
		
	#elif FOUR_CONNECTED_GRIDWORLD
    double Q[4];
	#endif
	
	 double sumQ;
	 double maxQ;
	
	 int reward;
#endif	
	
	 //--------------------------------------------------------------------------------- 
	 //TYPE: 0 - traversable, 1 - blocked, 9 - unknown, 6 - start vertex, 7 - goal vertex
    char type; 
	//---------------------------------------------------------------------------------
	char getType(void) {reads++; return type;}
	void setType(char x) {writes++; type = x;}

	int row;
	int col;

	int getRow(void){reads++; return row;}
	void setRow(int x){writes++; row = x;}

	int getCol(void){reads++; return col;}
	void setCol(int x){writes++; col = x;}

	char status = '0';
	
	int x1,y1,x2,y2;
	Coordinates centre; //centre x, centre y

	int reads = 0;
	int writes = 0;

}; 

extern int MAX_MOVES;

extern vector<vector<vertex> > map;
extern vertex startVertex;
extern vertex goalVertex;



typedef struct vector<CellPosition> PathType;

/*******************************************************************************************************************/
extern int fieldX1, fieldY1, fieldX2, fieldY2; //playing field boundaries
extern float WORLD_MAXX;
extern float WORLD_MAXY;
// colour constants
extern int BACKGROUND_COLOUR;
extern int LINE_COLOUR;

extern int cellWidth;
extern int cellHeight;

/********************************************************************************************************************/
//8-connected gridworld
#define DIRECTIONS 8 //clockwise sequence of moves (8-connected gridworld)



struct LpaStarCell
{
    LpaStarCell* move[DIRECTIONS];
    LpaStarCell* predecessor[DIRECTIONS];
	 double linkCost[DIRECTIONS];
    LpaStarCell* parent;
    LpaStarCell* trace;
    short obstacle;
    int x, y;

	 double g;
    double rhs;
	 double h;
    double key[2];
	
	 //~ int g;
    //~ int rhs;
    //~ int key[2];
	
	
	 //---------------------
	 //TYPE: 0 - traversable, 1 - blocked, 9 - unknown, 6 - start vertex, 7 - goal vertex
	 char type;
	 //----------------------
	

	
    int generated;
    int heapindex;
};

extern bool SHOW_MAP_DETAILS;

/********************************************************************************************************************/



#endif
