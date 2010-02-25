/*
 *  Graph.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <iostream>

#include "Graph.h"
#include "tezuka.h"

Node::Node(int name)
{
	spin = 0;
	status = notVisited;
	idnum = name;
}

Node::~Node()
{
	edges.clear();
}

int
Node::getID()
{
	return idnum;
}

void
Node::setID(const int n)
{
	idnum = n;
}

void
Node::setSpin(const int sp)
{
	if(sp == -1 || sp == 1) {
		spin = sp;
		return;
	}
	else {
		std::cerr << "Error: spin can be 1 or -1 only." << std::endl;
		return;
	}
}

int
Node::getSpin()
{
	return spin;
}

int
Node::degree()
{ 
	return edges.size();
}

void
Node::addNghbor(Edge* pE)
{
	edges.push_back(pE);
}

Edge&
Node::operator[](const int i)
{
	if (i >= 0 && i < degree())
		return *(edges[i]);
	else {
		std::cerr << "Error: invalid edge index." << std::endl;
		return *(Edge*) 0;
	}
}

void
Node::setStatus(const Status s)
{
	status = s;
}

Status
Node::getStatus()
{
	return status;
}

Point3D&
Node::getCoords()
{
	return coords;
}

void
Node::setCoords(const double x, const double y, const double z)
{
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}


Edge::Edge(Node& n, Node& m, int name)
{
	status = notVisited;
	idnum = name;
	v1 = &n;
	v2 = &m;
}

int
Edge::getID()
{
	return idnum;
}

void
Edge::setStatus(const Status s)
{
	status = s;
}

Status
Edge::getStatus()
{
	return status;
}

Node&
Edge::getOtherEnd(const Node& n)
{
	if (v1 == &n)
		return *v2;
	else if (v2 == &n)
		return *v1;
	else {
		std::cerr << "Error: vertex not an endpoint." << std::endl;
		return *(Node*) 0;
	}
}


Graph::Graph(int N)
{
	edges.reserve(N);
	nodes.reserve(N);
}

Graph::~Graph()
{
	for (unsigned int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	for (unsigned int i = 0; i < edges.size(); i++) {
		delete edges[i];
	}
	edges.clear();
	nodes.clear();
}

int
Graph::size() const
{
	return nodes.size();
}

void
Graph::addNode(const int N)
{
	static int j = 0;
	for (int i = 0; i < N; i++) {
		Node* pN = new Node(j++);
		nodes.push_back(pN);
	}
}

void
Graph::addEdge(Node& n, Node& m)
{
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

Node&
Graph::operator[](const int i)
{
	if (i >= 0 && i < size()) {
		return *(nodes[i]);
	}
	else {
		std::cerr << "Error: "<<i<< " is an invalid node index." << std::endl;
		return *(Node*) 0;
	}
}

Node&
Graph::operator[](const int i) const
{
	if (i >= 0 && i < size()) {
		return *(nodes[i]);
	}
	else {
		std::cerr << "Error: "<<i<< " is an invalid node index." << std::endl;
		return *(Node*) 0;
	}
}

void
Graph::initRect(const int L, const int W)
{
	if (size() != 0) {
		for (unsigned int i = 0; i < nodes.size(); i++) {
			delete nodes[i];
		}
		for (unsigned int i = 0; i < edges.size(); i++) {
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

void
Graph::randSpin()
{
	for (int i = 0; i < size(); i++) {
		unsigned int rval = alea();
		if (rval % 2 == 0)
			nodes[i]->setSpin(-1);
		else
			nodes[i]->setSpin(1);
	}
}

void
Graph::resetStatus()
{
	for (unsigned int i = 0; i < edges.size(); i++)
		edges[i]->setStatus(notVisited);
	for (int i = 0; i < size(); i++)
		nodes[i]->setStatus(notVisited);
}

void
Graph::print(OutputType type, std::ostream & output)
{
	switch (type) {
		case python:
			prPython(output);
			break;
		case raw:
			prRaw(output);
			break;
		default:
			break;
	}
}

void
Graph::prPython(std::ostream & output)
{
	output << "import numpy as np\n"
	<< "from mpl_toolkits.mplot3d import Axes3D\n"
	<< "import matplotlib.pyplot as plt\n"
	<< "data = np.array([";
	
	for (int i = 0; i < size(); i++) {
		Point3D& pts = (*this)[i].getCoords();
		output.precision(14);
		output << "[" << pts[0] << ", " << pts[1] << ", "
		<< pts[2] << "],\n";
	}
	
	output << "])\n"
	<< "fig = plt.figure()" << std::endl
	<< "ax = Axes3D(fig)" << std::endl;
	
	resetStatus();
	
	for (int i = 0; i < size(); i++) {
		for (int j = 0; j < (*this)[i].degree(); j++) {
			if ((*this)[i][j].getStatus() == Visited) {
				continue;
			}
			(*this)[i][j].setStatus(Visited);
			Point3D& iCoords = (*this)[i].getCoords();
			Point3D& jCoords = ((*this)[i][j].getOtherEnd((*this)[i])).getCoords();
			output << "ax.plot(";
			for (int k = 0; k < 3; k++) {
				output << "[" << iCoords[k] << ", " << jCoords[k] << "],";
			}
			output << "'b' )\n";
		}
	}
	
	output
	<< "xs = data[:,0]" << std::endl
	<< "ys = data[:,1]" << std::endl
	<< "zs = data[:,2]" << std::endl
	<< "ax.scatter(xs, ys, zs)" << std::endl
	<< "plt.show()" << std::endl;
	resetStatus();
}

void
Graph::prRaw(std::ostream & output)
{
	resetStatus();
	output << size() << std::endl;
	int nEdge = 0;
	for (int i = 0; i < size(); i++) {
		nEdge += (*this)[i].degree();
	}
	nEdge /= 2;
	output << nEdge << std::endl;
	for (int i = 0; i < size(); i++) {
		for (int j = 0; j < (*this)[i].degree(); j++) {
			if ((*this)[i][j].getStatus() == Visited) {
				continue;
			}
			(*this)[i][j].setStatus(Visited);
			Node& neigh = (*this)[i][j].getOtherEnd((*this)[i]);
			output << (*this)[i].getID() << " " << neigh.getID() << std::endl;
		}
	}
}

void
Graph::readFile(std::string graphInFile)
{
}
