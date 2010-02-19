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


enum LatticeType {
	none,
	sphere_unif,
	sphere_even,
	rectangle
};

enum MeasureType {
	energy
};

enum OutputType {
	python,
	raw
};


class ConfParser {
	void parseCfgFile();
public:
	// Main args
	std::string cfgFile;
	LatticeType lattice;
	MeasureType measure;
	OutputType output;
	unsigned int nNodes;
	unsigned int rectN;
	unsigned int rectM;
	std::vector<double> temps;
	unsigned int nTemps;
	
	// For class Simul
	unsigned int decorrelIter;
	double correlTreshold;
	unsigned int maxDecorrelTime;
	unsigned int minDecorrelTime;
	int Jval;
	
	// For class Surface
	unsigned int rangeMultiplier;
	double dampingExp;
	unsigned int dampingSub;
	double objectiveRatio;
	
	// Parsers
	void parseArgs(int argc, char* argv[]);
	
	// Constructor sets default values
	ConfParser();
};

#endif