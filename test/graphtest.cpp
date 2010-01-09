/*
 *  graphtest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

/*
 * Here is the test graph:
 *          0        3
 *      0 ------ 1 ------ 2
 *      |\      / \      /
 *      | \2  4/   \5  6/
 *      |  \  /     \  /
 *    1 |   \/       \/
 *      |   /\       /\
 *      |  /  \     /  \
 *      | /    \   /    \
 *      |/      \ /  7   \
 *      3        4 ------ 5
 *
 */

#include "../src/Graph.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main(void) {
	// Create the test graph.
	Graph G;
	G.addNode(6);
	G.addEdge(0, 1);
	G.addEdge(0, 3);
	G.addEdge(0, 4);
	G.addEdge(1, 2);
	G.addEdge(1, 3);
	G.addEdge(1, 5);
	G.addEdge(2, 4);
	G.addEdge(4, 5);
	
	// Assign some spins and statuses.
	G[0].setSpin(-1);
	G[1].setSpin(-1);
	G[2].setSpin(1);
	G[3].setSpin(-1);
	G[4].setSpin(-1);
	G[5].setSpin(1);
	G.edges[0].setStatus(Visited);
	G.edges[2].setStatus(Visited);
	G.edges[3].setStatus(Visited);
	G.edges[4].setStatus(Visited);
	G.edges[5].setStatus(Visited);
	G.edges[7].setStatus(Visited);
	
	
	// Print graph info.
	cout << "Printing node info:" << endl
		 << "  node (spin): edge1 edge2 ..." << endl;
	for (int i = 0; i < G.size(); i++) {
		cout << "  " << G[i].getID() << " (" << setw(2)
			 << G[i].getSpin() << "):  ";
		for (int j = 0; j < G[i].degree(); j++) {
			cout << G[i][j] << "    ";
		}
		cout << endl;
	}
	
	cout << endl << "Printing edge info:" << endl
		 << "  edge (status): node1 node2" << endl;
	for (int i = 0; i < G.edges.size(); i++) {
		cout << "  "  << G.edges[i].getID() << " (" << setw(3)
		<< G.edges[i].getStatus() << "):  ";
		for (int j = 0; j < 2; j++) {
			cout << G.edges[i][j] << "    ";
		}
		cout << endl;
	}
	cout << endl;
	
	// Reset status.
	G.resetStatus();
	
	// Print graph info.
	cout << "Printing node info:" << endl
	<< "  node (spin): edge1 edge2 ..." << endl;
	for (int i = 0; i < G.size(); i++) {
		cout << "  " << G[i].getID() << " (" << setw(2)
		<< G[i].getSpin() << "):  ";
		for (int j = 0; j < G[i].degree(); j++) {
			cout << G[i][j] << "    ";
		}
		cout << endl;
	}
	
	cout << endl << "Printing edge info:" << endl
	<< "  edge (status): node1 node2" << endl;
	for (int i = 0; i < G.edges.size(); i++) {
		cout << "  "  << G.edges[i].getID() << " (" << setw(3)
		<< G.edges[i].getStatus() << "):  ";
		for (int j = 0; j < 2; j++) {
			cout << G.edges[i][j] << "    ";
		}
		cout << endl;
	}
	cout << endl;
	
	
	
	return 0;
}
