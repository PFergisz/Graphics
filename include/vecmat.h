//Extremely simple vector and matrix classes by Janusz Malinowski. 
#include <stdio.h>
#include <cmath>
#pragma once
class Vector4
{
 public:
 double data[4];
 Vector4();
 void Set(double x,double y,double z);
 double GetX();
 double GetY();
 double GetZ();
 Vector4 operator-(const Vector4 &);
 friend Vector4 operator*(const Vector4 &,double);

 void Homogenize();
 void Interpolate(const Vector4&, double, double);
};

class Matrix4
{
 public:
 double data[4][4];
 Matrix4();
 Matrix4 operator*(const Matrix4);
 friend Vector4 operator*(const Matrix4,const Vector4);

 void SetScale(double, double, double);
 void SetRotationX(double);
 void SetRotationY(double);
 void SetRotationZ(double);
 void SetTranslation(double, double, double);
 void SetIdentity();
 void SetWindowCoordinates(double, double, double, double);
};


