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


#ifndef CRIT_TEMPS
#define CRIT_TEMPS
const double squareCritBeta = 0.44068679350977151262;
const double triCritBeta = 0.27465307216702742285;
#endif


class Simul : public Graph {
	/*
	 * When initializing a simulation, passing an integer argument N
	 * reserves enough memory space for N Nodes. Simul inherits from
	 * Graph. The Graph contains all the information about nodes and
	 * edges which are then used to run the Swendsen-Wang-Wolff
	 * algorithm. The constructor also takes care of setting the
	 * parameters of the simulation to decent default values (the
	 * temperature is set to the critical temperature for the
	 * rectangular lattice and the interaction coefficient is set to -1
	 * for ferromagnetism).
	 *
	 * swendsen() runs one step of the SWW algorithm.
	 *
	 * thermalize() calls swendsen() n times.
	 *
	 * setParams() sets all the parameters.
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
	void swendsen();
	unsigned int decorrelIter;
	double correlTreshold;
	unsigned int maxDecorrelTime;
	unsigned int minDecorrelTime;
	int Jval;
	double currentBeta;
	std::vector<double> beta;
	unsigned int nMeasures;
	unsigned int nInitTherm;
public:
	Simul(int N = 10);
	void thermalize(const int n = 500);
	void setParams(unsigned int dIter,
				   double cTreshold,
				   unsigned int maxDTime,
				   unsigned int minDTime,
				   int J,
				   std::vector<double>& temps,
				   unsigned	int nMeasures,
				   unsigned int nInitTherm);
	double getBeta();
	int getJ();
	double measureE();
	int findDecorrelTime(double (Simul::*measure)());
	void runSimul(OutputType type = raw,
				  std::ostream & output = std::cout);
};


#endif // !SIMUL_H
