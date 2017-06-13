#ifndef YC_MYSQL_DATABASE_ENGINE_H_
#define YC_MYSQL_DATABASE_ENGINE_H_

#pragma once

//#include <WinSock.h>  //一定要包含这个，或者winsock2.h
#include<WinSock2.h>
#include "mysql.h"    //引入mysql头文件(一种方式是在vc目录里面设置，一种是文件夹拷到工程目录，然后这样包含)
#include <Windows.h>
//包含附加依赖项，也可以在工程--属性里面设置
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"./lib/libmysql.lib")


//加解密
typedef char *(_stdcall *tagencrypt1)(char *, char *);//第一次加密
typedef char *(_stdcall *tagencrypt2)(char *key, char *mail);//第二次加密

#define TABLE_COL_COUNT		128			//最多128个字段
#define TABLE_COL_NAME_LENGTH	256		//字段名长度最多256
class CYcDBEngine
{
	//对象初始化
public:
	//创建数据库单例
	static CYcDBEngine *CreateInstance();		
	//获取数据库对象指针
	static CYcDBEngine *GetInstance();	
	
	//数据库操作
public:
	//初始化
	bool Init(void *pListen);
	//链接
	bool Connect(char *strAccountName, char *strPwd, char *strDBName = NULL/*默认数据库*/, int port = 3308, char *unix_socket = NULL/*NULL使用windows命名管道*/, unsigned long client_flag = 0/*通常为0*/, char *strHostName="localhost"/*默认本地主机*/);
	//关闭
	bool Close();
	//执行sql语句
	bool ExecuteSql(char *strSql,char ****pRetOut, bool *pbExistResOut, int *pResRows, int *pColumn, int *pErrNO, void *pObject = NULL/*用于打印显示*/);

	//扩展功能
public:
	//查询账户
	bool QueryUserAccount(char *strQuerySql = "select *from account",  void *pObject=NULL/*用于打印显示*/);
	//注册账户
	bool RegisterAccount(char *useraccount, char *pwd, char *email, int *iErrCode, void *pListenDlg);
	//修改密码
	bool ModifyPwd(char *useraccount, char *oldpwd, char *newpwd, int *iErrCode, void *pListenDlg);

	//辅助函数
public:
	//释放结果集空间
	bool ReleaseResultOutGather(char ****pRetOut, int iYLines, int iXColumn);
	//获取锁实例
	void *GetLocker(){ return m_pLocker; }

	//void FreeConnect();
	//bool QueryDatabase1();  //查询1
	//bool QueryDatabase2();  //查询2
	//bool InsertData();
	//bool ModifyData();
	//bool DeleteData();

public:
	//加密存储
	tagencrypt1 m_encrypt1;
	tagencrypt2 m_encrypt2;
	void *		m_pListen;
	HINSTANCE m_hmDll;

	//数据库成员变量
protected:
	void *						m_pLocker;				//线程同步
	MYSQL						m_MySql;				//mysql连接	mysql
	MYSQL_FIELD *				m_fdColArray;			//字段列数组	fd
	char						m_strColName[TABLE_COL_COUNT][TABLE_COL_NAME_LENGTH];		//存字段名二维数组	field
	MYSQL_RES *					m_RowRes;				//这个结构代表返回行的一个查询结果集	res
	MYSQL_ROW					m_Column;				//一个行数据的类型安全(type-safe)的表示，表示数据行的列	column
	bool						m_bInitSuccess;			//初始化成功



	//内部
private:
	friend class CDBAutoFree;
	static CDBAutoFree m_GlobalFreeDB;
	CYcDBEngine();
	virtual ~CYcDBEngine();

private:
	static CYcDBEngine *m_pDBInstance;
};

//用于自动释放单例
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
