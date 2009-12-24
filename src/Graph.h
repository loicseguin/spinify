/*
 *  Graph.h
 *  spinify2
 *
 *  Created by Loïc Séguin-Charbonneau on 09-12-22.
 *  Copyright 2009 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>


class Node {
	public:int spin;
	std::vector<class Edge*> edges;

	Node();
	void setSpin(int);
	int getSpin();
	int degree();
	void addNghbor(class Edge&);
};


class Edge {
	public:int data;
	class Node* v1;
	class Node* v2;

	Edge(class Node&, class Node&);
	void setData(int);
	int getData();
	class Node* getOtherEnd(class Node&);
};


class Graph {
public:	std::vector<class Node> nodes;
	std::vector<class Edge> edges;

	Graph();
	~Graph();
	void addNode(int N = 1);
	void addEdge(class Node&, class Node&);
	Node& operator[](int i);
};

#endif