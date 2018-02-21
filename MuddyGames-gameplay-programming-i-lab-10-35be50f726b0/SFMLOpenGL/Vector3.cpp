#include "Vector3.h"
#include <math.h>

Vector3D::Vector3D()
{

}

Vector3D::Vector3D(double x, double y, double z) :
	x(x),
	y(y),
	z(z)
{

}


Vector3D::~Vector3D()
{
}

double Vector3D::X()
{
	return  x;
}

double Vector3D::Y()
{
	return y;
}
double Vector3D::Z()
{
	return z;
}


Vector3D Vector3D::operator +(const Vector3D other) const
{  // An overloaded operator + to return the sum of 2 vectors
	return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator -(const Vector3D other) const
{ // An overloaded operator - to return the difference of 2 vectors
	return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator-() const
{// An overloaded operator - to return the negation of a single vector
	Vector3D V1 = Vector3D(-x, -y, -z);
	return V1;
}

double Vector3D::operator *(const Vector3D other) const
{// An overloaded operator * to return the scalar product of 2 vectors
	return (x * other.x + y * other.y + z * other.z);
}

Vector3D Vector3D::operator *(double k) const
{// An overloaded operator * to return the product of a scalar by a vector
	return Vector3D(x * k, y * k, z * k);
}

Vector3D Vector3D::operator *(float k) const
{// An overloaded operator * to return the product of a scalar by a vector
	return Vector3D(x * k, y * k, z * k);
}

Vector3D Vector3D::operator *(int k) const
{// An overloaded operator * to return the product of a scalar by a vector
	return Vector3D(x * k, y * k, z * k);
}

Vector3D Vector3D::operator ^(const Vector3D other) const
{// An overloaded operator ^ to return the vector product of 2 vectors
	return Vector3D(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

double Vector3D::Length()
{
	return double(sqrt(x * x + y * y + z * z));
}
double Vector3D::LengthSquared()
{
	return double(x * x + y * y + z * z);
}

void Vector3D::Normalise()
{
	double length = Length();
	if (length > 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}
}