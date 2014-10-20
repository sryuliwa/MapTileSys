#include "StdAfx.h"
#include "ImageLayerWrapper.h"
#include <assert.h>

#define PRINT_TILE_INFO \
					{\
					if (key.valid())\
					{\
						if (!key.str().empty())\
							LOG("Key:"); LOG(key.str().c_str()); \
						unsigned int tx = 0, ty = 0; \
						key.getTileXY(tx, ty); \
						fprintf(fout, " TileXY: %d, %d ", tx, ty); \
						fprintf(fout, "LOD: %d", key.getLevelOfDetail()); \
					}\
					if (progress)\
						LOG(progress->message()); \
					LOG("\n"); \
				}

void saveTileImage(const TileKey& key,GeoImage& geoImg)
{
	//retrieve tile related infomation.
	osg::Image* img = geoImg.getImage();
	GeoExtent	geoExt = geoImg.getExtent();
	const SpatialReference* srs = geoImg.getSRS();

	if (img == NULL || !key.valid())
		return;
	
	//Save images,so we can build a database of our own.
	//After some experiments,a driver for database-based tile map service can be implemented.
	unsigned int tx = -1, ty = -1;
	char filename[_MAX_FNAME];
	char path[MAX_PATH];
	char buf[MAX_PATH];
	key.getTileXY(tx, ty);

	sprintf(path, "./Tile/%d", tx);

	if (!PathFileExistsA(path))
		CreateDirectoryA(path, NULL);

	sprintf(path, "./Tile/%d/%d", tx,ty);
	if (!PathFileExistsA(path))
		CreateDirectoryA(path, NULL);


	sprintf(filename, "./Tile/%s",key.str().c_str());
	strcat(filename, ".jpg");
	FILE* fout = fopen(filename, "wb");
	if (fout)
	{
		fwrite(img->getDataPointer(), img->getTotalSizeInBytes(), 1, fout);
		fclose(fout);
	}
	sprintf(buf, "size: %d bytes.\n", img->getImageSizeInBytes());
	LOG(filename); 
	LOG(buf);
	LOG("\n");

}
void replaceGeoImage(char* fbmp, GeoImage& OriginalGeoImg)
{
	GDALDriver* pDriver = GetGDALDriverManager()->GetDriverByName("BMP");
	if (pDriver == NULL)
	{
		//Handle Exception Here.

		return;
	}
	GDALDataset* pSrc = (GDALDataset*)GDALOpen(fbmp, GA_ReadOnly);
	if (pSrc == NULL)
	{
		//Handle Exception Here.

		return;

	}
	assert(pSrc->GetRasterXSize() == 256 && pSrc->GetRasterYSize() == 256 && pSrc->GetRasterCount() == 3);

	BYTE* pRasterBuf = (BYTE*)CPLCalloc(256*256*sizeof(BYTE),3);
	BYTE* pRgbBuf = (BYTE*)CPLCalloc(256 * 256 * sizeof(BYTE), 3);
	
	pSrc->RasterIO(GF_Read, 0, 0, 256, 256, pRasterBuf, 256, 256, GDT_Byte, 3, NULL, 3, 0, 1);
	int rasterSize = pSrc->GetRasterXSize()*pSrc->GetRasterYSize();
	BYTE* pB = pRasterBuf;
	BYTE* pG = pRasterBuf + rasterSize;
	BYTE* pR = pRasterBuf + rasterSize * 2;
	BYTE* pRgb = pRgbBuf;
	for (int i = 0; i < rasterSize; i++)
	{
		*(pRgbBuf++) = *(pR++);
		*(pRgbBuf++) = *(pG++);
		*(pRgbBuf++) = *(pB++);
	}

	osg::Image* img = OriginalGeoImg.getImage();
	//Replace the tile image.
	img->setImage(256, 256, 3, img->getInternalTextureFormat(), img->getPixelFormat(), img->getDataType(), pRgb, img->getAllocationMode(), img->getPacking());
	

	GDALClose(pSrc);
	CPLFree(pRasterBuf);
	return;
}

ImageLayerWrapper::ImageLayerWrapper(const ImageLayerOptions& options)
:ImageLayer(options)
{

#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::ImageLayerWrapper(const ImageLayerOptions& options) Init.\n");
#endif
	if (!PathFileExists(L"Tile"))
		CreateDirectory(L"Tile",NULL);
	GDALAllRegister();

}
ImageLayerWrapper::ImageLayerWrapper(const std::string& name, const TileSourceOptions& driverOptions)
: ImageLayer(name, driverOptions)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::ImageLayerWrapper(const std::string& name, const TileSourceOptions& driverOptions) Init.\n");
#endif

}
ImageLayerWrapper::ImageLayerWrapper(const ImageLayerOptions& options, TileSource* tileSource)
: ImageLayer(options, tileSource)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::ImageLayerWrapper(const ImageLayerOptions& options, TileSource* tileSource) Init.\n");
#endif
}


ImageLayerWrapper::~ImageLayerWrapper()
{
}


void ImageLayerWrapper::setOpacity(float opacity)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::setOpacity");

#endif
	return ImageLayer::setOpacity(opacity);
}

float ImageLayerWrapper::getOpacity() const
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::getOpacity");

#endif
	return ImageLayer::getOpacity();

}

void ImageLayerWrapper::disableLODBlending()
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::disableLODBlending");

#endif
	return ImageLayer::disableLODBlending();
}
bool ImageLayerWrapper::isLODBlendingEnabled() const
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::isLODBlendingEnabled");

#endif
	return ImageLayer::isLODBlendingEnabled();

}

GeoImage ImageLayerWrapper::createImage(const TileKey& key, ProgressCallback* progress, bool forceFallback)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::createImage");
	PRINT_TILE_INFO
#endif
	GeoImage geoImg = ImageLayer::createImage(key, progress, forceFallback);
	
	//Here we can get the query string for tile image.What we need to do is to replace the GeoImage with data stored in database.
	if (key.getLevelOfDetail() == 0)
		replaceGeoImage("Penguins.bmp", geoImg);
	return geoImg;
}

/**
* Creates an image that is in the image layer's native profile.
*/
GeoImage ImageLayerWrapper::createImageInNativeProfile(const TileKey& key, ProgressCallback* progress, bool forceFallback)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::createImageInNativeProfile");
	PRINT_TILE_INFO
#endif
	return ImageLayer::createImageInNativeProfile(key, progress, forceFallback);
}

CacheBin* ImageLayerWrapper::getCacheBin(const Profile* profile)
{
	return ImageLayer::getCacheBin(profile);

}
GeoImage ImageLayerWrapper::createImageInKeyProfile(const TileKey& key, ProgressCallback* progress, bool forceFallback)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::createImageInKeyProfile");
	PRINT_TILE_INFO
#endif
	return ImageLayer::createImageInKeyProfile(key, progress, forceFallback);
}
GeoImage ImageLayerWrapper::createImageFromTileSource(const TileKey& key, ProgressCallback* progress, bool forceFallback)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::createImageFromTileSource");
	PRINT_TILE_INFO
#endif
	return ImageLayer::createImageFromTileSource(key, progress, forceFallback);
}
GeoImage ImageLayerWrapper::assembleImageFromTileSource(const TileKey& key, ProgressCallback* progress, bool forceFallback)
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::assembleImageFromTileSource");
	PRINT_TILE_INFO
#endif
	return ImageLayer::assembleImageFromTileSource(key, progress, forceFallback);

}
void ImageLayerWrapper::initTileSource()
{
#ifdef _LOG_CALLS_
	LOG("ImageLayerWrapper::initTileSource");

#endif
	ImageLayer::initTileSource();

}
