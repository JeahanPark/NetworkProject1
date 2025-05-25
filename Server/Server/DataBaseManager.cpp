#include "pch.h"
#include "DataBaseManager.h"


DataBaseManager::DataBaseManager()
{
}

DataBaseManager::~DataBaseManager()
{
    Clear();
}

void DataBaseManager::Clear()
{
    while (!m_queDBObject.empty())
    {
        delete m_queDBObject.front();
        m_queDBObject.pop();
    }
    if (m_DBConnect != SQL_NULL_HANDLE)
    {
        SQLDisconnect(m_DBConnect);
        SQLFreeHandle(SQL_HANDLE_DBC, m_DBConnect);
    }

    if(m_Enviroment != SQL_NULL_HANDLE)
        SQLFreeHandle(SQL_HANDLE_ENV, m_Enviroment);
    
}

bool DataBaseManager::Connect()
{
    // SQL은 내가 생각하는 그 SQL이고 H는 핸들을 말한다.

    // SQL 함수 결과
    SQLRETURN retcode;

    // Allocate environment handle  
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_Enviroment);

    // Set the ODBC version environment attribute  
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLSetEnvAttr(m_Enviroment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
        {
            retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_Enviroment, &m_DBConnect);

            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
            {
                retcode = SQLSetConnectAttr(m_DBConnect, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
                {
                    HWND desktopHandle = GetDesktopWindow();   // desktop's window handle  
                    WCHAR stringBuffer[256] = { 0 };
                    ::wcscpy_s(stringBuffer, L"Driver={SQL Server};Server=Jeahan\\SQLEXPRESS;Database=GameServer;Trusted_Connection=Yes;");

                    SQLWCHAR OutConnStr[255];
                    SQLSMALLINT OutConnStrLen;


                    retcode = SQLDriverConnect( // SQL_NULL_HDBC  
                        m_DBConnect,
                        desktopHandle,
                        (SQLWCHAR*)stringBuffer,
                        _countof(stringBuffer),
                        OutConnStr,
                        255,
                        &OutConnStrLen,
                        SQL_DRIVER_PROMPT);

                    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
                    {
                        for (int i = 0; i < 1; ++i)
                        {
                            DBObject* dbObject = new DBObject(m_DBConnect);
                            m_queDBObject.push(dbObject);
                        }
                        return true;
                    }
                }
            }
            
        }
    }
    return false;
}

DBObject* DataBaseManager::PopDBObject()
{
    UniqueLock lock(m_lockSession);

    condition_variable cv;
    cv.wait(lock, [&]() {return !m_queDBObject.empty(); });

    DBObject* dbObject = m_queDBObject.front();
    m_queDBObject.pop();

    dbObject->SQLStmtFree();

    return dbObject;
}

void DataBaseManager::PushDBObject(DBObject* _dbObject)
{
    if (_dbObject == nullptr)
        CRASH("PushDBObject nullptr")

    LockGuard lock(m_lockSession);
    m_queDBObject.push(_dbObject);
}