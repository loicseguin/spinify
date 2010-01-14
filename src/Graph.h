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


class Edge;
class Node;


class Graph {
	/*
	 * A graph is composed of a vector of pointers to Nodes and a vector
	 * of pointer to Edges. The constructor take an optional integer
	 * argument which is used to reserve memory space for nodes and
	 * edges pointers.
	 *
	 * size() returns the number of Nodes in the graph.
	 * addNode() add the number of nodes specified as the argument.
	 * Passing references to two Nodes to addEdge() adds the
	 * corresponding Edge.
	 * 
	 * The subscript [] operator takes an index i and returns a
	 * reference to the Node pointed to by nodes[i].
	 *
	 * initRect() creates a rectangular lattice of width W and length L
	 * with the appropriate Edges. It does check to see whether the
	 * graph is empty or not and tries to empty it if it's not. However,
	 * it is probably best to call initRect() only on an empty graph.
	 *
	 * randSpin() assigns a spin to each Node randomly (uniformly chosen
	 * amongst -1 or 1).
	 *
	 * resetStatus() sets the Status of every Edge and every Node to
	 * notVisited.
	 *
	 * initRand() initializes a random metric graph on a Surface S (NOT
	 * IMPLEMENTED YET).
	 *
	 */
	std::vector<Node*> nodes;
	
public:
	std::vector<Edge*> edges;
	Graph(int N = 10);
	~Graph();
	
	int size();
	void addNode(int N = 1);
	void addEdge(Node& n, Node& m);
	Node& operator[](int i);
	void initRect(int L = 10, int W = 10);
	void randSpin();
	void resetStatus();
};



class Node {
	/* 
	 * A node has a spin in {-1, 1}, a numerical identifier, a status
	 * and a vector of incident Edges.
	 * 
	 * The Node constructor sets the spin to an invalid value of 0, the
	 * status to notVisited and admits an argument which sets the idnum.
	 * If no argument is provided, the idnum defaults to 0. The idnum is
	 * read by getID().
	 *
	 * The spin is set and read by setSpin and getSpin. setSpin
	 * checks whether or not the argument is valid, if not, it prints
	 * an error message.
	 *
	 * Pointers to incident Edges are contained in a vector called
	 * edges. These pointers can be accessed with the [] operator.
	 * 
	 *     e.g.: G[1][3] is a pointer to the fourth incident Edge to
	 *           node G[1]. For instance, the Edge id is accessed with
	 *           G[1][3]->getID().
	 * 
	 * The [] operator does check for the validity of the index. If
	 * the index is invalid an error message is printed and -1 is
	 * returned.
	 *
	 * degree() just returns the degree of the Node, i.e., the number
	 * of incident Edges.
	 *
	 * Adding a new incident Edge is done by passing a pointer to an
	 * Edge to addNghbor(). This will just push back that index onto
	 * the edges vector.
	 *
	 * Status (default value = notVisited) can be stored with
	 * setStatus(), read with getStatus().
	 *
	 */
	int spin;
	int idnum;
	Status status;
	std::vector<Edge*> edges;
	double coords[3];
	void addNghbor(Edge*);

public:
	Node(int idnum = 0);
	~Node();
	
	int getID();
	void setSpin(int);
	int getSpin();
	int degree();
	Edge& operator[](int i);
	void setStatus(Status);
	Status getStatus();
	double* getCoords();
	void setCoords(double x, double y, double z = 0);
	
	friend void Graph::addEdge(Node& n, Node& m);
};


class Edge {
	/*
	 * An Edge has a numerical identifier, a status
	 * and pointers to the two endpoints.
	 *
	 * The constructor takes references to the two endpoints and an
	 * (optional) numerical identifier which defaults to 0. The
	 * numerical identifier is read by getID().
	 *
	 * The status is set by setStatus() and read by getStatus().
	 *
	 * Given a refenrece to one of the endpoints of an Edge,
	 * getOtherEnd() returns a reference to the other endpoint.
	 *
	 * The enpoints can be accessed with the getV1() and getV2()
	 * functions which returns references.
	 *
	 */
	Status status;
	int idnum;
	Node* v1;
	Node* v2;

public:
	Edge(Node& n, Node& m, int idnum = 0);
	
	int getID();
	void setStatus(Status);
	Status getStatus();
	Node& getOtherEnd(Node&);
	Node& getV1() {return *v1;}
	Node& getV2() {return *v2;}
};


#endif