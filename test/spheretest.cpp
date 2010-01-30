/*
 *  spheretest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-13.
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
	Graph G;
	Sphere S;
	S.randNodes(G, 400);
	
	cout << "Minimum distance: " << S.minDistance(G) << endl;
	cout << "  (Objective was: " << 4./sqrt(G.size())<< ")\n";
	
	S.delaunay(G);
	cout << "Minimum distance: " << S.minDistance(G) << endl;

	
	string fileName;
	fileName = "/Users/loic/Projects/spinify/spinify/test/sphereNodes.py";
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
			<< "ax = Axes3D(fig)" << endl
			<< "xs = data[:,0]" << endl
			<< "ys = data[:,1]" << endl
			<< "zs = data[:,2]" << endl
			<< "ax.scatter(xs, ys, zs)" << endl
			<< "plt.show()" << endl;
	outFile.close();
	
	fileName = "/Users/loic/Projects/spinify/spinify/test/sphereEdges.py";
	outFile.open(fileName.c_str());
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
	
	return 0;
}
