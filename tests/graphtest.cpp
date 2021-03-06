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

#include <fstream>
#include <iomanip>
#include <iostream>

#include "../src/Graph.h"
#include "../src/Point3D.h"

using namespace std;

int
main(void)
{
	// Create the test graph.
	Graph G;
	G.addNode(6);
	G.addEdge(G[0], G[1]);
	G.addEdge(G[0], G[3]);
	G.addEdge(G[0], G[4]);
	G.addEdge(G[1], G[2]);
	G.addEdge(G[1], G[3]);
	G.addEdge(G[1], G[5]);
	G.addEdge(G[2], G[4]);
	G.addEdge(G[4], G[5]);
	G.addEdge(G[5], G[4]);
	
	// Assign some spins and statuses.
	G[0].setSpin(-1);
	G[1].setSpin(-1);
	G[2].setSpin(1);
	G[3].setSpin(-1);
	G[4].setSpin(-1);
	G[5].setSpin(1);
	G.edges[0]->setStatus(Visited);
	G.edges[2]->setStatus(Visited);
	G.edges[3]->setStatus(Visited);
	G.edges[4]->setStatus(Visited);
	G.edges[5]->setStatus(Visited);
	G.edges[7]->setStatus(Visited);
	
	
	// Print graph info.
	cout << "Printing node info:" << endl
		 << "  node (spin): edge1 edge2 ..." << endl;
	for (int i = 0; i < G.size(); i++) {
		cout << "  " << G[i].getID() << " (" << setw(2)
			 << G[i].getSpin() << "):  ";
		for (int j = 0; j < G[i].degree(); j++) {
			cout << G[i][j].getID() << "    ";
		}
		cout << endl;
	}
	
	cout << endl << "Printing edge info:" << endl
		 << "  edge (status): node1 node2" << endl;
	for (unsigned int i = 0; i < G.edges.size(); i++) {
		cout << "  "  << G.edges[i]->getID() << " (" << setw(3)
		<< G.edges[i]->getStatus() << "):  " << G.edges[i]->getV1().getID()
		<< "    " << G.edges[i]->getV2().getID() << endl;
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
			cout << G[i][j].getID() << "    ";
		}
		cout << endl;
	}
	
	cout << endl << "Printing edge info:" << endl
	<< "  edge (status): node1 node2" << endl;
	for (unsigned int i = 0; i < G.edges.size(); i++) {
		cout << "  "  << G.edges[i]->getID() << " (" << setw(3)
		<< G.edges[i]->getStatus() << "):  " << G.edges[i]->getV1().getID()
		<< "    " << G.edges[i]->getV2().getID() << endl;
	}
	cout << endl;
	
	G[0].setCoords(3.5, -12.325);
	Point3D& pts = G[0].getCoords();
	cout << "Coordinates of node " << 0 << ": (" << pts[0] << ", "
		 << pts[1] <<")" << endl;
	
	
	// Print Graph using builtin print() function.
	bool outFileRequested = true;
	
	std::ofstream realOutFile;
	std::ostream & output = outFileRequested
		? realOutFile.open("foo.txt", std::ios::out), realOutFile
		: std::cout;
	G.print(raw, output);
	
	return 0;
}
