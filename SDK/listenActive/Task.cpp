#include"StdAfx.h"
#include"Task.h"
#include "listenActiveDlg.h"
#include"TcpServer.h"
#include "mysql.h"    //引入mysql头文件(一种方式是在vc目录里面设置，一种是文件夹拷到工程目录，然后这样包含)


//启动网络服务
int CMonitorSockTask::Run()
{
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;
	if (!pListen) return 0;

	//pListen->DispMsg("线程池启动成功!\r\n");

	//启动网络服务
	CTcpServer Server;
	Server.SetMainWind(pListen);
	Server.SetIp(pListen->GetStrIP());
	Server.SetPort(pListen->GetPort());
	Server.StartServer();
	Server.CloseServer();

	pListen->DispMsg("监控结束!\r\n");

	return 0;
}

//读取显示客户端数据
int CReadDataTask::Run()
{
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;

	if (!pListen) return 0;
	int iAbort = 0;
	string strErr;
	char rizhi[1024] = { 0 };

	

	try
	{
		//异常1
		iAbort++;

		//pListen->DispMsg("Run 等待客户端数据...");

		//读取数据
		char  *pData = NULL;
		long lDatalen = 0;//
		CTcpServer Server;
		Server.SetAutoCloseSocket(false);//不自动关闭
		Server.SetSocketFD(m_iSockFD);
		Server.SetMainWind(m_pArg);

		strErr.append("CReadDataTask 读取数据=");

		Server.ReadDataFromClient((void **)&pData, &lDatalen);

		if (!pData)
		{
			strErr.append("客户端数据中断=");
			throw NULL;
		}

		switch (*((unsigned int *)pData))
		{
		case REG_ID:
		{
					   strErr.append("注册任务=");

					   //pListen->DispMsg("Run 注册任务...\r\n");
					 //  char rizhi[1024] = { 0 };
					  // sprintf(rizhi, "Run :账号：%s, 密码：%s, email:%s;\r\n", ((REGISTER_BODY_PACK_T *)pData)->account, ((REGISTER_BODY_PACK_T *)pData)->pwd
					//	   , ((REGISTER_BODY_PACK_T *)pData)->email);
					   //pListen->DispMsg(rizhi);

					   //拷贝数据
					   REGISTER_BODY_PACK_T stClntRegPack;
					   memset(&stClntRegPack, 0, S_REGISTER_BODY_PACK_T);
					   memmove(&stClntRegPack, pData, S_REGISTER_BODY_PACK_T);

					   memset(rizhi, 0, sizeof(rizhi));
					   sprintf(rizhi, "m_iSockFD :%d;预注册账户：%s, 密码：%s, email:%s= \r\nReleaseRecvDataOut =", m_iSockFD, stClntRegPack.account, stClntRegPack.pwd, stClntRegPack.email);
					   strErr.append(rizhi);

					   Server.ReleaseRecvDataOut();


					   memset(rizhi, 0, sizeof(rizhi));
					   sprintf(rizhi, "Addtask CRegTask=");
					   strErr.append(rizhi);

					   //注册任务
					   CTaskBase *pRegTask = new CRegTask(m_iSockFD, stClntRegPack, m_pArg);
					   CThreadPool::GetInstance()->AddTask(pRegTask);



					 //  REG_RETURN_PACK_T stRet;
					 //  memset(&stRet, 0, S_REG_RETURN_PACK_T);
					 //  stRet.uFunID = REG_ID;
					 //  strcpy(stRet.ret, "注册成功！");
					 //  Server.SendDataToClient(&stRet, sizeof(stRet));
					   //pListen->on

					   break;
		}
		case MODIFY_PWD_ID:
		{
							  //获取修改密码使能
							 // pListen->DispMsg("Run 修改密码任务...\r\n");
							 // char rizhi[1024] = { 0 };
							 // sprintf(rizhi, "Run :账号：%s, 旧密码：%s, 新密码:%s;\r\n", ((MODIFY_PWD_BODY_PACK_T *)pData)->account, ((MODIFY_PWD_BODY_PACK_T *)pData)->oldpwd
							//	  , ((MODIFY_PWD_BODY_PACK_T *)pData)->newpwd);
							 // pListen->DispMsg(rizhi);

							  memset(rizhi, 0, sizeof(rizhi));
							  sprintf(rizhi, "修改密码任务=");
							  strErr.append(rizhi);

							  //拷贝数据
							  MODIFY_PWD_BODY_PACK_T stClntModPack;
							  memset(&stClntModPack, 0, S_MODIFY_PWD_BODY_PACK_T);
							  memmove(&stClntModPack, pData, S_MODIFY_PWD_BODY_PACK_T);

							  memset(rizhi, 0, sizeof(rizhi));
							  sprintf(rizhi, "m_iSockeFD:%d; 预修改帐号：%s; 原密码：%s;新密码：%s=\r\n ReleaseRecvDataOut=", m_iSockFD, stClntModPack.account, stClntModPack.oldpwd, stClntModPack.newpwd);
							  strErr.append(rizhi);

							  Server.ReleaseRecvDataOut();

							  memset(rizhi, 0, sizeof(rizhi));
							  sprintf(rizhi, "Addtask CModPwdTask=");
							  strErr.append(rizhi);

							  //注册任务
							  CTaskBase *pModTask = new CModPwdTask(m_iSockFD, stClntModPack, m_pArg);
							  CThreadPool::GetInstance()->AddTask(pModTask);




							  break;
		}
		}

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "\r\n CReadDataTask 结束=");
		strErr.append(rizhi);

		//异常2
		iAbort++;

	}
	catch (...)
	{
		char rizhi2[1024] = { 0 };
		sprintf(rizhi2, "\r\nCReadDataTask 异常报告: %s", iAbort, strErr.c_str());
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


		//异常1
		iAbort++;
		//pListen->DispMsg("Run 执行数据库引擎...");
		int iErr = 0;
		CTcpServer Server;
		Server.SetSocketFD(m_hSock);
		Server.SetMainWind(m_pArg);
		REG_RETURN_PACK_T stRet;
		memset(&stRet, 0, S_REG_RETURN_PACK_T);
		stRet.uFunID = REG_ID;

		strErr.append("异常开始=");

		//获取注册使能
		bool bRegisterEnable = pListen->GetRegisterEnable();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "注册使能：%s", bRegisterEnable ? "开启" : "禁用");
		strErr.append(rizhi);

		if (!bRegisterEnable)
		{
			sprintf(stRet.ret, "抱歉，已关闭注册，请联系管理员！");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("发送回执成功");
			}
			else
			{
				//pListen->DispMsg("发送回执失败");
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

		//启动mysql线程初始化
		mysql_thread_init();




		bExeSuccess=CYcDBEngine::GetInstance()->RegisterAccount(m_stClntRegPack.account, m_stClntRegPack.pwd, m_stClntRegPack.email, &iErr, pListen);
		
		//mysql线程结束清理
		mysql_thread_end();
		
		((CSynLocker *)CYcDBEngine::GetInstance()->GetLocker())->UnLock();

		if (bExeSuccess)
		{
			  strcpy(stRet.ret, "注册成功！");
			  if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			  {
				//  pListen->DispMsg("发送回执成功");
			  }
			  else
			  {
				//  pListen->DispMsg("发送回执失败");
			  }
			//  pListen->DispMsg("注册成功\r\n");
		}
		else
		{
			
			//sprintf( stRet.ret, "帐号：%s; 密码：%s;email:%s, 注册失败代码：%d\r\n", m_stClntRegPack.account, m_stClntRegPack.pwd, m_stClntRegPack.email, iErr);
			//pListen->DispMsg(stRet.ret);
			memset(stRet.ret, 0, RET_LEN);
			if (iErr == 1062)
			{
				sprintf(stRet.ret, "用户名已存在,注册失败！");
			}
			else
			{
				sprintf(stRet.ret, "注册失败%d", iErr);
			}
			
			//sprintf(stRet.ret, "注册失败！");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("发送回执成功");
			}
			else
			{
			//	pListen->DispMsg("发送回执失败");
			}
		}


		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "mysql_thread_end=\r\n");
		strErr.append(rizhi);



		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CRegTask 结束=\r\n");
		strErr.append(rizhi);
		//pListen->DispMsg(strErr.c_str());

	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "\r\nCRegTask 异常报告：%s\r\n", strErr.c_str());
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

		strErr.append("CModPwdTask 开始");

		//异常1
		iAbort++;
		//pListen->DispMsg("Run 执行数据库引擎...");
		int iErr = 0;
		CTcpServer Server;
		Server.SetSocketFD(m_hSock);
		Server.SetMainWind(m_pArg);
		REG_RETURN_PACK_T stRet;
		memset(&stRet, 0, S_REG_RETURN_PACK_T);
		stRet.uFunID = MODIFY_PWD_ID;

		//获取注册使能
		bool bModifyPwdEnable = pListen->GetModifyPwdEnalbe();

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CModPwdTask修改密码使能：%s=", bModifyPwdEnable?"开启":"禁用");
		strErr.append(rizhi);

		if (!bModifyPwdEnable)
		{
			sprintf(stRet.ret, "抱歉，已关闭修改密码，请联系管理员！");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("发送回执成功");
			}
			else
			{
				//pListen->DispMsg("发送回执失败");
			}

			return 0;
		}

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "mysql_thread_init =");
		strErr.append(rizhi);

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "ModifyPwd（acount=%s, oldpwd=%s, newpwd=%s）=\r\n"
			, m_stClntModPack.account, m_stClntModPack.oldpwd, m_stClntModPack.newpwd);
		strErr.append(rizhi);

		bool bExeSuccess = false;
		((CSynLocker *)CYcDBEngine::GetInstance()->GetLocker())->Lock();

		//启动mysql线程初始化
		mysql_thread_init();

		bExeSuccess = CYcDBEngine::GetInstance()->ModifyPwd(m_stClntModPack.account, m_stClntModPack.oldpwd, m_stClntModPack.newpwd, &iErr, pListen);
		
		//mysql线程结束清理
		mysql_thread_end();

		((CSynLocker *)CYcDBEngine::GetInstance()->GetLocker())->UnLock();

		if (bExeSuccess)
		{
			//日志
			//char rizhid[100] = { 0 };
			//sprintf(rizhid, "CModPwdTask 密码修改成功：%d\r\n", iErr);
			//if (pListen)
			//	pListen->DispMsg(rizhid);

			strcpy(stRet.ret, "密码修改成功！");
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("CModPwdTask 发送回执成功\r\n");
			}
			else
			{
				//pListen->DispMsg("发送回执失败\r\n");
			}
		}
		else
		{
			if (OLD_PWD_WRONG == iErr)
			{
				//原密码输入有误
				sprintf(stRet.ret, "原密码输入错误！");
			}
			else if (USER_ID_WRONG == iErr)
			{
				//账户不存在
				sprintf(stRet.ret, "您输入的账号不存在！");
			}
			else
			{
				//其他错误
				sprintf(stRet.ret, "修改失败！");
			}

			//日志
			//char rizhid[100] = { 0 };
			//sprintf(rizhid, "帐号：%s;原密码：%s;新密码：%s;密码修改失败代码：%d\r\n", m_stClntModPack.account, m_stClntModPack.oldpwd, m_stClntModPack.newpwd, iErr);
			//if (pListen)
			//	pListen->DispMsg(rizhid);

			
			if (Server.SendDataToClient(&stRet, sizeof(stRet)))
			{
				//pListen->DispMsg("CModPwdTask 发送回执成功\r\n");
			}
			else
			{
				//pListen->DispMsg("CModPwdTask 发送回执失败\r\n");
			}
		}

		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "mysql_thread_end =");
		strErr.append(rizhi);



		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "CModPwdTask 结束 =\r\n");
		strErr.append(rizhi);

	}
	catch (...)
	{
		memset(rizhi, 0, sizeof(rizhi));
		sprintf(rizhi, "\r\nCModPwdTask 异常报告：%s\r\n", strErr.c_str());
		pListen->DispMsg(rizhi);
	}



	return 0;
}

//////////////////////////////////////////////////////////

//数据库断开自动重连
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
		Sleep(m_lReconnectTime);	//间隔m_lReconnectTime ping一次
		pLocker->Lock();
		bSuccess=mysql_ping(pMySql);
		pLocker->UnLock();

		if (pListen) pListen->DispMsg("ping 一次MySQL");
		if (bSuccess)
		{
			if (pListen) pListen->DispMsg("ping 成功\r\n");
		}
		else
		{
			if (pListen) pListen->DispMsg("ping 失败\r\n");
		}
	}

	if (pListen) pListen->DispMsg("结束ping 任务\r\n");
	return 0;
}

//任务执行体
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