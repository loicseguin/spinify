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
#include "Maths.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>


Simul::Simul(int N) : Graph(N) {
	decorrelIter = 5000;
	correlTreshold = 0.05;
	maxDecorrelTime = 99;
	minDecorrelTime = 5;
	Jval = -1;
	currentBeta = squareCritBeta;
	nMeasures = 100;
	nInitTherm = 500;
	return;
}

void Simul::thermalize(const int n) {
	for (int i = 0; i < n; i++)
		swendsen();
	return;
}

void Simul::swendsen() {
	resetStatus();
	for (int i = 0; i < size(); i++) {
		if ((*this)[i].getStatus() == notVisited) {
			int oldSpin = (*this)[i].getSpin();
			int newSpin;
			unsigned int rval = alea();
			if (rval % 2 == 0)
				newSpin = -1;
			else
				newSpin = 1;
			std::vector<Node*> toTest;
			toTest.push_back(&((*this)[i]));
			for (unsigned int j = 0; j < toTest.size(); j++) {
				Node& next = *toTest[j];
				next.setStatus(Visited);
				next.setSpin(newSpin);
				for (int k = 0; k < next.degree(); k++) {
					Edge& nEdge = next[k];
					Node& otherEnd = nEdge.getOtherEnd(next);
					if (nEdge.getStatus() == notVisited
						&& otherEnd.getSpin() == oldSpin
						&& (alea() / RANDMAX) < 1 - exp(2*getBeta()*getJ())) {
						nEdge.setStatus(Visited);
						toTest.push_back(&otherEnd);
					}
					else if (nEdge.getStatus() == notVisited)
						nEdge.setStatus(Visited);
				}
			}
		}
	}
	return;
}

void Simul::setParams(unsigned int dIter,
					  double cTreshold,
					  unsigned int maxDTime,
					  unsigned int minDTime,
					  int J,
					  std::vector<double>& temps,
					  unsigned int nM,
					  unsigned int nITherm) {
	decorrelIter = dIter;
	correlTreshold = cTreshold;
	maxDecorrelTime = maxDTime;
	minDecorrelTime = minDTime;
	Jval = J;
	for (int i = 0; i < temps.size(); i++) {
		beta.push_back(1./temps[i]);
	}
	nMeasures = nM;
	nInitTherm = nITherm;
	return;
}

double Simul::getBeta() { return currentBeta; }

int Simul::getJ() { return Jval; }

double Simul::measureE() {
	// u = (J*\sum_{adjacent nodes i, j} s_i s_j)/N
	int sumSpin = 0;
	for (int i = 0; i < size(); i++) {
		Node& v1 = (*this)[i];
		for (int j = 0; j < v1.degree(); j++) {
			Node& v2 = v1[j].getOtherEnd(v1);
			sumSpin += v1.getSpin() * v2.getSpin();
		}
	}
	return getJ() * sumSpin * 0.5 / size();
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

void Simul::runSimul(OutputType type, std::ostream & output) {
	if (type == python) {
		output << "import numpy as np\n"
		<< "import matplotlib.pyplot as plt\n"
		<< "data = np.array([";
	}
	
	for (int i = 0; i < beta.size(); i++) {
		currentBeta = beta[i];
		thermalize(nInitTherm);
		//std::cerr << "Thermalized at temperature beta = " << Sim.getBeta() << std::endl;
		int K = findDecorrelTime(&Simul::measureE);
		double Data[nMeasures];
		for (int j = 0; j < nMeasures; j++) {
			thermalize(K);
			Data[j] = measureE();
		}
		double avgData = avg(Data, nMeasures);
		if (type == python) {
			output.precision(14);
			output << "[" << getBeta() << ", " << avgData << "]," << std::endl;
		}
		else {
			output.precision(14);
			output << getBeta() << " " << avgData << std::endl;
		}
	}
	
	if (type == python) {
		output << "])\n"
		<< "x = 1/data[:,0]\n"
		<< "y = data[:,1]\n"
		<< "plt.plot(x, y)\n"
		<< "plt.show()" << std::endl;
	}
	return;
}
