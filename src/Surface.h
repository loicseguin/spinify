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

/*
 * Template for Surface:
 *
 *	class Surface {
 *		double rangeU[2];
 *		double rangeV[2];
 *	public:
 *		const in randNodes(Graph& G, int N = 1)
 *	};
 *
 */

class Sphere/*: public Surface*/ {
	const int uniform(Graph& G, int N);
	void repulse(Graph& G);
	double distance(Point3D& a, Point3D& b);
public:
	const int randNodes(Graph& G, int N = 1);
	double minDistance(Graph& G);
};

#endif
