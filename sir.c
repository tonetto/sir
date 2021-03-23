// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for SIR on networks by Petter Holme (2018)

#include "sir.h"

GLOBALS g;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// this routine does the bookkeeping for an infection event

void infect () {
        unsigned int i;
	NODE *you, *me = g.heap[1];
	float t, now = me->time;

	del_root();
	me->I_OR_R = TRUE;
	// get the recovery time
	me->time += g.rexp[pcg_16()] * g.beta; // bcoz g.rexpr has a / g.beta factor
	if (me->time > g.t) g.t = me->time;
	g.s++;

	// go through the neighbors of the infected node
	for (i = 0; i < me->deg; i++) {
		you = me->nb[i];
		if (you->I_OR_R == FALSE) { // if `you` is S, `you` can be infected
			t = now + g.rexp[pcg_16()]; // get the infection time
			if ((t < me->time) && (t < you->time)) {
				you->time = t;
				if (you->is_heaped == FALSE) {
				    g.heap[++g.nheap] = you;
				    you->is_heaped = TRUE;
				    you->heap = g.nheap;
				}
				up_heap(you->heap); // this works bcoz the only heap relationship that can be violated is the one between `you` and its parent
			}
		}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// this routine runs one SIR outbreak from a random seed node

void sir () {
	unsigned int i, source;
	
	g.t = 0.0;
	g.s = 0;

	// initialize
	for (i = 0; i < g.n; i++) {
	        g.nodes[i].is_heaped = FALSE;  // if a node is already in the heap
     	        g.nodes[i].time = DBL_MAX;  // to a large value
		g.nodes[i].I_OR_R = FALSE;  // not I_OR_R
		g.nodes[i].heap = 0;        // will keep the location of a node in the heap
	}

	// get & infect the source
	source = pcg_32_bounded(g.n);
	g.nodes[source].time = 0.0;
	g.nodes[source].heap = 1;
	g.heap[g.nheap = 1] = &g.nodes[source];

	// run the outbreak
	while (g.nheap > 0) infect();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// main function handling input

int main (int argc, char *argv[]) {
	unsigned int i;
	double st1 = 0.0, st2 = 0.0, ss1 = 0.0, ss2 = 0.0; // for averages
	FILE *fp;
	
	// just a help message
	if (argc != 4) {
		fprintf(stderr, "usage: ./sir [nwk file] [beta] [seed]\n");
		return 1;
	}

	// read stuff
	g.beta = atof(argv[2]);
	g.state = (uint64_t) strtoull(argv[3], NULL, 10);
	 
	// read network data file
	fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "can't open '%s'\n", argv[1]);
		return 1;
	}
	read_data(fp);
	fclose(fp);

	for (i = 0; i < 0x10000; i++)
		g.rexp[i] = -log((i + 1.0) / 0x10000) / g.beta;

	// run the simulations and summing for averages
	for (i = 0; i < NAVG; i++) {
		sir();
		// saving stats for averages
		ss1 += (double) g.s;
		ss2 += SQ((double) g.s);
		st1 += g.t;
		st2 += SQ(g.t);
	}

	// make averages
	ss1 /= NAVG;
	ss2 /= NAVG;
	st1 /= NAVG;
	st2 /= NAVG;

	// print result
	printf("avg. outbreak size: %g (%g)\n", ss1, sqrt((ss2 - SQ(ss1)) / (NAVG - 1)));
	printf("avg. time to extinction: %g (%g)\n", st1, sqrt((st2 - SQ(st1)) / (NAVG - 1)));

	// cleaning up
	for (i = 0; i < g.n; i++) free(g.nodes[i].nb);
	free(g.nodes);
	free(g.heap);
	 
	return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
