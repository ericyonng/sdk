// MySqlDataBase.h: interface for the CMySqlDataBase class.
//
//////////////////////////////////////////////////////////////////////
#include "AFXDB.h"
#import "c:\program files\common files\system\ado\msado15.dll" \
	no_namespace \
rename ( "EOF", "adoEOF" ) 
#if !defined(AFX_MYSQLDATABASE_H__686E6EF3_3DDB_413F_9DFB_ED560F685AA4__INCLUDED_)
#define AFX_MYSQLDATABASE_H__686E6EF3_3DDB_413F_9DFB_ED560F685AA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySqlDataBase  
{
public:
	CString connStr;
	void setDbName(CString strCon);
	void PrintComError(_com_error &e);
//	CDatabase m_db;
	_ConnectionPtr m_db;
	int excuteSql(CString strSql);
	CStringArray* excuteQuery(CString strSql,long* lngRowCount);
	bool close();
	bool connectDB();
	CMySqlDataBase();
	virtual ~CMySqlDataBase();

};

#endif // !defined(AFX_MYSQLDATABASE_H__686E6EF3_3DDB_413F_9DFB_ED560F685AA4__INCLUDED_)
