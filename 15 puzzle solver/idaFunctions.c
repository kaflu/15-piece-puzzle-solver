/** Relevant Libraries */

#include "idaFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <time.h>

/** Constants */
#define INFINITY INT_MAX
#define BLANK 0
#define SIZE 16
#define INVALID -1
#define COST_VALUE 1
#define INDEXBOUND 3
#define DIMENSIONS 2
#define MAX_POSSIBLE_MOVES 4
#define COLHEIGHT 4
#define ROWWIDTH 4

/** Control Loop Logic */
node *IDAControlLoop(int state[SIZE], node *Node, long long *generatedNodes, long long *expandedNodes)
{
    /** Pointers and Variables */
    node *result = NULL;
    *generatedNodes = 0;
    *expandedNodes = 0;

    int *prevBlankPosition = malloc(sizeof(int));
    int *tempBound  = malloc(sizeof(int));

    /** Let bound equal the heuristic of the state
      * B := H(s0)
      */
    int bound = heuristicUnweightedValue(state);

    /** Log the initial states */
    printInitialState(state);

    /** Log the Heuristic Estimate */
    printf("Initial Estimate: %d\n", bound);

    /** Log the Threshold as it updates */
    printf("Threshold = ");

    while(result == NULL)
    {
        /** Set the temporary bound to INFINITY */
        *tempBound = INFINITY;

        /** Copy the state into the starting node */
        Node->state = state;

        /** Set the cost to zero */
        Node->cost = 0;

        /** Run the iterative deepening algorithm */
        result = iterativeDeepeningAStar(Node, bound, tempBound, generatedNodes, expandedNodes, prevBlankPosition);

        /** Print the bound when it's updating */
        printf("%d ", bound);

        /** If result is null, set the bound to the temporary bound */
        if(result == NULL) {bound = *tempBound;}
    } 

    /** Solution will have to be the last bound */
    printf("\nSolution = %d\n", bound);

    return result;
}

/** IDA* Algorithm */
node *iterativeDeepeningAStar(node *Node, int bound, int *tempBound, long long *generatedNodes, long long *expandedNodes,  int *prevBlankPosition)
{
    /** Iterator */
    int i;
    
    /** Temporary Node and Result node*/
    node *tempNode = malloc(sizeof(node));
    node *result          = malloc(sizeof(node));

    /** Move list of possible actions */
    int moveList[MAX_POSSIBLE_MOVES];

    /** State after it has been moved */
    int newState[SIZE];

    /** Get the possible moves into move list */
    generateMoves(Node, moveList);

    /** IDA Star algorithm 
      * Loop through maximum actions possible 
      */
    for(i = 0; i < MAX_POSSIBLE_MOVES; i++)
    {
        /** Only perform valid moves */
        if(moveList[i] == INVALID) {continue;}
        
        /** A node was generated */
        *generatedNodes = *generatedNodes + 1;

        /** Update the state and store it in newState 
          * n'.s = f(a, n.s) 
          */
        tempNode->state = actionState(moveList[i], Node->state, newState, prevBlankPosition);

        /** Increment the cost 
          *  n'.g(n)  = n.g(n) + c(a, n.s)
          */
        tempNode->cost = Node->cost + COST_VALUE;
        
        /** Update the threshold 
          *  n'.f(n) = n'.g(n) + h(n'.s)
          */
        tempNode->threshold = tempNode->cost + heuristicUnweightedValue(tempNode->state);

        /* Check if the threshold is greater than the bound and set it to the minimum
         * Chek if n'.f(n) > B 
         */
        if(tempNode->threshold > bound) 
        {
        	*tempBound = minimum(tempNode->threshold, *tempBound);
        }

        else 
        {
            /* If we have reached our goal, return the node
             * h(n'.s) := 0 
             */
            if(heuristicUnweightedValue(tempNode->state) == 0) {return tempNode;}

            /** A node was expanded upon */
            *expandedNodes = *expandedNodes + 1;
           /*  printf("Expanding: %d TempBound: %d\n", *expandedNodes, *tempBound); */

            /** Recursively repeat */
            result = iterativeDeepeningAStar(tempNode, bound, tempBound, generatedNodes, expandedNodes, prevBlankPosition);

            if(result != NULL) {return result;}
        }  
    }

    /** Free the node */
    free(tempNode);    

    return NULL;
}

/** Perform the move on the state */
int *actionState(int movePosition, int state[SIZE], int newState[SIZE],  int *prevBlankPosition)
{
    /** Iterator */
    int i;

    /** Check if we are moving back to a previous Blank space 
      *  If we are, return the original state 
      */
    if(movePosition == *prevBlankPosition) {return state;}

    int blankIndex = 0;
    int value = state[movePosition];

    /** Find the blank index */
    for(i = 0; i < SIZE; i++)
    {
       if(state[i] == BLANK)
       {
            blankIndex = i;
            *prevBlankPosition = i;
       }
       
       /** Fill in the newState with the values of the old state */
       else
            newState[i] =  state[i];
    }

    /** Perform the swaps */
    newState[blankIndex] = value;
    newState[movePosition] = BLANK;

    return newState;
}

/** Generate possible moves */
void generateMoves(node *Node, int moveList[MAX_POSSIBLE_MOVES])
{
    /** Iterator */
    int i;
    
    /** 2D position of the BLANK space */
    int posX = 0, posY = 0;

    /** Moves possible stored into a 2D array */
    /** 
     * Up      {-1, 0}
     * Down  { 1, 0} 
     * Left     { 0,-1} 
     * Right   { 0, 1}
     */
    int movesPossible[MAX_POSSIBLE_MOVES][DIMENSIONS] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    /** Loop to find the BLANK space */
    for(i = 0; i < SIZE; i++)
    {
        if(Node->state[i] == BLANK)
        {
            /** Get the 2D position for the Blank space */
            posX = i / ROWWIDTH;
            posY = i % COLHEIGHT;
            break;
        }
    }

    /** Loop to determine the possible moves */
    for(i = 0; i < MAX_POSSIBLE_MOVES; i++)
    {
        /** The tile position after performing each of the moves possible */
        int x = posX + movesPossible[i][0];
         int y = posY + movesPossible[i][1];

        /** Check if it is a valid action */
        moveList[i] = checkActions(x, y);
    }
}

/** Check if the move was valid */
int checkActions(int x, int y)
{
    /** Shifts outside bound in x direction */
    if(x < 0 || x > INDEXBOUND)
        return INVALID;
    /** Shifts outside bound in y direction */
    else if((y < 0 || y > INDEXBOUND))
        return INVALID;
    /** Return the appropriate shift and store in move list array */
    else
        return y + x * ROWWIDTH; 
}

/** Function to find the heuristic value */
int heuristicUnweightedValue(int state[SIZE])
{
    /** Position and heuristic value */
    int position;
    int heuristicValue = 0;

    /** Loop through the array */
    for(position = 0; position < SIZE; position++)
    {
        heuristicValue += manhattanDistance(state[position], position);
    }

    return heuristicValue;
}

/** Function to find the Manhattan threshold */
int manhattanDistance(int value, int position)
{
    /** If the value is a blank space, ignore it */
    if(value == BLANK) { return 0;}

    else
    {
        /** The 2D position of the input state */
        int x, y;

        x = position  /  ROWWIDTH;
        y = position % COLHEIGHT;


        /** The 2D position of the goal state */
        int dX, dY;

        dX  = value  /  ROWWIDTH;
        dY  = value % COLHEIGHT;


        /** Return the Manhattan threshold */
        return (abs(x - dX) + abs(y - dY));
    }
}

/** Return the minimum of two integers */
int minimum(int a, int b) {return (a < b) ? a:b;}

/** Print out the initial state as a 2D array */
void printInitialState(int state[SIZE])
{
    /** Iterators */
    int i, count = 0;

    /** Initial State */
    printf("Initial State:\n");
    for(i = 0; i < SIZE; i++)
    {
        if(count == ROWWIDTH - 1)
        {
            count = 0;
            printf("%3d\n",state[i]);
        }
        else
        {
            printf("%3d",state[i]);
            count++;
        }
    }    
}