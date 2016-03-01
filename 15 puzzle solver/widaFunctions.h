#ifndef __widaFunctions__
#define __widaFunctions__

/** Relevant Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <time.h>

/** Constants */
#define INFINITY INT_MAX
#define WEIGHT 1.5
#define BLANK 0
#define SIZE 16
#define INVALID -1
#define COST_VALUE 1
#define INDEXBOUND 3
#define DIMENSIONS 2
#define MAX_POSSIBLE_MOVES 4
#define COLHEIGHT 4
#define ROWWIDTH 4

/** Structure of the node used */
typedef struct node
{
    int threshold;            /*  f(n)  */
    int cost;                    /*  g(n)  */
    int *state;                 /* state */
}node;

/** Returns the unweighted heuristic of the state passed in */
int heuristicUnweightedValue(int state[SIZE]);

/** Returns the manhattan distance between two points */
int manhattanDistance(int value, int position);

/** Control Loop for the IDA algorithm */
node *IDAControlLoop(int state[SIZE], node *Node, long long *generatedNodes, long long *expandedNodes);

/** IDA* Algorithm for solving */
node *iterativeDeepeningAStar(node *Node, int bound, int *tempBound, long long *generatedNodes, long long *expandedNode);

/** Generate all possible moves for a given state */
void generateMoves(node *Node, int moveList[MAX_POSSIBLE_MOVES]);

/** Check if the generated move was valid */
int checkActions(int x, int y);

/** Perform the move on the stage and update it in the new state */
int *actionState(int position, int state[SIZE], int newState[SIZE]);

/** Returns the minimum of two integers */
int minimum(int a, int b);

/** Print the initial state */
void printInitialState(int state[SIZE]);

#endif /** widaFunctions */