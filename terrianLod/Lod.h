

#ifndef _LOD_H_
#define _LOD_H_

#include "stdafx.h"
#include "Terrain.h"
class CTerrain;

typedef unsigned char VS; //VS;
const VS VS_DISABLE =  0x00;
const VS VS_ACTIVE  =  0x01;
const VS VS_CULLED  =  0x02;
const VS VS_STOPED  =  0x04;
const VS VS_UNREACH =  0x08;

const int NV_L = 0;//同一级的左邻
const int NV_R = 1;
const int NV_U = 2;
const int NV_D = 3; 

const int SN_LU = 0;//下一级的左上
const int SN_LD = 1;
const int SN_RD = 2;
const int SN_RU = 3;

class CLOD
{
	typedef struct _NODE
	{
		int _x,_y;
	}QNODE;
public :
// 	void AfterRender();
// 	void PreRender();
	void SetViewer(/*Camera* pCamera*/);

 	CLOD(CTerrain* t,CCamera* cam);
	~CLOD(){ }

	void DrawNode(int cx,int cy,int d);
	void DrawPrim(int cx,int cy);

	BOOL NodeCanDivid(int cx,int cy,int d);
	void DividNode(int cx,int cy,int d);
	void DisableNode(int cx,int cy,int d);
// 	bool IsViewerInRect(int cx,int cy,int d){return true;}//????????目前先默认为可见，以后要修改

	void CheckNeighbor(int cx,int cy,int d);
    VS   CanActive(int x,int y,int d);
	void InitLODInfo();
    void RenderLOD();
	void Render();
	void CalculateDHMatrix();//??????????????????????????????????

	float m_fc;     // 全局粗糙度因子
	float m_fC;     // 全局距离因子
	int   m_nSize;  // 渲染地图的宽度,不一定是Terrain的高度图的宽度,可能是其中一块

private:
	void DrawPrim_FILL(int x, int y);
	void DrawPrim_TEXTURE(int x, int y);
	void DrawPrim_FRAME(int x, int y);
	void DrawNode_FILL(int x,int y, int d);
	void DrawNode_TEXTURE(int x, int y,int d);
	void DrawNode_FRAME(int x, int y,int d);

	int   m_delta[MAX_LOD_LEVEL];   //里面放置第i级LOD正方形的半边长
//	int   m_maxlevel;               //指定的最大LOD等级
	float m_ViewX,m_ViewY,m_ViewZ;  //视点位置, 即摄像机位置

	int   m_neighbor[4]; //?????
	int   m_subnode[4];  //?????

	CTerrain*      m_pTerrain;
	CMatrix<VS>    m_LodMatrix;   //这里是否应该设置为指针或者引用
	CMatrix<float> m_DHMatrix;

	CCamera*       m_pCamera;
};



#endif 