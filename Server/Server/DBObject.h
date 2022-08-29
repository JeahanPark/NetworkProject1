#pragma once
#define BINDPARAM_STARTINDEX 1

class DBObject
{
public:
	DBObject(SQLHDBC _connect);
	~DBObject();

public:
	void SQLStmtFree();
	bool IsValidData();
	bool Query(const WCHAR* _query);
	bool BindCol(SQLUSMALLINT _columnIndex, SQLSMALLINT _cType, SQLULEN _len, SQLPOINTER _value, SQLLEN* _index);
	
	bool BindParam(const WCHAR* _param, SQLLEN* _outIndex);
	bool BindParam(int* _param, SQLLEN* _outIndex);


private:
	bool BindParam(SQLUSMALLINT _paramIndex, SQLSMALLINT _cType, SQLSMALLINT _sqlType, SQLULEN _len, SQLPOINTER _ptr, SQLLEN* _index);
	void DBErrorHandle(SQLRETURN _return);

private:
	//Statement 상태를 관리하는 핸들
	SQLHSTMT		m_Statement = SQL_NULL_HANDLE;

	SQLUSMALLINT m_bindParam = BINDPARAM_STARTINDEX;
};

