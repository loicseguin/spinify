/*
 *  point3dtest.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-15.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "../src/Point3D.h"

using namespace std;

void prPoint(Point3D pt) {
	cout << "(" << pt[0] << ", " << pt[1] << ", " << pt[2] << ")\n";
}

int main() {
	Point3D pt1(1.,2.,3.);
	Point3D pt2(-1.,0.,2.);
	cout << "Point 1: ";
	prPoint(pt1);
	cout << "Point 2: ";
	prPoint(pt2);
	cout << "Point 1 + Point 2: ";
	prPoint(pt1+pt2);
	cout << "Point 1 - Point 2: ";
	prPoint(pt1-pt2);
	cout << "Point 1 * Point 2: ";
	prPoint(pt1*pt2);
	cout << "Point 1 norm: " << pt1.norm() << endl;
	cout << "Point 1 normalized: ";
	pt1.normalize();
	prPoint(pt1);
	cout << "Point 1 norm: " << pt1.norm() << endl;
	Point3D pt3 = pt2+pt2;
	cout << "Point 3: ";
	prPoint(pt3);
	cout << "Point 2 * Point 3 sum: " << (pt2 * pt3).sum() << endl;
	
	cout << "Distance between pt1 and pt2: " << (pt1 - pt2).norm() << endl;
	
	Basis B;
	Point3D a(1/sqrt(2),0,1/sqrt(2));
	B.GramSchmidt(a);
	cout << "Basis: " << endl;
	prPoint(B[0]);
	prPoint(B[1]);
	prPoint(B[2]);
}
