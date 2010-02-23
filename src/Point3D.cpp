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


Point3D::Point3D(const double xx, const double yy, const double zz) :
	x(xx), y(yy), z(zz) {}

double&
Point3D::operator[](const int i)
{
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

const double&
Point3D::operator[](const int i) const
{
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

double
Point3D::norm()
{
	return sqrt(x*x + y*y + z*z);
}

double
Point3D::normSq()
{
	return x*x + y*y + z*z;
}

void
Point3D::normalize()
{
	double n = norm();
	x /= n;
	y /= n;
	z /= n;
}

double
Point3D::sum()
{
	return x + y + z;
}

Point3D
Point3D::divide(const double q)
{
	x /= q;
	y /= q;
	z /= q;
	return *this;
}

Point3D
Point3D::operator+(const Point3D& pt) const
{
	return Point3D(x + pt.x, y + pt.y, z + pt.z);
}

Point3D
Point3D::operator-(const Point3D& pt) const
{
	return Point3D(x - pt.x, y - pt.y, z - pt.z);
}

Point3D
Point3D::operator*(const Point3D& pt) const
{
	return Point3D(x * pt.x, y * pt.y, z * pt.z);
}

Point3D&
Point3D::operator=(const Point3D& pt)
{
	x = pt.x;
	y = pt.y;
	z = pt.z;
	return *this;
}


double
dot(Point3D& v1, Point3D& v2)
{
	return (v1*v2).sum();
}

Point3D
vectorProd(Point3D& v1, Point3D& v2)
{
	return Point3D(v1[1]*v2[2] - v1[2]*v2[1],
				   v1[2]*v2[0] - v1[0]*v2[2],
				   v1[0]*v2[1] - v1[1]*v2[0]);
}


Basis::Basis()
{
	B[0] = Point3D(1,0,0);
	B[1] = Point3D(0,1,0);
	B[2] = Point3D(0,0,1);
}

void
Basis::genOrthonormal(const Point3D& v1)
{
	B[0] = v1;
	B[1] = Point3D(2*v1[1]*v1[2], -v1[0]*v1[2], -v1[0]*v1[1]);
	B[1].normalize();
	B[2] = vectorProd(B[0], B[1]);
}

Point3D&
Basis::operator[](int i)
{
	return B[i];
}
