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

/** Main Function */
int main(int argc, const char * argv[])
{
    /** Variable Declarations */

    /** Clock Timer */
    clock_t start = clock(), diff;  

    /** Nodes */
    node *Node = malloc(sizeof(node));
    node *result = malloc(sizeof(node));

    /** Counters for generated and expaned nodes */
    long long *generatedNodes = malloc(sizeof(long long));
    long long *expandedNodes = malloc(sizeof(long long));

    /** Initial state of the board */
    int state[SIZE];

    /** Get the input stream*/
    FILE *inputFile = fopen(argv[1], "r");

    /** Check for valid file */
    if(inputFile == 0)
    {
        printf("Could not open file\n");
        return 0;
    }

    /** Perform action once a state is read in */
    int position = 0;
    int num;

    while(fscanf(inputFile, "%d", &num) == 1)
    {
          state[position] = num;
          position++;
    }

    /** State has been recorded so input stream is closed */
    fclose(inputFile);

    /** Perform calculation */    
    result = IDAControlLoop(state, Node, generatedNodes, expandedNodes);

    /** Record the time when puzzle was finished */
    diff = clock() - start;
    float seconds = (diff * 1000 / CLOCKS_PER_SEC) / 1000.0;

    /** Print the results once solving has completed */
    printf("Generated = %"PRIu64" \n", *generatedNodes);
    printf("Expanded =  %"PRIu64" \n", *expandedNodes);
    printf("Time = %.2f\n", seconds);
    printf("Expanded/Second = %.0f\n", (*expandedNodes * 1.0) / seconds);
    

    return 0;
}