/*
 *  swendsentest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-02.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */


#include <iomanip>
#include <iostream>

#include "../src/Graph.h"
#include "../src/Simul.h"
#include "../src/tezuka.h"

// The global flags indicate what measurements are made.
bool internalEnergy = false;
bool magnetization = false;
bool susceptibility = false;


void
prGraph(Graph *pG, int L, int W)
{
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < W; j++) {
			std::cout << std::setw(3) << (*pG)[i+j*W].getSpin();
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


int
main (int argc, char * const argv[])
{
	std::cout << "SPINIFY\nThe Ising model simulator\n\n";
	int L = 40;
	int W = 40;
    Graph H;
    H.initRect(L, W);
    H.randSpin();
    Simul G(H);
	//S.setParams(0.001, -1);
	//prGraph(&G, L, W);
	G.thermalize(1000);
	//prGraph(&G, L, W);
	/*std::cout << "Internal energy per spin: "
		<< G.measureInternalEnergy() << std::endl;
	std::cout << "Decorrelation time: "
		<< G.findDecorrelTime(&Simul::measureInternalEnergy)
		<< std::endl;
	*/
    return 0;
}
