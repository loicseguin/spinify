/*
 *  swendsentest.cpp
 *  spinify
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
	Graph G;
	int L = 40;
	int W = 40;
	G.initRect(L, W);
	G.randSpin();
	Simul S(&G);
	//S.setParams(0.001, -1);
	//prGraph(&G, L, W);
	S.thermalize(1000);
	//prGraph(&G, L, W);
	std::cout << "Internal energy per spin: " << S.measureE() << std::endl;
	std::cout << "Decorrelation time: " << S.findDecorrelTime(&Simul::measureE) << std::endl;
	
    return 0;
}
