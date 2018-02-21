#pragma once
class Vector3D
{
public:
	Vector3D();
	Vector3D(double x, double y, double z);
	~Vector3D();

	double x;
	double y;
	double z;

	double X();
	double Y();
	double Z();


	Vector3D operator +(const Vector3D other) const;

	Vector3D operator -(const Vector3D other) const;

	Vector3D operator-() const;

	double operator *(const Vector3D other) const;

	Vector3D operator *(double k) const;

	Vector3D operator *(float k) const;

	Vector3D operator *(int k) const;

	Vector3D operator ^(const Vector3D other) const;

	double Length();
	double LengthSquared();

	void Normalise();
};