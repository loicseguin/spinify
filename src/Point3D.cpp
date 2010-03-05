/*
 *  Point3D.cpp
 *  spinify
 *
 *  Created by Loïc Séguin-Charbonneau on 10-01-14.
 *  Copyright 2010 Loïc Séguin-Charbonneau. All rights reserved.
 *
 */


#include "Point3D.h"


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


void
Point3D::normalize()
{
	double n = norm();
	x /= n;
	y /= n;
	z /= n;
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



Basis::Basis()
{
	// By the default, a basis initializes to the canonical basis.
	B[0] = Point3D(1,0,0);
	B[1] = Point3D(0,1,0);
	B[2] = Point3D(0,0,1);
}

void
Basis::genOrthonormal(const Point3D& v1)
{
	// From v1, we can generate an orthonormal basis by first generating
	// a vector that is orthogonal to v1, then normalizing this vector,
	// and finally generating a third vector which is the vector product
	// of the first two.
	
	B[0] = v1;
	
	// Here
	//   dot(B[0], B[1]) = v1[0] (2*v1[1]*v1[2])
	//					   + v1[1] (-v1[0]*v1[2])
	//					   + v1[2] (-v1[0]*v1[1])
	//					 = 2*v1[0]*v1[1]*v1[2]
	//					   - v1[0]*v1[1]*v1[2]
	//					   - v1[0]*v1[1]*v1[2]
	//					 = 0
	B[1] = Point3D(2*v1[1]*v1[2], -v1[0]*v1[2], -v1[0]*v1[1]);
	B[1].normalize();
	B[2] = vectorProd(B[0], B[1]);
}

Point3D&
Basis::operator[](int i)
{
	return B[i];
}
