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

	// SQL�� ���� �����ϴ� �� SQL�̰� H�� �ڵ��� ���Ѵ�.
	
	// DataBaseConnection ����...
	// Connection�� �ϱ����� �ڵ�
	SQLHDBC			m_DBConnect = SQL_NULL_HANDLE;

	// ȯ���� ����ϴ� �ڵ�?
	SQLHENV			m_Enviroment = SQL_NULL_HANDLE;


	queue<DBObject*>	m_queDBObject;
};

