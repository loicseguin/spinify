/*
 *  swendsentest.cpp
 *  spinify2
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-02.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */


#include "../src/Graph.h"
#include "../src/Simul.h"
#include "../src/tezuka.h"
#include <iostream>
#include <iomanip>

void prGraph(Graph *pG, int L, int W) {
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < W; j++) {
			std::cout << std::setw(3) << (*pG)[i+j*W].getSpin();
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}


int main (int argc, char * const argv[]) {
	std::cout << "SPINIFY\nThe Ising model simulator\n\n";
	set_seed();
	Graph G;
	int L = 40;
	int W = 40;
	G.initRect(L, W);
	G.randSpin();
	Simul S(&G);
	//S.setParams(10, -1);
	prGraph(&G, L, W);
	for (int i = 0; i < 2000; i++) {
		S.thermalize();
	}
	prGraph(&G, L, W);
	
    return 0;
}
