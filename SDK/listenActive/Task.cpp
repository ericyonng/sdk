#include"StdAfx.h"
#include"Task.h"
#include "listenActiveDlg.h"
#include"TcpServer.h"
#include "mysql.h"    //����mysqlͷ�ļ�(һ�ַ�ʽ����vcĿ¼�������ã�һ�����ļ��п�������Ŀ¼��Ȼ����������)


//�����������
int CMonitorSockTask::Run()
{
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;
	if (!pListen) return 0;

	//pListen->DispMsg("�̳߳������ɹ�!\r\n");

	//�����������
	CTcpServer Server;
	Server.SetMainWind(pListen);
	Server.SetIp(pListen->GetStrIP());
	Server.SetPort(pListen->GetPort());
	Server.StartServer();
	Server.CloseServer();

	pListen->DispMsg("��ؽ���!\r\n");

	return 0;
}

//��ȡ��ʾ�ͻ�������
int CReadDataTask::Run()
{
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;

	if (!pListen) return 0;
	int iAbort = 0;
	string strErr;
	char rizhi[1024] = { 0 };

	

	try
	{
		//�쳣1
		iAbort++;

		//pListen->DispMsg("Run �ȴ��ͻ�������...");

		//��ȡ����
		char  *pData = NULL;
		long lDatalen = 0;//
		CTcpServer Server;
		Server.SetAutoCloseSocket(false);//���Զ��ر�
		Server.SetSocketFD(m_iSockFD);
		Server.SetMainWind(m_pArg);

		strErr.append("CReadDataTask ��ȡ����=");

		Server.ReadDataFromClient((void **)&pData, &lDatalen);

		if (!pData)
		{
			strErr.append("�ͻ��������ж�=");
			throw NULL;
		}

		switch (*((unsigned int *)pData))
		{
		case REG_ID:
		{
					   strErr.append("ע������=");

					   //pListen->DispMsg("Run ע������...\r\n");
					 //  char rizhi[1024] = { 0 };
					  // sprintf(rizhi, "Run :�˺ţ�%s, ���룺%s, email:%s;\r\n", ((REGISTER_BODY_PACK_T *)pData)->account, ((REGISTER_BODY_PACK_T *)pData)->pwd
					//	   , ((REGISTER_BODY_PACK_T *)pData)->email);
					   //pListen->DispMsg(rizhi);

					   //��������
					   REGISTER_BODY_PACK_T stClntRegPack;
					   memset(&stClntRegPack, 0, S_REGISTER_BODY_PACK_T);
					   memmove(&stClntRegPack, pData, S_REGISTER_BODY_PACK_T);

					   memset(rizhi, 0, sizeof(rizhi));
					   sprintf(rizhi, "m_iSockFD :%d;Ԥע���˻���%s, ���룺%s, email:%s= \r\nReleaseRecvDataOut =", m_iSockFD, stClntRegPack.account, stClntRegPack.pwd, stClntRegPack.email);
					   strErr.append(rizhi);

					   Server.ReleaseRecvDataOut();


					   memset(rizhi, 0, sizeof(rizhi));
					   sprintf(rizhi, "Addtask CRegTask=");
					   strErr.append(rizhi);

					   //ע������
					   CTaskBase *pRegTask = new CRegTask(m_iSockFD, stClntRegPack, m_pArg);
					   CThreadPool::GetInstance()->AddTask(pRegTask);



					 //  REG_RETURN_PACK_T stRet;
					 //  memset(&stRet, 0, S_REG_RETURN_PACK_T);
					 //  stRet.uFunID = REG_ID;
					 //  strcpy(stRet.ret, "ע��ɹ���");
					 //  Server.SendDataToClient(&stRet, sizeof(stRet));
					   //pListen->on

					   break;
		}
		case MODIFY_PWD_ID:
		{
							  //��ȡ�޸�����ʹ��
							 // pListen->DispMsg("Run �޸���������...\r\n");
							 // char rizhi[1024] = { 0 };
							 // sprintf(rizhi, "Run :�˺ţ�%s, �����룺%s, ������:%s;\r\n", ((MODIFY_PWD_BODY_PACK_T *)pData)->account, ((MODIFY_PWD_BODY_PACK_T *)pData)->oldpwd
							//	  , ((MODIFY_PWD_BODY_PACK_T *)pData)->newpwd);
							 // pListen->DispMsg(rizhi);

							  memset(rizhi, 0, sizeof(rizhi));
							  sprintf(rizhi, "�޸���������=");
							  strErr.append(rizhi);

							  //��������
							  MODIFY_PWD_BODY_PACK_T stClntModPack;
							  memset(&stClntModPack, 0, S_MODIFY_PWD_BODY_PACK_T);
							  memmove(&stClntModPack, pData, S_MODIFY_PWD_BODY_PACK_T);

							  memset(rizhi, 0, sizeof(rizhi));
							  sprintf(rizhi, "m_iSockeFD:%d; Ԥ�޸��ʺţ�%s; ԭ���룺%s;�����룺%s=\r\n ReleaseRecvDataOut=", m_iSockFD, stClntModPack.account, stClntModPack.oldpwd, stClntModPack.newpwd);
							  strErr.append(rizhi);

							  Server.ReleaseRecvDataOut();

							  memset(rizhi, 0, sizeof(rizhi));
							  sprintf(rizhi, "Addtask CModPwdTask=");
							  strErr.append(rizhi);

							  //ע������
							  CTaskBase *pModTask = new CModPwdTask(m_iSockFD, stClntModPack, m_pArg);
							  CThreadPool::GetInstance()->AddTask(pModTask);




							  break;
		}
		}

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "\r\n CReadDataTask ����=");
		strErr.append(rizhi);

		//�쳣2
		iAbort++;

	}
	catch (...)
	{
		char rizhi2[1024] = { 0 };
		sprintf(rizhi2, "\r\nCReadDataTask �쳣����: %s", iAbort, strErr.c_str());
		pListen->DispMsg(rizhi2);

	}
	return 0;
}

int CRegTask::Run()
{
	int iAbort = 0;
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;

	string strErr;
	char rizhi[1024] = { 0 };

	try
	{


		//�쳣1
		iAbort++;
		//pListen->DispMsg("Run ִ�����ݿ�����...");
		int iErr = 0;
		CTcpServer Server;
		Server.SetSocketFD(m_hSock);
		Server.SetMainWind(m_pArg);
		REG_RETURN_PACK_T stRet;
		memset(&stRet, 0, S_REG_RETURN_PACK_T);
		stRet.uFunID = REG_ID;

		strErr.append("�쳣��ʼ=");

		//��ȡע��ʹ��
		bool bRegisterEnable = pListen->GetRegisterEnable();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "ע��ʹ�ܣ�%s", bRegisterEnable ? "����" : "����");
		strErr.append(rizhi);

		if (!bRegisterEnable)
		{
			sprintf(stRet.ret, "��Ǹ���ѹر�ע�ᣬ����ϵ����Ա��");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("���ͻ�ִ�ɹ�");
			}
			else
			{
				//pListen->DispMsg("���ͻ�ִʧ��");
			}

			return 0;
		}

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "mysql_thread_init=");
		strErr.append(rizhi);
		
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "RegisterAccount(account=%s, pwd=%s, email=%s)=\r\n", m_stClntRegPack.account, m_stClntRegPack.pwd, m_stClntRegPack.email);
		strErr.append(rizhi);

		bool bExeSuccess = false;
		((CSynLocker *)CYcDBEngine::GetInstance()->GetLocker())->Lock();

		//����mysql�̳߳�ʼ��
		mysql_thread_init();




		bExeSuccess=CYcDBEngine::GetInstance()->RegisterAccount(m_stClntRegPack.account, m_stClntRegPack.pwd, m_stClntRegPack.email, &iErr, pListen);
		
		//mysql�߳̽�������
		mysql_thread_end();
		
		((CSynLocker *)CYcDBEngine::GetInstance()->GetLocker())->UnLock();

		if (bExeSuccess)
		{
			  strcpy(stRet.ret, "ע��ɹ���");
			  if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			  {
				//  pListen->DispMsg("���ͻ�ִ�ɹ�");
			  }
			  else
			  {
				//  pListen->DispMsg("���ͻ�ִʧ��");
			  }
			//  pListen->DispMsg("ע��ɹ�\r\n");
		}
		else
		{
			
			//sprintf( stRet.ret, "�ʺţ�%s; ���룺%s;email:%s, ע��ʧ�ܴ��룺%d\r\n", m_stClntRegPack.account, m_stClntRegPack.pwd, m_stClntRegPack.email, iErr);
			//pListen->DispMsg(stRet.ret);
			memset(stRet.ret, 0, RET_LEN);
			if (iErr == 1062)
			{
				sprintf(stRet.ret, "�û����Ѵ���,ע��ʧ�ܣ�");
			}
			else
			{
				sprintf(stRet.ret, "ע��ʧ��%d", iErr);
			}
			
			//sprintf(stRet.ret, "ע��ʧ�ܣ�");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("���ͻ�ִ�ɹ�");
			}
			else
			{
			//	pListen->DispMsg("���ͻ�ִʧ��");
			}
		}


		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "mysql_thread_end=\r\n");
		strErr.append(rizhi);



		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CRegTask ����=\r\n");
		strErr.append(rizhi);
		//pListen->DispMsg(strErr.c_str());

	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "\r\nCRegTask �쳣���棺%s\r\n", strErr.c_str());
		pListen->DispMsg(rizhi);

	}



	return 0;
}


int CModPwdTask::Run()
{
	int iAbort = 0;
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;

	string strErr;
	char rizhi[1024] = { 0 };

	try
	{

		strErr.append("CModPwdTask ��ʼ");

		//�쳣1
		iAbort++;
		//pListen->DispMsg("Run ִ�����ݿ�����...");
		int iErr = 0;
		CTcpServer Server;
		Server.SetSocketFD(m_hSock);
		Server.SetMainWind(m_pArg);
		REG_RETURN_PACK_T stRet;
		memset(&stRet, 0, S_REG_RETURN_PACK_T);
		stRet.uFunID = MODIFY_PWD_ID;

		//��ȡע��ʹ��
		bool bModifyPwdEnable = pListen->GetModifyPwdEnalbe();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CModPwdTask�޸�����ʹ�ܣ�%s=", bModifyPwdEnable?"����":"����");
		strErr.append(rizhi);

		if (!bModifyPwdEnable)
		{
			sprintf(stRet.ret, "��Ǹ���ѹر��޸����룬����ϵ����Ա��");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("���ͻ�ִ�ɹ�");
			}
			else
			{
				//pListen->DispMsg("���ͻ�ִʧ��");
			}

			return 0;
		}

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "mysql_thread_init =");
		strErr.append(rizhi);

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "ModifyPwd��acount=%s, oldpwd=%s, newpwd=%s��=\r\n"
			, m_stClntModPack.account, m_stClntModPack.oldpwd, m_stClntModPack.newpwd);
		strErr.append(rizhi);

		bool bExeSuccess = false;
		((CSynLocker *)CYcDBEngine::GetInstance()->GetLocker())->Lock();

		//����mysql�̳߳�ʼ��
		mysql_thread_init();

		bExeSuccess = CYcDBEngine::GetInstance()->ModifyPwd(m_stClntModPack.account, m_stClntModPack.oldpwd, m_stClntModPack.newpwd, &iErr, pListen);
		
		//mysql�߳̽�������
		mysql_thread_end();

		((CSynLocker *)CYcDBEngine::GetInstance()->GetLocker())->UnLock();

		if (bExeSuccess)
		{
			//��־
			//char rizhid[100] = { 0 };
			//sprintf(rizhid, "CModPwdTask �����޸ĳɹ���%d\r\n", iErr);
			//if (pListen)
			//	pListen->DispMsg(rizhid);

			strcpy(stRet.ret, "�����޸ĳɹ���");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("CModPwdTask ���ͻ�ִ�ɹ�\r\n");
			}
			else
			{
				//pListen->DispMsg("���ͻ�ִʧ��\r\n");
			}
		}
		else
		{
			if (OLD_PWD_WRONG == iErr)
			{
				//ԭ������������
				sprintf(stRet.ret, "ԭ�����������");
			}
			else if (USER_ID_WRONG == iErr)
			{
				//�˻�������
				sprintf(stRet.ret, "��������˺Ų����ڣ�");
			}
			else
			{
				//��������
				sprintf(stRet.ret, "�޸�ʧ�ܣ�");
			}

			//��־
			//char rizhid[100] = { 0 };
			//sprintf(rizhid, "�ʺţ�%s;ԭ���룺%s;�����룺%s;�����޸�ʧ�ܴ��룺%d\r\n", m_stClntModPack.account, m_stClntModPack.oldpwd, m_stClntModPack.newpwd, iErr);
			//if (pListen)
			//	pListen->DispMsg(rizhid);

			
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("CModPwdTask ���ͻ�ִ�ɹ�\r\n");
			}
			else
			{
				//pListen->DispMsg("CModPwdTask ���ͻ�ִʧ��\r\n");
			}
		}

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "mysql_thread_end =");
		strErr.append(rizhi);



		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CModPwdTask ���� =\r\n");
		strErr.append(rizhi);

	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "\r\nCModPwdTask �쳣���棺%s\r\n", strErr.c_str());
		pListen->DispMsg(rizhi);
	}



	return 0;
}

//////////////////////////////////////////////////////////

//���ݿ�Ͽ��Զ�����
int CDBReconnectTask::Run()
{
	if (!m_pMySql) return 0;
	if (!m_pLocker) return 0;

	MYSQL *pMySql = (MYSQL *)m_pMySql;
	CSynLocker *pLocker = (CSynLocker *)m_pLocker;
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pListenDlg;
	bool bSuccess = false;
	while (pListen->GetMonitor())
	{
		Sleep(m_lReconnectTime);	//���m_lReconnectTime pingһ��
		pLocker->Lock();
		bSuccess=mysql_ping(pMySql);
		pLocker->UnLock();

		if (pListen) pListen->DispMsg("ping һ��MySQL");
		if (bSuccess)
		{
			if (pListen) pListen->DispMsg("ping �ɹ�\r\n");
		}
		else
		{
			if (pListen) pListen->DispMsg("ping ʧ��\r\n");
		}
	}

	if (pListen) pListen->DispMsg("����ping ����\r\n");
	return 0;
}

//����ִ����
int CListenSizeLimitTask::Run()
{
	HWND hListenWnd = (HWND)m_pArg;
	if (!hListenWnd) return 0;
	
	CRect rcClient(0,0,0,0);

	try
	{
		while (true)
		{
			Sleep(2000);
			if (IsWindow(hListenWnd))
			{
				::GetWindowRect(hListenWnd, &rcClient);
				if (rcClient.Width() > 496 || rcClient.Height() > 338)
				{
					::SetWindowPos(hListenWnd, NULL, 0, 0, 496, 338, SWP_NOMOVE | SWP_SHOWWINDOW);
				}
			}
			else
			{
				break;
			}

		}
	}
	catch (...)
	{
	}


	return 0;
}