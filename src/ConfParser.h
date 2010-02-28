/*
 *  ConfParser.h
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-02-18.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#ifndef CONFPARSER_H
#define CONFPARSER_H

#include <string>
#include <vector>


// This is the maximum number of command line arguments.
const int maxArgs = 50;
// This is the minimum number of command line arguments.
const int minArgs = 1;
// When only a minimum and a maximum temperatures are given, divide the
// interval in defaultNbTemps values.
const unsigned int defaultNbTemps = 50;


#ifndef CRIT_TEMPS
#define CRIT_TEMPS
const double squareCritBeta = 0.44068679350977151262;
const double triCritBeta = 0.27465307216702742285;
#endif


enum LatticeType {
	// Available lattice types for the simulation.
	none,
	sphere_unif,
	sphere_even,
	rectangle,
	file
};

#ifndef OUTPUT_ENUM
#define OUTPUT_ENUM
enum OutputType {
	// Output Python friendly content or raw data.
	python,
	raw
};
#endif


class ConfParser {
	/*
	 * The ConfParser's job is to read the command line arguments as
	 * well as the configuration file and to set the various parameters
	 * accordingly.
	 *
	 * The constructor sets default values for most parameters that
	 * should be fine for most uses. By providing the necessary
	 * arguments, the user has the freedom to try and tweak these
	 * values.
	 *
	 * The user has to provide at least a lattice type and corresponding
	 * size. Failure to do so results in the program exiting.
	 *
	 * The parseArgs() parses the command line arguments and also the
	 * configuration file via a function call to parseCfgFile().
	 *
	 * The list of all available options is in file doc/CONFIG as well
	 * as in the man page.
	 *
	 */
	bool parseCfgFile();
public:
	// Main args
	std::string cfgFile[2];
	LatticeType lattice;
	unsigned int nNodes;
	unsigned int rectN;
	unsigned int rectM;
	std::vector<double> temps;
	unsigned int nTemps;
	std::string graphInFile;
	
	// What and where to output
	OutputType output;
	std::string graphOutFile;
	bool outToFile;
	std::string simulOutFile;
	
	// For class Simul
	unsigned int decorrelIter;
	double correlTreshold;
	unsigned int maxDecorrelTime;
	unsigned int minDecorrelTime;
	int Jval;
	unsigned int nMeasures;
	unsigned int nInitTherm;
	
	// For class Surface
	unsigned int rangeMultiplier;
	double dampingExp;
	unsigned int dampingSub;
	double objectiveRatio;
	
	// Parsers
	void parseArgs(int argc, char* const argv[]);
	
	// Constructor sets default values
	ConfParser();
};

#endif // !CONFPARSER_H
