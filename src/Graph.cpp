/*
 *  Graph.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include "Graph.h"
#include "tezuka.h"
#include <iostream>

Node::Node(int name = 0) {
	spin = 0;
	status = notVisited;
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

void Node::addNghbor(int n) { edges.push_back(n); }

int Node::operator[](int i) {
	if (i >= 0 && i < degree())
		return edges[i];
	else {
		std::cerr << "Error: invalid edge index." << std::endl;
		return -1;
	}
}

void Node::setStatus(Status s) { status = s; }

Status Node::getStatus() { return status; }


Edge::Edge(int n, int m, int name) {
	status = notVisited;
	idnum = name;
	v1 = n;
	v2 = m;
}

int Edge::getID() { return idnum; }

void Edge::setStatus(Status s) { status = s; }

Status Edge::getStatus() { return status; }

int Edge::getOtherEnd(int n) {
	if (v1 == n)
		return v2;
	else if (v2 == n)
		return v1;
	else {
		std::cerr << "Error: vertex not an endpoint." << std::endl;
		return -1;
	}

}

int Edge::operator[](int i) {
	if (i == 0)
		return v1;
	if (i == 1)
		return v2;
	else {
		std::cerr << "Error: invalid edge endpoint index." << std::endl;
		return -1;
	}
}


Graph::Graph(int N) {
	edges.reserve(N);
	nodes.reserve(N);
}

Graph::~Graph() {}

int Graph::size() { return nodes.size(); }

void Graph::addNode(int N) {
	static int j = 0;
	for(int i = 0; i < N; i++)
		nodes.push_back(Node(j++));
}

void Graph::addEdge(int n, int m) {
	static int j = 0;
	edges.push_back(Edge(n, m, j++));
	int edge_index = edges.size() - 1;
	nodes[n].addNghbor(edge_index);
	nodes[m].addNghbor(edge_index);
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

void Graph::initRect(int L, int W) {
	if (size() != 0) {
		edges.clear();
		nodes.clear();
	}
	int N = L * W;
	addNode(N);
	for (int i = 0; i < N; i++) {
		addEdge(i, (i + 1) % W + W * (i / W));
		addEdge(i, (i + W) % N);
	}
}

void Graph::randSpin() {
	//set_seed();
	for (int i = 0; i < size(); i++) {
		unsigned int rval = alea();
		if (rval % 2 == 0)
			nodes[i].setSpin(-1);
		else
			nodes[i].setSpin(1);
	}
}

void Graph::resetStatus() {
	for (unsigned int i = 0; i < edges.size(); i++)
		edges[i].setStatus(notVisited);
	for (unsigned int i = 0; i < size(); i++)
		nodes[i].setStatus(notVisited);
}
