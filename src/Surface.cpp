/*
 *  Surface.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-08.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "Surface.h"
#include "tezuka.h"
#include <cmath>

const int Sphere::randNodes(Graph& G, int N) {
	int curIndex = G.size() - 1;
	int counter = 0;
	
	for (int i = 0; i < N; i++) {
		double v1 = 1.;
		double v2 = 1.;
		double s = v1*v1 + v2*v2;
		while (s >= 1) {
			v1 = -1. + 2.*alea()/RANDMAX;
			v2 = -1. + 2.*alea()/RANDMAX;
			s = v1*v1 + v2*v2;
		}
		G.addNode();
		curIndex++;
		G[curIndex].setCoords(2*v1*sqrt(1 - s), 2*v2*sqrt(1 - s), 1 - 2*s);
		counter++;
	}
	return counter;
}

