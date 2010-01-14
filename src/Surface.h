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

class Surface {
	double rangeU[2];
	double rangeV[2];
public:
	//double dist() {};
};

class Sphere: public Surface {
public:
	const int randNodes(Graph& G, int N = 1);
};

#endif
