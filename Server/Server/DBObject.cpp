#include "pch.h"
#include "DBObject.h"

DBObject::DBObject(SQLHDBC _connect)
{
    SQLRETURN retcode;
    retcode = SQLAllocHandle(SQL_HANDLE_STMT, _connect, &m_Statement);

    //if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
        //cout << "DBObject Init Fail" << endl;
}

DBObject::~DBObject()
{
    if (m_Statement != SQL_NULL_HANDLE)
        SQLFreeHandle(SQL_HANDLE_STMT, m_Statement);
}

void DBObject::SQLStmtFree()
{
    ::SQLFreeStmt(m_Statement, SQL_UNBIND);
    ::SQLFreeStmt(m_Statement, SQL_RESET_PARAMS);
    ::SQLFreeStmt(m_Statement, SQL_CLOSE);

    m_bindParam = BINDPARAM_STARTINDEX;
}
bool DBObject::Query(const WCHAR* _query)
{
    SQLRETURN ret = ::SQLExecDirectW(m_Statement, (SQLWCHAR*)_query, SQL_NTSL);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        return true;

    DBErrorHandle(ret);

    return false;
}

bool DBObject::BindCol(SQLUSMALLINT _columnIndex, SQLSMALLINT _cType, SQLULEN _len, SQLPOINTER _value, SQLLEN* _index)
{
    // SQLBindCol �� ���ø����̼� ������ ���۸� ��� ������ ���� ���ε��մϴ�.
    // DB���� �Ѱ��� �����͸� ���� ������ �����ϴ��Լ�?
    SQLRETURN ret = ::SQLBindCol(m_Statement, _columnIndex, _cType, _value, _len, _index);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        return false;


    return true;
}

bool DBObject::BindParam(const WCHAR* _param, SQLLEN* _outIndex)
{
    // \n�� ���� �߰��ؼ� 1++, wchar�̱⶧���� * 2
    SQLULEN userIDSize = static_cast<SQLULEN>((::wcslen(_param) + 1) * 2);
    *_outIndex = SQL_NTSL;
    
    return BindParam(m_bindParam++, SQL_C_WCHAR, SQL_WVARCHAR, userIDSize, (SQLPOINTER)_param, _outIndex);
}

bool DBObject::BindParam(int* _param, SQLLEN* _outIndex)
{
    return BindParam(m_bindParam++, SQL_C_LONG, SQL_INTEGER, sizeof(int), _param, _outIndex);
}

bool DBObject::BindParam(SQLUSMALLINT _paramIndex, SQLSMALLINT _cType, SQLSMALLINT _sqlType, SQLULEN _len, SQLPOINTER _ptr, SQLLEN* _index)
{
    // SQLBindParameter�� SQL ���� �Ű� ���� ��Ŀ�� ���۸� ���ε��մϴ�.
    // SQLBindParameter �� �⺻ ����̹��� �����ڵ� �����͸� �������� �ʴ���
    // �����ڵ� C ������ ���Ŀ� ���� ���ε��� �����մϴ�.

    SQLRETURN ret = ::SQLBindParameter(m_Statement, _paramIndex, SQL_PARAM_INPUT, _cType, _sqlType, _len, 0, _ptr, 0, _index);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        return false;

    return true;
}

void DBObject::DBErrorHandle(SQLRETURN _return)
{
    if (_return == SQL_SUCCESS)
        return;

    SQLSMALLINT index = 1;
    SQLWCHAR sqlState[MAX_PATH] = { 0 };
    SQLINTEGER nativeErr = 0;
    SQLWCHAR errMsg[MAX_PATH] = { 0 };
    SQLSMALLINT msgLen = 0;
    SQLRETURN errorRet = 0;

    while (true)
    {
        errorRet = ::SQLGetDiagRecW(
            SQL_HANDLE_STMT,
            m_Statement,
            index,
            sqlState,
            OUT & nativeErr,
            errMsg,
            _countof(errMsg),
            OUT & msgLen
        );

        if (errorRet == SQL_NO_DATA)
            break;

        if (errorRet != SQL_SUCCESS && errorRet != SQL_SUCCESS_WITH_INFO)
            break;

        // TODO : Log
        //wcout << errMsg << endl;

        index++;
    }
}

bool DBObject::IsValidData()
{
    //SQLFetch �� ��� ���տ��� �������� ���� �� ������ �������� ��� ���ε��� ���� ���� �����͸� ��ȯ�մϴ�.
    SQLRETURN ret = ::SQLFetch(m_Statement);

    switch (ret)
    {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        return true;
    case SQL_NO_DATA:
        return false;
    case SQL_ERROR:
        CRASH("DB-ERROR");
        return false;
    default:
        return true;
    }
}
