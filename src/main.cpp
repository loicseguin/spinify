#include "Graph.h"
#include "Surface.h"
#include "Simul.h"
#include "Maths.h"
#include "ConfParser.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>


using namespace std;


int main (int argc, char * const argv[]) {
	ConfParser cfg;
	cfg.parseArgs(argc, argv);
	
	Simul G(cfg.nNodes);
	G.setParams(cfg.decorrelIter,
				cfg.correlTreshold,
				cfg.maxDecorrelTime,
				cfg.minDecorrelTime,
				cfg.Jval,
				cfg.temps,
				cfg.nMeasures,
				cfg.nInitTherm);
	
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
		default:
			break;
	}
	
	std::ofstream outFile;
	if (cfg.graphOutFile != "") {
		std::ostream & graphOutput = true
		? outFile.open(cfg.graphOutFile.c_str(), std::ios::out), outFile
		: std::cout;
		G.print(cfg.output, graphOutput);
		outFile.close();
	}
	
	G.randSpin();
	
	// The following (rather cryptic) trick comes from
	// http://stackoverflow.com/questions/428630/assigning-cout-to-a-variable-name/428821#428821
	
	std::ostream & simulOutput = cfg.outToFile
		? outFile.open(cfg.simulOutFile.c_str(), std::ios::out), outFile
		: std::cout;
	
	G.runSimul(cfg.output, simulOutput);
	cerr << "Done!" << endl;
	
    return 0;
}
