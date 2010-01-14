/*
 *  spheretest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-13.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */


#include <iostream>
#include "../src/Graph.h"
#include "../src/Surface.h"
#include <fstream>
#include <string>


using namespace std;


int main (void) {
	Graph G;
	Sphere S;
	S.randNodes(G, 8000);
	
	string fileName;
	fileName = "/Users/loic/Projects/spinify/spinify/test/sphereNodes.py";
	ofstream outFile(fileName.c_str());
	outFile << "import numpy as np\n"
			<< "from mpl_toolkits.mplot3d import Axes3D\n"
			<< "import matplotlib.pyplot as plt\n"
			<< "data = np.array([";
	
	for (int i = 0; i < G.size(); i++) {
		double* pts = G[i].getCoords();
		outFile << "[" << *pts << ", " << *(pts + 1) << ", "
				<< *(pts + 2) << "],\n";
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
	return 0;
}
