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
	const int uniform(Graph& G, int N);
	void repulse(Graph& G);
	double distance(Point3D& a, Point3D& b);
	double distanceSq(Point3D& a, Point3D& b);
public:
	const int randNodes(Graph& G, int N = 1);
	void delaunay(Graph& G);
	double minDistance(Graph& G);
};

class Plane {
public:
	void delaunay(Graph& G);
};


#endif
