/*
 * Header file for random number generator defined in
 *   Tezuka, S. and L'Ecuyer, P. 1991. Efficient and portable combined
 *   Tausworthe random number generators. ACM Trans. Model. Comput.
 *   Simul. 1, 2 (Apr. 1991), 99-112. DOI= http://doi.acm.org/10.1145/116890.116892
 *
 * Calling set_seed() sets the necessary seeds for the generator.
 * The first time it is called, alea() calls set_seed() so the user no
 * longer has to worry about this. Then, alea() returns a random number
 * between 1 and RANDMAX = 2147483648.
 *
 */


#ifndef TEZUKA_H
#define TEZUKA_H

#include <sys/time.h>

#define Q1                 13
#define Q2                  2
#define S1                 12
#define S2                 17
#define P1mS1              19
#define P2mS2              12
#define P1mP2               2
#define Mask1      2147483647
#define Mask2       536870911

#define RANDMAX   2147483648.	// 2^31, float
#define RANDINT   2147483648	// 2^31, integer
#define RANDMAX2  1073741824	// 2^30
#define NNN	  100000

unsigned int alea (void);
void set_seed (void);

#endif
