#include"StdAfx.h"
//#include"YCMySqlDataBaseEngine.h"
#include"listenActiveDlg.h"
//#include"../�ṹ����/BusinessLayerProtocol.h"
//#include<Windows.h>
//#include<WinDef.h>
//#include<tchar.h>
//#include<wchar.h>
#include"Task.h"
//#include"../Thread/ThreadHead.h"

//#pragma comment(lib, "../���ļ�/Unicode/Thread.lib")

//ʵ����
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
	m_MySql.reconnect = 1;	//�Ͽ��Զ�����
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
	m_MySql.reconnect = 1;	//�Ͽ��Զ�����
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

//�������ݿⵥ��
CYcDBEngine *CYcDBEngine::CreateInstance(void *pListen)
{
	if (CYcDBEngine::m_pDBInstance == NULL)
	{
		CYcDBEngine::m_pDBInstance = new CYcDBEngine(pListen);
	}
	
	return CYcDBEngine::m_pDBInstance;
}

//��ȡ���ݿ����ָ��
CYcDBEngine *CYcDBEngine::GetInstance()
{
	return CYcDBEngine::m_pDBInstance;
}


//��ʼ��
bool CYcDBEngine::Init(void *pListen)
{
	m_fdColArray = NULL;
	m_RowRes = NULL;
	memset(m_strColName, 0, TABLE_COL_COUNT*TABLE_COL_NAME_LENGTH);
	memset(&m_MySql, 0, sizeof(MYSQL));
	
	
	//���̼���
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

	//�����Զ�����
	//char value = 1;
	//if (!mysql_options(&m_MySql, MYSQL_OPT_RECONNECT, &value))
	//{
	//	((CListenActiveDlg *)m_pListen)->DispMsg("�Զ��������óɹ���\r\n");
	//}
	//else
	//{
	//	((CListenActiveDlg *)m_pListen)->DispMsg("�Զ���������ʧ�ܣ�\r\n");
	//}

	
	//���ؼ��ܺ���
	if (!m_encrypt1 || !m_encrypt2)
	{
		try{
			CString strFileName = "endll.dll";
			if ((m_hmDll = LoadLibrary(TEXT("endll.dll"))) == NULL)
			{
				if (m_pListen)
				{
					((CListenActiveDlg *)m_pListen)->DispMsg("ȱ��endll.dll\r\n");
				}
				return FALSE;
			}
				
			m_encrypt1 = (tagencrypt1)::GetProcAddress(m_hmDll, "str_encrypt_reversible");
			m_encrypt2 = (tagencrypt2)::GetProcAddress(m_hmDll, "str_encrypt2_reversible");

			if (m_encrypt1 == NULL || m_encrypt2 == NULL)
			{
				if (m_pListen)
				{
					((CListenActiveDlg *)m_pListen)->DispMsg("ȱ�ټ��ܺ���\r\n");
				}
			}
		}
		catch (CException *e){
			e->Delete();
			if (m_pListen)
			{
				((CListenActiveDlg *)m_pListen)->DispMsg("����Dllʧ��!\n");
			}

			return false;
		}

		if (m_pListen)
		{
			//((CListenActiveDlg *)m_pListen)->DispMsg("����Dll�ɹ�!!\n");
		}

	}
	
	return m_bInitSuccess;  //����mysql�����ݿ�
}

//����
bool CYcDBEngine::Connect(char *strAccountName, char *strPwd, char *strDBName/*Ĭ�����ݿ�*/, int port, char *unix_socket/*NULLʹ��windows�����ܵ�*/, unsigned long client_flag /*ͨ��Ϊ0*/, char *strHostName /*Ĭ�ϱ�������*/)
{
	try
	{
		//����У��
		if (!m_bInitSuccess || NULL == strAccountName || NULL == strPwd || NULL == strHostName) return false;

		//�����Զ�����
		CListenActiveDlg * pListen = ((CListenActiveDlg *)m_pListen);

		//((CSynLocker *)m_pLocker)->Lock();

		//����false������ʧ�ܣ�����true�����ӳɹ�
		if (!(mysql_real_connect(&m_MySql, strHostName, strAccountName, strPwd, strDBName, port, unix_socket, client_flag))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ
		{
			//printf("Error connecting to database:%s\n", mysql_error(&m_MySql));
		//	((CSynLocker *)m_pLocker)->UnLock();
			return false;
		}




		//�������ӳ�ʱʱ��

		//char sqlstring[64] = { 0 };
		//sprintf(sqlstring, "set interactive_timeout=%s", pListen->GetMysqlConnectTimeOut().c_str());
		//if (ExecuteSql(sqlstring, NULL, NULL, NULL, NULL, NULL, m_pListen))
		//{
		//	pListen->DispMsg("�޸ĳ�ʱ�ɹ���\r\n");
		//	pListen->DispMsg(sqlstring);
		//	pListen->DispMsg("\r\n");
		//}
		//else
		//{
		//	pListen->DispMsg("�޸ĳ�ʱʧ�ܣ�\r\n");
		//}
		
		//((CSynLocker *)m_pLocker)->UnLock();
		//��Ӷ�ʱ���Ͽ����Զ�����	1Сʱ���һ��
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

//�ر�
bool CYcDBEngine::Close()
{
	try
	{
		//�ͷ���Դ
		if (m_RowRes)
		{
			mysql_free_result(m_RowRes);
			m_RowRes = NULL;
		}
		m_bInitSuccess = false;
		mysql_close(&m_MySql);
		mysql_library_end();	//�ͷ�δ���ͷŵ��ڴ�
		CListenActiveDlg * pListen = ((CListenActiveDlg *)m_pListen);

		//mysql_thread_end();
	}
	catch (...)
	{
		if (((CListenActiveDlg *)m_pListen))
			((CListenActiveDlg *)m_pListen)->DispMsg("�Ͽ����ݿ��쳣��\r\n");
		return false;
	}

	return true;
}

//ִ��sql���
bool CYcDBEngine::ExecuteSql(char *strSql, char ****pRetOut, bool *pbExistResOut, int *pResRows, int *pColumn, int *pErrNO, void *pObject/*���ڴ�ӡ��ʾ*/)
{
	//����У��
	if (m_bInitSuccess==false||strSql == NULL) return false;

	//ִ��sql���
	//mysql_query(&m_MySql, "set names gbk");
	//((CSynLocker *)m_pLocker)->Lock();
	
	int iAbornormalPara = 0;
	try
	{
		//�쳣1
		iAbornormalPara++;

		//ִ��SQL���
		//����0 ��ѯ�ɹ�������1��ѯʧ��
		while (mysql_query(&m_MySql, strSql))
		{
			//�����������
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
					((CListenActiveDlg *)pObject)->DispMsg("���ӳɹ�!\r\n");
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

		//�쳣2
		iAbornormalPara++;

		//��ȡ�����
		m_RowRes = mysql_store_result(&m_MySql);

		if (!m_RowRes)
		{
			//�����������
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
//		sprintf(strm_RowRes, "mysql_store_result m_RowRes :%s\r\n", m_RowRes ? "�н����" : "�޽����");
	//	if (pObject)
		//	((CListenActiveDlg *)pObject)->DispMsg(strm_RowRes);

		if (!m_RowRes)
		{
			//((CSynLocker *)m_pLocker)->UnLock();
			return true;		//�޽����sql
		}

		//�쳣3
		iAbornormalPara++;

		//��ӡ��������	��ȡ����
		int iResLine = mysql_affected_rows(&m_MySql);	//sqlִ����Ӱ��������������ֶ���
		if (iResLine <= 0)
		{
			//�����������
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

		//�н�������
		if (iResLine > 0 && pRetOut)
		{			
			(*pRetOut) = new char **[iResLine];
			
			if (*pRetOut)
			{
				memset(*pRetOut, 0, iResLine);
			}
		}

		//��������
		if (pResRows)
			*pResRows = iResLine;

	//	char striResLine[50] = { 0 };
	//	sprintf(striResLine, "mysql_affected_rows ret iResLine:%d\r\n", iResLine);
	//	if (pObject)
		//	((CListenActiveDlg *)pObject)->DispMsg(striResLine);

		//�쳣4
		iAbornormalPara++;

		if (iResLine < 0)
		{
			//�ͷ���Դ
			if (m_RowRes)
			{
				mysql_free_result(m_RowRes);
				m_RowRes = NULL;
			}

			//((CSynLocker *)m_pLocker)->UnLock();
			//if (pObject)
		//		((CListenActiveDlg *)pObject)->DispMsg("mysql_affected_rows Below Zero\r\n");
			return false;		//ִ��ʧ��
		}


		//�쳣5
		iAbornormalPara++;
		//printf("number of dataline returned: %d\n", iResLine);

		//�н������������ iResLine>0 ���� m_RowRes��ΪNULL
		if (m_RowRes)
		{
			//��ȡ�ֶ���
			memset(m_strColName, 0, TABLE_COL_COUNT*TABLE_COL_NAME_LENGTH);	//��ʼ���ֶ���
			int iColNameLength = 0;
			for (int i = 0; m_fdColArray = mysql_fetch_field(m_RowRes); i++)  //��ȡ�ֶ���
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
				//�����������
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

			//�쳣6
			iAbornormalPara++;

			// ��ȡ����
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

			//��������
			if (pColumn) *pColumn = iResColCount;


			//�쳣7
			iAbornormalPara++;

		//	char strResColCount[50] = { 0 };
		//	sprintf(strResColCount, "mysql_num_fields ret num:%d\r\n", iResColCount);
		//	if (pObject)
		//		((CListenActiveDlg *)pObject)->DispMsg(strResColCount);

			//�쳣8
			iAbornormalPara++;

			//for (int i = 0; i < iResColCount; i++)  //��ӡ�ֶ���
		//	{
		//		if (pObject)
			//	{
		//			((CListenActiveDlg *)pObject)->DispMsg(m_strColName[i]);
		//			((CListenActiveDlg *)pObject)->DispMsg("\t\t");
		//		}
		//	}

		//	if (pObject)
		//		((CListenActiveDlg *)pObject)->DispMsg("\r\n");

			//�쳣9
			iAbornormalPara++;

			//�����ָ��
			int m = 0;
			char ***pTmp3 = NULL;
			if (bRetGatherCreate) pTmp3 = *pRetOut;
			

			//��ȡ��һ�� һ��һ�л�ȡ������ΪNULL
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

					//��ȡ��������	m_Column[i] ȫΪ�ַ���
					//m_Column[i];
				}

				//��ά��ƫ��
				m++;

			//	if (pObject)
			//		((CListenActiveDlg *)pObject)->DispMsg("\r\n");
			}

		}
	
		//�쳣10
		iAbornormalPara++;

		//�ͷ���Դ
		if (m_RowRes)
		{
			mysql_free_result(m_RowRes);
			m_RowRes = NULL;
		}

	//	if (pObject)
	//		((CListenActiveDlg *)pObject)->DispMsg("Execute sql end!\r\n");

		//�쳣11
		iAbornormalPara++;
	}
	catch (...)
	{
		//�����������
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

//��ѯ�˻�
bool CYcDBEngine::QueryUserAccount(char *strQuerySql, void *pObject /*���ڴ�ӡ��ʾ*/)
{
	//sprintf(query, "select * from user"); //ִ�в�ѯ��䣬�����ǲ�ѯ���У�user�Ǳ��������ü����ţ���strcpyҲ����
	//mysql_query(&m_MySql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������

	return ExecuteSql(strQuerySql,NULL, NULL, NULL, NULL,NULL,  pObject);

}

//ע���˻�
bool CYcDBEngine::RegisterAccount(char *useraccount, char *pwd, char *email, int *iErrCode, void *pListenDlg)
{
	int iAbortNormal = 0;

	//����У��
	if (!useraccount || !pwd || !email)	return false;

	//��ʾ
	CListenActiveDlg * pListen = ((CListenActiveDlg *)m_pListen);
	if (!pListen) return false;

	try
	{
		//��ʼ�����ݿ�
		if (!Init(m_pListen)) return false;

		//�쳣1
		iAbortNormal++;

		//��ʾ��
		//pListen->DispMsg("userid:");
	//	pListen->DispMsg(useraccount);
	//	pListen->DispMsg(" pwd:");
	//	pListen->DispMsg(pwd);
	//	pListen->DispMsg(" email:");
	//	pListen->DispMsg(email);
	//	pListen->DispMsg("\r\n");

		//�쳣2
		iAbortNormal++;

		//���ܺ����account
		char key[4096] = { 0 };
		memset(key, 0, 4096);
		strcpy(key, (m_encrypt1)(pwd, useraccount));

	//	char rizhi[100] = { 0 };
	//	sprintf(rizhi, "m_encrypt1 key1 len:%d\r\n", strlen(key));
	//	pListen->DispMsg(rizhi);
		key[4095] = 0;

		//�쳣3
		iAbortNormal++;
		char key2[4096] = { 0 };
		strcpy(key2, (m_encrypt2)((char *)key, (char *)email));

		//memset(rizhi, 0, 100);
	//	sprintf(rizhi, "m_encrypt2 key2 len:%d\r\n", strlen(key2));
	//	pListen->DispMsg(rizhi);
		key2[4095] = 0;



		//�쳣4
		iAbortNormal++;
		//pListen->DispMsg(key2);

		//�쳣5
		iAbortNormal++;

		char keyCache[4096] = { 0 };
		memmove(keyCache, key2, 4096);
		keyCache[4095] = 0;
		//CString strKeyd ;
		//strKeyd += key2;

		//�쳣6
		iAbortNormal++;

		//	strSql = "insert into account(userid,password,fullname,email,account_create,permission_level,pts_balance,pts_method,pts_start,next_pts_method,suspend_end,first_login,last_login, login_count) values('"
		//		+ activeuser.userid + "','" + strKey + "','" + activeuser.userid + "','"
		//		+ activeuser.email + "',NOW(), 0, 0, 'LIFE', now(), 'LIFE', default, default, default, default)";

		char strSql[1024] = { 0 };
		sprintf(strSql, "insert into account(userid, password, fullname, email, account_create, unused_dur_end,pts_method,pts_start,next_pts_method)values ('%s', '%s', '%s','%s', now(), '2199-01-01 00:00:00', 'LIFE', now(), 'LIFE');"
			, useraccount, keyCache, useraccount, email);  //������취ʵ���ֶ��ڿ���̨�ֶ�����ָ��

		//�쳣7
		iAbortNormal++;

		bool bResult = false;
		int iRowResult = 0, iErr = 0;
		char strResult[100] = { 0 };
		bool bExeResult = false;


		//�������ݿ�
		if (!Connect((char *)pListen->m_strMysqlAdminAccount.c_str(), (char *)pListen->m_strMysqlAdminPwd.c_str(), (char *)pListen->m_strMysqlDBName.c_str())) return false;
		//ִ�в���
		
		
		//((CSynLocker *)m_pLocker)->Lock();
		bExeResult = ExecuteSql(strSql, NULL, &bResult, &iRowResult, NULL, &iErr, m_pListen);
		//((CSynLocker *)m_pLocker)->UnLock();

		//�ر����ݿ�
		Close();

		if (bExeResult)
		{
			//memset(strResult, 0, 100);
			//sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
			//pListen->DispMsg(strResult);

			//pListen->DispMsg("ע��ɹ�!\r\n");
		}
		else
		{
			*iErrCode = iErr;
			//memset(strResult, 0, 100);
			//sprintf(strResult, "CYcDBEngine::RegisterAccount��uid=%s, pwd=%s,email=%s,�� iErr %d\r\n", useraccount, pwd, email, iErr);
			//pListen->DispMsg(strResult);
			//pListen->DispMsg("ע��ʧ��!\r\n");
			return false;
		}

		
			

		//memset(strResult, 0, 100);
		//sprintf(strResult, "bResult %s,iRowResult %d, iErr %d\r\n", bResult ? "��" : "û��", iRowResult, iErr);
		//pListen->DispMsg(strResult);

		//�쳣8
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

//�޸�����
bool CYcDBEngine::ModifyPwd(char *useraccount, char *oldpwd, char *newpwd, int *iErrCode, void *pListenDlg)
{
	//У�����
	if (!useraccount || !oldpwd || !newpwd) return false;

	//��ӡ
	CListenActiveDlg *pListen = (CListenActiveDlg *)pListenDlg;

	char ***pRetOut = NULL;		//�����
	char strSQL[4096] = { 0 };
	bool bHaveResult = false;
	int iRowNum = 0, iColumnNum=0;
	sprintf(strSQL, "select userid, password, email from account where userid='%s' limit 1", useraccount);

	bool bExeResult = false;

	//��ʼ�����ݿ�
	if (!Init(m_pListen)) return false;
	//�������ݿ�
	if (!Connect((char *)pListen->m_strMysqlAdminAccount.c_str(), (char *)pListen->m_strMysqlAdminPwd.c_str(), (char *)pListen->m_strMysqlDBName.c_str())) return false;
	//ִ�в���

//	((CSynLocker *)m_pLocker)->Lock();
	bExeResult = ExecuteSql(strSQL, &pRetOut, &bHaveResult, &iRowNum, &iColumnNum, NULL, pListenDlg);
	//((CSynLocker *)m_pLocker)->UnLock();



	//��ѯ�˻��õ�userid pwd email
	if (!bExeResult)
	{
		ReleaseResultOutGather(&pRetOut, iRowNum, iColumnNum);
		*iErrCode = USER_ID_WRONG;

		//�ر����ݿ�
		Close();

		//if (pListen)
		//	pListen->DispMsg("ModifyPwd �˻������� ��ִ�г���\r\n");
		return false;
	}

	//����
	char userid[ACCOUNT_LEN] = { 0 };
	char email[EMAIL_LEN] = { 0 };
	char pwd[2048] = { 0 };

	//�н���� ȡ���
	if (bHaveResult&&iRowNum > 0 && iColumnNum>0)
	{
		for (int n = 0; n < iColumnNum; n++)
		{
			switch (n)
			{
			case 0:
				{
					  strcpy(userid, (pRetOut)[0][n]);
					  //if (pListen) pListen->DispMsg("ModifyPwd �˻�:");
					 // if (pListen) pListen->DispMsg(userid);
					 // if (pListen) pListen->DispMsg("\t");
					break;
				}
			case 1:
			{
					  strcpy(pwd, (pRetOut)[0][n]);
					 // if (pListen) pListen->DispMsg("ModifyPwd ����:");
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

		//�ر����ݿ�
		Close();

		//if (pListen)
		//	pListen->DispMsg("ModifyPwd �˻�������\r\n");
		return false;
	}

	//����oldpwd userid email 
	//1�μ���
	char key[4096] = { 0 };
	memset(key, 0, 4096);
	strcpy(key, (m_encrypt1)(oldpwd, useraccount));
	key[4095] = 0;

	//2�μ���
	char key2[4096] = { 0 };
	memset(key2, 0, 4096);
	strcpy(key2, (m_encrypt2)((char *)key, (char *)email));
	key2[4095] = 0;

	//���ܽ����pwd�Ƚ�����ͬ����޸����룬������ͬ�򷵻�ԭ�����������
	if (strcmp(key2, pwd) != 0)
	{
		//ԭ�������
		*iErrCode = OLD_PWD_WRONG;

		//�ر����ݿ�
		Close();
		//if (pListen)
		//	pListen->DispMsg("ModifyPwd ԭ�����������\r\n");
		return false;
	}
	else
	{
		//ԭ������ȷ
		*iErrCode = SUCCESS_MOD;

		//ִ�и�������
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

		//��������
		if (bExeResult)
		{
			*iErrCode = SUCCESS_MOD;
			//if (pListen)
			//	pListen->DispMsg("ModifyPwd �޸ĳɹ�\r\n");
		}
		else
		{
			*iErrCode = UPDATE_ERROR;
			//if (pListen)
			//	pListen->DispMsg("ModifyPwd ����ʧ��\r\n");
			//�ر����ݿ�
			Close();
			return false;
		}
	}

	//�ر����ݿ�
	Close();

	//�ͷſռ�
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