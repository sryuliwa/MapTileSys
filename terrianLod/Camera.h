#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vector3.h"

//第一人称摄像机

class CCamera {
public:
	CCamera();
	// 用来返回Camera信息
	CVector3 Position()	{ return m_vPosition;	}
	CVector3 View()		{ return m_vView;		}
	CVector3 UpVector()	{ return m_vUpVector;	}
	CVector3 Strafe()	{ return m_vStrafe;		}
	float    Speed()    { return m_fSpeed;      }

	void SetPosition(const CVector3& pos)	{   m_vPosition = pos;	}
	void SetView(const CVector3& view)		{   m_vView = view;		}
	void SetUpVector(const CVector3& up)	{   m_vUpVector = up;	}
	void SetSpeed(float speed)	            {   m_fSpeed = speed;	}
	//在不改变视线方向的情况下，改变相机的位置信息
	void SetPosX(float x);
	void SetPosY(float y);//就是SetHeight
	void SetPosZ(float z);
	void SetHeight(float h);

	// 初始化Camera属性
	void PositionCamera(float posX,  float posY,  float posZ,
			 		    float viewX, float viewY, float viewZ,
						float upX,   float upY,   float upZ);
	void PositionCamera(const CVector3 &pos, const CVector3 &view, const CVector3 &up);
	
	// 使用gluLookAt()在场景中摆放Camera
	void Look();

private:
	// 通过移动鼠标移动Camera的方向(第一人称)
	void SetViewByMouse(void); 

	// 绕给出的轴旋转Camera的方向
	void RotateView(float angle, const CVector3 &vAxis);

	// 左右移动Camera(速度:speed)
	void StrafeCamera(float speed);

	// 前后移动Camera(速度:speed)
	void MoveCamera(float speed);
	
	// 确认键盘控制的移动
	void CheckForMovement();
	
	// 更新Camere的方向和其他信息
	void Update();
	
	CVector3 m_vPosition;	// Camera 的位置
	CVector3 m_vView;		// Camera 的视点
	CVector3 m_vUpVector;	// Camera 向上的向量
	CVector3 m_vStrafe;		// Camera 水平的向量
	float    m_fSpeed;      // Camera 速度
};

#endif //__CAMERA_H__
