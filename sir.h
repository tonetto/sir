// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// code for SIR on networks by Petter Holme (2018)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

#define NAVG 100000 // number of runs for averages

//#define I_OR_R (UINT_MAX - 1)
#define NONE UINT_MAX

#define S(x) (n[(x)].time > now)

#define TRUE 1
#define FALSE 0

// auxiliary macro
#define SQ(x) ((x) * (x))

typedef struct GLOBALS {
	// INPUT PARAMETERS
	float beta; // infection rate
	// NETWORK SPECS
	unsigned int n;
	// OTHER GLOBALS
        unsigned int nheap;
        struct NODE **heap;
        struct NODE *nodes;
	// OUTBREAK STATS
	unsigned int s;
	float t;
	// FOR RNG
	uint64_t state;
	float rexp[0x10000];
} GLOBALS;

typedef struct NODE {
    unsigned int I_OR_R;
    unsigned int is_heaped;
    unsigned int heap;
    unsigned int deg; // degree
    struct NODE **nb;  // network neighbors
    float time;
} NODE;

// heap.c
extern void up_heap (unsigned int);
extern void del_root ();

// misc.c
extern void init_rng ();
extern void read_data (FILE *);

// pcg_rnd.c
extern uint16_t pcg_16 ();
extern uint32_t pcg_32 ();
extern uint32_t pcg_32_bounded (uint32_t);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
