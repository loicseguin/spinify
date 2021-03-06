/*
 *  main.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */


#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "ConfParser.h"
#include "Graph.h"
#include "Maths.h"
#include "Simul.h"
#include "Surface.h"


// The global flags indicate what measurements are made.
bool internalEnergy = false;
bool magnetization = false;
bool susceptibility = false;


using namespace std;


int
main (int argc, char * const argv[])
{
	/*
	 * This is the main routine for the program Spinify. It is
	 * responsible for handling the command line arguments to the
	 * parser, creating the necessary graph and calling the simulation
	 * routine for this graph.
	 *
	 */
	
	// Start by parsing the commandline arguments.
	ConfParser cfg;
	cfg.parseArgs(argc, argv);
	
	// Create and fill the graph with the appropriate nodes and edges
    // depending on the lattice type.
    Graph G(cfg.nNodes);
	switch (cfg.lattice) {
		case sphere_even:
		{
			Sphere S;
			S.setParams(cfg.rangeMultiplier,
						cfg.dampingExp,
						cfg.dampingSub,
						cfg.objectiveRatio);
			S.evenNodes(G, cfg.nNodes);
			S.delaunay(G);
			break;
		}
		case sphere_unif:
		{
			Sphere S;
			S.uniform(G, cfg.nNodes);
			S.delaunay(G);
			break;
		}
		case rectangle:
			G.initRect(cfg.rectN, cfg.rectM);
			break;
		case file:
			G.readFile(cfg.graphInFile);
			break;
		default:
			break;
	}
    
    // Then, create a simul with the appropriate parameters.
	Simul simul(G);
	simul.setParams(cfg.decorrelIter,
                    cfg.correlTreshold,
                    cfg.maxDecorrelTime,
                    cfg.minDecorrelTime,
                    cfg.Jval,
                    cfg.temps,
                    cfg.nMeasures,
                    cfg.nInitTherm);
	
	// If required by the user, print the graph data to file.
	std::ofstream outFile;
	if (cfg.graphOutFile != "") {
		std::ostream & graphOutput = true
		? outFile.open(cfg.graphOutFile.c_str(), std::ios::out), outFile
		: std::cout;
		G.print(cfg.output, graphOutput);
		outFile.close();
	}
	
	// Set the temperature to infinity.
	G.randSpin();
	
	// The following (rather cryptic) trick comes from
	// http://stackoverflow.com/questions/428630/assigning-cout-to-a-variable-name/428821#428821
	
	std::ostream & simulOutput = cfg.outToFile
		? outFile.open(cfg.simulOutFile.c_str(), std::ios::out), outFile
		: std::cout;
	
	// Finally, run the simulation and exit.
	simul.runSimul(cfg.output, simulOutput);
	cerr << "Done!" << endl;
	
    return 0;
}
