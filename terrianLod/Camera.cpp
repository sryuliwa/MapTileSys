
#include "camera.h"
#include <windows.h>
#include <gl\glu.h>
#include <math.h>

// 默认初始化Camera属性
CCamera::CCamera()
{
	// 初始化Camera为OpenGL的默认方向

	m_vPosition = CVector3(0.0, 0.0, 0.0);  
	m_vView     = CVector3(0.0, 0.0,-1.0);
	m_vUpVector = CVector3(0.0, 1.0, 0.0);
    m_fSpeed    = 2.0f;

 	ShowCursor(FALSE);
}

// 设置Camera位置方向
void CCamera::PositionCamera( float posX,  float posY,  float posZ,
							  float viewX, float viewY, float viewZ,
							  float upX,   float upY,   float upZ   )
{
	// 初始化Camera
	m_vPosition	= CVector3(posX,  posY,  posZ);
	m_vView	    = CVector3(viewX, viewY, viewZ);
	m_vUpVector	= CVector3(upX,   upY,   upZ);

	// 将视点m_vView到相机所在点m_vPosition的向量单位化
	m_vView = m_vPosition + (m_vView-m_vPosition).GetNormalized();
}

// 设置Camera位置
void CCamera::PositionCamera(const CVector3 &pos, const CVector3 &view, const CVector3 &up)
{
	// 初始化Camera
	m_vPosition = pos;
	m_vView     = view;
	m_vUpVector = up;

	// 将m_vView到m_vPosition的矢量单位化
	m_vView = m_vPosition + (m_vView-m_vPosition).GetNormalized();
}

// 通过移动鼠标移动Camera的方向(第一人称)
void CCamera::SetViewByMouse(void)
{
	POINT mousePos;						// 存储鼠标位置的结构体
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1;	// 窗口宽度的一半
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1;	// 窗口高度的一半
	float angleY = 0.0f;				// 存储向上看、向下看的角度
	float angleZ = 0.0f;				// 存储向左看、向右看的角度
	static float currentRotX = 0.0f;	// 存储总的向上、向下的旋转角度
	
	// 获得当前鼠标位置
	GetCursorPos(&mousePos);						
	
	// 如果鼠标仍然在正中间，不更新场景
	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	// 将鼠标置回屏幕的中央
	SetCursorPos(middleX, middleY);

	// 获得鼠标移动的方向
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	// 保存一个当前向上或向下旋转过的角度，可以限制Camera上下做360度旋转
	currentRotX -= angleZ;  

	// 如果当前的旋转弧度大于1.3，不让Camera继续向上旋转
	if (currentRotX > 1.3f)
	{
		currentRotX = 1.3f;
	}
	// 如果当前的旋转弧度小于1.3，不让Camera继续向下旋转
	else if (currentRotX < -1.3f)
	{
		currentRotX = -1.3f;
	}
	// 否则绕所处位置旋转视线
	else
	{
		// 绕Camera的水平向量旋转Camera（上下）
		RotateView(angleZ, m_vStrafe);
	}

	// 绕Camera的向上向量旋转Camera（左右）
	RotateView(angleY, CVector3(0,1,0));
}

// 绕给出的轴旋转Camera的方向
void CCamera::RotateView(float angle, const CVector3 &vAxis)
{
	CVector3 vNewView;
	CVector3 vView = m_vView - m_vPosition;	
	
	vNewView = vView.GetRotatedAxis(angle, vAxis);//????????????????????????????????????? 精髓

	m_vView = m_vPosition + vNewView;
}

// 确认键盘控制的移动
void CCamera::CheckForMovement()
{
	// 判断是否按下向上键或'W'键
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)
	{
		MoveCamera(m_fSpeed);		// 向前移动Camera
	}

	// 判断是否按下向下键或'S'键
	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)
	{
		MoveCamera(-m_fSpeed);		// 向后移动Camera
	}

	// 判断是否按下向左键或'A'键
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)
	{
		StrafeCamera(-m_fSpeed);	// 向左移动Camera
	}

	// 判断是否按下向右键或'D'键
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)
	{
		StrafeCamera(m_fSpeed);	    // 向右移动Camera
	}

	// 相机加速
	if (GetKeyState('E') & 0x80)
	{
		if (m_fSpeed < 10)
			m_fSpeed += 0.5f;
	}
	// 相机减速
	if (GetKeyState('R') & 0x80)
	{
		if (m_fSpeed > 0.5f)
			m_fSpeed -= 0.5f;
	}
}

// 左右移动Camera
void CCamera::StrafeCamera(float speed)
{
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

// 前后移动Camera
void CCamera::MoveCamera(float speed)
{
	CVector3 vView = m_vView - m_vPosition;
	m_vPosition.x += vView.x * speed;
	m_vPosition.z += vView.z * speed;
	m_vView.x += vView.x * speed;
	m_vView.z += vView.z * speed;
}

// 更新Camera位置和方向
void CCamera::Update(void)
{
	// 更新Camera水平向量
	m_vStrafe = ((m_vView - m_vPosition).CrossProduct(m_vUpVector)).GetNormalized();

	// 鼠标移动Camera
	SetViewByMouse();

	// 键盘移动Camera
	CheckForMovement();
}

// 在场景中放置Camera
void CCamera::Look(void)
{
	// 更新Camera信息
	Update();

	// 放置Camera
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
			  m_vView.x,	 m_vView.y,     m_vView.z,
			  m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}

//相机垂直升高h
void CCamera::SetHeight(float h)
{
	CVector3 vView = m_vView - m_vPosition;	
	m_vPosition.y = h;
	m_vView = vView + m_vPosition;
}

void CCamera::SetPosX(float x)
{
	CVector3 vView = m_vView - m_vPosition;	
	m_vPosition.x = x;
	m_vView = vView + m_vPosition;
}
void CCamera::SetPosY(float y)
{
	SetHeight(y);
}
void CCamera::SetPosZ(float z)
{
	CVector3 vView = m_vView - m_vPosition;	
	m_vPosition.z = z;
	m_vView = vView + m_vPosition;
}