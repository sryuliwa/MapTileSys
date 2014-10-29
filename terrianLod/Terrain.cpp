#include "Terrain.h"

CTerrain::CTerrain(CCamera* cam)
{	
	m_iDrawMode = TEXTURE;
	m_iLodMode  = QUADTREE;

	m_pCamera = cam;

	m_fScale  = HEIGHT_SCALE;                  ///???????????
	m_nSize   = LOD_MAP_SIZE;
    m_HMMatrix.Reset(LOD_MAP_SIZE, LOD_MAP_SIZE);
	LoadRawFile(TERRAIN_RAW_FILE);
	m_pLOD = new CLOD(this,m_pCamera);
	m_texture[0].Load("data\\terrain.bmp");
}

CTerrain::~CTerrain()
{	
	if(m_pLOD)  delete m_pLOD;
	m_pLOD = NULL;
}

void CTerrain::Render()
{
	if (m_iLodMode == QUADTREE)//使用四叉树
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);   
		glPushMatrix();

//		glScalef(1.0,m_fScale,1.0);  /////////
		if (m_iDrawMode==TEXTURE)
		{
			glEnable(GL_TEXTURE_2D);  
			glBindTexture(GL_TEXTURE_2D, m_texture[0].ID);
		}
		m_pLOD->Render();              //绘制
		if (m_iDrawMode!=TEXTURE)
			glDisable(GL_TEXTURE_2D); 
 		glPopMatrix();
	}
// 	else if (m_iLodMode == ROAM)//使用ROAM
// 	{
// 	}
	else
	{
		MessageBox(NULL,"No LOD created,SimNature will exit!","error",MB_OK);
		exit(0);
	}
//  自动调整相机高度    //??????????????????????????????????
	m_pCamera->SetHeight(GetAveHeight(m_pCamera->Position().x, m_pCamera->Position().z) + EYES_HEIGHT);
}

bool CTerrain::LoadRawFile(const char* fileName)//将Raw文件中的数据存入 m_HeightMapMatrix
{
	int width = m_nSize/* - 1*/;  //width=1025
	FILE *fp = fopen( fileName, "rb" );	
	if ( fp == NULL )	
	{
		MessageBox(NULL, "打开高度图文件失败！", "错误", MB_OK);
		exit(0);
	}	
	BYTE* tempHeightMap = new BYTE[width*width];//地图载入1024*1024 ???????????????????????????
	fread( tempHeightMap, 1, width*width, fp );	
	m_HMMatrix.Reset(width, width);
    m_HMMatrix.SetData(tempHeightMap);
	delete[] tempHeightMap;
	fclose(fp);
	return true;
}


int  CTerrain::GetHeight(int x, int z)//获得实际高度
{
	return ( m_HMMatrix(x,z)*m_fScale );//
}

float CTerrain::GetAveHeight(float x,float y)//双线性插值取得(x,z)处的高度
{
	int xl, xh, yl, yh;
	if (x<0 || x> m_nSize || y<0 || y>m_nSize) return 0;
	xl = x;
	xh = xl+1;
	yl = y;
	yh = yl+1;   
	float dx = x - xl ;
	float dy = y - yl ;	
	float hll = (    dy  * dx    ) * m_HMMatrix(xh, yh);
	float hlh = ( (1-dy) * dx    ) * m_HMMatrix(xh, yl);
        float hhl = (    dy  * (1-dx)) * m_HMMatrix(xl, yh);
        float hhh = ( (1-dy) * (1-dx)) * m_HMMatrix(xl, yl);
	
	return (hll+hlh+hhh+hhl) * m_fScale;	
}


