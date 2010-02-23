/*
 *  tezuka.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-02-22.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "tezuka.h"
#include <cstdlib>

unsigned int s1, s2, b;

void set_seed (void)
{
	// Initialization of seeds.
	srand(time(NULL));
	s1 = rand();
	s2 = rand();
	
	return;
}

unsigned int alea (void)
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
