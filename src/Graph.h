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

struct Node {
	int spin;
	std::vector<struct Node*> nghbor;
	std::vector<struct Edge*> edges;
	Node();
	int setSpin(int);
	void addNghbor(struct Node*, struct Edge*);
};


struct Edge {
	struct Node* endpt[2];
	Edge();
};
	

struct Graph {
	std::vector<struct Node*> nodes;
	std::vector<struct Edge*> edges;
	Graph();
	~Graph();
	void addNode(int N = 1);
	void addEdge(struct Node*, struct Node*);
};

#endif