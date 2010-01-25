/*
 *  planedelaunaytest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-24.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */


#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "../src/Graph.h"
#include "../src/Surface.h"
#include "../src/Point3D.h"

using namespace std;

int main (void) {
	Plane P;
	Graph G;
	G.addNode(9);
	G[0].setCoords(0,0,0);
	G[1].setCoords(1,1,0);
	G[2].setCoords(-1,1,0);
	G[3].setCoords(-1,-1,0);
	G[4].setCoords(0.5,-0.6,0);
	G[5].setCoords(0.9,-.2,0);
	G[6].setCoords(0.3,0.7,0);
	G[7].setCoords(1,0.4,0);
	G[8].setCoords(0.2,-0.4,0);
	P.delaunay(G);
	
	string fileName = "/Users/loic/Projects/spinify/spinify/test/sphereEdges.py";
	ofstream outFile(fileName.c_str());
	outFile << "import numpy as np\n"
	<< "import matplotlib.pyplot as plt\n"
	<< "data = np.array([";
	
	for (int i = 0; i < G.size(); i++) {
		Point3D& pts = G[i].getCoords();
		outFile.precision(14);
		outFile << "[" << pts[0] << ", " << pts[1] << "],\n";
	}
	
	outFile << "])\n"
	<< "fig = plt.figure()" << endl;
	
	for (int i = 0; i < G.size(); i++) {
		cout << "Degree of node " << i << ": " << G[i].degree() << endl;
		for (int j = 0; j < G[i].degree(); j++) {
			if (G[i][j].getStatus() == Visited) {
				continue;
			}
			G[i][j].setStatus(Visited);
			Point3D& iCoords = G[i].getCoords();
			Point3D& jCoords = (G[i][j].getOtherEnd(G[i])).getCoords();
			outFile << "plt.plot(";
			for (int k = 0; k < 2; k++) {
				outFile << "[" << iCoords[k] << ", " << jCoords[k] << "],";
			}
			outFile << ")\n";
		}
	}
	
	outFile
	<< "xs = data[:,0]" << endl
	<< "ys = data[:,1]" << endl
	<< "plt.scatter(xs, ys)" << endl
	<< "plt.show()" << endl;
	outFile.close();
	
	return 0;
	
	
}
