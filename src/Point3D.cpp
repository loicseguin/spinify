/*
 *  Point3D.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-14.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#include <cmath>
#include "Point3D.h"

//Point3D::Point3D() {
//	x = y = z = 0.;
//}

Point3D::Point3D(double xx, double yy, double zz) {
	x = xx;
	y = yy;
	z = zz;
}

double& Point3D::operator[](int i) {
	switch (i) {
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			return x;
			break;
	}
}

double Point3D::norm() { return sqrt(x*x + y*y + z*z); }

void Point3D::normalize() {
	double n = norm();
	x /= n;
	y /= n;
	z /= n;
}

double Point3D::sum() { return x + y + z; }

Point3D Point3D::divide(double q) {
	x /= q;
	y /= q;
	z /= q;
	return *this;
}

Point3D Point3D::operator+(Point3D pt) {
	return Point3D(x + pt[0], y + pt[1], z + pt[2]);
}

Point3D Point3D::operator-(Point3D pt) {
	return Point3D(x - pt[0], y - pt[1], z - pt[2]);
}

Point3D Point3D::operator*(Point3D pt) {
	return Point3D(x * pt[0], y * pt[1], z * pt[2]);
}

Point3D Point3D::operator=(Point3D pt) {
	x = pt[0];
	y = pt[1];
	z = pt[2];
	return *this;
}
