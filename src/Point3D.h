/*
 *  Point3D.h
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-14.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */

#ifndef POINT3D_H
#define POINT3D_H

class Point3D {
	double x;
	double y;
	double z;
public:
	//Point3D();
	Point3D(double xx = 0, double yy = 0, double zz = 0);
	double& operator[](int i);
	double norm();
	double normSq();
	void normalize();
	double sum();
	Point3D divide(double q);
	Point3D operator+(Point3D pt);
	Point3D operator-(Point3D pt);
	Point3D operator*(Point3D pt);
	Point3D operator=(Point3D pt);
};

#endif
