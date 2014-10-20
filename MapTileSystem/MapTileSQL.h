#pragma once
#include "MySQLConnecter.h"


#ifndef _TABLE_LAYOUT_MEDIUUBLOB 
#define _TABLE_LAYOUT_MEDIUUBLOB \
								" (QuadKey VARCHAR(50),\
								Level INT,\
								TileX INT,\
								TileY INT,\
								PATH VARCHAR(50),\
								imageData LONGBLOB)"
#endif


#ifndef _TABLE_LAYOUT_PATH
#define _TABLE_LAYOUT_PATH \
							" (QuadKey VARCHAR(50),\
							Level INT,\
							TileX INT,\
							TileY INT,\
							PATH VARCHAR(50))"

#endif


#define _FIELD_PATH		"PATH"
#define _FIELD_BLOB		"imageData"
#define _FIELD_QUADKEY  "QuadKey"
#define _FIELD_TILE_X	"TileX"
#define _FIELD_TILE_Y	"TileY"
#define _FIELD_LEVEL	"Level"


enum DATA_TYPE
{
	DATA_TYPE_PATH=0,
	DATA_TYPE_BLOB=1
};


class MapTileSQL :
	public MySQLConnecter
{
public:
	MapTileSQL(const sql::SQLString& hostname, const sql::SQLString& username, const sql::SQLString& password, const sql::SQLString& dbName, int mode);
	~MapTileSQL();
public:
	void createTable(const sql::SQLString& dbName,int dataType);
	//Insert PathType Value
	int insert(const sql::SQLString& full_value);
	//Insert BlobType Value
	void insert(const sql::SQLString& part_value, char* buf, int len);
	//Query the path using QuadKey
	sql::SQLString queryPath(const sql::SQLString& quadKey);
	//Query the Image of Vetor Data Using QuadKey
	void queryBLOB(const sql::SQLString& quadKey, char* blob, long& len);
	

	
};

