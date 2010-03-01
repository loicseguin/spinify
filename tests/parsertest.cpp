/*
 *  parsertest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-02-19.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <iomanip>
#include <iostream>

#include "../src/ConfParser.h"

// The global flags indicate what measurements are made.
bool internalEnergy = false;
bool magnetization = false;
bool susceptibility = false;


using namespace std;

int
main (int argc, char* argv[])
{
	ConfParser cfg;
	cfg.parseArgs(argc, argv);
	
	cout
	<< setw(17) << "cfgFile[0]" << setw(15) << cfg.cfgFile[0] << endl
	<< setw(17) << "cfgFile[1]" << setw(15) << cfg.cfgFile[1] << endl
	<< setw(17) << "lattice" << setw(15) << cfg.lattice << endl
	<< setw(17) << "output" << setw(15) << cfg.output << endl
	<< setw(17) << "nNodes" << setw(15) << cfg.nNodes << endl
	<< setw(17) << "rectN" << setw(15) << cfg.rectN << endl
	<< setw(17) << "rectM" << setw(15) << cfg.rectM << endl
	<< setw(17) << "nTemps" << setw(15) << cfg.nTemps << endl
	<< setw(17) << "temps" << setw(15) << cfg.temps[0] << endl;
	for (int i = 1; i < cfg.nTemps; i++) {
		cout << setw(17) << "" << setw(15) << cfg.temps[i] << endl;
	}
	
	cout
	<< setw(17) << "graphInFile" << setw(15) << cfg.graphInFile << endl
	<< setw(17) << "graphOutFile" << setw(15) << cfg.graphOutFile << endl
	<< setw(17) << "outToFile" << setw(15) << cfg.outToFile << endl
	<< setw(17) << "simulOutFile" << setw(15) << cfg.simulOutFile << endl
	<< setw(17) << "decorrelIter" << setw(15) << cfg.decorrelIter << endl
	<< setw(17) << "correlTreshold" << setw(15) << cfg.correlTreshold << endl
	<< setw(17) << "maxDecorrelTime" << setw(15) << cfg.maxDecorrelTime << endl
	<< setw(17) << "minDecorrelTime" << setw(15) << cfg.minDecorrelTime << endl
	<< setw(17) << "Jval" << setw(15) << cfg.Jval << endl
	<< setw(17) << "nMeasures" << setw(15) << cfg.nMeasures << endl
	<< setw(17) << "nInitTherm" << setw(15) << cfg.nInitTherm << endl
	<< setw(17) << "rangeMultiplier" << setw(15) << cfg.rangeMultiplier << endl
	<< setw(17) << "dampingExp" << setw(15) << cfg.dampingExp << endl
	<< setw(17) << "dampingSub" << setw(15) << cfg.dampingSub << endl
	<< setw(17) << "objectiveRatio" << setw(15) << cfg.objectiveRatio << endl;
	
	return 0;
}