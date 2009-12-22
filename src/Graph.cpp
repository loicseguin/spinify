/*
 *  Graph.cpp
 *  spinify2
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "Graph.h"
//#include <iostream>

Node::Node() {
	spin = 0;
}

int Node::setSpin(int sp) {
	if(sp == -1 || sp == 1) {
		spin = sp;
		return 0;
	}
	else {
		return 1;
	}
}

void Node::addNghbor(Node* pN, Edge* pE) {
	nghbor.push_back(pN);
	edges.push_back(pE);
}

Edge::Edge() {
	endpt[0] = NULL;
	endpt[1] = NULL;
}

Graph::Graph() {
}

Graph::~Graph() {
	// std::cout << "freeing ..." << std::endl;
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	for (int i = 0; i < edges.size(); i++) {
		delete edges[i];
	}
}

void Graph::addNode(int N) {
	for(int i = 0; i < N; i++) {
		Node* n = new Node;
		nodes.push_back(n);
	}
}

void Graph::addEdge(Node* pN, Node* pM) {
	Edge* e = new Edge;
	e->endpt[0] = pN;
	e->endpt[1] = pM;
	pN->addNghbor(pM, e);
	pM->addNghbor(pN, e);
	edges.push_back(e);
}
	

	
	
	
