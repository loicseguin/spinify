/*
 *  Simul.cpp
 *  spinify2
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-01.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "Simul.h"
#include "tezuka.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>


Simul::Simul(Graph* pH) {
	if (pH == NULL) {
		std::cerr << "Error: no Graph pointer passed to Simul(), nothing to do."
				  << std::endl;
		exit(1);
	}
	else
		pG = pH;
	setParams();
	return;
}

void Simul::thermalize() {
	swendsen();
}

void Simul::swendsen() {
	pG->resetData();
	for (int i = 0; i < pG->size(); i++) {
		if ((*pG)[i].getData() == 0) {
			int oldSpin = (*pG)[i].getSpin();
			int newSpin;
			unsigned int rval = alea();
			if (rval % 2 == 0)
				newSpin = -1;
			else
				newSpin = 1;
			std::vector<int> toTest;
			toTest.push_back(i);
			for (int j = 0; j < toTest.size(); j++) {
				Node& next = (*pG)[toTest[j]];
				next.setData(1);
				next.setSpin(newSpin);
				for (int k = 0; k < next.degree(); k++) {
					Edge& nEdge = pG->edges[next[k]];
					Node& otherEnd = (*pG)[nEdge.getOtherEnd(next.getID())];
					if (nEdge.getData() == 0
						&& otherEnd.getSpin() == oldSpin
						&& (alea() / RANDMAX) < 1 - exp(2*getBeta()*getJ())) {
						nEdge.setData(1);
						toTest.push_back(otherEnd.getID());
					}
				}
			}
		}
	}
	return;
}

void Simul::setParams(double betaval, int Jval) {
	params.beta = betaval;
	params.J = Jval;
	return;
}

double Simul::getBeta() { return params.beta; }

int Simul::getJ() { return params.J; }
