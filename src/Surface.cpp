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
#include "Surface.h"
#include "tezuka.h"
#include "Point3D.h"

const int Sphere::uniform(Graph& G, int N) { // Marsaglia 1972
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
	double minDist = 0.;
	int counter = 0;
	
	while ((objectiveDist - minDist) / objectiveDist > 0.20) {
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
			force.divide(G.size() * 10);
			iCoords = force + iCoords;
			iCoords.normalize();
		}
		counter++;
		minDist = minDistance(G);
	}
	std::cout << "counter: " << counter << std::endl;
	return;
}

double Sphere::distance(Point3D& a, Point3D& b) {
	return (a - b).norm();
}

double Sphere::distanceSq(Point3D& a, Point3D& b) {
	return (a - b).normSq();
}

const int Sphere::randNodes(Graph& G, int N) {
	int counter = uniform(G, N);
	repulse(G);
	return counter;
}

double Sphere::minDistance(Graph& G) {
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
