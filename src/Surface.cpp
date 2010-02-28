/*
 *  Surface.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-08.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>

#include "Point3D.h"
#include "Surface.h"
#include "tezuka.h"


Sphere::Sphere()
{
	rangeMultiplier = 10;
	dampingExp = 1.2915;
	dampingSub = 10000;
	objectiveRatio = 0.21;
}

int
Sphere::uniform(Graph& G, int N)
{
	// This generates points that are uniformly distributed on the
	// unit sphere. The method for generating these points comes from
	// Marsaglia, 1972 (see Surface.h for complete refenrence).
	//
	// The function returns the number of points that were added to the
	// sphere.
	
	int curIndex = G.size() - 1;
	int counter = 0;
	
	for (int i = 0; i < N; i++) {
		double v1 = 1.;
		double v2 = 1.;
		double s = v1*v1 + v2*v2;
		while (s >= 1) {
			v1 = -1. + 2.*alea()/RANDMAX;
			v2 = -1. + 2.*alea()/RANDMAX;
			s = v1*v1 + v2*v2;
		}
		G.addNode();
		curIndex++;
		G[curIndex].setCoords(2*v1*sqrt(1 - s), 2*v2*sqrt(1 - s), 1 - 2*s);
		counter++;
	}
	return counter;
}

void
Sphere::repulse(Graph& G)
{
	// An even node distribution is defined to be a node distribution
	// for which the minimum distance between any two nodes is
	// maximized. In order to achieve this, we start with a uniform
	// distribution and we perform a serie of repulsions. Repulsions are
	// based on an electric repulsion between electrons on the sphere.
	//
	// Imagine every node has the same electric charge. Then, they will
	// repel each other with a force proportional to the inverse of the
	// square of the distance between them. If we let the nodes repel
	// for sufficiently long time while constraining them to stay on the
	// sphere, they will eventually find a very good approximation for
	// optimal solution.
	
	// If every node is the center of a circle, that no circle
	// overlap, and that we want all N nodes to lie on the unit sphere,
	// then the radius r of the small circles can be approximated by
	//   4 \pi / (\pi r^2) = N
	//   r = 2 / \sqrt{N}
	// Of course, this is an over estimation.
	const double expectedRadius = 2./sqrt(G.size());
	
	// Computing the force on a node becomes long if the number of nodes
	// is large. However, since the force decreases with the square of
	// the distance, it is reasonable to compute only the force due to
	// nodes that are nearby, i.e, the force we use has a finite range.
	const double range = rangeMultiplier * expectedRadius;
	
	// The minimum distance between two nodes should be close to twice
	// radius of the disks.
	const double objectiveDist = 4./sqrt(G.size());
	
	// If the force is not dampened, it quickly become so big that the
	// nodes just move randomly on the sphere. To avoid this problem, we
	// dampen the forces by a factor dependent on the size of the graph.
	// The following damping coefficient is a crude guess based on
	// a couple of experiments. More thoughts should be put into this.
	const double dampingCoeff =
		std::max(pow(G.size(), dampingExp),
				 pow(G.size(), dampingExp) - dampingSub);
	
	double minDist = 0.;
	int counter = 0;
	
	// While the minimum distance is not sufficiently close to the
	// expectedRadius, repeat the repulsion process.
	while ((objectiveDist - minDist) / objectiveDist > objectiveRatio) {
		
		// We process each node in turn. We compute the force on a node
		// by summing the force due to each node within a distance of
		// range and we move the node along the direction of the force.
		// Then, the node position is normalized so that it lies on the
		// sphere.
		for (int i = 0; i < G.size(); i++){
			Point3D force(0);
			Point3D& iCoords = G[i].getCoords();
			for(int j = 0; j < G.size(); j++) {
				if (j == i)
					continue;
				Point3D& jCoords = G[j].getCoords();
				double dSq = distanceSq(iCoords, jCoords);
				if (dSq > range)
					continue;
				double d = distance(iCoords, jCoords);
				Point3D temp = (iCoords - jCoords).divide(dSq*d);
				force = force + temp;
			}
			force.divide(dampingCoeff);
			iCoords = force + iCoords;
			iCoords.normalize();
		}
		counter++;
		minDist = minDistance(G);
	}
	// Print the number of repulsion. While this is useful for
	// debugging, it should probably be removed or be controlled by a
	// user flag.
	std::cerr << "Number of repulsions: " << counter << std::endl;
}

double
Sphere::distance(const Point3D& a, const Point3D& b) const
{
	return (a - b).norm();
}

double
Sphere::distanceSq(const Point3D& a, const Point3D& b) const
{
	return (a - b).normSq();
}

int
Sphere::evenNodes(Graph& G, int N)
{
	// Generate a graph on the sphere with even node distribution.
	int counter = uniform(G, N);
	repulse(G);
	return counter;
}

void
Sphere::delaunay(Graph& G)
{
	// To compute the Delaunay triangulation of nodes on the sphere, we
	// process each node in turn. We build an auxiliary graph H where
	// the node being processed lies at the origin and every node in a
	// neighbourhood of this node is projected on the tangent plane.
	// Then, we perform a Delaunay triangulation on this plane graph.
	
	const double expectedRadius = 2./sqrt(G.size());
	const double range = rangeMultiplier * expectedRadius;
	Basis B;
	for (int i = 0; i < G.size(); i++) {
		Graph H;
		int nH = 0;
		H.addNode();
		nH++;
		H[nH - 1].setID(i);
		H[nH - 1].setCoords(0, 0);
		Point3D& r = G[i].getCoords();
		
		// To find the tangent plane, create an orthonormal basis
		// containing the position of the node. The two other basis
		// vectors will generate the tangent plane. 
		B.genOrthonormal(r);
		for (int j = 0; j < G.size(); j++) {
			if (j == i)
				continue;
			Point3D& pj = G[j].getCoords();
			if (distanceSq(r, pj) < range*range) {
				H.addNode();
				nH++;
				H[nH - 1].setID(j);
				// Project neighbouring nodes on the tangent plane.
				H[nH - 1].setCoords(dot(pj, B[1]), dot(pj, B[2]));
			}
		}
		Plane P;
		P.delaunay(H);
		
		// Add edges in G according to which edge is present in H
		for (int j = 0; j < H.size(); j++) {
			int jID = H[j].getID();
			for (int k = 0; k < H[j].degree(); k++) {
				Node& neigh = H[j][k].getOtherEnd(H[j]);
				int neighID = neigh.getID();
				G.addEdge(G[jID], G[neighID]);
			}
		}
	}
}

double
Sphere::minDistance(const Graph& G) const
{
	double min = 2.;
	for (int i = 0; i < G.size(); i++) {
		for (int j = i + 1; j < G.size(); j++) {
			double temp = distance(G[i].getCoords(), G[j].getCoords());
			if (temp < min)
				min = temp;
		}
	}
	return min;
}

void
Sphere::setParams(unsigned int rMultiplier,
				  double dExp,
				  unsigned int dSub,
				  double oRatio)
{
	rangeMultiplier = rMultiplier;
	dampingExp = dExp;
	dampingSub = dSub;
	objectiveRatio = oRatio;
	return;
}


bool
compare_angles(Angle a, Angle b)
{
	if (a.angle <= b.angle)
		return true;
	return false;
}

void
Plane::delaunay(Graph& G)
{
	// We use a flip algorithm to find the Delaunay triangulation
	// of node (0,0).
	Point3D v(1, 0, 0);
	bool removed = true;
	G.resetStatus();
	
	std::list<Angle> allAngles;
	std::list<Node*> nodeList;
	
	for (int i = 1; i < G.size(); i++) {
		// This for loop fills allAngles with the list of angles between
		// the position vector of each Node and the x axis. It also
		// fills nodeList with all the nodes in Graph G.
		Point3D& iCoords = G[i].getCoords();
		double theta = acos(dot(iCoords, v)/iCoords.norm());
		if (iCoords[1] < 0)
			theta = 2*Pi - theta;
		Angle a = {theta, G[i]};
		allAngles.push_back(a);
		nodeList.push_back(&(G[i]));
	}
	// Sort angles from smallest to largest.
	allAngles.sort(compare_angles);
	
	// Move on to do the flipping.
	while (removed) {
		removed = false;
		std::list<Angle> anglesList;
		std::list<Angle>::iterator it;
		
		// angleList contains a sublist of allAngles corresponding to
		// Angles of the Nodes that have not been removed (i.e., whose
		// status is notVisited).
		for (it = allAngles.begin(); it != allAngles.end(); it++) {
			if (it->node.getStatus() == notVisited) {
				anglesList.push_back(*it);
			}
		}
		
		// Say two adjacent triangles A and B have a common edge e. Let
		// a1 be the angle in A opposite to e, and a2 the angle in B
		// opposite to e. If a1 + a2 > Pi, then e is not part of the
		// Delaunay triangulation and should be replaced by the edge going
		// from a1 to a2.
		for (it = anglesList.begin(); it != anglesList.end(); it++) {
			if (it->node.getStatus() == Visited)
				continue;
			double p1 = (it->node.getCoords()).norm();
			double a1, a2;
			a1 = a2 = Pi;
			std::list<Angle>::iterator jt = it;
			std::list<Angle>::iterator kt;
			
			while (a1 + a2 > Pi) {
				jt++;
				kt = jt;
				if (jt == anglesList.end()) {
					jt = anglesList.begin();
					kt = jt;
					kt++;
				}
				else if (++kt == anglesList.end()) {
					kt = anglesList.begin();
				}			
				double p2 = (jt->node.getCoords()).norm();
				double p3 = (kt->node.getCoords()).norm();
				double theta1 = jt->angle - it->angle;
				double theta2 = kt->angle - jt->angle;
				double L1 = sqrt(p1*p1 + p2*p2
								 - 2*p1*p2*cos(theta1));
				double L2 = sqrt(p2*p2 + p3*p3
								 - 2*p2*p3*cos(theta2));
				a1 = acos((-p2*p2 + p1*p1 + L1*L1)/(2*p1*L1));
				a2 = acos((-p2*p2 + p3*p3 + L2*L2)/(2*p3*L2));
				if (a1 + a2 > Pi) {
					// Then p2 is not a neighbour of the origin.
					nodeList.remove(&(jt->node));
					jt->node.setStatus(Visited);
					removed = true;
				}
			}
		}
	}
	
	// Add edges between the origin and all Nodes still in nodeList.
	std::list<Node*>::iterator ot;
	for (ot = nodeList.begin(); ot != nodeList.end(); ot++) {
		G.addEdge(G[0], **ot);
	}
}
