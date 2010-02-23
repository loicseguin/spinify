#include "tezuka.h"

unsigned int s1, s2, b;

void
set_seed (void)
{
	struct timeval tp;		// Variables to access the  
	struct timezone tpz;		// computer's internal clock.
	
	// Initialization of seeds.
	
	for (s1 = Mask1; s1 >= Mask1 || s1 == 0;) {
		gettimeofday (&tp, &tpz);
		s1 = tp.tv_usec;
	}
	
	for (s2 = Mask2 + 1; s2 > Mask2 || s2 == 0;) {
		gettimeofday (&tp, &tpz);
		s2 = tp.tv_usec;
	}
}

unsigned int
alea (void)
{
	static int flag = 0;
	if (flag == 0) {
		set_seed();
		flag = 1;
	}
	b = ((s1 << Q1) ^ s1) & Mask1;
	s1 = ((s1 << S1) ^ (b >> P1mS1)) & Mask1;
	b = ((s2 << Q2) ^ s2) & Mask2;
	s2 = ((s2 << S2) ^ (b >> P2mS2)) & Mask2;
	return (s1 ^ (s2 << P1mP2));
}
