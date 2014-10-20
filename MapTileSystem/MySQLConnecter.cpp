#include "stdafx.h"
#include "MySQLConnecter.h"



MySQLConnecter::MySQLConnecter()
{
	m_pDriver = NULL;
	m_pCon = NULL;
	m_pStmt = NULL;
	m_pRes = NULL;
	m_working_tableName = sql::SQLString("__NOT_INITIALIZED");
	m_working_tableName = sql::SQLString("__NOT_INITIALIZED");

}

MySQLConnecter::~MySQLConnecter()
{
}

int MySQLConnecter::connect(const sql::SQLString& hostname, const sql::SQLString& username, const sql::SQLString& password, const sql::SQLString& dbName, int mode)
{
	try
	{
		m_pDriver = get_driver_instance();
		m_pCon = m_pDriver->connect(hostname, username, password);
		m_pStmt = m_pCon->createStatement();
		switch (mode)
		{
		case MODE::CREATE_FORCE:
			m_pStmt->execute(SQL_OPERATION[DATABASE_DROP_IF_EXISTS] + dbName);
		case MODE::CREATE:
			m_pStmt->execute(SQL_OPERATION[DATABASE_CREATE] + dbName);
		case MODE::CREATE_OPEN_EXISTING:
			m_pCon->setSchema(dbName);
			break;
		default:
			break;
		}
		m_working_tableName = dbName;

	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;

}
int MySQLConnecter::createDataBase(const sql::SQLString& dbName)
{

	try
	{
		m_pStmt->execute(SQL_OPERATION[DATABASE_DROP_IF_EXISTS] + dbName);
		m_pStmt->execute(SQL_OPERATION[DATABASE_CREATE] + dbName);
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;
}
int MySQLConnecter::createTable(const sql::SQLString& tableName, const sql::SQLString& tableLayout,int mode)
{
	try
	{

		m_working_tableName = tableName;

		switch (mode)
		{
		
		case MODE::CREATE_FORCE:
			//m_pStmt->execute(SQL_OPERATION[TABLE_DROP_IF_EXISTS] + m_working_tableName);
			m_pStmt->execute(SQL_OPERATION[TABLE_CREATE_IF_NOT_EXISTS] + m_working_tableName + tableLayout);
			break;
		case MODE::CREATE:
			m_pStmt->execute(SQL_OPERATION[TABLE_CREATE_IF_NOT_EXISTS] + m_working_tableName + tableLayout);
			break;
		case MODE::CREATE_OPEN_EXISTING:

			break;
		default:
			break;
		}


	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}

	return 1;

}
int MySQLConnecter::insert(const sql::SQLString& value)
{
	try
	{
	
		m_pStmt->execute(SQL_OPERATION[TABLE_INSERT] + 
							m_working_tableName +
							SQL_OPERATION[TABLE_INSERT_VALUE] + 
							value);
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;


}
int  MySQLConnecter::insert(const sql::SQLString& part_value,std::iostream& blob)
{
	try
	{
		sql::PreparedStatement* pstmt = getConnection()->prepareStatement(SQL_OPERATION[TABLE_INSERT] +
																		m_working_tableName +
																		SQL_OPERATION[TABLE_INSERT_VALUE] +
																		part_value);
	
		pstmt->setBlob(1, &blob);
		pstmt->executeUpdate();
		delete pstmt;
		
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;

}
sql::ResultSet* MySQLConnecter::query(const sql::SQLString& what_to_select, sql::SQLString& conditions_to_satisfy)
{
	try
	{
		
		m_pRes = m_pStmt->executeQuery(SQL_OPERATION[DATABASE_SELECT] + what_to_select +
			SQL_OPERATION[DATABASE_SELECT_FROM] + m_working_tableName +
			SQL_OPERATION[DATABASE_SELECT_WHERE] + conditions_to_satisfy);
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return m_pRes;

}
int MySQLConnecter::excute(const sql::SQLString& queryStr)
{
	try
	{

		m_pStmt->execute(queryStr);
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;

}

int MySQLConnecter::deleteDataBase(const sql::SQLString& dbName)
{
	try
	{
		m_pStmt->execute(SQL_OPERATION[DATABASE_DROP] + dbName);

	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;
}
int MySQLConnecter::deleteTable(const sql::SQLString& tableName)
{
	try
	{
		m_pStmt->execute(SQL_OPERATION[TABLE_DROP] + tableName);
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;
}

int MySQLConnecter::setWorkingDatabase(const sql::SQLString& dnName)
{
	try
	{
		m_working_databaseName = dnName;
		m_pStmt->execute(SQL_OPERATION[DATABASE_USE] + dnName);
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;
}
int MySQLConnecter::setWorkingTable(const sql::SQLString& tableName)
{
	try
	{
		m_working_tableName = tableName;
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;

}
sql::SQLString MySQLConnecter::getWorkingDatabase()
{
	return m_working_databaseName;
}
sql::SQLString MySQLConnecter::getWorkingTable()
{
	return m_working_tableName;
}

int MySQLConnecter::loadLocalFile(const sql::SQLString& filename)
{
	try
	{
		m_pStmt->execute(SQL_OPERATION[TABLE_LOAD_LOCAL_FILE]+filename+
						SQL_OPERATION[TABLE_LOAD_LOCAL_INTO_TABLE]+m_working_tableName );
	}
	catch (sql::SQLException& e)
	{
		PRINT_EXCEPTION(e);
	}
	return 1;
}

