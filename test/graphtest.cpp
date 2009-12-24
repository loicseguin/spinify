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
	G.addEdge(G.nodes[0], G.nodes[1]);
	G.addEdge(G.nodes[0], G.nodes[3]);
	G.addEdge(G.nodes[0], G.nodes[4]);
	G.addEdge(G.nodes[1], G.nodes[2]);
	G.addEdge(G.nodes[1], G.nodes[3]);
	G.addEdge(G.nodes[1], G.nodes[5]);
	G.addEdge(G.nodes[2], G.nodes[4]);
	G.addEdge(G.nodes[4], G.nodes[5]);
	
	cout << "Node           Edge(s)" << endl;
	for (int i = 0; i < G.nodes.size(); i++) {
		cout << &G.nodes[i] << "    ";
		for (int j = 0; j < G.nodes[i].degree(); j++) {
			cout << G.nodes[i].edges[j] << "    ";
		}
		cout << endl;
	}
	cout << endl << "Edge           Nodes" << endl;
	for (int i = 0; i < G.edges.size(); i++) {
		cout << &G.edges[i] << "    " << G.edges[i].v1 << 
				"    " << G.edges[i].getOtherEnd(*G.edges[i].v1) << endl;
	}
	G.nodes[2].setSpin(1);
	cout << endl << "Node           Spin" << endl;
	for (int i = 0; i < G.nodes.size(); i++) {
		cout << &G[i] << "    " << G[i].getSpin() << endl;
	}
	return 0;
}
