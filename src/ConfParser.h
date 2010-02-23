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


const int maxArgs = 25;
const int minArgs = 2;
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
	rectangle
};

enum MeasureType {
	// Available quantities to measure.
	energy
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
	 * The parseArgs() function calls parseCfgFile().
	 *
	 * The list of all available options is in file doc/CONFIG as well
	 * as in the man page.
	 *
	 */
	void parseCfgFile();
public:
	// Main args
	std::string cfgFile;
	LatticeType lattice;
	MeasureType measure;
	unsigned int nNodes;
	unsigned int rectN;
	unsigned int rectM;
	std::vector<double> temps;
	unsigned int nTemps;
	
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
