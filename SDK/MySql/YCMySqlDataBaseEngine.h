#ifndef YC_MYSQL_DATABASE_ENGINE_H_
#define YC_MYSQL_DATABASE_ENGINE_H_

#pragma once

//#include <WinSock.h>  //һ��Ҫ�������������winsock2.h
#include<WinSock2.h>
#include "mysql.h"    //����mysqlͷ�ļ�(һ�ַ�ʽ����vcĿ¼�������ã�һ�����ļ��п�������Ŀ¼��Ȼ����������)
#include <Windows.h>
//�������������Ҳ�����ڹ���--������������
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"./lib/libmysql.lib")


//�ӽ���
typedef char *(_stdcall *tagencrypt1)(char *, char *);//��һ�μ���
typedef char *(_stdcall *tagencrypt2)(char *key, char *mail);//�ڶ��μ���

#define TABLE_COL_COUNT		128			//���128���ֶ�
#define TABLE_COL_NAME_LENGTH	256		//�ֶ����������256
class CYcDBEngine
{
	//�����ʼ��
public:
	//�������ݿⵥ��
	static CYcDBEngine *CreateInstance();		
	//��ȡ���ݿ����ָ��
	static CYcDBEngine *GetInstance();	
	
	//���ݿ����
public:
	//��ʼ��
	bool Init(void *pListen);
	//����
	bool Connect(char *strAccountName, char *strPwd, char *strDBName = NULL/*Ĭ�����ݿ�*/, int port = 3308, char *unix_socket = NULL/*NULLʹ��windows�����ܵ�*/, unsigned long client_flag = 0/*ͨ��Ϊ0*/, char *strHostName="localhost"/*Ĭ�ϱ�������*/);
	//�ر�
	bool Close();
	//ִ��sql���
	bool ExecuteSql(char *strSql,char ****pRetOut, bool *pbExistResOut, int *pResRows, int *pColumn, int *pErrNO, void *pObject = NULL/*���ڴ�ӡ��ʾ*/);

	//��չ����
public:
	//��ѯ�˻�
	bool QueryUserAccount(char *strQuerySql = "select *from account",  void *pObject=NULL/*���ڴ�ӡ��ʾ*/);
	//ע���˻�
	bool RegisterAccount(char *useraccount, char *pwd, char *email, int *iErrCode, void *pListenDlg);
	//�޸�����
	bool ModifyPwd(char *useraccount, char *oldpwd, char *newpwd, int *iErrCode, void *pListenDlg);

	//��������
public:
	//�ͷŽ�����ռ�
	bool ReleaseResultOutGather(char ****pRetOut, int iYLines, int iXColumn);
	//��ȡ��ʵ��
	void *GetLocker(){ return m_pLocker; }

	//void FreeConnect();
	//bool QueryDatabase1();  //��ѯ1
	//bool QueryDatabase2();  //��ѯ2
	//bool InsertData();
	//bool ModifyData();
	//bool DeleteData();

public:
	//���ܴ洢
	tagencrypt1 m_encrypt1;
	tagencrypt2 m_encrypt2;
	void *		m_pListen;
	HINSTANCE m_hmDll;

	//���ݿ��Ա����
protected:
	void *						m_pLocker;				//�߳�ͬ��
	MYSQL						m_MySql;				//mysql����	mysql
	MYSQL_FIELD *				m_fdColArray;			//�ֶ�������	fd
	char						m_strColName[TABLE_COL_COUNT][TABLE_COL_NAME_LENGTH];		//���ֶ�����ά����	field
	MYSQL_RES *					m_RowRes;				//����ṹ�������е�һ����ѯ�����	res
	MYSQL_ROW					m_Column;				//һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���	column
	bool						m_bInitSuccess;			//��ʼ���ɹ�



	//�ڲ�
private:
	friend class CDBAutoFree;
	static CDBAutoFree m_GlobalFreeDB;
	CYcDBEngine();
	virtual ~CYcDBEngine();

private:
	static CYcDBEngine *m_pDBInstance;
};

//�����Զ��ͷŵ���
class CDBAutoFree
{
public:
	CDBAutoFree(){}
	~CDBAutoFree()
	{
		if (CYcDBEngine::m_pDBInstance)
			delete CYcDBEngine::m_pDBInstance;
		
		CYcDBEngine::m_pDBInstance = NULL;
	}
private:
	//static CDBAutoFree GlobalFreeDB;
};




#endif // !YC_MYSQL_DATABASE_ENGINE_H_
