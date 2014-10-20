#include "StdAfx.h"
#include "MapTileGenerator.h"



MapTileGenerator::MapTileGenerator(char* filename, char* format, int levelCount)
{
	GDALAllRegister();


	m_iLevelCount = levelCount;
	initGDAL(filename, format);
	m_pMySQL = new MapTileSQL(SQL_HOST,SQL_USER,SQL_PASSWORD, "MAP_TILE", MODE::CREATE_FORCE);


}
void MapTileGenerator::setTileParam(TILE_PARAMS& tileOptoin)
{
	m_TileParam = tileOptoin;
	m_threadId = m_TileParam._id;

}
void MapTileGenerator::initGDAL(char* filename, char* format)
{

	m_pDriver = GetGDALDriverManager()->GetDriverByName(format);
	char **papszMetadata;
	papszMetadata = m_pDriver->GetMetadata();
	if (!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE))
		printf("Driver %s not supports Create() method.\n", format);
	if (!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATECOPY, FALSE))
		printf("Driver %s not supports CreateCopy() method.\n", format);
	m_pSrc = (GDALDataset*)GDALOpen(filename, GA_ReadOnly);
	m_iRasterSizeX = m_pSrc->GetRasterXSize();
	m_iRasterSizeY = m_pSrc->GetRasterYSize();
	m_iBands = m_pSrc->GetRasterCount();
	m_iLevelCount = this->getTileSize(m_iRasterSizeX, m_iRasterSizeY, m_iTileWidth, m_iTileHeight);

	return;
}
MapTileGenerator::~MapTileGenerator()
{

	GDALClose(m_pSrc);
	GDALDestroyDriverManager();

}
void MapTileGenerator::Tile2Key(int tileX, int tileY, int level, char* quadkey)
{
	if (level > 32)
	{
		printf("Split too deep.");
		return;
	}
	else if (level == 0)
	{
		sprintf(quadkey, "'TopMiniLevel.'");
		return;
	}

	int m, n;

	for (int i = 0; i < level; i++)
	{
		m = (tileX >> i) & 1;
		n = (tileY >> i) & 1;

		quadkey[level - i - 1] = '0' + (m * 2) + n;

	}
	quadkey[level] = '\0';
}
int MapTileGenerator::Key2Tile(char* quadkey, int& tileX, int& tileY)
{
	int len = strlen(quadkey);
	tileX = 0;
	tileY = 0;
	for (int i = 0; i < len; i++)
	{
		int mask = 1 << (len - i - 1);
		switch (quadkey[i])
		{
		case '0':

			break;
		case '1':
			tileY |= mask;
			break;
		case '2':
			tileX |= mask;
			break;
		case '3':
			tileX |= mask;
			tileY |= mask;
			break;
		default:
			break;
		}
	}
	return len;
}

int MapTileGenerator::getTileSize(int rx, int ry, int& iBaseWidth, int& iBaseHeight)
{

	if (rx <= 0 || ry <= 0)
		return 0;
	float	m = log((float)rx) / log(2.0f);
	float	n = log((float)ry) / log(2.0f);
	int		exp = int(m < n ? m : n);
	if (m_iLevelCount != -1)
	{
		if (m_iLevelCount < exp)
			exp = m_iLevelCount;
	}
	float		ratio = pow(2.0, exp);

	iBaseWidth = int(rx / ratio);
	iBaseHeight = int(ry / ratio);

	return exp;
}

void MapTileGenerator::createTile(int level)
{

	GDALDataset* pDst = NULL;
	int resampleWidth = m_iTileWidth*(1 << (m_iLevelCount - level));
	int resampleHeight = m_iTileHeight*(1 << (m_iLevelCount - level));
	int ratio_x = (m_TileParam._brx - m_TileParam._tlx) / resampleWidth;
	int ratio_y = (m_TileParam._bry - m_TileParam._tly) / resampleHeight;
	//Generally we should process the image band by band,so here the number of bands read out at a time is set to 1.
	int szBands = 1;
	int size = m_iTileWidth*m_iTileHeight * szBands;
	BYTE* pBuffer = (BYTE*)CPLMalloc(sizeof(BYTE)*(m_iTileWidth)*(m_iTileHeight)*szBands);

	char quadKey[MAX_PATH];

	char buf[MAX_PATH];
	sprintf(buf, "./result/level_%d", level);

	if (PathFileExists(buf) == false)
		CreateDirectory(buf, NULL);

	sprintf(buf, "level_%d", level);

	m_pMySQL->createTable(buf, DATA_TYPE_BLOB);
	//m_pMySQL->createTable(buf, DATA_TYPE_PATH);

	int tile_x_offset = m_TileParam._tlx / resampleWidth;
	int tile_y_offset = m_TileParam._tly / resampleHeight;

	for (int i = tile_x_offset; i < ratio_x + tile_x_offset; i++)
	{
		for (int j = tile_y_offset; j < ratio_y + tile_y_offset; j++)
		{
			Tile2Key(i, j, level, quadKey);
			sprintf(buf, "./result/level_%d/%s.tif", level, quadKey);

			pDst = m_pDriver->Create(buf, m_iTileWidth, m_iTileHeight, m_iBands, GDT_Byte, NULL);



			for (int band = 1; band < m_iBands+1; band++)
			{
				//m_pSrc->RasterIO(GF_Read, i*resampleWidth + m_TileParam._tlx, j*resampleHeight + m_TileParam._tly, resampleWidth, resampleHeight, pBuffer, m_iTileWidth, m_iTileHeight, GDT_Byte, m_iBands, NULL, m_iBands, 0, 1);
				m_pSrc->RasterIO(GF_Read, i*resampleWidth, j*resampleHeight, resampleWidth, resampleHeight, pBuffer, m_iTileWidth, m_iTileHeight, GDT_Byte, 1, &band, 1, 0, 1);
				
				

				//pDst->RasterIO(GF_Write, 0, 0, tileWidth, tileHeight, pTempBuffer + tileSize*(i*ratio+j), tileWidth, tileHeight, GDT_Byte, bands, NULL, bands, 0, 1);

				pDst->RasterIO(GF_Write, 0, 0, m_iTileWidth, m_iTileHeight, pBuffer, m_iTileWidth, m_iTileHeight, GDT_Byte, 1, &band, 1, 0, 1);

				sprintf(buf, ",%d,%d,%d,'./result/level_%d/%d.tif'", level, i, j, level, i*ratio_y + j);

				//m_pMySQL->insert(sql::SQLString("(") + sql::SQLString(quadKey) + buf + sql::SQLString(")"));
				m_pMySQL->insert(sql::SQLString("(") + sql::SQLString(quadKey) + buf + sql::SQLString(",?)"), (char*)pBuffer, size);

				//printf("%s\n", buf);
				//pDst->RasterIO(GF_Write, pParam->_tlx / resampleWidth*tileWidth, pParam->_tly / resampleHeight*tileHeight, tileWidth*ratio, tileHeight*ratio, pBuffer, tileWidth*ratio, tileHeight*ratio, GDT_Byte, bands, NULL, bands, 0, 1);
				//pDataBase->insert("")

				//pDst = NULL;

			}
			
		}
	}
	CPLFree(pBuffer);

	GDALClose(pDst);
	printf("Thread: %d\t Fininshed Tiles Creation For \tLevel %d\n", this->getID(), level);
}

void MapTileGenerator::TileFunc(LPVOID params)
{
	MapTileGenerator* pMapTile = (MapTileGenerator*)params;
	for (int i =1; i <= pMapTile->getLevels(); i++)
	{
		
		pMapTile->createTile(i);
	
	}
	
	pMapTile->close();

}
void MapTileGenerator::run(TILE_PARAMS& tileOptoin)
{
	setTileParam(tileOptoin);
	m_isEnded = false;
	m_threadId = NULL;
	m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TileFunc, this, 0, &m_threadId);
	
	
	
}

void MapTileGenerator::run()
{
	m_isEnded = false;
	m_threadId = NULL;
	m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)(this->TileFunc), this, 0, &m_threadId);

}

void MapTileGenerator::close()
{
	if (m_isEnded == false)
	{
		m_isEnded = true;
		CloseHandle(m_hThread);
	}

}
