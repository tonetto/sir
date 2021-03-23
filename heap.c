// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for SIR on networks by Petter Holme (2018)

// routines for maintaining the binary heap (for the priority queue)
// the root of the heap is 1 (although it is allocated from 0) for simplicity

#include "sir.h"

extern GLOBALS g;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// performing down_heap (a.k.a. percolate down)
// it restores the heap property if there is an inconsistency between 'here'
// and its children (and no other inconsistencies)

void down_heap (unsigned int here) {
        NODE *ntmp;
	unsigned int smallest = here;
	unsigned int left = here << 1; // = here * 2 (I know this is silly and saves no time)
	unsigned int right = left | 1; // = left + 1

	while (left <= g.nheap) {
	  smallest = left;
	  if ((right <= g.nheap) && (g.heap[right]->time < g.heap[smallest]->time)) {
	    smallest = right;
	  }

	  if (g.heap[smallest]->time <= g.heap[here]->time) return;

	  // swap smallest and here
	  ntmp = g.heap[smallest];
	  g.heap[smallest] = g.heap[here];
	  g.heap[here] = ntmp;

	  g.heap[here]->heap = here;
	  g.heap[smallest]->heap = smallest;

	  // reset variables for next iteration
	  here = smallest;
	  left = here << 1;
	  right = left | 1;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// deleting the root of the heap

void del_root () {
    g.heap[1] = g.heap[g.nheap--];
    g.heap[1]->heap = 1;
    down_heap(1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// performing up_heap (a.k.a. percolate up)
// for adding an element to the heap

void up_heap (unsigned int start) {
        unsigned int above, here = start;
	NODE *mem = g.heap[start];
	//NODE *ntmp;

	while (here > 1) {
		above = here >> 1; // = here / 2

		if (mem->time >= g.heap[above]->time) break;
		g.heap[here] = g.heap[above];
		g.heap[here]->heap = here;

		here = above;
	}

	g.heap[here] = mem;
	g.heap[here]->heap = here;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
