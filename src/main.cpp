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
				cfg.temps);
	
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
	
	// The following (rather cryptic) trick comes from
	// http://stackoverflow.com/questions/428630/assigning-cout-to-a-variable-name/428821#428821
	
	std::ofstream outFile;
	std::ostream & graphOutput = (cfg.graphOutFile != "")
		? outFile.open(cfg.graphOutFile.c_str(), std::ios::out), outFile
		: std::cout;
	
	G.print(cfg.output, graphOutput);
	
	if (cfg.graphOutFile != "")
		outFile.close();
	
	G.randSpin();
	
	if (cfg.output == python) {
		cout << "import numpy as np\n"
		<< "import matplotlib.pyplot as plt\n"
		<< "data = np.array([";
	}
	
	int nMeasure = 100;
	
	for (int i = 0; i < cfg.nTemps; i++) {
		G.thermalize(500);
		//std::cerr << "Thermalized at temperature beta = " << Sim.getBeta() << std::endl;
		int K = G.findDecorrelTime(&Simul::measureE);
		double Data[nMeasure];
		for (int j = 0; j < nMeasure; j++) {
			G.thermalize(K);
			Data[j] = G.measureE();
		}
		double avgData = avg(Data, nMeasure);
		if (cfg.output == python) {
			cout.precision(14);
			cout << "[" << G.getBeta() << ", " << avgData << "]," << endl;
		}
		else {
			cout.precision(14);
			cout << G.getBeta() << " " << avgData << endl;
		}
	}
	
	if (cfg.output == python) {
		cout << "])\n"
		<< "x = 1/data[:,0]\n"
		<< "y = data[:,1]\n"
		<< "plt.plot(x, y)\n"
		<< "plt.show()" << endl;
	}
	cerr << "Done!" << endl;
	
    return 0;
}
