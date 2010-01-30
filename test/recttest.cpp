/*
 *  recttest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-29.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */


#include "../src/Graph.h"
#include "../src/tezuka.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(void) {
	//Create a rectangle toroidal graph.
	Graph G;
	G.initRect(10, 10);
	G.randSpin();
	
	// Print graph info.
	cout << "Printing node info:" << endl
	<< "node (spin):   edge1 edge2 ..." << endl;
	for (int i = 0; i < G.size(); i++) {
		cout << setw(3) << G[i].getID() << "   (" << setw(2)
		<< G[i].getSpin() << ") : ";
		for (int j = 0; j < G[i].degree(); j++) {
			cout << setw(6) << G[i][j].getID();
		}
		cout << endl;
	}
	
	cout << endl << "Printing edge info:" << endl
	<< "edge (status):  node1 node2" << endl;
	for (unsigned int i = 0; i < G.edges.size(); i++) {
		cout << setw(4)  << G.edges[i]->getID() << " (" << setw(3)
		<< G.edges[i]->getStatus() << ") :"
		<< setw(6) << G.edges[i]->getV1().getID() << setw(6)
		<< G.edges[i]->getV2().getID() << endl;
	}
	cout << endl;
	
	return 0;
}
