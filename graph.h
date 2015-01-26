#ifndef GRAPH_H_
#define GRAPH_H_

#include "stdlib.h"
#include "stdio.h"
#include <limits.h>

typedef struct Graph {

	int** matrix;
	int vertices_count;
	int edges_count;

} Graph;

Graph* init_graph(int vertices_count);
void add_edge(Graph* g, int v1, int v2, int wart);
void print_graph(Graph* g);
int** Floyd_Warshall(Graph* g);
void detect_cycles(int** shortest_paths, int vertices_count);

#endif /* GRAPH_H_ */
