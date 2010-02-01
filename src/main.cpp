#include "Graph.h"
#include "Surface.h"
#include "Simul.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>

#define VERSION "2.0"


using namespace std;


enum LatticeType {
	sphere,
	rectangle
};

enum MeasureType {
	energy
};

enum OutputType {
	python,
	raw
};

double avg2(double* pV, const int nV) {
	double sum = 0;
	for (int i = 0; i < nV; i++) {
		sum += pV[i];
	}
	return sum / nV;
}

double stdDev(double *pV, const int nV) {
	double avgV = avg2(pV, nV);
	double sum = 0;
	for (int i = 0; i < nV; i++) {
		sum += (pV[i] - avgV)*(pV[i] - avgV);
	}
	return sqrt(sum / nV);
}

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

int main (int argc, char * const argv[]) {
	if (argc < 4 || argc > 12) {
		usage();
		exit(1);
	}
	
	int nNodes = 0;
	int N, M;
	vector<double> temps;
	int nTemps = 0;
	LatticeType lattice;
	bool lattice_set = 0;
	OutputType output;
	bool output_set = 0;
	
	for (int i = 1; i < argc; i++) {
		string buf = argv[i];
		// Spherical lattice
		if (buf == "-s" ) {
			if (lattice_set) {
				usage();
				exit(1);
			}
			else if ((nNodes = strtol(argv[++i], NULL, 0))) {
				lattice = sphere;
				lattice_set = 1;
			}
			else {
				usage();
				exit(1);
			}
		}
		
		// Rectangular lattice
		else if (buf == "-r") {
			if (lattice_set) {
				usage();
				exit(1);
			}
			else if ((N = strtol(argv[++i], NULL, 0))
					 && (M = strtol(argv[++i], NULL, 0))) {
				nNodes = N * M;
				lattice = rectangle;
				lattice_set = 1;
			}
			else {
				usage();
				exit(1);
			}
		}
		
		
		
		// Beta
		else if (buf == "-T") {
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
					nTemps = 50;
					for (int j = 0; j < nTemps; j++) {
						temps.push_back(get[0] + j * (get[1] - get[0]) / nTemps);
					}
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
		else if (buf == "-R") {
			if (output_set) {
				usage();
				exit(1);
			}
			output = raw;
			output_set = 1;
		}
		
		// Python output
		else if (buf == "-P") {
			if (output_set) {
				usage();
				exit(1);
			}
			output = python;
			output_set = 1;
		}
	}
	
	if (nTemps == 0) {
		temps.push_back(1./squareCriticalBeta);
		nTemps = 1;
	}
	
	if (!output_set) {
		output = raw;
	}
	
	Graph G(nNodes);
	
	switch (lattice) {
		case sphere:
			Sphere S;
			S.randNodes(G, nNodes);
			S.delaunay(G);
			if (output == raw) {
				string fileName = "lattice.dat";
				ofstream outFile(fileName.c_str());
				outFile << G.size() << endl;
				int nEdge = 0;
				for (int i = 0; i < G.size(); i++) {
					nEdge += G[i].degree();
					
				}
				nEdge /= 2;
				outFile << nEdge << endl;
				for (int i = 0; i < G.size(); i++) {
					for (int j = 0; j < G[i].degree(); j++) {
						if (G[i][j].getStatus() == Visited) {
							continue;
						}
						G[i][j].setStatus(Visited);
						Node& neigh = G[i][j].getOtherEnd(G[i]);
						outFile << G[i].getID() << " " << neigh.getID() << endl;
					}
				}
				outFile.close();
			}
			if (output == python) {
				string fileName = "lattice.py";
				ofstream outFile(fileName.c_str());
				outFile << "import numpy as np\n"
				<< "from mpl_toolkits.mplot3d import Axes3D\n"
				<< "import matplotlib.pyplot as plt\n"
				<< "data = np.array([";
				
				for (int i = 0; i < G.size(); i++) {
					Point3D& pts = G[i].getCoords();
					outFile.precision(14);
					outFile << "[" << pts[0] << ", " << pts[1] << ", "
					<< pts[2] << "],\n";
				}
				
				outFile << "])\n"
				<< "fig = plt.figure()" << endl
				<< "ax = Axes3D(fig)" << endl;
				
				for (int i = 0; i < G.size(); i++) {
					for (int j = 0; j < G[i].degree(); j++) {
						if (G[i][j].getStatus() == Visited) {
							continue;
						}
						G[i][j].setStatus(Visited);
						Point3D& iCoords = G[i].getCoords();
						Point3D& jCoords = (G[i][j].getOtherEnd(G[i])).getCoords();
						outFile << "ax.plot(";
						for (int k = 0; k < 3; k++) {
							outFile << "[" << iCoords[k] << ", " << jCoords[k] << "],";
						}
						outFile << "'b' )\n";
					}
				}
				
				outFile
				<< "xs = data[:,0]" << endl
				<< "ys = data[:,1]" << endl
				<< "zs = data[:,2]" << endl
				<< "ax.scatter(xs, ys, zs)" << endl
				<< "plt.show()" << endl;
				outFile.close();
			}
			break;
		case rectangle:
			G.initRect(N, M);
		default:
			break;
	}
	
	G.randSpin();
	Simul Sim(G);
	
	if (output == python) {
		cout << "import numpy as np\n"
		<< "import matplotlib.pyplot as plt\n"
		<< "data = np.array([";
	}
	
	int nMeasure = 100;
	
	for (int i = 0; i < nTemps; i++) {
		Sim.setParams(1./temps[i], -1);
		Sim.thermalize(500);
		//std::cerr << "Thermalized at temperature beta = " << Sim.getBeta() << std::endl;
		//std::cerr.flush();
		int K = Sim.findDecorrelTime(&Simul::measureE);
		double Data[nMeasure];
		for (int j = 0; j < nMeasure; j++) {
			Sim.thermalize(K);
			Data[j] = Sim.measureE();
		}
		double avgData = avg2(Data, nMeasure);
		if (output == python) {
			cout.precision(14);
			cout << "[" << Sim.getBeta() << ", " << avgData << "]," << endl;
		}
		else {
			cout.precision(14);
			cout << Sim.getBeta() << " " << avgData << endl;
		}
	}
	
	if (output == python) {
		cout << "])\n"
		<< "x = 1/data[:,0]\n"
		<< "y = data[:,1]\n"
		<< "pl.plot(x, y)\n"
		<< "pl.show()" << endl;
	}
	cerr << "Done!" << endl;
	
    return 0;
}
