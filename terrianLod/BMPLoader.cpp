//======================================================================
// @file      CBMPLoader.cpp
//======================================================================

#include"BMPLoader.h"              // 包含头文件 

CBMPLoader::CBMPLoader()
{
   // 初始化成员值为0 
	image = 0;
	imageWidth  = 0;
	imageHeight = 0;
}

CBMPLoader::~CBMPLoader()
{
   FreeImage(); // 释放图像数据占据的内存 
}

// 装载一个位图文件 
bool CBMPLoader::LoadBitmap(const char *file)
{
	FILE *pFile = 0; 
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
  	unsigned char textureColors = 0;// 用于将图像颜色从BGR变换到RGB

	pFile = fopen(file, "rb"); 
	if(pFile == 0) return false;
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);	
	if(header.bfType != BITMAP_ID)
	   {
		   fclose(pFile);             // 若不是位图文件,则关闭文件并返回 
		   return false;
	   }
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
	imageWidth  = bitmapInfoHeader.biWidth;
    imageHeight = bitmapInfoHeader.biHeight;
    if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
      bitmapInfoHeader.biHeight * 3;
	fseek(pFile, header.bfOffBits, SEEK_SET);
	image = new unsigned char[bitmapInfoHeader.biSizeImage];
	if(!image)                        // 若分配内存失败则返回 
	   {
		   delete[] image;
		   fclose(pFile);
		   return false;
	   }
	// 读取图像数据 
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);
	// 将图像颜色数据格式进行交换,由BGR转换为RGB 
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3)
	   {
		   textureColors = image[index];
		   image[index] = image[index + 2];
		   image[index + 2] = textureColors;
	   }
  
	fclose(pFile);       // 关闭文件  
	return true;         // 成功返回  
}

//载入位图文件，并创建纹理
bool CBMPLoader::Load(const char* fileName)
{
	if(!LoadBitmap(fileName))
	{
		MessageBox(NULL,"载入位图文件失败!","错误",MB_OK);
		exit(0);
	}

	glGenTextures(1, &ID);   
    glBindTexture(GL_TEXTURE_2D, ID);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   
   	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, imageWidth,
	                  imageHeight, GL_RGB, GL_UNSIGNED_BYTE, image);
    return true;
}

void CBMPLoader::FreeImage()   // 释放分配的内存 
{
   if(image)
      {
         delete[] image;
         image = 0;
      }
}
