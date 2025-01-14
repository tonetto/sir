// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for SIR on networks by Petter Holme (2018)

// routines for maintaining the binary heap (for the priority queue)
// the root of the heap is 1 (although it is allocated from 0) for simplicity

#include "sir.h"

extern NODE *n;
extern GLOBALS g;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// performing down_heap (a.k.a. percolate down)
// it restores the heap property if there is an inconsistency between 'here'
// and its children (and no other inconsistencies)

void down_heap (unsigned int here) {
	unsigned int utmp, smallest = here;
	unsigned int left = here << 1; // = here * 2 (I know this is silly and saves no time)
	unsigned int right = left | 1; // = left + 1

	while (left <= g.nheap) {
	  smallest = left;
	  if ((right <= g.nheap) && (n[g.heap[right]].time < n[g.heap[smallest]].time)) {
	    smallest = right;
	  }

	  if (n[g.heap[smallest]].time <= n[g.heap[here]].time) return;

	  // swap smallest and here
	  utmp = g.heap[smallest];
	  g.heap[smallest] = g.heap[here];
	  g.heap[here] = utmp;

	  n[g.heap[smallest]].heap = smallest;
	  n[g.heap[here]].heap = here;

	  // reset initial variables for next iteration
	  here = smallest;
	  left = here << 1;
	  right = left | 1;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// deleting the root of the heap

void del_root () {

	g.heap[1] = g.heap[g.nheap--];
	n[g.heap[1]].heap = 1;
	down_heap(1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// performing up_heap (a.k.a. percolate up)
// for adding an element to the heap

void up_heap (unsigned int start) {
	unsigned int above, here = start, mem = g.heap[start];

	while (here > 1) {
		above = here >> 1; // = here / 2
		
		if (n[mem].time >= n[g.heap[above]].time) break;
		g.heap[here] = g.heap[above];
		n[g.heap[here]].heap = here;
		
		here = above;
	}
	
	n[g.heap[here] = mem].heap = here;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
