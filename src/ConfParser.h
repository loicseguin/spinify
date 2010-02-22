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

#include <vector>
#include <string>


const unsigned int maxArgs = 25;
const unsigned int minArgs = 2;
const unsigned int defaultNbTemps = 50;


#ifndef CRIT_TEMPS
#define CRIT_TEMPS
const double squareCritBeta = 0.44068679350977151262;
const double triCritBeta = 0.27465307216702742285;
#endif


enum LatticeType {
	none,
	sphere_unif,
	sphere_even,
	rectangle
};

enum MeasureType {
	energy
};

#ifndef OUTPUT_ENUM
#define OUTPUT_ENUM
enum OutputType {
	python,
	raw
};
#endif


class ConfParser {
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

#endif