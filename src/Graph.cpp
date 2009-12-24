/*
 *  Graph.cpp
 *  spinify2
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "Graph.h"
#include <iostream>

Node::Node() { spin = 0; }

void Node::setSpin(int sp) {
	if(sp == -1 || sp == 1) {
		spin = sp;
		return;
	}
	else {
		std::cerr << "spin can be 1 or -1 only." << std::endl;
		return;
	}
}

int Node::getSpin() { return spin; }

int Node::degree() { return edges.size(); }

void Node::addNghbor(Edge& rE) { edges.push_back(&rE); }


Edge::Edge(Node& rN, Node& rM) {
	data = 0;
	v1 = &rN;
	v2 = &rM;
}

void Edge::setData(int x) { data = x; }

int Edge::getData() { return data; }

Node* Edge::getOtherEnd(Node& rN) {
	if (v1 == &rN)
		return v2;
	else
		return v1;
}	


Graph::Graph() {
}

Graph::~Graph() {
	/*for (int i = 0; i < nodes.size(); i++) {
		std::cout << "freeing node " << i << std::endl;
		delete &nodes[i];
	}
	for (int i = 0; i < edges.size(); i++) {
		delete &edges[i];
	}*/
}

void Graph::addNode(int N) {
	for(int i = 0; i < N; i++)
		nodes.push_back(Node());
}

void Graph::addEdge(Node& rN, Node& rM) {
	edges.push_back(Edge(rN, rM));
	rN.addNghbor(edges.back());
	rM.addNghbor(edges.back());
}

Node& Graph::operator[](int i) { return nodes[i]; }
	

	
	
	
