// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for SIR on networks by Petter Holme (2018)


#include "sir.h"

extern GLOBALS g;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// reads the network, assumes an edge list with vertex label 0,N-1
// if your network has nodes with degree zero, make sure that none of them is
// the node with largest index

void read_data (FILE *fp) {
	unsigned int i, me, you;

	g.n = 0;

	// scan the system size
	while (2 == fscanf(fp, "%u %u\n", &me, &you)) {
		if (g.n < me) g.n = me;
		if (g.n < you) g.n = you;
	}

	g.n++;

	// allocate heap and list of nodes
	g.heap = malloc(g.n * sizeof(NODE*));
	g.nodes = malloc(g.n * sizeof(NODE));

	rewind(fp);

	// scan the degrees
	while (2 == fscanf(fp, "%u %u\n", &me, &you)) {
                g.nodes[me].deg++;
		g.nodes[you].deg++;
	}

	// allocate adjacency lists
	for (i = 0; i < g.n; i++) {
	        g.nodes[i].nb = malloc(g.nodes[i].deg * sizeof(NODE*));
	        g.nodes[i].deg = 0;
	}

	rewind(fp);

	// fill adjacency lists
	while (2 == fscanf(fp, "%u %u\n", &me, &you)) {
   	        g.nodes[me].nb[g.nodes[me].deg++] = &g.nodes[you];
	        g.nodes[you].nb[g.nodes[you].deg++] = &g.nodes[me];
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
