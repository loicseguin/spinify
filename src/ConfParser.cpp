/*
 *  ConfParser.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-02-18.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "ConfParser.h"
#include "Simul.h"
#include <cstdlib>
#include <iostream>

#define VERSION "2.2"


using namespace std;


void usage(void) {
	cerr
	<< "Spinify " << VERSION << " http://bitbucket.org/loicseguin/spinify/" << endl
	<< "Copyright (C) 2009-2010 Loïc Séguin-C. <loicseguin@gmail.com>\n" << endl
	<< "USAGE: spinify [lattice] [measure] [temperature options] [output options]\n" << endl
	<< "Lattice:" << endl
	<< "  -s N         random lattice on a sphere with N nodes" << endl
	<< "  -r N M       rectangular lattice on a plane torus of width N and height M\n" << endl
	//<< "  -l FILE      reads lattice from file" << endl
	//<< "Measure:" << endl
	//<< "  -u           measure internal energy per spin\n" << endl
	<< "Temperature:" << endl
	<< "  -T n m i     does measurements from temperature n to m, incrementing by" << endl
	<< "               i Kelvin every time\n" << endl
	//<< "  -b n m i     does measurements from beta n to m, incrementing by" << endl
	//<< "               i every time\n" << endl
	<< "Output:" << endl
	<< "  -R           raw output" << endl
	<< "  -P           Python output" << endl;
	//<< "  -f file      outputs to file" << endl;
}


ConfParser::ConfParser () {
	// Main args
	lattice = none;
	nNodes = 0;
	rectN = 0;
	rectM = 0;
	cfgFile = "~/.spinifyrc";
	measure = energy;
	temps.push_back(1./squareCritBeta);
	nTemps = 1;
	
	// What and where to output
	output = raw;
	graphOutFile = "";
	
	// For class Simul
	decorrelIter = 5000;
	correlTreshold = 0.05;
	maxDecorrelTime = 99;
	minDecorrelTime = 5;
	Jval = -1;
	
	// For class Surface
	rangeMultiplier = 10;
	dampingExp = 1.2915;
	dampingSub = 10000;
	objectiveRatio = 0.21;
	
	return;
}

void ConfParser::parseArgs(int argc, char* const argv[]) {
	unsigned int tmpi;
	int tmpsi;
	double tmpd;
	
	// Users must specify an appropriate number of arguments.
	if (argc < minArgs || argc > maxArgs) {
		usage();
		exit(1);
	}
	
	// If the user gives the -h or --help argument, print usage message
	// and exit program.
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-h" || arg == "--help" ) {
			usage();
			exit(0);
		}
	}
	
	// If the -c FILE option is passed, use the provided filename as the
	// configuration file.
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-c") {
			cfgFile = argv[++i];
		}
	}
	
	// Parse config file.
	parseCfgFile();
	
	// Parse the rest of the arguments.
	bool output_set = false;
	
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		// Spherical lattice with even node distribution
		if (arg == "-se" || arg == "-s") {
			if (lattice != none) {
				usage();
				exit(1);
			}
			else if ((nNodes = strtol(argv[++i], NULL, 0))) {
				lattice = sphere_even;
			}
			else {
				usage();
				exit(1);
			}
		}
		
		// Spherical lattice with uniform node distribution
		else if (arg == "-su") {
			if (lattice != none) {
				usage();
				exit(1);
			}
			else if ((nNodes = strtol(argv[++i], NULL, 0))) {
				lattice = sphere_unif;
			}
			else {
				usage();
				exit(1);
			}
		}
		
		// Rectangular lattice
		else if (arg == "-r") {
			if (lattice != none) {
				usage();
				exit(1);
			}
			else if ((rectN = strtol(argv[++i], NULL, 0))
					 && (rectM = strtol(argv[++i], NULL, 0))) {
				nNodes = rectN * rectM;
				lattice = rectangle;
			}
			else {
				usage();
				exit(1);
			}
		}
		
		// Beta
		else if (arg == "-T") {
			temps.pop_back();
			nTemps = 0;
			double get[4];
			int nget = 0;
			while (++i < argc && (get[nget] = strtod(argv[i], NULL))) {
				if (nget > 3) {
					usage();
					exit(1);
				}
				nget++;
			}
			i--;
			switch (nget) {
				case 1:
					nTemps = 1;
					temps.push_back(get[0]);
					break;
				case 2:
					nTemps = defaultNbTemps;
					for (unsigned int j = 0; j < nTemps; j++) {
						temps.push_back(get[0] + j * (get[1] - get[0]) / nTemps);
					}
					break;
				case 3:
					for (int j = 0;; j++) {
						temps.push_back(get[0] + j*get[2]);
						if (temps[j] > get[1]) {
							nTemps = j + 1;
							break;
						}
					}
				default:
					break;
			}
		}
		
		// Raw output
		else if (arg == "-R") {
			if (output_set) {
				usage();
				exit(1);
			}
			output = raw;
			output_set = true;
		}
		
		// Python output
		else if (arg == "-P") {
			if (output_set) {
				usage();
				exit(1);
			}
			output = python;
			output_set = true;
		}
		
		// Class Simul options
		else if (arg == "--decorrelIter" &&
				 (tmpi = strtol(argv[++i], NULL, 0))) {
			decorrelIter = tmpi;
		}
		else if (arg == "--correlTreshold" &&
				 (tmpd = strtod(argv[++i], NULL))) {
			correlTreshold = tmpd;
		}
		else if (arg == "--maxDecorrelTime" &&
				 (tmpi = strtol(argv[++i], NULL, 0))) {
			maxDecorrelTime = tmpi;
		}
		else if (arg == "--minDecorrelTime" &&
				 (tmpi = strtol(argv[++i], NULL, 0))) {
			minDecorrelTime = tmpi;
		}
		else if (arg == "--Jval" &&
				 (tmpsi = strtol(argv[++i], NULL, 0))) {
			Jval = tmpsi;
		}
		
		// Class Surface options
		else if (arg == "--rangeMultiplier" &&
				 (tmpi = strtol(argv[++i], NULL, 0))) {
			rangeMultiplier = tmpi;
		}
		else if (arg == "--dampingExp" &&
				 (tmpd = strtod(argv[++i], NULL))) {
			dampingExp = tmpd;
		}
		else if (arg == "--dampingSub" &&
				 (tmpi = strtol(argv[++i], NULL, 0))) {
			dampingSub = tmpi;
		}
		else if (arg == "--objectiveRatio" &&
				 (tmpd = strtod(argv[++i], NULL))) {
			objectiveRatio = tmpd;
		}
		
		else if (arg == "-c")
			++i;
		
		else {
			usage();
			exit(1);
		}
	}
	
	// If the lattice type has not been specified or the number of nodes
	// is not a positive integer, exit program.
	if (lattice == none || nNodes <= 0) {
		usage();
		exit(1);
	}
	
	return;
}

void ConfParser::parseCfgFile() {
}
