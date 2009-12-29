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

Node::Node(int name = 0) {
	spin = 0;
	idnum = name;
}

int Node::getID() { return idnum; }

void Node::setSpin(int sp) {
	if(sp == -1 || sp == 1) {
		spin = sp;
		return;
	}
	else {
		std::cerr << "Error: spin can be 1 or -1 only." << std::endl;
		return;
	}
}

int Node::getSpin() { return spin; }

int Node::degree() { return edges.size(); }

void Node::addNghbor(Edge& rE) {
	edges.push_back(&rE);
	std::cout << "  added edge " << edges.back()->getID() << " to node "
	<< getID() << std::endl;
}

Edge* Node::operator[](int i) {
	if (i >= 0 && i < degree())
		return edges[i];
	else {
		std::cerr << "Error: invalid edge index." << std::endl;
		return 0;
	}
}


Edge::Edge(Node& rN, Node& rM, int name) {
	data = 0;
	idnum = name;
	v1 = &rN;
	v2 = &rM;
}

int Edge::getID() { return idnum; }

void Edge::setData(int x) { data = x; }

int Edge::getData() { return data; }

Node* Edge::getOtherEnd(Node& rN) {
	if (v1 == &rN)
		return v2;
	else if (v2 == &rN)
		return v1;
	else {
		std::cerr << "Error: vertex not an endpoint." << std::endl;
		return 0;
	}

}

Node* Edge::operator[](int i) {
	if (i == 0)
		return v1;
	if (i == 1)
		return v2;
	else {
		std::cerr << "Error: invalid edge endpoint index." << std::endl;
		return 0;
	}
}


Graph::Graph() {
	edges.reserve(10);
	nodes.reserve(10);
}

Graph::~Graph() {}

int Graph::size() { return nodes.size(); }

void Graph::addNode(int N) {
	static int j = 0;
	for(int i = 0; i < N; i++)
		nodes.push_back(Node(j++));
}

void Graph::addEdge(Node& rN, Node& rM) {
	static int j = 0;
	edges.push_back(Edge(rN, rM, j++));
	Edge& rE = edges.back();
	std::cout << "Adding edge " << rE.getID() << "..." << std::endl;
	rN.addNghbor(rE);
	rM.addNghbor(rE);
}

Node& Graph::operator[](int i) {
	if (i >= 0 && i < size()) {
		return nodes[i];
	}
	else {
		std::cerr << "Error: invalid node index." << std::endl;
		return *(Node*) 0;
	}
}
	

	
	
	
