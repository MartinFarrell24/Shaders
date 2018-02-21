#include "Matrix3.h"

Matrix3::Matrix3()
{
}

Matrix3::Matrix3(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33) :
	A11{ a11 },
	A12{ a12 },
	A13{ a13 },
	A21{ a21 },
	A22{ a22 },
	A23{ a23 },
	A31{ a31 },
	A32{ a32 },
	A33{ a33 }
{
}


Matrix3::~Matrix3()
{
}

Vector3D Matrix3::operator *(const Vector3D vector) const
{// An overloaded operator * to return the  product of the matrix by a vector
	return Vector3D(
		A11 * vector.x + A12 * vector.y + A13 * vector.z,
		A21 * vector.x + A22 * vector.y + A23 * vector.z,
		A31 * vector.x + A32 * vector.y + A33 * vector.z
	);
}

Matrix3 Matrix3::operator +(const Matrix3 other) const
{// An overloaded operator + to return the  sum of two matrix 
	return Matrix3(
		A11 + other.A11,
		A12 + other.A12,
		A13 + other.A13,
		A21 + other.A21,
		A22 + other.A22,
		A23 + other.A23,
		A31 + other.A31,
		A32 + other.A32,
		A33 + other.A33)
		;
}
Matrix3 Matrix3::operator -(const Matrix3 other) const
{// An overloaded operator * to return the  difference of two matrix
	return Matrix3(
		A11 - other.A11,
		A12 - other.A12,
		A13 - other.A13,
		A21 - other.A21,
		A22 - other.A22,
		A23 - other.A23,
		A31 - other.A31,
		A32 - other.A32,
		A33 - other.A33
	);
}

Matrix3 Matrix3::operator *(const double scale) const
{// An overloaded operator * to return the  product of the matrix by a scalar
	Matrix3 answer = Matrix3
	(
		A11 * scale,
		A12 * scale,
		A13 * scale,
		A21 * scale,
		A22 * scale,
		A23 * scale,
		A31 * scale,
		A32 * scale,
		A33 * scale
	);

	return answer;
}

Matrix3 Matrix3::operator *(const Matrix3 other) const
{// An overloaded operator * to return the  product of two matrix
	Matrix3 answer = Matrix3(
		A11 * other.A11 + A12 * other.A21 + A13 * other.A31,
		A11 * other.A12 + A12 * other.A22 + A13 * other.A32,
		A11 * other.A13 + A12 * other.A23 + A13 * other.A33,

		A21 * other.A11 + A12 * other.A21 + A13 * other.A31,
		A21 * other.A12 + A12 * other.A22 + A13 * other.A32,
		A21 * other.A13 + A12 * other.A23 + A13 * other.A33,

		A31 * other.A11 + A32 * other.A21 + A33 * other.A31,
		A31 * other.A12 + A32 * other.A22 + A33 * other.A32,
		A31 * other.A13 + A32 * other.A23 + A33 * other.A33
	);

	return answer;
}

Matrix3 Matrix3::Transpose(Matrix3 M1)
{// a method to transpose a given matrix
	return Matrix3(M1.A11, M1.A21, M1.A31,
		M1.A12, M1.A22, M1.A32,
		M1.A13, M1.A23, M1.A33);
}

double Matrix3::Determinant(Matrix3 M1)
{// method to return the determinant of a 3x3 matrix
 //                     aei      -     ahf                  + dhc                     - gec                      +    gbh                    -     dbi   
	return M1.A11 * M1.A22 * M1.A33 - M1.A11 * M1.A32 * M1.A23 + M1.A21 * M1.A32 * M1.A13 - M1.A31 * M1.A22 * M1.A13 + M1.A31 * M1.A12 * M1.A23 - M1.A21 * M1.A12 * M1.A33;
}

Vector3D Matrix3::Row(int i)
{
	// a method to return as Row as vector3 0 == first row, default == last row
	switch (i)
	{
	case 0:
		return Vector3D(A11, A12, A13);
	case 1:
		return Vector3D(A21, A22, A23);
	case 2:
	default:
		return Vector3D(A31, A32, A33);
	}
}

Vector3D Matrix3::Column(int i)
{// a method to return as column as vector3 0 == first column, default == last column
	switch (i)
	{
	case 0:
		return Vector3D(A11, A21, A31);
	case 1:
		return Vector3D(A12, A22, A32);
	case 2:
	default:
		return Vector3D(A13, A23, A33);
	}
}

Matrix3 Matrix3::Inverse(Matrix3 M1)
{
	// method to return the inverse of a matrix if exists else zero vector
	double det = Determinant(M1);
	if (det == 0)
		return Matrix3(M1);
	else
	{
		double scale = 1 / det;
		Matrix3 answer = Matrix3(
			scale * (M1.A22 * M1.A33 - M1.A23 * M1.A32), // ei-fh
			scale * (M1.A13 * M1.A32 - M1.A12 * M1.A33), // ch-bi
			scale * (M1.A12 * M1.A23 - M1.A13 * M1.A22), // ch-bi

			scale * (M1.A23 * M1.A31 - M1.A21 * M1.A33), // fg-di
			scale * (M1.A11 * M1.A33 - M1.A13 * M1.A31), // ai-cg
			scale * (M1.A13 * M1.A21 - M1.A11 * M1.A23), // cd-af

			scale * (M1.A21 * M1.A32 - M1.A22 * M1.A31), // dh-eg
			scale * (M1.A12 * M1.A31 - M1.A11 * M1.A32), // bg-ah
			scale * (M1.A11 * M1.A22 - M1.A12 * M1.A21) // ae-bd
		);
		return answer;
	}

}

Matrix3 Matrix3::Rotation(int _angle)
{
	double radians = (22.0 / 7.0) / 180 * _angle;
	Matrix3 answer = Matrix3(
		cos(radians),
		sin(radians),
		0,
		-sin(radians),
		cos(radians),
		0,
		0,
		0,
		1
	);
	return answer;
}

Matrix3 Matrix3::Translate(int dx, int dy)
{
	Matrix3 answer = Matrix3(
		1,
		0,
		0,
		0,
		1,
		0,
		dx,
		dy,
		1
	);
	return answer;
}

Matrix3 Matrix3::Scale(int dx, int dy)
{
	Matrix3 answer = Matrix3(
		(double)dx / 100,
		0,
		0,
		0,
		(double)dy / 100,
		0,
		0,
		0,
		1
	);
	return answer;
}

Matrix3 Matrix3::operator-() const
{
	return Matrix3(
		-A11,
		-A12,
		-13,
		-A21,
		-A22,
		-A23,
		-A31,
		-A32,
		-A33
	);
}

Matrix3 Matrix3::RotationX(int _angle)
{
	double radians = (22.0 / 7.0) / 180 * _angle;
	Matrix3 answer = Matrix3(
		1,
		0,
		0,
		0,
		cos(radians),
		-sin(radians),
		0,
		sin(radians),
		cos(radians)
	);
	return answer;
}
Matrix3 Matrix3::RotationY(int _angle)
{
	double radians = (22.0 / 7.0) / 180 * _angle;
	Matrix3 answer = Matrix3(
		cos(radians),
		0,
		sin(radians),
		0,
		1,
		0,
		-sin(radians),
		0,
		cos(radians)
	);
	return answer;
}
Matrix3 Matrix3::RotationZ(int _angle)
{
	double radians = (22.0 / 7.0) / 180 * _angle;
	Matrix3 answer = Matrix3(
		cos(radians),
		-sin(radians),
		0,
		sin(radians),
		cos(radians),
		0,
		0,
		0,
		1
	);
	return answer;
}

Matrix3 Matrix3::Scale3D(int dx)
{
	Matrix3 answer = Matrix3(
		(double)dx / 100,
		0,
		0,
		0,
		(double)dx / 100,
		0,
		0,
		0,
		(double)dx / 100
	);
	return answer;
}