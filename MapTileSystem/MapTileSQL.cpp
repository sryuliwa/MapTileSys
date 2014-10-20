#include "StdAfx.h"
#include "MapTileSQL.h"


MapTileSQL::MapTileSQL(const sql::SQLString& hostname, const sql::SQLString& username, const sql::SQLString& password, const sql::SQLString& dbName, int mode)
:MySQLConnecter()
{
	MySQLConnecter::connect(hostname, username, password, dbName, mode);
}


MapTileSQL::~MapTileSQL()
{
}
void MapTileSQL::createTable(const sql::SQLString& dbName, int dataType)
{

	switch (dataType)
	{
	case DATA_TYPE::DATA_TYPE_PATH:
		MySQLConnecter::createTable(dbName, _TABLE_LAYOUT_PATH,MODE::CREATE_FORCE);
		break;
	case DATA_TYPE::DATA_TYPE_BLOB:
		MySQLConnecter::createTable(dbName, _TABLE_LAYOUT_MEDIUUBLOB, MODE::CREATE_FORCE);
		break;
	default:
		//Handle exception warnings here.
		break;

	}
}

int MapTileSQL::insert(const sql::SQLString& full_value)
{
	return MySQLConnecter::insert(full_value);
}
void MapTileSQL::insert(const sql::SQLString& part_value, char* buf, int len)
{
	membuf blob(buf, buf + len);
	std::iostream sBlob(&blob);

	MySQLConnecter::insert(part_value, sBlob);
}


sql::SQLString MapTileSQL::queryPath(const sql::SQLString& quadKey)
{

	sql::SQLString condition = _FIELD_QUADKEY + sql::SQLString("=") + quadKey;
	sql::ResultSet* pRes = MySQLConnecter::query(_FIELD_PATH, condition);
	sql::SQLString result;
	int cnt = 0;
	while (pRes->next()) {
		if (cnt++ > 1)
		{
			//Prompt warnings for duplicate quadkey.
			std::printf("QuadKey Repeating.\n");
			return NULL;
		}

		result = pRes->getString(_FIELD_PATH);
	}
	if (cnt == 0)
	{
		std::printf("No Record Retrived.\n");
		return NULL;
	}
	return result;
}


void MapTileSQL::queryBLOB(const sql::SQLString& quadKey, char* blob, long& len)
{
	sql::SQLString condition = _FIELD_QUADKEY + sql::SQLString("=") + quadKey;
	sql::ResultSet* pRes = MySQLConnecter::query(_FIELD_BLOB, condition);

	int cnt = 0;
	while (pRes->next()) {
		if (cnt++ > 1)
		{
			//Prompt warnings for duplicate quadkey.
			std::printf("QuadKey Repeating.\n");
			return;
		}

		std::istream* 	is = pRes->getBlob(_FIELD_BLOB);
		is->seekg(0, ios::end);
		len = is->tellg();
		is->seekg(0, ios::beg);
		is->read(blob, len);

	}
	if (cnt == 0)
	{
		std::printf("No Record Retrived.\n");
		return;
	}

	return;
}