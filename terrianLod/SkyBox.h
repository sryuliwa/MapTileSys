
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "stdafx.h"
#include "BMPLoader.h"
#include "Vector3.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F

/** 天空盒类 */
class CSkyBox
{
public:
	CSkyBox();
	~CSkyBox();

	bool init();
	void render(CCamera *cam);

	void SetDrawMode(int mode){m_iDrawMode = mode;}
private:
	CBMPLoader  m_texture[6];   /**< 天空盒纹理   */
	CVector3    m_CameraPos;    /**< 当前摄像机位置 */
	float       length;         /**< 长度 */
	float       width;          /**< 宽度 */
	float       height;         /**< 高度 */
	float       yRot;           /**< 绕Y轴旋转 */

	int         m_iDrawMode;
		
};


#endif ///__SKYBOX_H__