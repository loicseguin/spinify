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

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "Point3D.h"

enum Status {
	/*
	 * Used by simulation algorithms to determine when a Node or Edge
	 * has been visited or not.
	 *
	 */
	notVisited,
	Visited
};

#ifndef OUTPUT_ENUM
#define OUTPUT_ENUM
enum OutputType {
	python,
	raw
};
#endif


class Edge;
class Node;


class Graph {
	/*
	 * A graph is composed of a vector of pointers to Nodes and a vector
	 * of pointer to Edges. The constructor take an optional integer
	 * argument which is used to reserve memory space for nodes and
	 * edges pointers.
	 *
	 * Note that this class does not work for multigraph. Indeed,
	 * addEdge() verifies whether the Edge is already present, and if
	 * it is, it does not add the Edge again.
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
	 *
	 */
	std::vector<Node*> nodes;
	void prPython(std::ostream & output);
	void prRaw(std::ostream & output);
	
public:
	std::vector<Edge*> edges;
	Graph(int N = 10);
	~Graph();
	
	int size() const;
	void addNode(const int N = 1);
	void addEdge(Node& n, Node& m);
	Node& operator[](const int i);
	Node& operator[](const int i) const;
	void initRect(const int L = 10, const int W = 10);
	void randSpin();
	void resetStatus();
	void print(OutputType type = raw,
			   std::ostream & output = std::cout);
	void readFile(std::string graphInFile);
};



class Node {
	/* 
	 * A node has a spin in {-1, 1}, a numerical identifier, a status
	 * and a vector of incident Edges. It also has coordinates which
	 * are used for nodes on surfaces where the metric is non trivial.
	 * 
	 * The Node constructor sets the spin to an invalid value of 0, the
	 * status to notVisited and admits an argument which sets the idnum.
	 * If no argument is provided, the idnum defaults to 0. The
	 * coordinates default to (0,0,0).
	 *
	 * The idnum is read by getID().
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
	 * The coordinates are accessed with getCoords() and setCoords().
	 *
	 */
	int spin;
	int idnum;
	Status status;
	std::vector<Edge*> edges;
	Point3D coords;
	void addNghbor(Edge*);

public:
	Node(int idnum = 0);
	~Node();
	
	int getID();
	void setID(const int);
	void setSpin(const int);
	int getSpin();
	int degree();
	Edge& operator[](const int i);
	void setStatus(const Status);
	Status getStatus();
	Point3D& getCoords();
	void setCoords(const double x, const double y, const double z = 0);
	
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
	void setStatus(const Status);
	Status getStatus();
	Node& getOtherEnd(const Node&);
	Node& getV1() {return *v1;}
	Node& getV2() {return *v2;}
};


#endif // !GRAPH_H
