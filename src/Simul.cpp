/*
 *  Simul.cpp
 *  spinify
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

void Simul::thermalize(int n) {
	for (int i = 0; i < n; i++)
		swendsen();
	return;
}

void Simul::swendsen() {
	Graph& G = *pG;
	G.resetStatus();
	for (int i = 0; i < G.size(); i++) {
		if (G[i].getStatus() == notVisited) {
			int oldSpin = G[i].getSpin();
			int newSpin;
			unsigned int rval = alea();
			if (rval % 2 == 0)
				newSpin = -1;
			else
				newSpin = 1;
			std::vector<int> toTest;
			toTest.push_back(i);
			for (unsigned int j = 0; j < toTest.size(); j++) {
				Node& next = G[toTest[j]];
				next.setStatus(Visited);
				next.setSpin(newSpin);
				for (int k = 0; k < next.degree(); k++) {
					Edge& nEdge = G.edges[next[k]];
					Node& otherEnd = G[nEdge.getOtherEnd(next.getID())];
					if (nEdge.getStatus() == notVisited
						&& otherEnd.getSpin() == oldSpin
						&& (alea() / RANDMAX) < 1 - exp(2*getBeta()*getJ())) {
						nEdge.setStatus(Visited);
						toTest.push_back(otherEnd.getID());
					}
					else if (nEdge.getStatus() == notVisited)
						nEdge.setStatus(Visited);
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

double Simul::measureE() {
	// u = (J*\sum_{adjacent nodes i, j} s_i s_j)/N
	int sumSpin = 0;
	for (int i = 0; i < pG->size(); i++) {
		Node& v1 = (*pG)[i];
		for (int j = 0; j < v1.degree(); j++) {
			Node& v2 = (*pG)[ (pG->edges[v1[j]]).getOtherEnd(v1.getID()) ];
			sumSpin += v1.getSpin() * v2.getSpin();
		}
	}
	return getJ() * sumSpin * 0.5 / pG->size();
}

double avg(double* pV, const int nV) {
	double sum = 0;
	for (int i = 0; i < nV; i++) {
		sum += pV[i];
	}
	return sum / nV;
}

int Simul::findDecorrelTime(double (Simul::*measure)()) {
	double C_k = 1.;		// Autocorrelation function at step k
	double X[decorrelIter], X2[decorrelIter], X_k[decorrelIter];
	double avgX, avgX2, avgX_k;
	
	for (int i = 0; i < decorrelIter; i++) {
		swendsen();
		X[i] = (this->*measure)();
		X2[i] = X[i] * X[i];
	}
	
	avgX = avg(X, decorrelIter);
	avgX2 = avg(X2, decorrelIter);
	// std::cout << "avgX = " << avgX << std::endl;
	// std::cout << "avgX2 = " << avgX2 << std::endl;
	
	int k;
	for (k = 0; C_k >= correlTreshold; k++) {
		for (int i = 0; i < decorrelIter - k; i++)
			X_k[i] = X[i] * X[i + k];
		avgX_k = avg(X_k, decorrelIter - k);
		C_k = (avgX_k - avgX * avgX) / (avgX2 - avgX * avgX);
		// std::cout << "C_" << k << " = " << C_k << std::endl;
		// Avoid disasters...
		if (k > maxDecorrelTime) break;
	}
	
	// Better safe than sorry: always do at least five iterations
	// between every measure.
	if (k < minDecorrelTime)
		k = minDecorrelTime;
	
	return k;
}
