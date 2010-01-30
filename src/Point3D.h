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
	/*
	 * A Point3D is just a three dimensional vector. Coordinates are
	 * accessed by the subscript operator.
	 *
	 * Basic methods are implemented such as norm(), which returns the
	 * norm of the vector and normSq() which returns the square of the norm
	 * (this function exists because the sqrt() operation in norm() is
	 * time consumming, and there are many occasions where we are
	 * actually looking for the square of the norm). sum() returns the
	 * sum of the elements in the vector.
	 *
	 * Addition and subtraction work as usual, i.e., adds and subtract
	 * component by component. Multiplication also works component by
	 * component. To multiply or divide the vector by a scalar, divide()
	 * is the function to use. Of course, it divides by the amount given
	 * as an argument. Thus, to multiply a vector v by 3, you should
	 * call:
	 *     v.divide(1./3.);
	 *
	 * normalize() just normalizes the vector.
	 *
	 */
	double x;
	double y;
	double z;
public:
	//Point3D();
	Point3D(const double xx = 0, const double yy = 0, const double zz = 0);
	double& operator[](const int i);
	const double& operator[](const int i) const;
	double norm();
	double normSq();
	void normalize();
	double sum();
	Point3D divide(const double q);
	Point3D operator+(const Point3D& pt) const;
	Point3D operator-(const Point3D& pt) const;
	Point3D operator*(const Point3D& pt) const;
	Point3D& operator=(const Point3D& pt);
};

// The following two function take the dot product and the vector
// product of two Point3D, respectively.
double dot(Point3D& v1, Point3D& v2);
Point3D vectorProd(Point3D& v1, Point3D& v2);


class Basis {
	/*
	 * A basis is a set of three linearly independent vectors of R^3.
	 * Given a unit vector, genOrthonormal() uses simple linear algebra
	 * to find an orthonormal basis containing that vector.
	 *
	 */
	Point3D B[3];
public:
	Basis();
	void genOrthonormal(const Point3D& v1);
	Point3D& operator[](int i);
};



#endif
