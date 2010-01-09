/*
 * Graph.h
 * spinify
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


enum Status {
	/*
	 * Used by simulation algorithms to determine when a Node or Edge
	 * has been visited or not.
	 *
	 */
	notVisited,
	Visited
};


class Node;
class Edge;


class Graph {
	/*
	 * A graph is composed of a vector of Nodes and a vector of Edges.
	 * The constructor take an optional integer argument which is used
	 * to reserve memory space for the nodes and edges.
	 *
	 * size() returns the number of Nodes in the graph.
	 * addNode() add the number of nodes specified as the argument.
	 * Passing the indices of two Nodes to addEdge() add the
	 * corresponding Edge.
	 * 
	 * The subscript [] operator takes an index i and returns a
	 * reference to the nodes[i].
	 *
	 * initRect() creates a rectangular lattice of width W and length L
	 * with the appropriate Edges. It does check to see whether the
	 * graph is empty or not and tries to empty it if it's not. However,
	 * it is probably best to call initRect() only on an empty graph.
	 *
	 * randSpin() assigns a spin to each Node randomly (uniformly chosen
	 * amongst -1 or 1).
	 *
	 * resetStatus() sets the status of every Edge and every Node to
	 * notVisited.
	 *
	 */
	std::vector<class Node> nodes;
public:
	std::vector<class Edge> edges;
	Graph(int N = 10);
	~Graph();
	int size();
	void addNode(int N = 1);
	void addEdge(int n, int m);
	Node& operator[](int i);
	void initRect(int L = 10, int W = 10);
	void randSpin();
	void resetStatus();
};


class Node {
	/* 
	 * A node has a spin in {-1, 1}, a status, a numerical identifier
	 * and a set of incident Edges.
	 * 
	 * The Node constructor sets the spin to an invalid value of 0, the
	 * status to notVisited and admits an argument which sets the idnum. If no
	 * argument is provided, the idnum defaults to 0. The idnum is read
	 * by getID().
	 *
	 * The spin is set and read by setSpin and getSpin. setSpin
	 * checks whether or not the argument is valid, if not, it prints
	 * an error message.
	 *
	 * Indices of incident Edges are contained in a vector called
	 * edges. These indices can be accessed with the [] operator.
	 * 
	 *     e.g.: G[1][3] is an integer to the fourth incident Edge to
	 *           node G[1]. The Edge itself is accessed with
	 *           G.edges[G[1][3]].
	 * 
	 * The [] operator does check for the validity of the index. If
	 * the index is invalid an error message is printed and -1 is returned.
	 *
	 * degree() just returns the degree of the Node, i.e., the number
	 * of incident Edges.
	 *
	 * Adding a new incident Edge is done by passing the index of an
	 * Edge to addNghbor(). This will just push back that index onto
	 * the edges vector.
	 *
	 * Status (default value = notVisited) can be stored with setStatus(),
	 * read with getStatus().
	 *
	 */
	int spin;
	int idnum;
	Status status;
	std::vector<int> edges;
	void addNghbor(int);
public:
	Node(int idnum);
	int getID();
	void setSpin(int);
	int getSpin();
	int degree();
	int operator[](int i);
	void setStatus(Status);
	Status getStatus();
	
	friend void Graph::addEdge(int n, int m);
};


class Edge {
	/*
	 * An Edge has a numerical identifier, a status
	 * and the indices of the two endpoints.
	 *
	 * The constructor takes the indices of the two endpoints and an
	 * (optional) numerical identifier which defaults to 0. The
	 * numerical identifier is read by getID() (which might be
	 * deprecated in the near future).
	 *
	 * The status is set by setStatus() and read by getStatus().
	 *
	 * Given the node index of one endpoint of an Edge, getOtherEnd()
	 * returns the index of the other endpoint.
	 *
	 * The endpoints indices can be accessed with the [] operator. It
	 * takes an argument which is either 0 or 1 and returns the index
	 * of the corresponding endpoint.
	 *
	 */
	Status status;
	int idnum;
	int v1;
	int v2;
public:
	Edge(int n, int m, int idnum = 0);
	int getID();
	void setStatus(Status);
	Status getStatus();
	int getOtherEnd(int);
	int operator[](int i);
};


#endif
