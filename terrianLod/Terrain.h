#ifndef __LodTerrain_H__
#define __LodTerrain_H__

#include "stdafx.h"
#include "Vector3.h"                 /**< 包含向量类头文件 */
#include "BMPLoader.h"
#include "Matrix.h"
#include "Camera.h"
#include "Lod.h"

class CTerrain
{
	friend class CLOD;
public:
	CTerrain(CCamera* cam);
	~CTerrain();

	void  Render(/*Camera* camera*/);
	float GetAveHeight(float x,float z);     //通过双线性插值得到高度(x,z)，用于摄像机调整高度
    void  SetScale(float scale){m_fScale = scale;}
	void  SetDrawMode(int mode){ if(mode<3)m_iDrawMode = mode; }

	int   m_iDrawMode;
	int   m_iLodMode;
//private:
 	bool  LoadRawFile(const char* fileName); //把Raw文件中的高度信息载入到
	int   GetHeight(int x, int z);           //获得(x,z)经缩放的高度y

 	CBMPLoader    m_texture[1];   //地面纹理//和纹理细节
	int           m_nSize;        //LOD地图宽度 1025
	float         m_fScale;       //设定一个高度单位表示多少米
	CMatrix<BYTE> m_HMMatrix;     //???????????????????改名称
	CCamera*      m_pCamera;
	CLOD*         m_pLOD;         //四叉树对象（也可以用指针）
};

#endif //__LodTerrain_H__

	
