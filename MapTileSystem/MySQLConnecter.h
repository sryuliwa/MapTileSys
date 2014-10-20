#pragma once

#include "stdafx.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
#pragma comment(lib,"mysqlcppconn.lib")

#define PRINT_EXCEPTION(e) {\
	std::cout << "# ERR: SQLException in " << __FILE__; \
	std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl; \
	std::cout << "# ERR: " << e.what(); \
	std::cout << " (MySQL error code: " << e.getErrorCode(); \
	std::cout << ", SQLState: " << e.getSQLState() << " )" << endl; \
	getchar(); \
}
struct membuf : std::streambuf
{
	membuf(char* begin, char* end) {
		this->setg(begin, begin, end);
	}
};


enum OPERATION
{
	DATABASE_CREATE = 0,
	DATABASE_DROP = 1,
	DATABASE_SHOW = 2,
	DATABASE_SHOW_LIKE = 3,
	DATABASE_SELECT = 4,
	DATABASE_SELECT_FROM = 5,
	DATABASE_SELECT_WHERE = 6,
	DATABASE_USE = 7,
	TABLE_CREATE = 8,
	TABLE_DROP = 9,
	TABLE_INSERT = 10,
	TABLE_INSERT_VALUE = 11,
	TABLE_DROP_IF_EXISTS = 12,
	TABLE_SHOW_LIKE = 13,
	DATABASE_DROP_IF_EXISTS=14,
	TABLE_LOAD_LOCAL_FILE=15,
	TABLE_LOAD_LOCAL_INTO_TABLE=16,
	TABLE_CREATE_IF_NOT_EXISTS=17
};
enum MODE
{
	CREATE_FORCE = 0,
	CREATE = 1,
	CREATE_OPEN_EXISTING = 2
};
static sql::SQLString SQL_OPERATION[18] =
{
	sql::SQLString("CREATE DATABASE "),
	sql::SQLString("DROP DATABASE "),
	sql::SQLString("SHOW DATABASES "),
	sql::SQLString("SHOW DATABASES LIKE "),
	sql::SQLString("SELECT "),
	sql::SQLString(" FROM "),
	sql::SQLString(" WHERE "),
	sql::SQLString("USE "),
	sql::SQLString("CREATE TABLE "),
	sql::SQLString("DROP TABLE "),
	sql::SQLString("INSERT INTO "),
	sql::SQLString(" VALUE "),
	sql::SQLString("DROP TABLE IF EXISTS "),
	sql::SQLString("SHOW TABLES LIKE "),
	sql::SQLString("DROP DATABASE IF EXISTS "),
	sql::SQLString("LOAD DATA LOCAL "),
	sql::SQLString(" INTO TABLE "),
	sql::SQLString("CREATE TABLE IF NOT EXISTS "),
};
class MySQLConnecter
{
public:
	MySQLConnecter();
	~MySQLConnecter();
public:
	int connect(const sql::SQLString& hostname, const sql::SQLString& username, const sql::SQLString& password, const sql::SQLString& dbName,int mode);
	virtual int createDataBase(const sql::SQLString& dbName);
	virtual int createTable(const sql::SQLString& tableName, const sql::SQLString& tableLayout,int mode);
	virtual int insert(const sql::SQLString& value);
	virtual int insert(const sql::SQLString& part_value, std::iostream& blob);
	virtual sql::ResultSet* query(const sql::SQLString& what_to_select, sql::SQLString& conditions_to_satisfy);
	int excute(const sql::SQLString& queryStr);
	int deleteDataBase(const sql::SQLString& dbName);
	int deleteTable(const sql::SQLString& tableName);
	int loadLocalFile(const sql::SQLString& filename);
public:
	
	int setWorkingDatabase(const sql::SQLString& dnName);
	int setWorkingTable(const sql::SQLString& tableName);
	sql::SQLString getWorkingDatabase();
	sql::SQLString getWorkingTable();
	sql::Connection* getConnection(){ return m_pCon; }
	sql::Driver*	getDriver(){ return m_pDriver; };
	sql::Statement* getStatement(){ return m_pStmt; }
private:
	int				m_mode;
	sql::SQLString	m_working_databaseName;
	sql::SQLString	m_working_tableName;
	sql::Driver*	m_pDriver;
	sql::Connection* m_pCon;
	sql::Statement* m_pStmt;
	sql::ResultSet* m_pRes;
};

