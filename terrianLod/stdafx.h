/**  
*   @file   stdafx.h
*/

#ifndef __STDAFX_H__
#define __STDAFX_H__

/** 包含常用的头文件 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>                            
#define GLUT_DISABLE_ATEXIT_HACK
/** 包含gl头文件 */
#include <gl\gl.h>				 
#include <gl\glu.h>
#include <gl\glaux.h>
#include <gl\glut.h>

/** 包含OpenGL链接库文件 */

/** 禁止编译器出现类型转换的警告 */
#pragma warning(disable: 4311)                                 
#pragma warning(disable: 4312)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 4267)

//------定义地面网格 MAP_WIDTH, CELL_WIDTH
#define TERRAIN_IMAGE_FILE "data/terrain.bmp" //纹理图片的文件名
#define TERRAIN_RAW_FILE "data/terrain.raw"   //地形高度图文件名
//------绘制方式
const int TEXTURE = 0;   //纹理
const int FRAME   = 1;   //线框
const int FILL    = 2;   //填充
const int QUADTREE  = 0;
const int ROAM      = 1;

///视景体信息
const double FURSTUM_NEAR = 1.0;      //视景体的near(平头截体)
const double FURSTUM_FAR  = 2000.0;    //视景体的far (平头截体)
const double FURSTUM_FOVY = 80;       //??
//-----LOD算法中的一些常量
const unsigned int MAX_LOD_LEVEL=10;
const unsigned int LOD_MAP_SIZE = 1025 ;//这个限制了相机的移动范围
const int EYES_HEIGHT = 10;  //第一人称相机的眼睛高度
const float HEIGHT_SCALE = 1.0f;//地形高度缩放因子的默认值
#endif