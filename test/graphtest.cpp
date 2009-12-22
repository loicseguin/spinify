/*
 *  graphtest.cpp
 *  spinify2
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "../src/Graph.h"
#include <iostream>

using namespace std;

int main(void) {
	Graph G;
	G.addNode(6);
	G.addEdge(G.nodes[0], G.nodes[4]);
	G.addEdge(G.nodes[3], G.nodes[4]);
	cout << "Node           Edge(s)" << endl;
	for (int i = 0; i < G.nodes.size(); i++) {
		cout << G.nodes[i] << "    ";
		for (int j = 0; j < G.nodes[i]->edges.size(); j++) {
			cout << G.nodes[i]->edges[j] << "    ";
		}
		cout << endl;
	}
	cout << endl << "Edge           Nodes" << endl;
	for (int i = 0; i < G.edges.size(); i++) {
		cout << G.edges[i] << "    " << G.edges[i]->endpt[0] << 
				"    " << G.edges[i]->endpt[1] << endl;
	}
	G.nodes[2]->setSpin(1);
	cout << endl << "Node           Spin" << endl;
	for (int i = 0; i < G.nodes.size(); i++) {
		cout << G.nodes[i] << "    " << G.nodes[i]->spin << endl;
	}
	return 0;
}