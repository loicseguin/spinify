/*
 *  ConfParser.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-02-18.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "ConfParser.h"

#define VERSION "2.2"


using namespace std;


bool parsingFile = false;
bool cmdlineGraph = false;


void
usage(void)
{
	cout
	<< "Spinify " << VERSION << endl
	<< "USAGE: spinify [-s N | -r N M | -g FILE] [options]" << endl
	<< endl
	<< "For more detailed help type:" << endl
	<< "  spinify --help" << endl;
}

void
help()
{
	cout
	<< "Spinify " << VERSION << endl
	<< "Copyright (C) 2009, 2010 Loïc Séguin-C. <loicseguin@gmail.com>" << endl
	<< endl
	<< "USAGE: spinify [-s N | -r N M | -g FILE] [-ekm] [options]" << endl
	<< endl
	<< "General options:" << endl
	<< "  -c FILE,  --config-file FILE     Read configuration from FILE." << endl
	<< "  -h,  --help                      Print this message and exit." << endl
	<< "  -v,  --version                   Print version information and exit." << endl
	<< endl
	<< "Graph options:" << endl
	<< "  -g FILE,  --graph-infile FILE    Read graph information from FILE." << endl
	<< "  -r N M,  --rectangle N M         Rectangular graph on a plane torus of width" << endl
	<< "                                   N and height M." << endl
	<< "  -s N,  --sphere-even N           Random graph with even node distribution on" << endl
	<< "                                   a sphere with N nodes." << endl
	<< "  -u N,  --sphere-uniform N        Random graph with uniform node distribution" << endl
	<< "                                   on a sphere with N nodes." << endl
	<< "  -w FILE,  --write-graph FILE     Write graph information to FILE." << endl
	<< endl
	<< "Temperature options:" << endl
	<< "  -T n m i,  --temperatures n m i  Do measurements from temperature n to m," << endl
	<< "                                   incrementing by i Kelvin every time." << endl
	<< endl
	<< "Measure flags:" << endl
	<< "  -e,  --internalEnergy            Measure internal energy per spin." << endl
	<< "  -k,  --susceptibility            Measure magnetic susceptinility." << endl
	<< "  -m,  --magnetization             Measure magnetization." << endl
	<< endl
	<< "Output options:" << endl
	<< "  -o FILE,  --output FILE          Write simulation output to FILE." << endl
	<< "  -p,  --python                    Output Python formatted data." << endl
	<< "  -r,  --raw                       Output raw data." << endl
	<< endl
	<< "Simulation options:" << endl
	<< "  --correlTreshold X               Correlation coefficient must be below X." << endl
	<< "  --decorrelIter N                 Do N measures to compute decorrelation time." << endl
	<< "  --Jval N                         Spin interaction coefficient is N." << endl
	<< "  --maxDecorrelTime N              Decorrelation time is at most N." << endl
	<< "  --minDecorrelTime N              Decorrelation time is at least N." << endl
	<< "  --nInitTherm N                   Do N iterations of Swedsen-Wang-Wolff" << endl
	<< "                                   before starting the simulation." << endl
	<< "  --nMeasures N                    Take N measures at every temperature." << endl
	<< endl
	<< "Surface options:" << endl
	<< "  --dampingExp X                   Damping factor is" << endl
	<< "  --dampingSub N                   (number of nodes)^X - N" << endl
	<< "  --objectiveRatio X               Do electric repulsion until minimum" << endl 
	<< "                                   distance is within X% of optimum." << endl
	<< "  --rangeMultiplier N              Multiplier for the range of electric force." << endl
	<< endl
	<< "Report bugs to: <http://bitbucket.org/loicseguin/spinify/issues/>" << endl
	<< "Spinify home page: <http://bitbucket.org/loicseguin/spinify/>" << endl << endl;
}

void
version(void)
{
	cout
	<< "Spinify " << VERSION << endl
	<< "Copyright (C) 2009, 2010 Loïc Séguin-C. <loicseguin@gmail.com>" << endl
	<< "Distributed under a BSD license." << endl
	<< "This is free software: you are free to change and redistribute it." << endl
	<< "There is NO WARRANTY, to the extent permitted by law." << endl;
}

void
err_argument(string& option)
{
	cerr << "Error: option " << option
		 << " requires an argument." << endl;
	usage();
	exit(1);	
}

void
err_lattice_defined(string& option)
{
	cerr << "Error: you must specify only one graph type." << endl;
	usage();
	exit(1);
}

void
err_positive(string& option)
{
	cerr << "Error: option " << option
		 << " requires positive arguments." << endl;
	usage();
	exit(1);
}

void
err_output_set(string& option)
{
	cerr << "Error: you must specify only one of -r or -p." << endl;
	usage();
	exit(1);
}

void
err_bounded(string& option, double a, double b)
{
	cerr << "Error: option " << option
	<< " requires an argument between " << a << " and " << b << endl;
	usage();
	exit(1);
}

void
err_argument_file(string& option)
{
	cerr << "Error: in configuration file, option " << option
	<< " requires an argument which is either True of False." << endl;
	usage();
	exit(1);
}


ConfParser::ConfParser ()
{
	// Main args
	lattice = none;
	nNodes = 0;
	rectN = 0;
	rectM = 0;
	cfgFile[0] = ".spinifyrc";
	cfgFile[1] = "~/.spinifyrc";
	temps.push_back(1./squareCritBeta);
	nTemps = 1;
	graphInFile = "";
	
	// What and where to output
	output = raw;
	graphOutFile = "";
	bool outToFile = false;
	simulOutFile = "";
	
	// For class Simul
	decorrelIter = 5000;
	correlTreshold = 0.05;
	maxDecorrelTime = 99;
	minDecorrelTime = 5;
	Jval = -1;
	nMeasures = 100;
	nInitTherm = 500;
	
	// For class Surface
	rangeMultiplier = 10;
	dampingExp = 1.2915;
	dampingSub = 10000;
	objectiveRatio = 0.21;
	
	return;
}

void
ConfParser::parseArgs(int argc, char* const argv[])
{
	extern bool internalEnergy;
	extern bool magnetization;
	extern bool susceptibility;	
	
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
			help();
			exit(0);
		}
	}
	
	// If the user gives the -v or --version argument, print version
	// message and exit program.
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-v" || arg == "--version" ) {
			version();
			exit(0);
		}
	}
	
	// If the -c FILE option is passed, use the provided filename as the
	// configuration file.
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		if (arg == "-c" || arg == "--config-file") {
			if (++i < argc) {
				cfgFile[0] = argv[i];
			}
			else
				err_argument(arg);
		}
	}
	
	// Parse config file.
	if (!parsingFile)
		parseCfgFile();
	
	// Parse the rest of the arguments.
	bool output_set = false;
	
	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		
		// Read graph info from file
		if(arg == "-g" || arg == "--graph-infile") {
			if (cmdlineGraph)
				err_lattice_defined(arg);
			else if (++i < argc) {
				graphInFile = argv[i];
				lattice = file;
				if (!parsingFile)
					cmdlineGraph = true;
			}
			else 
				err_argument(arg);
		}
		
		// Spherical lattice with even node distribution
		else if (arg == "-s" || arg == "--sphere-even") {
			if (cmdlineGraph)
				err_lattice_defined(arg);
			else if (++i < argc && (nNodes = strtol(argv[i], NULL, 0))) {
				lattice = sphere_even;
				if (!parsingFile)
					cmdlineGraph = true;
			}
			else 
				err_argument(arg);
		}
		
		// Spherical lattice with uniform node distribution
		else if (arg == "-u" || arg == "--sphere-uniform") {
			if (cmdlineGraph)
				err_lattice_defined(arg);
			else if (++i < argc && (nNodes = strtol(argv[i], NULL, 0))) {
				lattice = sphere_unif;
				if (!parsingFile)
					cmdlineGraph = true;
			}
			else
				err_argument(arg);
		}
		
		// Rectangular lattice
		else if (arg == "-r" || arg == "--rectangle") {
			if (cmdlineGraph)
				err_lattice_defined(arg);
			else if (++i + 1 < argc && (rectN = strtol(argv[i], NULL, 0))
					 && (rectM = strtol(argv[++i], NULL, 0))) {
				nNodes = rectN * rectM;
				lattice = rectangle;
				if (!parsingFile)
					cmdlineGraph = true;
			}
			else 
				err_argument(arg);
		}
		
		// Beta
		else if (arg == "-T" || arg == "--temperatures") {
			temps.pop_back();
			nTemps = 0;
			double get[4];
			int nget = 0;
			while (++i < argc && (get[nget] = strtod(argv[i], NULL))) {
				if (nget > 3) {
					cerr << "Error: too many arguments for option "
						 << arg << endl;
					usage();
					exit(1);
				}
				nget++;
			}
			i--;
			switch (nget) {
				case 1:
					nTemps = 1;
					if (get[0] <= 0)
						err_positive(arg);
					temps.push_back(get[0]);
					break;
				case 2:
					nTemps = defaultNbTemps;
					if (get[0] < 0 || get[1] < 0)
						err_positive(arg);
					for (unsigned int j = 0; j < nTemps; j++) {
						temps.push_back(get[0] + j * (get[1] - get[0]) / nTemps);
					}
					break;
				case 3:
					for (int j = 0;; j++) {
						if ((get[1] - get[0]) * get[2] < 0
							|| get[2] == 0) {
							cerr << "Error: option " << arg
							<< " requires a bound n, a bound m and an"
							<< " increment i from n to m." << endl;
							usage();
							exit(1);
						}
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
		
		// Measures
		else if (arg == "-e" || arg == "--internalEnergy") {
			if (parsingFile) {
				if ( ++i < argc) {
					string argvi = argv[i];
					if (argvi == "true" || argvi == "True"
						|| argvi == "yes" ||argvi == "Yes")
						internalEnergy = true;
					else
						internalEnergy = false;
				}
				else
					err_argument_file(arg);
			}
			else
				internalEnergy = true;
		}
		else if (arg == "-k" || arg == "--susceptibility") {
			if (parsingFile) {
				if ( ++i < argc) {
					string argvi = argv[i];
					if (argvi == "true" || argvi == "True"
						|| argvi == "yes" ||argvi == "Yes")
						susceptibility = true;
					else
						susceptibility = false;
				}
				else
					err_argument_file(arg);
			}
			else
				susceptibility = true;
		}
		else if (arg == "-m" || arg == "--magnetization") {
			if (parsingFile) {
				if ( ++i < argc) {
					string argvi = argv[i];
					if (argvi == "true" || argvi == "True"
						|| argvi == "yes" ||argvi == "Yes")
						magnetization = true;
					else
						magnetization = false;
				}
				else
					err_argument_file(arg);
			}
			else
				magnetization = true;
		}
		
		// Raw output
		else if (arg == "-r" || arg == "--raw") {
			if (output_set)
				err_output_set(arg);
			output = raw;
			output_set = true;
		}
		
		// Python output
		else if (arg == "-p" || arg == "--python") {
			if (output_set)
				err_output_set(arg);
			output = python;
			output_set = true;
		}
		
		// Output files
		else if (arg == "-w" || arg == "--write-graph") {
			if (++i < argc)
				graphOutFile = argv[i];
			else
				err_argument(arg);
		}
		
		else if (arg == "-o" || arg == "--output") {
			if (++i < argc) {
				outToFile = true;
				simulOutFile = argv[i];
			}
			else
				err_argument(arg);
		}
		
		// Class Simul options
		else if (arg == "--decorrelIter") {
			if(++i < argc && (tmpi = strtol(argv[i], NULL, 0)))
				decorrelIter = tmpi;
			else
				err_argument(arg);
		}
		
		else if (arg == "--correlTreshold") {
			if(++i < argc && (tmpd = strtod(argv[i], NULL))) {
				if (tmpd > -2 && tmpd < 2)
					correlTreshold = tmpd;
				else
					err_bounded(arg, -2, 2);
			}
			else
				err_argument(arg);
		}
		
		else if (arg == "--maxDecorrelTime") {
			if(++i < argc && (tmpi = strtol(argv[i], NULL, 0)))
				maxDecorrelTime = tmpi;
			else
				err_argument(arg);
		}

		else if (arg == "--minDecorrelTime") {
			if (++i < argc && (tmpi = strtol(argv[i], NULL, 0)))
				minDecorrelTime = tmpi;
			else
				err_argument(arg);
		}
		
		else if (arg == "--Jval") {
			if (++i < argc && (tmpsi = strtol(argv[i], NULL, 0)))
				Jval = tmpsi;
			else
				err_argument(arg);
		}
		
		else if (arg == "--nMeasures") {
			if (++i < argc && (tmpi = strtol(argv[i], NULL, 0)))
				nMeasures = tmpi;
			else
				err_argument(arg);
		}
		
		else if (arg == "--nInitTherm") {
			if (++i < argc && (tmpi = strtol(argv[i], NULL, 0)))
				nInitTherm = tmpi;
			else
				err_argument(arg);
		}
		
		// Class Surface options
		else if (arg == "--rangeMultiplier") {
			if (++i < argc && (tmpi = strtol(argv[i], NULL, 0))) {
				if (tmpi > 1)
					rangeMultiplier = tmpi;
				else {
					cerr << "Error: option " << arg
					<< " should be greater than 1." << endl;
					usage();
					exit(1);
				}
			}
			else
				err_argument(arg);
		}
		
		else if (arg == "--dampingExp") {
			if (++i < argc && (tmpd = strtod(argv[i], NULL)))
				dampingExp = tmpd;
			else
				err_argument(arg);
		}
		
		else if (arg == "--dampingSub") {
			if (++i < argc && (tmpi = strtol(argv[i], NULL, 0)))
				dampingSub = tmpi;
			else
				err_argument(arg);
		}
		
		else if (arg == "--objectiveRatio") {
			if (++i < argc && (tmpd = strtod(argv[i], NULL))) {
				if (tmpd > 0 && tmpd < 1)
					objectiveRatio = tmpd;
				else
					err_bounded(arg, 0, 1);
			}
			else
				err_argument(arg);
		}
		
		else if (arg == "-c" || arg == "--config-file")
			++i;
		
		else {
			cerr << "Error: unrecognized option " << arg << endl;
			usage();
			exit(1);
		}
	}
	
	// If the lattice type has not been specified or the number of nodes
	// is not a positive integer, exit program.
	if (!parsingFile && (lattice == none || nNodes <= 0)) {
		cerr << "Error: lattice not defined." << endl;
		usage();
		exit(1);
	}
	
	return;
}

bool
ConfParser::parseCfgFile()
{
	vector<string> cmdLine;
	cmdLine.push_back("spinify");
	ifstream cfgFilePtr;
	
	for (int i = 0; i < 2; i++) {
		cfgFilePtr.open(cfgFile[0].c_str(), ifstream::in);
		if (cfgFilePtr.is_open())
			break;
	}
	
	if (cfgFilePtr.is_open()) {
		string line;
		while (getline(cfgFilePtr, line)) {
			size_t found;
			found = line.find_first_not_of(" \n\t\v");
			if((found != string::npos && line[found] == '#')
			   || found == string::npos)
				continue;
			size_t equal;
			equal = line.find('=');
			if (equal == string::npos) {
				cerr << "While reading line:" << endl
				<< line << endl;
				cerr << "Error: malformed configuration file." << endl;
				exit(1);
			}
			string option;
			string argument;
			option = line.substr(found, equal);
			size_t blank;
			blank = option.find_first_of(" \t");
			if (blank != string::npos)
				option = option.substr(0, blank);
			cmdLine.push_back("--" + option);
			
			argument = line.substr(equal + 1);
			blank = argument.find_first_not_of(" \t");
			if (blank != string::npos)
				argument = argument.substr(blank);
			string remainder;
			while ((blank = argument.find_first_of(" \t"))
				   != string::npos) {
				remainder = argument.substr(blank);
				argument = argument.substr(0, blank);
				cmdLine.push_back(argument);
				argument = remainder;
				blank = argument.find_first_not_of(" \t");
				if (blank != string::npos)
					argument = argument.substr(blank);
			}
			cmdLine.push_back(argument);
		}
		
		char* argv[maxArgs];
		for (int i = 0; i < cmdLine.size(); i++) {
			//cout << cmdLine[i] << endl;
			argv[i] = (char*)cmdLine[i].c_str();
			cout << cmdLine[i] << endl;
		}
		parsingFile = true;
		parseArgs(cmdLine.size(), argv);
		parsingFile = false;
		return true;
	}
	
	return false;
}
