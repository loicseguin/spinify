/*
 *  Surface.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-08.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>
#include "Surface.h"
#include "tezuka.h"
#include "Point3D.h"

int Sphere::uniform(Graph& G, int N) { // Marsaglia 1972
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

void Sphere::repulse(Graph& G) {
	const double expectedRadius = 2./sqrt(G.size());
	const double range = 10 * expectedRadius;
	const double objectiveDist = 4./sqrt(G.size());
	
	// The following damping coefficient is a crude guess based on
	// a couple of experiments. More thoughts should be put into this.
	const double dampingCoeff =
		std::max(pow(G.size(), 1.2915), pow(G.size(), 1.2915) - 10000);
	
	double minDist = 0.;
	int counter = 0;
	
	while ((objectiveDist - minDist) / objectiveDist > 0.21) {
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
	std::cerr << "Number of repulsions: " << counter << std::endl;
	return;
}

double Sphere::distance(const Point3D& a, const Point3D& b) const {
	return (a - b).norm();
}

double Sphere::distanceSq(const Point3D& a, const Point3D& b) const {
	return (a - b).normSq();
}

int Sphere::randNodes(Graph& G, int N) {
	int counter = uniform(G, N);
	repulse(G);
	return counter;
}

void Sphere::delaunay(Graph& G) {
	const double expectedRadius = 2./sqrt(G.size());
	const double range = 10 * expectedRadius;
	Basis B;
	for (int i = 0; i < G.size(); i++) {
		Graph H;
		int nH = 0;
		H.addNode();
		nH++;
		H[nH - 1].setID(i);
		H[nH - 1].setCoords(0, 0);
		Point3D& r = G[i].getCoords();
		B.genOrthonormal(r);
		for (int j = 0; j < G.size(); j++) {
			if (j == i)
				continue;
			Point3D& pj = G[j].getCoords();
			if (distanceSq(r, pj) < range*range) {
				H.addNode();
				nH++;
				H[nH - 1].setID(j);
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

double Sphere::minDistance(const Graph& G) const {
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

void Plane::delaunay(Graph& G) {
	// Start by writing node coordinates in qhull_data.tmp
	std::ofstream outFile("qhull_data.tmp");
	outFile << "2\n" << G.size() << std::endl;
	for (int i = 0; i < G.size(); i++) {
		Point3D& iCoords = G[i].getCoords();
		outFile.precision(14);
		outFile << iCoords[0] << " " << iCoords[1] << std::endl;
	}
	outFile.close();
	
	// Call qdelaunay and tell it to write output to qhull.out
	system("qdelaunay Qt FN i TO qhull.out < qhull_data.tmp");
	
	// Read and process info from qhull.out
	int n;
	int facet;
	std::list<int> facets;
	std::ifstream inFile("qhull.out");
	inFile >> n; // n contains number of nodes
	inFile >> n; // n contains number of facets in which node 0 is
	for (int i = 0; i < n; i++) {
		inFile >> facet; // num id of facet
		facets.push_back(facet);
	}
	facets.sort(); //facets ordered from lowest to highest
	
	// This does nothing except move the file pointer to the description
	// of facets.
	for (int i = 0; i < G.size() - 1; i++) {
		inFile >> n;
		for (int j = 0; j < n; j++) {
			inFile >> facet;
		}
	}
	
	inFile >> n; // n now contains number of facets
		
	// Iterate through the facets and add edges accordingly.
	std::list<int>::iterator it;
	int counter = 0;
	for (it = facets.begin(); it != facets.end(); it++) {
		int lineNb = *it;
		int diff = lineNb - counter;
		if (diff != 0) {
			for (int i = 0; i < diff*3; i++) {
				inFile >> n;
			}
		}
		for (int j = 0; j < 3; j++) {
			int candidate;
			inFile >> candidate;
			if (candidate != 0)
				G.addEdge(G[0], G[candidate]);
		}
		counter = lineNb + 1;
	}
	inFile.close();
	
	// Remove temporary files.
	system("rm qhull_data.tmp qhull.out");
	
	return;
}
