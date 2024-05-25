#include "vecmat.h"

Vector4::Vector4()
{
 data[0]=0.0; data[1]=0.0; data[2]=0.0; data[3]=1.0;
}

void Vector4::Set(double x,double y,double z)
{
 data[0]=x; data[1]=y; data[2]=z;
}

double Vector4::GetX()
{
	return data[0];
}

double Vector4::GetY()
{
	return data[1];
}

double Vector4::GetZ()
{
	return data[2];
}

Vector4 Vector4::operator- (const Vector4 &gVector)
{
 unsigned int i;
 Vector4 Result;
 for (i=0;i<4;i++) Result.data[i]=data[i]-gVector.data[i];
 return Result;
}

Vector4 operator* (const Vector4 &gVector,double val)
{
 unsigned int i;
 Vector4 Result;
 for (i=0;i<4;i++) Result.data[i]=gVector.data[i]*val;
 return Result;
}

Matrix4::Matrix4()
{
 data[0][0]=0.0; data[0][1]=0.0; data[0][2]=0.0; data[0][3]=0.0;
 data[1][0]=0.0; data[1][1]=0.0; data[1][2]=0.0; data[1][3]=0.0;
 data[2][0]=0.0; data[2][1]=0.0; data[2][2]=0.0; data[2][3]=0.0;
 data[3][0]=0.0; data[3][1]=0.0; data[3][2]=0.0; data[3][3]=1.0;
}

Matrix4 Matrix4::operator* (const Matrix4 gMatrix)
{
 int i,j,k;
 Matrix4 tmp;

 for (i=0;i<4;i++)
    for (j=0;j<4;j++)
     {
      tmp.data[i][j]=0.0;
      for (k=0;k<4;k++)
       tmp.data[i][j]=tmp.data[i][j]+(data[i][k]*gMatrix.data[k][j]);
     }
 return tmp;
}

Vector4 operator* (const Matrix4 gMatrix,const Vector4 gVector)
{
 unsigned int i,j;
 Vector4 tmp;

 for (i=0;i<4;i++)
  {
   tmp.data[i]=0.0;
   for (j=0;j<4;j++) tmp.data[i]=tmp.data[i]+(gMatrix.data[i][j]*gVector.data[j]);
  }
 return tmp;
}

void Matrix4::SetScale(double scaleX, double scaleY, double scaleZ) {
    data[0][0] = scaleX;
    data[1][1] = scaleY;
    data[2][2] = scaleZ;
}

void Matrix4::SetRotationX(double alpha) {
    data[0][0] = 1.0;
    data[0][1] = 0.0;
    data[0][2] = 0.0;
    data[0][3] = 0.0;

    data[1][0] = 0.0;
    data[1][1] = cos(alpha);
    data[1][2] = sin(alpha);
    data[1][3] = 0.0;

    data[2][0] = 0.0;
    data[2][1] = -sin(alpha);
    data[2][2] = cos(alpha);
    data[2][3] = 0.0;

    data[3][0] = 0.0;
    data[3][1] = 0.0;
    data[3][2] = 0.0;
    data[3][3] = 1.0;
}

void Matrix4::SetRotationY(double alpha) {
    data[0][0] = cos(alpha);
    data[0][1] = 0.0;
    data[0][2] = -sin(alpha);
    data[0][3] = 0.0;

    data[1][0] = 0.0;
    data[1][1] = 1.0;
    data[1][2] = 0.0;
    data[1][3] = 0.0;

    data[2][0] = sin(alpha);
    data[2][1] = 0.0;
    data[2][2] = cos(alpha);
    data[2][3] = 0.0;

    data[3][0] = 0.0;
    data[3][1] = 0.0;
    data[3][2] = 0.0;
    data[3][3] = 1.0;
}

void Matrix4::SetRotationZ(double alpha) {
    data[0][0] = cos(alpha);
    data[0][1] = sin(alpha);
    data[0][2] = 0.0;
    data[0][3] = 0.0;
    
    data[1][0] = -sin(alpha);
    data[1][1] = cos(alpha);
    data[1][2] = 0.0;
    data[1][3] = 0.0;
    
    data[2][0] = 0.0;
    data[2][1] = 0.0;
    data[2][2] = 1.0;
    data[2][3] = 0.0;
    
    data[3][0] = 0.0;
    data[3][1] = 0.0;
    data[3][2] = 0.0;
    data[3][3] = 1.0;

}

void Matrix4::SetTranslation(double tx, double ty, double tz) {
    data[0][0] = 1.0;
    data[0][1] = 0.0;
    data[0][2] = 0.0;
    data[0][3] = tx;

    data[1][0] = 0.0;
    data[1][1] = 1.0;
    data[1][2] = 0.0;
    data[1][3] = ty;

    data[2][0] = 0.0;
    data[2][1] = 0.0;
    data[2][2] = 1.0;
    data[2][3] = tz;

    data[3][0] = 0.0;
    data[3][1] = 0.0;
    data[3][2] = 0.0;
    data[3][3] = 1.0;

}

void Matrix4::SetIdentity() {
    data[0][0] = 1.0;
    data[1][1] = 1.0;
    data[3][2] = 1.0 / 2.0;
}

void Matrix4::SetWindowCoordinates(double minX, double minY, double maxX, double maxY) {
    data[0][0] = minX;
    data[1][1] = minY;
    data[0][3] = maxX;
    data[1][3] = maxY;

}

void Vector4::Homogenize() {
    if (data[3] != 0.0) {
        for (int i = 0; i < 3; ++i) {
            data[i] /= data[3];
        }
        data[3] = 1.0;
    }
}

void Vector4::Interpolate(const Vector4& v, double tmp, double clip) {
    data[0] = (v.data[0] - data[0]) * tmp + data[0];
    data[1] = (v.data[1] - data[1]) * tmp + data[1];
    data[2] = clip;
}
