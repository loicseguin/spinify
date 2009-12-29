/*
 * Graph.h
 * spinify2
 *
 * Created by Loïc Séguin-Charbonneau on 09-12-22.
 * Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

/*
 * Graph is a basic data structure for graphs implemented as two
 * vectors: a vector of Edges and a vector of Nodes. Each Edge
 * has pointers to its two endpoints. Each Node has a spin and a vector
 * of edges to which it is adjacent.
 *
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>


class Node {
	/* 
	 * A node has a spin in {-1, 1}, a numerical identifier and a set
	 * of incident Edges.
	 * 
	 * The Node constructor sets the spin to an invalid value of 0 and
	 * admits an argument which sets the idnum. If no argument is
	 * provided, the idnum defaults to 0. The idnum is read by getID().
	 *
	 * The spin is set and read by setSpin and getSpin. setSpin
	 * checks whether or not the argument is valid, if not, it prints
	 * an error message.
	 *
	 * Pointers to incident Edges are contained in a vector called
	 * edges. These pointers can be accessed with the [] operator.
	 * 
	 *     e.g.: v[3] is a pointer to the fourth incident Edge.
	 * 
	 * The [] operator does check for the validity of the index. If
	 * the index is invalid an error message is printed and a NULL
	 * pointer is returned.
	 *
	 * degree() just returns the degree of the Node, i.e., the number
	 * of incident Edges.
	 *
	 * Adding a new incident Edge is done by passing a reference to an
	 * Edge to addNghbor(). This will just push back a pointer to the
	 * new incident edge onto the edges vector.
	 *
	 */
	int spin;
	int idnum;
	std::vector<int> edges;
public:
	Node(int idnum);
	int getID();
	void setSpin(int);
	int getSpin();
	int degree();
	void addNghbor(int);
	int operator[](int i);
};


class Edge {
	/*
	 *
	 */
	int data;
	int idnum;
	int v1;
	int v2;
public:
	Edge(int n, int m, int idnum);
	int getID();
	void setData(int);
	int getData();
	int getOtherEnd(int);
	int operator[](int i);
};


class Graph {
	std::vector<class Node> nodes;
public:
	std::vector<class Edge> edges;
	Graph(int N = 10);
	~Graph();
	int size();
	void addNode(int N = 1);
	void addEdge(int n, int m);
	Node& operator[](int i);
};

#endif