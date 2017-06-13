#include"StdAfx.h"
//#include"YCMySqlDataBaseEngine.h"
#include"listenActiveDlg.h"
//#include"../结构定义/BusinessLayerProtocol.h"
//#include<Windows.h>
//#include<WinDef.h>
//#include<tchar.h>
//#include<wchar.h>
#include"Task.h"
//#include"../Thread/ThreadHead.h"

//#pragma comment(lib, "../库文件/Unicode/Thread.lib")

//实例化
//extern CDBAutoFree FreeDB;

CDBAutoFree CYcDBEngine::m_GlobalFreeDB;
CYcDBEngine *CYcDBEngine::m_pDBInstance=NULL;

CYcDBEngine::CYcDBEngine()
{
	m_bInitSuccess = false;
	m_fdColArray = NULL;
	m_RowRes = NULL;
	memset(m_strColName, 0, TABLE_COL_COUNT*TABLE_COL_NAME_LENGTH);
	memset(&m_MySql, 0, sizeof(MYSQL));
	m_MySql.reconnect = 1;	//断开自动重连
	m_encrypt1 = NULL;
	m_encrypt2 = NULL;
	m_pListen = NULL;
	m_pLocker = NULL;
	m_pLocker = new CSynLocker;
	

}

CYcDBEngine::CYcDBEngine(void *pListen)
{
	m_bInitSuccess = false;
	m_fdColArray = NULL;
	m_RowRes = NULL;
	memset(m_strColName, 0, TABLE_COL_COUNT*TABLE_COL_NAME_LENGTH);
	memset(&m_MySql, 0, sizeof(MYSQL));
	m_MySql.reconnect = 1;	//断开自动重连
	m_encrypt1 = NULL;
	m_encrypt2 = NULL;
	m_pListen = pListen;
	m_pLocker = NULL;
	m_pLocker = new CSynLocker;
}

CYcDBEngine::~CYcDBEngine()
{
	Close();

	if (m_pLocker) delete m_pLocker;
	
}

//创建数据库单例
CYcDBEngine *CYcDBEngine::CreateInstance(void *pListen)
{
	if (CYcDBEngine::m_pDBInstance == NULL)
	{
		CYcDBEngine::m_pDBInstance = new CYcDBEngine(pListen);
	}
	
	return CYcDBEngine::m_pDBInstance;
}

//获取数据库对象指针
CYcDBEngine *CYcDBEngine::GetInstance()
{
	return CYcDBEngine::m_pDBInstance;
}


//初始化
bool CYcDBEngine::Init(void *pListen)
{
	m_fdColArray = NULL;
	m_RowRes = NULL;
	memset(m_strColName, 0, TABLE_COL_COUNT*TABLE_COL_NAME_LENGTH);
	memset(&m_MySql, 0, sizeof(MYSQL));
	
	
	//进程监视
	if (!m_pListen)
		m_pListen = pListen;

	//my_init();
	//mysql_thread_init();
	if (NULL==mysql_init(&m_MySql))
	{
		m_bInitSuccess = false;
	}
	else
	{
		m_bInitSuccess = true;
	}

	//设置自动重连
	//char value = 1;
	//if (!mysql_options(&m_MySql, MYSQL_OPT_RECONNECT, &value))
	//{
	//	((CListenActiveDlg *)m_pListen)->DispMsg("自动重连设置成功！\r\n");
	//}
	//else
	//{
	//	((CListenActiveDlg *)m_pListen)->DispMsg("自动重连设置失败！\r\n");
	//}

	
	//加载加密函数
	if (!m_encrypt1 || !m_encrypt2)
	{
		try{
			CString strFileName = "endll.dll";
			if ((m_hmDll = LoadLibrary(TEXT("endll.dll"))) == NULL)
			{
				if (m_pListen)
				{
					((CListenActiveDlg *)m_pListen)->DispMsg("缺少endll.dll\r\n");
				}
				return FALSE;
			}
				
			m_encrypt1 = (tagencrypt1)::GetProcAddress(m_hmDll, "str_encrypt_reversible");
			m_encrypt2 = (tagencrypt2)::GetProcAddress(m_hmDll, "str_encrypt2_reversible");

			if (m_encrypt1 == NULL || m_encrypt2 == NULL)
			{
				if (m_pListen)
				{
					((CListenActiveDlg *)m_pListen)->DispMsg("缺少加密函数\r\n");
				}
			}
		}
		catch (CException *e){
			e->Delete();
			if (m_pListen)
			{
				((CListenActiveDlg *)m_pListen)->DispMsg("加载Dll失败!\n");
			}

			return false;
		}

		if (m_pListen)
		{
			//((CListenActiveDlg *)m_pListen)->DispMsg("加载Dll成功!!\n");
		}

	}
	
	return m_bInitSuccess;  //连接mysql，数据库
}

//链接
bool CYcDBEngine::Connect(char *strAccountName, char *strPwd, char *strDBName/*默认数据库*/, int port, char *unix_socket/*NULL使用windows命名管道*/, unsigned long client_flag /*通常为0*/, char *strHostName /*默认本地主机*/)
{
	try
	{
		//参数校验
		if (!m_bInitSuccess || NULL == strAccountName || NULL == strPwd || NULL == strHostName) return false;

		//设置自动重连
		CListenActiveDlg * pListen = ((CListenActiveDlg *)m_pListen);

		//((CSynLocker *)m_pLocker)->Lock();

		//返回false则连接失败，返回true则连接成功
		if (!(mysql_real_connect(&m_MySql, strHostName, strAccountName, strPwd, strDBName, port, unix_socket, client_flag))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
		{
			//printf("Error connecting to database:%s\n", mysql_error(&m_MySql));
		//	((CSynLocker *)m_pLocker)->UnLock();
			return false;
		}




		//更改链接超时时间

		//char sqlstring[64] = { 0 };
		//sprintf(sqlstring, "set interactive_timeout=%s", pListen->GetMysqlConnectTimeOut().c_str());
		//if (ExecuteSql(sqlstring, NULL, NULL, NULL, NULL, NULL, m_pListen))
		//{
		//	pListen->DispMsg("修改超时成功！\r\n");
		//	pListen->DispMsg(sqlstring);
		//	pListen->DispMsg("\r\n");
		//}
		//else
		//{
		//	pListen->DispMsg("修改超时失败！\r\n");
		//}
		
		//((CSynLocker *)m_pLocker)->UnLock();
		//添加定时检测断开并自动重连	1小时检测一次
	//	CTaskBase *pReconnect = new CDBReconnectTask(10000, &m_MySql, m_pLocker, m_pListen);
	//	CThreadPool::GetInstance()->AddTask(pReconnect);
	}
	catch (...)
	{
		//((CSynLocker *)m_pLocker)->UnLock();
		return false;
	}


	return true;
}

//关闭
bool CYcDBEngine::Close()
{
	try
	{
		//释放资源
		if (m_RowRes)
		{
			mysql_free_result(m_RowRes);
			m_RowRes = NULL;
		}
		m_bInitSuccess = false;
		mysql_close(&m_MySql);
		mysql_library_end();	//释放未被释放的内存
		CListenActiveDlg * pListen = ((CListenActiveDlg *)m_pListen);

		//mysql_thread_end();
	}
	catch (...)
	{
		if (((CListenActiveDlg *)m_pListen))
			((CListenActiveDlg *)m_pListen)->DispMsg("断开数据库异常！\r\n");
		return false;
	}

	return true;
}

//执行sql语句
bool CYcDBEngine::ExecuteSql(char *strSql, char ****pRetOut, bool *pbExistResOut, int *pResRows, int *pColumn, int *pErrNO, void *pObject/*用于打印显示*/)
{
	//参数校验
	if (m_bInitSuccess==false||strSql == NULL) return false;

	//执行sql语句
	//mysql_query(&m_MySql, "set names gbk");
	//((CSynLocker *)m_pLocker)->Lock();
	
	int iAbornormalPara = 0;
	try
	{
		//异常1
		iAbornormalPara++;

		//执行SQL语句
		//返回0 查询成功，返回1查询失败
		while (mysql_query(&m_MySql, strSql))
		{
			//带出错误编码
			if (pErrNO)
			{
				*pErrNO = mysql_errno(&m_MySql);
			}
			if (pObject)
			{
				((CListenActiveDlg *)pObject)->DispMsg(" CYcDBEngine::ExecuteSql mysql_query failed:");
				((CListenActiveDlg *)pObject)->DispMsg(mysql_error(&m_MySql));
				((CListenActiveDlg *)pObject)->DispMsg("\r\n");
			}

			if (*pErrNO == 2006)
			{
				Close();
				Init(pObject);
				if(Connect((char *)((CListenActiveDlg *)pObject)->m_strMysqlAdminAccount.c_str(), (char *)((CListenActiveDlg *)pObject)->m_strMysqlAdminPwd.c_str()
					, (char *)((CListenActiveDlg *)pObject)->m_strMysqlDBName.c_str()))	
				{
					((CListenActiveDlg *)pObject)->DispMsg("链接成功!\r\n");
				}
				else
				{
					//((CSynLocker *)m_pLocker)->UnLock();
					return false;
				}
			}
			else
			{
				//((CSynLocker *)m_pLocker)->UnLock();
				return false;
			}
			//printf("Query failed (%s)\n", mysql_error(&m_MySql));
			//return false;
		}

		//异常2
		iAbornormalPara++;

		//获取结果集
		m_RowRes = mysql_store_result(&m_MySql);

		if (!m_RowRes)
		{
			//带出错误编码
			if (pErrNO)
			{
				*pErrNO = mysql_errno(&m_MySql);
			}
			if (pObject)
			{
				((CListenActiveDlg *)pObject)->DispMsg(mysql_error(&m_MySql));
				//((CListenActiveDlg *)pObject)->DispMsg("\r\n");
			}
			if (pbExistResOut)
				*pbExistResOut = false;
		}
		else
		{
			if (pbExistResOut)
				*pbExistResOut = true;
		}


	//	char strm_RowRes[50] = { 0 };
//		sprintf(strm_RowRes, "mysql_store_result m_RowRes :%s\r\n", m_RowRes ? "有结果集" : "无结果集");
	//	if (pObject)
		//	((CListenActiveDlg *)pObject)->DispMsg(strm_RowRes);

		if (!m_RowRes)
		{
			//((CSynLocker *)m_pLocker)->UnLock();
			return true;		//无结果集sql
		}

		//异常3
		iAbornormalPara++;

		//打印数据行数	获取行数
		int iResLine = mysql_affected_rows(&m_MySql);	//sql执行所影响的行数不包括字段名
		if (iResLine <= 0)
		{
			//带出错误编码
			if (pErrNO)
			{
				*pErrNO = mysql_errno(&m_MySql);
			}

			if (pObject)
			{
				((CListenActiveDlg *)pObject)->DispMsg(mysql_error(&m_MySql));
				//((CListenActiveDlg *)pObject)->DispMsg("\r\n");
			}
		}

		//有结果集输出
		if (iResLine > 0 && pRetOut)
		{			
			(*pRetOut) = new char **[iResLine];
			
			if (*pRetOut)
			{
				memset(*pRetOut, 0, iResLine);
			}
		}

		//带出行数
		if (pResRows)
			*pResRows = iResLine;

	//	char striResLine[50] = { 0 };
	//	sprintf(striResLine, "mysql_affected_rows ret iResLine:%d\r\n", iResLine);
	//	if (pObject)
		//	((CListenActiveDlg *)pObject)->DispMsg(striResLine);

		//异常4
		iAbornormalPara++;

		if (iResLine < 0)
		{
			//释放资源
			if (m_RowRes)
			{
				mysql_free_result(m_RowRes);
				m_RowRes = NULL;
			}

			//((CSynLocker *)m_pLocker)->UnLock();
			//if (pObject)
		//		((CListenActiveDlg *)pObject)->DispMsg("mysql_affected_rows Below Zero\r\n");
			return false;		//执行失败
		}


		//异常5
		iAbornormalPara++;
		//printf("number of dataline returned: %d\n", iResLine);

		//有结果集才输出结果 iResLine>0 或者 m_RowRes不为NULL
		if (m_RowRes)
		{
			//获取字段名
			memset(m_strColName, 0, TABLE_COL_COUNT*TABLE_COL_NAME_LENGTH);	//初始化字段区
			int iColNameLength = 0;
			for (int i = 0; m_fdColArray = mysql_fetch_field(m_RowRes); i++)  //获取字段名
			{
				if (i <= TABLE_COL_COUNT - 1)
				{
					iColNameLength = strlen(m_fdColArray->name);
					memmove(m_strColName[i]
						, m_fdColArray->name
						, (iColNameLength > (TABLE_COL_NAME_LENGTH - 1)) ? (TABLE_COL_NAME_LENGTH - 1) : iColNameLength);
				}
			}

			if (!m_fdColArray)
			{
				//带出错误编码
				if (pErrNO)
				{
					*pErrNO = mysql_errno(&m_MySql);
				}

				if (pObject)
				{
					((CListenActiveDlg *)pObject)->DispMsg(mysql_error(&m_MySql));
					//((CListenActiveDlg *)pObject)->DispMsg("\r\n");
				}
			}

			//异常6
			iAbornormalPara++;

			// 获取列数
			int iResColCount = mysql_num_fields(m_RowRes);

			bool bRetGatherCreate = false;
			if (pRetOut)
			{
				if (*pRetOut)
				{
					for (int j = 0; j < iResLine; j++)
					{
						(*pRetOut)[j] = new char* [iResColCount];
						if ((*pRetOut)[j])
						{
							memset((*pRetOut)[j], 0, iResColCount);
						}
						bRetGatherCreate = true;
					}

				}
			}

			//带出列数
			if (pColumn) *pColumn = iResColCount;


			//异常7
			iAbornormalPara++;

		//	char strResColCount[50] = { 0 };
		//	sprintf(strResColCount, "mysql_num_fields ret num:%d\r\n", iResColCount);
		//	if (pObject)
		//		((CListenActiveDlg *)pObject)->DispMsg(strResColCount);

			//异常8
			iAbornormalPara++;

			//for (int i = 0; i < iResColCount; i++)  //打印字段名
		//	{
		//		if (pObject)
			//	{
		//			((CListenActiveDlg *)pObject)->DispMsg(m_strColName[i]);
		//			((CListenActiveDlg *)pObject)->DispMsg("\t\t");
		//		}
		//	}

		//	if (pObject)
		//		((CListenActiveDlg *)pObject)->DispMsg("\r\n");

			//异常9
			iAbornormalPara++;

			//结果集指针
			int m = 0;
			char ***pTmp3 = NULL;
			if (bRetGatherCreate) pTmp3 = *pRetOut;
			

			//获取下一行 一行一行获取，结束为NULL
			while (m_Column = mysql_fetch_row(m_RowRes))
			{
				char **pTmp2 = NULL;
				if (pTmp3)	pTmp2 = *(pTmp3 + m);

				for (int i = 0; i < iResColCount; i++)
				{
					if (pTmp2)
					{
						pTmp2[i] = new char[strlen(m_Column[i]) + 1];
						if (pTmp2[i])
						{
							memset(pTmp2[i], 0, strlen(m_Column[i]) + 1);
							memmove(pTmp2[i], m_Column[i], strlen(m_Column[i]));
						}
					}
						
					//if (pObject)
				//	{
				//		if (pTmp2)
				//		{
					//		if (pTmp2[i])
				//			{
								//((CListenActiveDlg *)pObject)->DispMsg(pTmp2[i]);
				//			}
				//		}
						
				//		((CListenActiveDlg *)pObject)->DispMsg("\t\t");
				//	}

					//获取各列数据	m_Column[i] 全为字符串
					//m_Column[i];
				}

				//二维表偏移
				m++;

			//	if (pObject)
			//		((CListenActiveDlg *)pObject)->DispMsg("\r\n");
			}

		}
	
		//异常10
		iAbornormalPara++;

		//释放资源
		if (m_RowRes)
		{
			mysql_free_result(m_RowRes);
			m_RowRes = NULL;
		}

	//	if (pObject)
	//		((CListenActiveDlg *)pObject)->DispMsg("Execute sql end!\r\n");

		//异常11
		iAbornormalPara++;
	}
	catch (...)
	{
		//带出错误编码
		if (pErrNO)
		{
			*pErrNO = mysql_errno(&m_MySql);
		}

		char error[50] = { 0 };
		sprintf(error, "CYcDBEngine::Execute sql Abnormal error:%d\r\n", iAbornormalPara);
		if (pObject)
		{
			((CListenActiveDlg *)pObject)->DispMsg(error);
			((CListenActiveDlg *)pObject)->DispMsg(mysql_error(&m_MySql));
			((CListenActiveDlg *)pObject)->DispMsg("\r\n");
		}
		
		//((CSynLocker *)m_pLocker)->UnLock();
		return false;
	}
	
	//((CSynLocker *)m_pLocker)->UnLock();
	return true;

}

//查询账户
bool CYcDBEngine::QueryUserAccount(char *strQuerySql, void *pObject /*用于打印显示*/)
{
	//sprintf(query, "select * from user"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以
	//mysql_query(&m_MySql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码

	return ExecuteSql(strQuerySql,NULL, NULL, NULL, NULL,NULL,  pObject);

}

//注册账户
bool CYcDBEngine::RegisterAccount(char *useraccount, char *pwd, char *email, int *iErrCode, void *pListenDlg)
{
	int iAbortNormal = 0;

	//参数校验
	if (!useraccount || !pwd || !email)	return false;

	//显示
	CListenActiveDlg * pListen = ((CListenActiveDlg *)m_pListen);
	if (!pListen) return false;

	try
	{
		//初始化数据库
		if (!Init(m_pListen)) return false;

		//异常1
		iAbortNormal++;

		//显示：
		//pListen->DispMsg("userid:");
	//	pListen->DispMsg(useraccount);
	//	pListen->DispMsg(" pwd:");
	//	pListen->DispMsg(pwd);
	//	pListen->DispMsg(" email:");
	//	pListen->DispMsg(email);
	//	pListen->DispMsg("\r\n");

		//异常2
		iAbortNormal++;

		//加密后插入account
		char key[4096] = { 0 };
		memset(key, 0, 4096);
		strcpy(key, (m_encrypt1)(pwd, useraccount));

	//	char rizhi[100] = { 0 };
	//	sprintf(rizhi, "m_encrypt1 key1 len:%d\r\n", strlen(key));
	//	pListen->DispMsg(rizhi);
		key[4095] = 0;

		//异常3
		iAbortNormal++;
		char key2[4096] = { 0 };
		strcpy(key2, (m_encrypt2)((char *)key, (char *)email));

		//memset(rizhi, 0, 100);
	//	sprintf(rizhi, "m_encrypt2 key2 len:%d\r\n", strlen(key2));
	//	pListen->DispMsg(rizhi);
		key2[4095] = 0;



		//异常4
		iAbortNormal++;
		//pListen->DispMsg(key2);

		//异常5
		iAbortNormal++;

		char keyCache[4096] = { 0 };
		memmove(keyCache, key2, 4096);
		keyCache[4095] = 0;
		//CString strKeyd ;
		//strKeyd += key2;

		//异常6
		iAbortNormal++;

		//	strSql = "insert into account(userid,password,fullname,email,account_create,permission_level,pts_balance,pts_method,pts_start,next_pts_method,suspend_end,first_login,last_login, login_count) values('"
		//		+ activeuser.userid + "','" + strKey + "','" + activeuser.userid + "','"
		//		+ activeuser.email + "',NOW(), 0, 0, 'LIFE', now(), 'LIFE', default, default, default, default)";

		char strSql[1024] = { 0 };
		sprintf(strSql, "insert into account(userid, password, fullname, email, account_create, unused_dur_end,pts_method,pts_start,next_pts_method)values ('%s', '%s', '%s','%s', now(), '2199-01-01 00:00:00', 'LIFE', now(), 'LIFE');"
			, useraccount, keyCache, useraccount, email);  //可以想办法实现手动在控制台手动输入指令

		//异常7
		iAbortNormal++;

		bool bResult = false;
		int iRowResult = 0, iErr = 0;
		char strResult[100] = { 0 };
		bool bExeResult = false;


		//链接数据库
		if (!Connect((char *)pListen->m_strMysqlAdminAccount.c_str(), (char *)pListen->m_strMysqlAdminPwd.c_str(), (char *)pListen->m_strMysqlDBName.c_str())) return false;
		//执行操作
		
		
		//((CSynLocker *)m_pLocker)->Lock();
		bExeResult = ExecuteSql(strSql, NULL, &bResult, &iRowResult, NULL, &iErr, m_pListen);
		//((CSynLocker *)m_pLocker)->UnLock();

		//关闭数据库
		Close();

		if (bExeResult)
		{
			//memset(strResult, 0, 100);
			//sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "有" : "没有", iRowResult, iErr);
			//pListen->DispMsg(strResult);

			//pListen->DispMsg("注册成功!\r\n");
		}
		else
		{
			*iErrCode = iErr;
			//memset(strResult, 0, 100);
			//sprintf(strResult, "CYcDBEngine::RegisterAccount（uid=%s, pwd=%s,email=%s,） iErr %d\r\n", useraccount, pwd, email, iErr);
			//pListen->DispMsg(strResult);
			//pListen->DispMsg("注册失败!\r\n");
			return false;
		}

		
			

		//memset(strResult, 0, 100);
		//sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "有" : "没有", iRowResult, iErr);
		//pListen->DispMsg(strResult);

		//异常8
		iAbortNormal++;
	}
	catch (...)
	{
		char error[50] = { 0 };
		sprintf(error, " CYcDBEngine::RegisterAccount  Abnormal error:%d\r\n", iAbortNormal);
		pListen->DispMsg(error);

		return false;
	}

	return true;
}

//修改密码
bool CYcDBEngine::ModifyPwd(char *useraccount, char *oldpwd, char *newpwd, int *iErrCode, void *pListenDlg)
{
	//校验参数
	if (!useraccount || !oldpwd || !newpwd) return false;

	//打印
	CListenActiveDlg *pListen = (CListenActiveDlg *)pListenDlg;

	char ***pRetOut = NULL;		//结果集
	char strSQL[4096] = { 0 };
	bool bHaveResult = false;
	int iRowNum = 0, iColumnNum=0;
	sprintf(strSQL, "select userid, password, email from account where userid='%s' limit 1", useraccount);

	bool bExeResult = false;

	//初始化数据库
	if (!Init(m_pListen)) return false;
	//链接数据库
	if (!Connect((char *)pListen->m_strMysqlAdminAccount.c_str(), (char *)pListen->m_strMysqlAdminPwd.c_str(), (char *)pListen->m_strMysqlDBName.c_str())) return false;
	//执行操作

//	((CSynLocker *)m_pLocker)->Lock();
	bExeResult = ExecuteSql(strSQL, &pRetOut, &bHaveResult, &iRowNum, &iColumnNum, NULL, pListenDlg);
	//((CSynLocker *)m_pLocker)->UnLock();



	//查询账户得到userid pwd email
	if (!bExeResult)
	{
		ReleaseResultOutGather(&pRetOut, iRowNum, iColumnNum);
		*iErrCode = USER_ID_WRONG;

		//关闭数据库
		Close();

		//if (pListen)
		//	pListen->DispMsg("ModifyPwd 账户不存在 或执行出错\r\n");
		return false;
	}

	//定义
	char userid[ACCOUNT_LEN] = { 0 };
	char email[EMAIL_LEN] = { 0 };
	char pwd[2048] = { 0 };

	//有结果集 取结果
	if (bHaveResult&&iRowNum > 0 && iColumnNum>0)
	{
		for (int n = 0; n < iColumnNum; n++)
		{
			switch (n)
			{
			case 0:
				{
					  strcpy(userid, (pRetOut)[0][n]);
					  //if (pListen) pListen->DispMsg("ModifyPwd 账户:");
					 // if (pListen) pListen->DispMsg(userid);
					 // if (pListen) pListen->DispMsg("\t");
					break;
				}
			case 1:
			{
					  strcpy(pwd, (pRetOut)[0][n]);
					 // if (pListen) pListen->DispMsg("ModifyPwd 密码:");
					 // if (pListen) pListen->DispMsg(pwd);
					 // if (pListen) pListen->DispMsg("\t");
					  break;
			}
			case 2:
			{
					  strcpy(email, (pRetOut)[0][n]);
					 // if (pListen) pListen->DispMsg("ModifyPwd email:");
					 // if (pListen) pListen->DispMsg(email);
					 // if (pListen) pListen->DispMsg("\t\r\n");
					  break;
			}
			}
				
		}
	}
	else
	{
		ReleaseResultOutGather(&pRetOut, iRowNum, iColumnNum);
		*iErrCode = USER_ID_WRONG;

		//关闭数据库
		Close();

		//if (pListen)
		//	pListen->DispMsg("ModifyPwd 账户不存在\r\n");
		return false;
	}

	//加密oldpwd userid email 
	//1次加密
	char key[4096] = { 0 };
	memset(key, 0, 4096);
	strcpy(key, (m_encrypt1)(oldpwd, useraccount));
	key[4095] = 0;

	//2次加密
	char key2[4096] = { 0 };
	memset(key2, 0, 4096);
	strcpy(key2, (m_encrypt2)((char *)key, (char *)email));
	key2[4095] = 0;

	//加密结果与pwd比较若相同则可修改密码，若不相同则返回原密码输入错误
	if (strcmp(key2, pwd) != 0)
	{
		//原密码错误
		*iErrCode = OLD_PWD_WRONG;

		//关闭数据库
		Close();
		//if (pListen)
		//	pListen->DispMsg("ModifyPwd 原密码输入错误\r\n");
		return false;
	}
	else
	{
		//原密码正确
		*iErrCode = SUCCESS_MOD;

		//执行更改密码
		memset(key, 0, 4096);
		strcpy(key, (m_encrypt1)(newpwd, useraccount));
		key[4095] = 0;

		memset(key2, 0, 4096);
		strcpy(key2, (m_encrypt2)((char *)newpwd, (char *)email));
		key2[4095] = 0;

		memset(strSQL, 0, 4096);
		sprintf(strSQL, "update account set password='%s' where userid='%s'", key2, useraccount);

		bExeResult = false;
		//((CSynLocker *)m_pLocker)->Lock();
		bExeResult=ExecuteSql(strSQL, NULL, NULL, NULL, NULL, iErrCode, pListenDlg);
		//((CSynLocker *)m_pLocker)->UnLock();

		//更新密码
		if (bExeResult)
		{
			*iErrCode = SUCCESS_MOD;
			//if (pListen)
			//	pListen->DispMsg("ModifyPwd 修改成功\r\n");
		}
		else
		{
			*iErrCode = UPDATE_ERROR;
			//if (pListen)
			//	pListen->DispMsg("ModifyPwd 更新失败\r\n");
			//关闭数据库
			Close();
			return false;
		}
	}

	//关闭数据库
	Close();

	//释放空间
	ReleaseResultOutGather(&pRetOut, iRowNum, iColumnNum);

	return true; 
}

bool CYcDBEngine::ReleaseResultOutGather(char ****pRetOut, int iYLines, int iXColumn)
{
	if (!pRetOut) return false;

	char ***pRet3 = NULL;
	if (*pRetOut) pRet3 = *pRetOut;
	char **pRet2 = NULL;
	char *pRet = NULL;
	for (int i = 0; i < iYLines; i++)
	{
		if (pRet3) pRet2 = *(pRet3 + i);
		
		for (int j = 0; j < iXColumn; j++)
		{
			if (pRet2) pRet = *(pRet2 + j);

			if (pRet) delete [] pRet;
			pRet = NULL;
		}

		if (pRet2) delete [] pRet2;
		pRet2 = NULL;
	}

	if (pRet3) delete [] pRet3;
	pRet3 = NULL;

	return true;
}