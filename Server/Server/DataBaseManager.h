#pragma once
#include <windows.h>
#include <sqlext.h>
#include "DBObject.h"

#define DBOBJECT_COUNT 5

class DataBaseManager
{
public:
	DataBaseManager();
	~DataBaseManager();

public:
	static DataBaseManager* GetInstance()
	{
		static DataBaseManager instance;
		return &instance;
	}

	void Clear();
	bool Connect();

	DBObject* PopDBObject();
	void PushDBObject(DBObject* _dbObject);

private:
	mutex					m_lockSession;

	// SQL은 내가 생각하는 그 SQL이고 H는 핸들을 말한다.
	
	// DataBaseConnection 약자...
	// Connection을 하기위한 핸들
	SQLHDBC			m_DBConnect = SQL_NULL_HANDLE;

	// 환경을 담당하는 핸들?
	SQLHENV			m_Enviroment = SQL_NULL_HANDLE;


	queue<DBObject*>	m_queDBObject;
};

