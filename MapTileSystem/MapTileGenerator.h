#pragma once
#include "MapTileSQL.h"
#ifndef _DATABASE_LAYOUT_
#define _DATABASE_LAYOUT_ "(QuadKey VARCHAR(50),Level INT,TileX INT,TileY INT,PATH VARCHAR(50))"
#endif

struct TILE_PARAMS
{
	int _tlx;
	int _tly;
	int _brx;
	int _bry;
	int _m;
	int _n;
	DWORD _id;
	TILE_PARAMS()
	{
		_tlx = 0;
		_tly = 0;
		_brx = 2048;
		_bry = 1024;
		_m = 1;
		_n = 1;

	}
	TILE_PARAMS
		(int tlx, int tly, int brx, int bry, int m, int n,DWORD id) :
		_tlx(tlx), _tly(tly), _brx(brx), _bry(bry), _m(m), _n(n), _id(id)
	{

	}
};

class MapTileGenerator
{
public:

	MapTileGenerator(char* filename, char* format, int levelCount);
	~MapTileGenerator();
public:
	void run();
	void run(TILE_PARAMS& tileOptoin);
	void setTileParam(TILE_PARAMS& tileOptoin);
	void close();
public:
	void initGDAL(char* filename, char* format);

	static void TileFunc(LPVOID params);
	void createTile(int level);
	void Tile2Key(int tileX, int tileY, int level, char* quadkey);
	int Key2Tile(char* quadkey, int& tileX, int& tileY);

	int getTileWidth(){ return m_iTileWidth; }
	int getTileHeight(){ return m_iTileHeight; }
	int getRasterSizeX(){ return m_iRasterSizeX; }
	int getRasterSizeY(){ return m_iRasterSizeY; }
	int getBands(){ return m_iBands; }
	int getLevels(){ return m_iLevelCount; }
	GDALDriver * getDriver(){ return m_pDriver; };
	GDALDataset* getSrc(){ return m_pSrc; }
	int getTileSize(int rx, int ry, int& iBaseWidth, int& iBaseHeight);
	MapTileSQL* getDataBase(){ return m_pMySQL; }
	bool isEnded(){ return m_isEnded; }
	DWORD getID(){ return m_threadId; }
private:
	int				m_iLevelCount;
	int				m_iTileWidth;
	int				m_iTileHeight;
	int				m_iBands;
	int				m_iRasterSizeX;
	int				m_iRasterSizeY;
	int				m_iDPI;
	TILE_PARAMS		m_TileParam;
	GDALDriver *	m_pDriver;
	GDALDataset*	m_pSrc;
	MapTileSQL*		m_pMySQL;
	bool			m_isEnded;
	//ThreadParams
	HANDLE			m_hThread;
	DWORD			m_threadId;

};

