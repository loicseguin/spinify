/*
 *  Graph.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Graph.h"
#ifdef _WIN32
	#include "win32/tezuka.h"	
#else
	#include "tezuka.h"
#endif


Node::~Node()
{
	edges.clear();
}

void
Node::setSpin(const int sp)
{
	// In the Ising model, the spin can be only + or -. 
	if(sp == -1 || sp == 1) {
		spin = sp;
		return;
	}
	else {
		std::cerr << "Error: spin can be 1 or -1 only." << std::endl;
		return;
	}
}

void
Node::addNghbor(Edge* pE)
{
	edges.push_back(pE);
    nodes.push_back(&(pE->getOtherEnd(*this)));
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



Node&
Edge::getOtherEnd(const Node& n)
{
	// Given an node n which is an endpoint of edge e, this function
	// returns the other endpoint of edge e.
	if (v1 == &n)
		return *v2;
	else if (v2 == &n)
		return *v1;
	else {
		std::cerr << "Error: vertex not an endpoint." << std::endl;
		return *(Node*) 0;
	}
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
	
	// Check if edge is already in the graph.
	for (int i = 0; i < n.degree(); i++) {
		if (&(n[i].getOtherEnd(n)) == &m) {
			return; 
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
		std::cerr << "Error: "<< i << " is an invalid node index." << std::endl;
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
		std::cerr << "Error: "<< i << " is an invalid node index." << std::endl;
		return *(Node*) 0;
	}
}

void
Graph::initRect(const int L, const int W)
{
	// This function initializes a rectangular graph on the plane torus.
	// A rectangular graph of length L and height W is simply a lattice
	// isomorphic to the subset of the Z^2 lattice going from (x, y) =
	// (0, 0) to (x, y) = (L, W). To this lattice we add edges so that
	// the left most nodes are adjacent to the rightmost nodes, and the
	// topmost nodes are adjacent to the bottommost nodes.
	
	// If the graph is not empty, try to empty it.
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
	
	// Add nodes and edges.
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
	// Give every node a random spin. Note that this corresponds to a
	// high temperature configuration.
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
	// Reset every node and every edge to notVisited.
	for (unsigned int i = 0; i < edges.size(); i++)
		edges[i]->setStatus(notVisited);
	for (int i = 0; i < size(); i++)
		nodes[i]->setStatus(notVisited);
}

void
Graph::print(OutputType type, std::ostream & output)
{
	// Print graph data to output using either Python syntax or raw
	// syntax. The latter is just a list of edges (more details in the
	// description of the prRaw function).
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
	// This prints the graph information in a file that can be called
	// from the Python interpreter. It uses NumPy and matplotlib to plot
	// the graph on a three dimensional sphere.
	
	// Start with the header.
	output << "import numpy as np\n"
	<< "from mpl_toolkits.mplot3d import Axes3D\n"
	<< "import matplotlib.pyplot as plt\n"
	<< "data = np.array([";
	
	// Print the coordinate of every node.
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
	
	// Print every edge as a line whose endpoints are the coordinates of
	// the endpoints of the edge.
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
	
	// Actually plot the points.
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
	// The raw format is very simple. The first line is the number of
	// nodes, the second line is the number of edges. Then, every
	// subsequent line is an edge specified by the id of its two
	// endpoints. The ouput looks as follows:
	//   12
	//   8
	//   0 3
	//   0 9
	//   2 6
	//   4 5
	//   ... (8 more lines of edges)
	
	resetStatus();
	// Print number of nodes.
	output << size() << std::endl;
	
	// This is a routine to compute the number of edges based on the
	// fact that nEdge = (1/2) \sum_{v \in V} deg(v)
	int nEdge = 0;
	for (int i = 0; i < size(); i++) {
		nEdge += (*this)[i].degree();
	}
	nEdge /= 2;
	
	// Print the number of edges.
	output << nEdge << std::endl;
	
	// Print each edge on a separate line.
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
	// Reads the graph information from a file. The file has to be in
	// raw format.
	
	std::ifstream graphInPtr;
	graphInPtr.open(graphInFile.c_str(), std::ifstream::in);
	if (!(graphInPtr.is_open())) {
		std::cerr << "Error: Can't open graph input file." << std::endl;
		exit(1);
	}
	
	int nNodes = 0;
	unsigned int nEdges = 0;
	graphInPtr >> nNodes;
	graphInPtr >> nEdges;
	addNode(nNodes);
	for (unsigned int i = 0; i < nEdges; i++) {
		int index1;
		int index2;
		graphInPtr >> index1;
		graphInPtr >> index2;
		addEdge((*this)[index1], (*this)[index2]);
	}
}
