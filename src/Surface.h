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


const double Pi = 3.141592653589793;


class Sphere {
	/*
	 * Sphere is an implementation of S^2 embedded in R^3.
	 * When given a Graph reference, Sphere can generate Nodes for that
	 * graph that are either uniformly or evenly distributed.
	 *
	 * Uniform distribution means that every surface element on the
	 * sphere has the same probability of containing a Node. The
	 * uniform() method generates N Nodes with this probability
	 * distribution. The algorithm is taken from:
	 *
	 *   Marsaglia, G. 1972. Choosing a Point from the Surface of a
	 *   Sphere. The Annals of Mathematical Statistics 43, 2
	 *   (Apr. 1972), 645-646.
	 * 
	 * Even distribution of Nodes means that the minimum distance
	 * between any two Nodes is maximized. The approach taken here is
	 * to approximate an even distribution by making the Nodes behave
	 * as electrons subject to electric repulsion. This is taken care
	 * of by the repulse(). Repulsion is performed as long as the
	 * minimum distance is below a given lower bound.
	 *
	 * distance() and distanceSq() return the distance and the square
	 * of the distance (respectively) between any two Nodes on the
	 * Sphere.
	 *
	 * evenNodes() adds N Nodes to Graph G and ensures that they are
	 * (approximately) evenly distributed. It proceeds by first
	 * generating Nodes uniformly and then calling repulse().
	 *
	 * Once Nodes are placed on the sphere, they can be triangulated in
	 * order to determine the closest neighbours and add the
	 * corresponding Edges to the Graph. Triangulation is done as
	 * follows. For every Node, a neighbourhood is considered and
	 * projected on the tangent Plane. Then, the Nodes on the tangent
	 * Plane are triangulated using the Plane::delaunay() procedure.
	 *
	 * minDistance() calculates the minimum distance between any two
	 * Nodes on the sphere.
	 *
	 */
	unsigned int rangeMultiplier;
	double dampingExp;
	unsigned int dampingSub;
	double objectiveRatio;	
	void repulse(Graph& G);
	double distance(const Point3D& a, const Point3D& b) const;
	double distanceSq(const Point3D& a, const Point3D& b) const;
public:
	Sphere();
	int uniform(Graph& G, const int N);
	int evenNodes(Graph& G, const int N = 1);
	void delaunay(Graph& G);
	double minDistance(const Graph& G) const;
	void setParams(unsigned int rMultiplier,
				   double dExp,
				   unsigned int dSub,
				   double oRatio);
};

inline
Sphere::Sphere()
    : rangeMultiplier(10), dampingExp(1.2915),
      dampingSub(10000), objectiveRatio(0.21) {}

inline double
Sphere::distance(const Point3D& a, const Point3D& b) const
{
	return (a - b).norm();
}

inline double
Sphere::distanceSq(const Point3D& a, const Point3D& b) const
{
	return (a - b).normSq();
}




class Plane {
	/*
	 * delaunay() takes a Graph with a node at the origin and finds the
	 * nearest neighbours of the origin. A Node is a nearest neighbour
	 * of the origin if it has an edge to the origin in the Delaunay
	 * triangulation of the Graph.
	 *
	 */
public:
	void delaunay(Graph& G);
};

struct Angle {
	double angle;
	Node& node;
};

#endif // !SURFACE_H
