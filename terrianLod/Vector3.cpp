#include <cmath>
#include "Vector3.h"                       

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// 设置向量的三个属性
void CVector3::Set(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

// 计算向量的叉乘
CVector3 CVector3::CrossProduct(const CVector3 &rhs) const
{
	return CVector3(y*rhs.z-z*rhs.y, z*rhs.x-x*rhs.z, x*rhs.y-y*rhs.x);
}

// 计算向量的点乘
float CVector3::DotProduct(const CVector3 &rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

// 向量单位化
void CVector3::Normalize()
{
	float length=GetLength();

	if(length==1 || length==0)
		return;

	float scalefactor = 1.0f/length;
	x *= scalefactor;
	y *= scalefactor;
	z *= scalefactor;
}

// 向量单位化
CVector3 CVector3::GetNormalized() const
{
	CVector3 result(*this);

	result.Normalize();

	return result;
}

// 计算向量的长度
float CVector3::GetLength() const
{
	return (float)sqrt((x*x)+(y*y)+(z*z));
}

// 计算向量的长度的平方
float CVector3::GetSquaredLength() const
{
	return (x*x)+(y*y)+(z*z);
}

// 绕X轴旋转向量,返回旋转后的向量
CVector3 CVector3::GetRotatedX(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);

	return CVector3(x, y*cosAngle - z*sinAngle, y*sinAngle + z*cosAngle);
}

// 绕X轴旋转向量
void CVector3::RotateX(double angle)
{
	(*this)=GetRotatedX(angle);
}

// 绕Y轴旋转向量,返回旋转后的向量
CVector3 CVector3::GetRotatedY(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);

	return CVector3(x*cosAngle + z*sinAngle, y, -x*sinAngle + z*cosAngle);
}

// 绕Y轴旋转向量
void CVector3::RotateY(double angle)
{
	(*this)=GetRotatedY(angle);
}

// 绕Y轴旋转向量,返回旋转后的向量
CVector3 CVector3::GetRotatedZ(double angle) const
{
	if(angle==0.0)
		return (*this);

	float sinAngle=(float)sin(M_PI*angle/180);
	float cosAngle=(float)cos(M_PI*angle/180);
	
	return CVector3(x*cosAngle - y*sinAngle,
					x*sinAngle + y*cosAngle,
					z);
}

// 绕Y轴旋转向量
void CVector3::RotateZ(double angle)
{
	(*this)=GetRotatedZ(angle);
}

// 绕给定轴旋转向量,返回旋转后的向量
CVector3 CVector3::GetRotatedAxis(double angle, const CVector3 & axis) const
{
	if(angle==0.0)
		return (*this);

	CVector3 u=axis.GetNormalized();

	CVector3 result;

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	result.x  = (float)(cosTheta + (1 - cosTheta) * u.x * u.x) * x;
	result.x += (float)((1 - cosTheta) * u.x * u.y - u.z * sinTheta) * y;
	result.x += (float)((1 - cosTheta) * u.x * u.z + u.y * sinTheta) * z;

	result.y  = (float)((1 - cosTheta) * u.x * u.y + u.z * sinTheta) * x;
	result.y += (float)(cosTheta + (1 - cosTheta) * u.y * u.y) * y;
	result.y += (float)((1 - cosTheta) * u.y * u.z - u.x * sinTheta) * z;

	result.z  = (float)((1 - cosTheta) * u.x * u.z - u.y * sinTheta) * x;
	result.z += (float)((1 - cosTheta) * u.y * u.z + u.x * sinTheta) * y;
	result.z += (float)(cosTheta + (1 - cosTheta) * u.z * u.z) * z;

	return result;
}

// 绕给定轴旋转向
void CVector3::RotateAxis(double angle, const CVector3 & axis)
{
	(*this)=GetRotatedAxis(angle, axis);
}

// 运算符重载 +
CVector3 CVector3::operator+(const CVector3 &rhs) const
{
	return CVector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

// 运算符重载 -
CVector3 CVector3::operator-(const CVector3 &rhs) const
{
	return CVector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

// 运算符重载 *
CVector3 CVector3::operator*(const float rhs) const
{
	return CVector3(x*rhs, y*rhs, z*rhs);
}

// 运算符重载 /
CVector3 CVector3::operator/(const float rhs) const
{
	return (rhs==0.0f) ? CVector3(0.0f, 0.0f, 0.0f) : CVector3(x/rhs, y/rhs, z/rhs);
}

// 判断两个向量是否相同
bool CVector3::operator==(const CVector3 & rhs) const
{
	if(x==rhs.x && y==rhs.y && z==rhs.z)
		return true;

	return false;
}

// 判断两个向量是否不同
bool CVector3::operator!=(const CVector3 & rhs) const
{
	return !((*this)==rhs);
}
