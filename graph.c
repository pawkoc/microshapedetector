#include "graph.h"

Graph* init_graph(int vertices_count) {

	int i, j, tmp=1;
	Graph* new_graph = malloc(sizeof(Graph));

	if(new_graph == NULL) {
		printf("Error allocation graph\n");
		return NULL;
	}

	new_graph->matrix = malloc(sizeof(int*) * vertices_count);

	if(new_graph->matrix == NULL) {
		printf("Error allocation graph\n");
		return NULL;
	}

	for(i=0; i<vertices_count; i++) {
		new_graph->matrix[i] = malloc(sizeof(int) * tmp);

		if(new_graph->matrix[i] == NULL) {
			printf("Error allocation graph\n");
			return NULL;
		}

		for(j=0; j<tmp; j++) {
			new_graph->matrix[i][j] = 0;
		}

		tmp++;
	}

	new_graph->vertices_count = vertices_count;
	new_graph->edges_count = 0;

	return new_graph;
}

void add_edge(Graph* g, int v1, int v2, int wart) {

	int min, max;

	g->edges_count++;

	if(v1 > v2) {
		min = v2;
		max = v1;
	} else {
		min = v1;
		max = v2;
	}

	g->matrix[max][min] = wart;
}

void print_graph(Graph* g) {

	int i, j, tmp=1;
	for(i=0; i<g->vertices_count; i++, tmp++) {
		for(j=0; j<tmp; j++) {

			if(g->matrix[i][j]) {
				printf("%d <-> %d\n", i, j);
			}
		}
	}
}

int** Floyd_Warshall(Graph* g) {

	int i, j, k;

	int** shortest_paths = malloc(sizeof(int*) * g->vertices_count);
	if(shortest_paths == NULL) {
		printf("Error allocating shortest_paths\n");
		return NULL;
	}

	for(i=0; i<g->vertices_count; i++) {
		shortest_paths[i] = malloc(sizeof(int) * (i+1));

		if(shortest_paths[i] == NULL) {
			printf("Error allocating shortest_paths\n");
			return NULL;
		}

		for(j=0; j<=i; j++) {
			if(i == j)
				shortest_paths[i][j] = 0;
			else if(!g->matrix[i][j])
				shortest_paths[i][j] = INT_MAX/2;
			else
				shortest_paths[i][j] = g->matrix[i][j];
		}
	}

	for(i=0; i<g->vertices_count; i++) {
		for(j=0; j<=i; j++) {
			printf("sp[%d][%d] = %d ",i,j, shortest_paths[i][j]);
		}
		printf("\n");
	}

	int min_i, max_i, min_j, max_j;
	for(k=0; k < g-> vertices_count; k++) {
		for(i=0; i < g->vertices_count; i++) {
			for(j=0; j <= i; j++) {

				printf("k=%d, i=%d, j=%d\n", k, i, j);

//				if(i==2 && j==1)
//				printf("sp[%d][%d] = %d, sp[%d][%d] = %d, sp[%d][%d] = %d\n", i,j,shortest_paths[i][j],
//											i,k,shortest_paths[i][k], j,k,shortest_paths[j][k]);

				if(k>=i) {
					min_i = i;
					max_i = k;
				} else {
					min_i = k;
					max_i = i;
				}

				if(k>=j) {
					min_j = j;
					max_j = k;
				} else {
					min_j = k;
					max_j = j;
				}

				if(shortest_paths[i][j] > shortest_paths[max_i][min_i] + shortest_paths[max_j][min_j]) {
//					printf("sp[%d][%d] = %d, sp[%d][%d] = %d, sp[%d][%d] = %d\n", i,j,shortest_paths[i][j],
//							i,k,shortest_paths[i][k], j,k,shortest_paths[j][k]);
					shortest_paths[i][j] = shortest_paths[max_i][min_i] + shortest_paths[max_j][min_j];
				}
			}
		}
	}

	for(i=0; i<g->vertices_count; i++) {
		for(j=0; j<=i; j++) {
			printf("%d ", shortest_paths[i][j]);
		}
		printf("\n");
	}

	return shortest_paths;
}

void detect_cycles(int** shortest_paths, int vertices_count) {

	int i, j, k;

	for(k=0; k<vertices_count; k++) {
		for(i=0; i<vertices_count; i++) {
			for(j=0; j<=i; j++) {

				if(k!=i && k!=j) {

				}
			}
		}
	}
}
