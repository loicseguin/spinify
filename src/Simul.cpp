/*
 *  Simul.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-01.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "Maths.h"
#include "Simul.h"
#include "tezuka.h"

Simul::Simul(int N) : Graph(N)
{
	decorrelIter = 5000;
	correlTreshold = 0.05;
	maxDecorrelTime = 99;
	minDecorrelTime = 5;
	Jval = -1;
	currentBeta = squareCritBeta;
	nMeasures = 200;
	nInitTherm = 500;
}

void
Simul::thermalize(const int n)
{
	// This function is useful for quickly reaching the right
	// temperature.
	for (int i = 0; i < n; i++)
		swendsen();
}

void
Simul::swendsen()
{
	// This is an implementation of a mixture of the Swendsen-Wang
	// algorithm and the Wolff algorithm:
	//
	//   Swendsen, R. H. and Wang, J.-S. Nonuniversal critical dynamics
	//   in Monte Carlo simulations. Physical Review Letters, 58 (2) :
	//   86-88, 1987.
	//
	//   Wolff, U. Collective Monte Carlo updating for spin systems.
	//   Physical Review Letters, 62 (4) : 361-364, 1989.
	//
	// A state is a given configuration of spin values on the graph. To
	// go from one state to the pther, the idea is to grow sets of nodes
	// that have the same spin and to change the spin of all nodes in
	// that set randomly. This is done in such a way that the
	// probability of finding a given state is very close to the
	// probability predicted by the Gibbs distribution.
	
	// Set all statuses to not visited.
	resetStatus();
	
	// Repeat the procedure for all nodes.
	for (int i = 0; i < size(); i++) {
		
		// If the node is not visited, run the algorithm. Otherwise, the
		// node was part of a previous set and there is nothing to do.
		if ((*this)[i].getStatus() == notVisited) {
			
			// Start by keeping the value of the node's spin in a
			// variable called oldSpin. Then, randomly choose a new spin
			// value that will be assigned to every node in the set.
			int oldSpin = (*this)[i].getSpin();
			int newSpin;
			unsigned int rval = alea();
			if (rval % 2 == 0)
				newSpin = -1;
			else
				newSpin = 1;
			
			// We build a list of nodes that will be in the set.
			std::vector<Node*> toTest;
			toTest.push_back(&((*this)[i]));
			for (unsigned int j = 0; j < toTest.size(); j++) {
				// Test the first untested node.
				Node& next = *toTest[j];
				next.setStatus(Visited);
				next.setSpin(newSpin);
				
				// Check its neighbour one by one. If the edge is
				// already visited, skip it. Otherwise, mark the edge as
				// visited and check if the other end of the edge has
				// already been visited. If not, check if the neighbour
				// node has the same spin. If it does, then add this
				// node to the set with probability
				//   1 - e^{2 \beta J}
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
}

void
Simul::setParams(unsigned int dIter,
				 double cTreshold,
				 unsigned int maxDTime,
				 unsigned int minDTime,
				 int J,
				 std::vector<double>& temps,
				 unsigned int nM,
				 unsigned int nITherm)
{
	decorrelIter = dIter;
	correlTreshold = cTreshold;
	maxDecorrelTime = maxDTime;
	minDecorrelTime = minDTime;
	Jval = J;
	for (unsigned int i = 0; i < temps.size(); i++) {
		beta.push_back(1./temps[i]);
	}
	nMeasures = nM;
	nInitTherm = nITherm;
}

double
Simul::getBeta()
{
	return currentBeta;
}

int
Simul::getJ()
{
	return Jval;
}

double
Simul::measureInternalEnergy()
{
	// u = (J / N) \sum_{adjacent nodes i, j} s_i s_j
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

double
Simul::measureMagnetization()
{
	// M = (1/N) \sum_{i = 1}^N s_i
	double sumSpin = 0;
	for (int i = 0; i < size(); i++)
		sumSpin += (*this)[i].getSpin();
	return sumSpin / size();
}

double
Simul::thermalInternalEnergy()
{
	// Repeat the measure of internal energy per spin nMeasures times
	// and return the average value (this is called the thermal
	// average).
	double Data[nMeasures];
	for (unsigned int j = 0; j < nMeasures; j++) {
		thermalize(decorrelTime);
		Data[j] = measureInternalEnergy();
	}
	return avg(Data, nMeasures);
}

double
Simul::thermalMagnetization()
{
	// Repeat the measure of magnetization nMeasures times
	// and return the average value (this is called the thermal
	// average).
	double Data[nMeasures];
	for (unsigned int j = 0; j < nMeasures; j++) {
		thermalize(decorrelTime);
		Data[j] = fabs(measureMagnetization());
	}
	return avg(Data, nMeasures);
}

double
Simul::thermalSusceptibility()
{
	// Magnetic susceptibility is defined to be
	//   \chi = \beta N (<M^2> - <M>^2)
	// where M is the magnetization and <> represent thermal average.
	double Data[nMeasures];
	double DataSq[nMeasures];
	for (unsigned int j = 0; j < nMeasures; j++) {
		thermalize(decorrelTime);
		Data[j] = measureMagnetization();
		DataSq[j] = Data[j] * Data[j];
	}
	double avgSq = avg(Data, nMeasures);
	avgSq *= avgSq;
	return getBeta() * size()
		   * (avg(DataSq, nMeasures) - avgSq);
}


unsigned int
Simul::findDecorrelTime(double (Simul::*measure)())
{
	// To determine whether two measures are statistically independant,
	// we evaluate the correlation function function between measures
	// taken at a given number of Swendsen-Wang-Wolff iterations away.
	// As soon as the correlation function drops below a user chosen
	// threshold, we say the measures are decorrelated and we call that
	// number of iterations the decorrelation time.
	
	double C_k = 1.;		// Autocorrelation function at step k
	double X[decorrelIter], X2[decorrelIter], X_k[decorrelIter];
	double avgX, avgX2, avgX_k;
	
	for (unsigned int i = 0; i < decorrelIter; i++) {
		swendsen();
		X[i] = (this->*measure)();
		X2[i] = X[i] * X[i];
	}
	
	avgX = avg(X, decorrelIter);
	avgX2 = avg(X2, decorrelIter);
	
	unsigned int k;
	for (k = 0; C_k >= correlTreshold; k++) {
		for (unsigned int i = 0; i < decorrelIter - k; i++)
			X_k[i] = X[i] * X[i + k];
		avgX_k = avg(X_k, decorrelIter - k);
		C_k = (avgX_k - avgX * avgX) / (avgX2 - avgX * avgX);
		// We keep the decorrelation time to at most maxDecorrelTime.
		if (k > maxDecorrelTime) break;
	}
	
	// Better safe than sorry: always do at least minDecorrelTime
	// iterations between every measure.
	if (k < minDecorrelTime)
		k = minDecorrelTime;
	
	decorrelTime = k;
	return k;
}

void
Simul::runSimul(OutputType type, std::ostream & output)
{
	// Run the simulation.
	
	// The following are boolean flags that are declared in main.cpp and
	// whose value depends on the user's choices.
	extern bool internalEnergy;
	extern bool magnetization;
	extern bool susceptibility;
	
	if (type == python) {
		output << "import numpy as np\n"
		<< "import matplotlib.pyplot as plt\n"
		<< "data = np.array([";
	}
	
	for (unsigned int i = 0; i < beta.size(); i++) {
		// We start by setting the Simul variable currentBeta to the
		// next beta value from the user's temperature vector.
		currentBeta = beta[i];
		
		if (type == python)
			output << "[" << getBeta() << ", ";
		else
			output << getBeta() << " ";
		
		// We run a couple of iterations of SWW in order to ensure that
		// we are at the proper temperature.
		thermalize(nInitTherm);
		//std::cerr << "Thermalized at temperature beta = " << Sim.getBeta() << std::endl;
		
		// Find the decorrelation time for the given measure. If the
		// user want to measure more than one quantity, take the
		// decorrelation time to be the maximum of the time for each
		// measure. Then, we are assured that the measures are all
		// independant.
		unsigned int dTimeEnergy = 0;
		unsigned int dTimeMagnetization = 0;
		if (internalEnergy)
			dTimeEnergy =
				findDecorrelTime(&Simul::measureInternalEnergy);
		if (magnetization || susceptibility)
			dTimeMagnetization =
				findDecorrelTime(&Simul::measureMagnetization);
		decorrelTime = std::max(dTimeEnergy, dTimeMagnetization);
		
		// Make the measurements and output them to the output stream.
		if (internalEnergy) {
			output << thermalInternalEnergy();
			if (type == python)
				output << ", ";
			else
				output << " ";
		}
		if (magnetization) {
			output << thermalMagnetization();
			if (type == python)
				output << ", ";
			else
				output << " ";
		}
		if (susceptibility)
			output << thermalSusceptibility();
		
		if (type == python)
			output << "]," << std::endl;
		else
			output << std::endl;
	}
	
	// This is a couple of Pythonic goodies.
	if (type == python) {
		int nCols = 0;
		if (internalEnergy)
			nCols++;
		if (magnetization)
			nCols++;
		if (susceptibility)
			nCols++;
		
		output << "])\n"
		<< "x = 1/data[:,0]\n";
		for (int i = 0; i < nCols; i++) {
			output << "y" << i+1 << " = data[:," << i+1 << "]\n"
			<< "plt.plot(x, y" << i+1 << ")\n"
			<< "plt.show()" << std::endl;
		}
	}
}
