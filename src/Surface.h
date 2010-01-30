/*
 *  Surface.h
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-08.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#ifndef SURFACE_H
#define SURFACE_H


#include "Graph.h"


class Sphere {
	int uniform(Graph& G, const int N);
	void repulse(Graph& G);
	double distance(const Point3D& a, const Point3D& b) const;
	double distanceSq(const Point3D& a, const Point3D& b) const;
public:
	int randNodes(Graph& G, const int N = 1);
	void delaunay(Graph& G);
	double minDistance(const Graph& G) const;
};

class Plane {
public:
	void delaunay(Graph& G);
};


#endif
