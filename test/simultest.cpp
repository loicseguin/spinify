/*
 *  simultest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-03.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "../src/Graph.h"
#include "../src/Simul.h"
#include "../src/tezuka.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


double avg2(double* pV, const int nV) {
	double sum = 0;
	for (int i = 0; i < nV; i++) {
		sum += pV[i];
	}
	return sum / nV;
}

double stdDev(double *pV, const int nV) {
	double avgV = avg2(pV, nV);
	double sum = 0;
	for (int i = 0; i < nV; i++) {
		sum += (pV[i] - avgV)*(pV[i] - avgV);
	}
	return sqrt(sum / nV);
}


int main (int argc, char * const argv[]) {
	int testNb = 1;
	if (argc > 1) {
		testNb = strtol(argv[1], NULL, 10);
	}
	
	std::cout << "SPINIFY\nThe Ising model simulator\n\n";
	std::cout << "=== Tests on the 100x100 rectangular lattice ===\n\n";
	
	const int L = 100;
	const int W = 100;
	const int nTherm = 3000;
	const int nMeasure = 4000;
	const int nTemps = 100;
	const int nMeasure2 = 50;
	
	Graph G;
	G.initRect(L, W);
	G.randSpin();
	Simul S(G);
	
	if (testNb == 1) {
		// Test 1
		std::cout << "Test 1. Thermalized " << nTherm <<" times, " << nMeasure << " measures.\n";
		S.thermalize(nTherm);
		int K = S.findDecorrelTime(&Simul::measureE);
		std::cout << "        Beta: " << S.getBeta() << std::endl;
		std::cout << "        Decorrelation time: " << K << std::endl;
		
		double Data[nMeasure];
		for (int i = 0; i < nMeasure; i++) {
			S.thermalize(K);
			Data[i] = S.measureE();
		}
		
		double avgData = avg2(Data, nMeasure);
		double sdData = stdDev(Data, nMeasure);
		std::cout.precision(10);
		std::cout << "        Average Internal energy per spin: " << avgData << std::endl;
		std::cout.precision(10);
		std::cout << "        Standard deviation: " << sdData << std::endl;
	}
	
	else if (testNb == 2) {
		// Test 2
		std::cout << "Test 2. " << nMeasure2 << " measures for " << nTemps << " temps.\n";
		std::string fileName;
		fileName = "/Users/loic/Projects/spinify/test/testResults.py";
		std::ofstream outFile(fileName.c_str());
		outFile << "import numpy as np\n"
		<< "import matplotlib.pylab as pl\n"
		<< "data = np.array([";
		outFile.close();
		
		for (int i = 0; i < nTemps; i++) {
			S.setParams(0.2 + 0.01 * i, -1);
			S.thermalize(100);
			std::cout << "Thermalized at temperature beta = " << S.getBeta() << std::endl;
			std::cout.flush();
			int K = S.findDecorrelTime(&Simul::measureE);
			double Data[nMeasure2];
			for (int j = 0; j < nMeasure2; j++) {
				S.thermalize(K);
				Data[j] = S.measureE();
			}
			double avgData = avg2(Data, nMeasure2);
			outFile.open(fileName.c_str(), std::ofstream::app);
			outFile << "[" << S.getBeta() << ", " << avgData << "],\n";
			outFile.close();
		}
		
		outFile.open(fileName.c_str(), std::ofstream::app);
		outFile << "])\n"
		<< "x = 1/data[:,0]\n"
		<< "y = data[:,1]\n"
		<< "pl.plot(x, y)\n"
		<< "pl.show()\n";
		outFile.close();
		std::cout << "        Wrote results to test/testResults.py\n";
	}
	else {
		std::cout << "Abort: No such test.\n";
	}

	
    return 0;
}
