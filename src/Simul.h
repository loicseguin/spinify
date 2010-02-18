/*
 *  Simul.h
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-01.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

/*
 * The Simul class is responsible for configuring and running the Ising
 * model simulation.
 *
 */

#ifndef SIMUL_H
#define SIMUL_H


#include "Graph.h"


const int decorrelIter = 5000;
const double correlTreshold = 0.05;
const int maxDecorrelTime = 99;
const int minDecorrelTime = 5;
const double squareCritBeta = 0.44068679350977151262;
const double triCritBeta = 0.27465307216702742285;
const int defaultJ = -1;



class Simul {
	/*
	 * When initializing a simulation, a pointer to the Graph must be
	 * passed to the Simul constructor. The Graph contains all the
	 * information about nodes and edges which are then used to run the
	 * Swendsen-Wang-Wolff algorithm. The constructor also takes care of
	 * setting the parameters of the simulation (temperature and
	 * interaction coefficient) to some default values (the temperature
	 * is set to the critical temperature for the rectangular lattice
	 * and the interaction coefficient is set to -1 for ferromagnetism).
	 *
	 * swendsen() runs one step of the SWW algorithm.
	 *
	 * thermalize() calls swendsen() n times.
	 *
	 * setParams() set the temperature and the interaction coefficient.
	 *
	 * During a simulation, there are many different quantities that can
	 * be measured. One of them is the internal energy per spin
	 *     \[u = \frac{1}{n}\sum_{<i,j>} s_i s_j\]
	 * where the sum runs over all edges <i,j>. This quantity is
	 * measured by measureE(). findDecorrelTime() is used to find how
	 * many swendsen() iterations are needed between every measure to
	 * ensure that the measures are statistically independent.
	 *
	 */
	Graph& G;
	void swendsen();
	struct Params {
		double beta;
		int J;
	} params;
public:
	Simul(Graph& H);
	void thermalize(const int n = 500);
	void setParams(const double betaval = squareCritBeta,
				   const int Jval = defaultJ);
	double getBeta();
	int getJ();
	double measureE();
	int findDecorrelTime(double (Simul::*measure)());
};


#endif
