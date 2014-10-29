
#ifndef __VECTOR_H__
#define __VECTOR_H__


class CVector3
{
public:
	// 初始化
	CVector3(void) : x(0.0f), y(0.0f), z(0.0f) {}
	CVector3(float newX, float newY, float newZ) : x(newX), y(newY), z(newZ) {}
	CVector3(const float * rhs)	: x(*rhs), y(*(rhs+1)), z(*(rhs+2)) {}
	CVector3(const CVector3 & rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}
	
	// 类的析构函数
	~CVector3() {}
	
	// 设置向量的三个属性
	void Set(float newX, float newY, float newZ);
	
	// 计算向量的叉乘
	CVector3 CrossProduct(const CVector3 &rhs) const;
	
	// 计算向量的点乘
	float DotProduct(const CVector3 &rhs) const;
	
	// 向量单位化
	void Normalize();
	CVector3 GetNormalized() const;
	
	// 计算向量的长度
	float GetLength() const;
	
	// 计算向量的长度的平方
	float GetSquaredLength() const;
	
	// 向量的旋转
	void RotateX(double angle);
	CVector3 GetRotatedX(double angle) const;
	void RotateY(double angle);
	CVector3 GetRotatedY(double angle) const;
	void RotateZ(double angle);
	CVector3 GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const CVector3 &axis);
	CVector3 GetRotatedAxis(double angle, const CVector3 &axis) const;
	
	// 运算符重载
	CVector3 operator+(const CVector3 &rhs) const;
	CVector3 operator-(const CVector3 &rhs) const;
	CVector3 operator*(const float rhs) const;
	CVector3 operator/(const float rhs) const;
	
	bool operator==(const CVector3 &rhs) const;
	bool operator!=(const CVector3 &rhs) const;
	
	// 类成员
public:
	float x;
	float y;
	float z;
};
// void    SetMag(float fMag);
// float   GetMag();
// 	void    Normalize();

#endif //__VECTOR_H__

