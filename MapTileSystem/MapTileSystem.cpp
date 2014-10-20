// MapTileSystem.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MapTileGenerator.h"
#include "MapTileSQL.h"
#define TEST_FILE "land_shallow_topo_east.tif"
#define TEST_FILE_MINI "world.tif"
#define TEST_FILE_TEST "testResample.tif"


void testGeoTiffCompressionIO();

int _tmain(int argc, _TCHAR* argv[])
{
	
	
	int M = 2, N = 2;
	DWORD start = GetTickCount();
	
	TILE_PARAMS task[16];
	MapTileGenerator* mapTileThread[16];
	int index = 0;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			
			mapTileThread[index] = new MapTileGenerator(TEST_FILE, "GTiff", 4);
			task[index] = TILE_PARAMS(i*mapTileThread[index]->getRasterSizeX() / M, j*mapTileThread[index]->getRasterSizeY() / N,
										(i + 1)*mapTileThread[index]->getRasterSizeX() / M, (j + 1)*mapTileThread[index]->getRasterSizeY() / N,
										M, N, i*N + j);
			index++;

		}
	}
	for (int i = 0; i < M*N; i++)
	{
		//mapTileThread[i]->setTileParam(task[i]);

		mapTileThread[i]->run(task[i]);

	}
	bool res;
	while (1)
	{
		res = true;
		for (int i = 0; i < M*N; i++)
			res &= mapTileThread[i]->isEnded();
		if (res)
			break;
	}
	DWORD end = GetTickCount();
	printf("Elapsed Time: %f\n", (end - start) / 1000.0);

	return 0;
}
void testGeoTiffCompressionIO()
{
	GDALAllRegister();
	char* format = "GTiff";
	GDALDriver *	pdriver = GetGDALDriverManager()->GetDriverByName(format);
	char **papszMetadata;
	papszMetadata = pdriver->GetMetadata();
	if (!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE))
		printf("Driver %s not supports Create() method.\n", format);
	if (!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATECOPY, FALSE))
		printf("Driver %s not supports CreateCopy() method.\n", format);
	
	GDALDataset* pSrc = (GDALDataset*)GDALOpen(TEST_FILE, GA_ReadOnly);
	
	//printf("%s\n", pSrc->GetMetadataItem("COMPRESS", GDAL_DMD_CREATIONOPTIONLIST));

	int rasterX = pSrc->GetRasterXSize();
	int rasterY = pSrc->GetRasterYSize();
	int szBands = pSrc->GetRasterCount();
	int bandMap[1] = { -1 };
	char **papszOptions = NULL;

	papszOptions = CSLSetNameValue(papszOptions, "COMPRESS", "PACKBITS");


	/*GDALDataset* pDst = pdriver->CreateCopy("testResample.tif", pSrc, FALSE,
		papszOptions, NULL, NULL);*/

	GDALDataset* pDst = pdriver->Create("testResample.tif", rasterX, rasterY, szBands, GDT_Byte, NULL);


	try
	{

		//BYTE* pBuffer = (BYTE*)CPLCalloc(1 * rasterX*rasterY, sizeof(BYTE));
		BYTE* pBuffer = (BYTE*)CPLMalloc(1 * rasterX*rasterY*sizeof(BYTE));

		for (int band = 0; band < szBands; band++)
		{
			printf("%d\n", band);
			bandMap[0] = band + 1;
			//pSrc->RasterIO(GF_Read, pSrc->GetRasterXSize() / 2, pSrc->GetRasterYSize() / 2, pSrc->GetRasterXSize() / 2, pSrc->GetRasterYSize() / 2, pBuffer, pDst->GetRasterXSize(), pDst->GetRasterYSize(), GDT_Byte, 1, bandMap, 1, 0, 1);
			pSrc->RasterIO(GF_Read, 0,0, pSrc->GetRasterXSize(), pSrc->GetRasterYSize(), pBuffer, pDst->GetRasterXSize(), pDst->GetRasterYSize(), GDT_Byte, 1, bandMap, 1, 0, 1);

			pDst->RasterIO(GF_Write, 0, 0, pDst->GetRasterXSize(), pDst->GetRasterYSize(), pBuffer, pDst->GetRasterXSize(), pDst->GetRasterYSize(), GDT_Byte, 1, bandMap, 1, 0, 1);
			
		}
		CPLFree(pBuffer);


	}
	catch (std::bad_alloc& ba){
		cout << ba.what() << endl;
	}
	GDALDestroyDriverManager();

}
