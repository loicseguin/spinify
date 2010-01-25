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

Node::Node(int name) {
	spin = 0;
	status = notVisited;
	idnum = name;
}

Node::~Node() {
	edges.clear();
}

int Node::getID() { return idnum; }

void Node::setID(int n) { idnum = n; }

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

void Node::addNghbor(Edge* pE) { edges.push_back(pE); }

Edge& Node::operator[](int i) {
	if (i >= 0 && i < degree())
		return *(edges[i]);
	else {
		std::cerr << "Error: invalid edge index." << std::endl;
		return *(Edge*) 0;
	}
}

void Node::setStatus(Status s) { status = s; }

Status Node::getStatus() { return status; }

Point3D& Node::getCoords() { return coords; }

void Node::setCoords(double x, double y, double z) {
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}


Edge::Edge(Node& n, Node& m, int name) {
	status = notVisited;
	idnum = name;
	v1 = &n;
	v2 = &m;
}

int Edge::getID() { return idnum; }

void Edge::setStatus(Status s) { status = s; }

Status Edge::getStatus() { return status; }

Node& Edge::getOtherEnd(Node& n) {
	if (v1 == &n)
		return *v2;
	else if (v2 == &n)
		return *v1;
	else {
		std::cerr << "Error: vertex not an endpoint." << std::endl;
		return *(Node*) 0;
	}

}


Graph::Graph(int N) {
	edges.reserve(N);
	nodes.reserve(N);
}

Graph::~Graph() {
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	for (int i = 0; i < edges.size(); i++) {
		delete edges[i];
	}
	edges.clear();
	nodes.clear();
}

int Graph::size() { return nodes.size(); }

void Graph::addNode(int N) {
	static int j = 0;
	for (int i = 0; i < N; i++) {
		Node* pN = new Node(j++);
		nodes.push_back(pN);
	}
}

void Graph::addEdge(Node& n, Node& m) {
	static int j = 0;
	for (int i = 0; i < n.degree(); i++) {
		if (&(n[i].getOtherEnd(n)) == &m) {
			return; // Edge is already in the graph
		}
	}
	Edge* pE = new Edge(n, m, j++);
	edges.push_back(pE);
	n.addNghbor(pE);
	m.addNghbor(pE);
}

Node& Graph::operator[](int i) {
	if (i >= 0 && i < size()) {
		return *(nodes[i]);
	}
	else {
		std::cerr << "Error: "<<i<< " is an invalid node index." << std::endl;
		return *(Node*) 0;
	}
}

void Graph::initRect(int L, int W) {
	if (size() != 0) {
		for (int i = 0; i < nodes.size(); i++) {
			delete nodes[i];
		}
		for (int i = 0; i < edges.size(); i++) {
			delete edges[i];
		}
		edges.clear();
		nodes.clear();
	}
	int N = L * W;
	addNode(N);
	for (int i = 0; i < N; i++) {
		addEdge(*(nodes[i]), *(nodes[(i + 1) % W + W * (i / W)]));
		addEdge(*(nodes[i]), *(nodes[(i + W) % N]));
	}
}

void Graph::randSpin() {
	for (int i = 0; i < size(); i++) {
		unsigned int rval = alea();
		if (rval % 2 == 0)
			nodes[i]->setSpin(-1);
		else
			nodes[i]->setSpin(1);
	}
}

void Graph::resetStatus() {
	for (unsigned int i = 0; i < edges.size(); i++)
		edges[i]->setStatus(notVisited);
	for (unsigned int i = 0; i < size(); i++)
		nodes[i]->setStatus(notVisited);
}

