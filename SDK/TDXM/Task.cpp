#include"StdAfx.h"
#include"Task.h"
//#include "listenActiveDlg.h"
#include"TcpClient.h"
#include"DlgLogIn.h"
#include"TDXMWnd.h"
/*
//�����������
int CTestTask::Run()
{
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;
	if (!pListen) return 0;

	//�����������
	CTcpServer Server;
	Server.SetMainWind(pListen);
	Server.StartServer();
	Server.CloseServer();

	return 0;
}

//��ȡ��ʾ�ͻ�������
int CReadDataTask::Run()
{
	CListenActiveDlg *pListen = (CListenActiveDlg *)m_pArg;
	CTcpServer *pServer = (CTcpServer *)m_pServer;
	if (!pListen || !pServer) return 0;

	//��ȡ����
	void *pData = NULL;
	long lDatalen = 0;
	pServer->ReadDataFromClient(&pData, &lDatalen);
	
	if (pData)
	{
		pListen->DispMsg((char *)pData);
		delete []pData;
	}

	return 0;
}*/

int CSendDataToServerTask::Run()
{
	CTcpClient Client;

	//Client.StartNetService();
	Client.ConnectServer();
	char dd[4096] = "�Բ�";
	dd[3333] = 'D';
	Client.SendDataToServer(dd, 333);

	//MessageBox(NULL, TEXT("SendDataToServer"), NULL, 0);

	return 0;
}

int CConnectServerTask::Run()
{
	if (!m_pArg || m_pClient==NULL) return 0;
	CDlgLogIn *pLogIn = (CDlgLogIn *)m_pArg;

	CTcpClient *pClient = (CTcpClient *)m_pClient;
	//Client.SetSocketFD(m_hSock);

	if (pClient->ConnectServer())
	{
		pLogIn->SetLinkResult(TEXT("���ӷ������ɹ���"));
	}
	else
	{
		pLogIn->SetLinkResult(TEXT("���ӷ�����ʧ�ܣ�"));
	}

	//�Ͽ�����
	pClient->CloseLink();

	return 0;
}


///////////////////////////////////////////

//ע��
int CRegTask::Run()
{

	CTcpClient Client;
	Client.ConnectServer();
	//Client.SetSocketFD(m_hSock);
	char *pRecvData = NULL;
	long lRecvDataLen = 0;
	if (Client.SendDataToServer(&m_stRegPack, sizeof(m_stRegPack)))
	{
		Client.RecvDataFromServer((void **)&pRecvData, &lRecvDataLen);
		if (pRecvData)
		{
			if (((REG_RETURN_PACK_T *)pRecvData)->uFunID == REG_ID)
			{
				TCHAR szRet[512] = { 0 };
				CWriteDailyLog::AnsiToUnicode(((REG_RETURN_PACK_T *)pRecvData)->ret, szRet, 512 * sizeof(TCHAR));
				AfxMessageBox(szRet);
				Client.ReleaseRecvDataOut();
			}
		}

	}
	else
	{
		AfxMessageBox(TEXT("����æ�����Ժ����ԣ�"));
	}

	return 0;
}



//ע��
int CModPwdTask::Run()
{

	CTcpClient Client;
	Client.ConnectServer();
	//Client.SetSocketFD(m_hSock);
	char *pRecvData = NULL;
	long lRecvDataLen = 0;
	if (Client.SendDataToServer(&m_stModPwdPack, sizeof(m_stModPwdPack)))
	{
		Client.RecvDataFromServer((void **)&pRecvData, &lRecvDataLen);
		if (pRecvData)
		{
			if (((REG_RETURN_PACK_T *)pRecvData)->uFunID == MODIFY_PWD_ID)
			{
				TCHAR szRet[512] = { 0 };
				CWriteDailyLog::AnsiToUnicode(((REG_RETURN_PACK_T *)pRecvData)->ret, szRet, 512 * sizeof(TCHAR));
				AfxMessageBox(szRet);
				Client.ReleaseRecvDataOut();
			}
		}
		else
		{
			AfxMessageBox(TEXT("����æ�����Ժ����ԣ�"));
		}

	}
	else
	{
		AfxMessageBox(TEXT("����æ�����Ժ����ԣ�"));
	}

	return 0;
}


//GET����URL�ַ���
//#define HTTP_GET_API_STR				("GET %s HTTP/1.1\r\nHost:%s\r\nAccept:*/*\r\n"\
//	"User-Agent:Mozilla/4.0 (compatible; MSIE 6.00; Windows NT 5.1)\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nConnection:Keep-Alive\r\n\r\n")
//#define HTTP_GET_API_STR				("GET %s HTTP/1.1\r\nHost:%s\r\nContent-Type: image/png\r\nAccept: */*\r\n"\
//	"User-Agent:Mozilla/4.0 (compatible; MSIE 6.00; Windows NT 5.1)\r\nPragma: no-cache\r\nCache-Control: no-cache\r\nConnection:Keep-Alive\r\n\r\n")
/*
//��Ӫ�÷�����API
#define GAME_SERVER_API					"http://wan.360.cn/game/sxjol"
#define DOWN_FILE_NAME					"390187956.html"
//#define GAME_SERVER_API					"/"

//�޸�����
int CModPwdTask::Run()
{

	CHttpClient Client;
	Client.RefreshConfig((char *)CTDXMFrame::GetInstance()->m_strServerAddr.c_str(), strlen((char *)CTDXMFrame::GetInstance()->m_strServerAddr.c_str()), CTDXMFrame::GetInstance()->m_ServerPort, true);
	Client.ConnectServer();
	//Client.SetSocketFD(m_hSock);
	char *pRecvData = NULL;
	long lRecvDataLen = 0;

	char cData[2048] = { 0 };
	sprintf(cData, HTTP_GET_API_STR, GAME_SERVER_API, CTDXMFrame::GetInstance()->m_strServerAddr.c_str());
	if (Client.SendDataToServer(cData, strlen(cData)))
	{
		Client.RecvDataFromServer((void **)&pRecvData, &lRecvDataLen);
		if (pRecvData)
		{
			if (((REG_RETURN_PACK_T *)pRecvData)->uFunID == MODIFY_PWD_ID)
			{
				//д��־
				//CWriteDailyLog::WriteLog(pRecvData, lRecvDataLen, "RecvResult.txt");
				//CWriteDailyLog::SaveDataAsFile(pRecvData, lRecvDataLen, "cod.png");

				TCHAR szRet[512] = { 0 };
				CWriteDailyLog::AnsiToUnicode(((REG_RETURN_PACK_T *)pRecvData)->ret, szRet, 512 * sizeof(TCHAR));
				AfxMessageBox(szRet);
				Client.ReleaseRecvDataOut();
			}
		}
		else
		{
			AfxMessageBox(TEXT("����æ�����Ժ����ԣ�"));
		}

	}
	else
	{
		AfxMessageBox(TEXT("����æ�����Ժ����ԣ�"));
	}

	CHttpClient dowload;
	string strHttpLink;
	strHttpLink += "http://";
	strHttpLink += (char *)CTDXMFrame::GetInstance()->m_strServerAddr.c_str();
	strHttpLink += "/";
	dowload.DownLoadFile(GAME_SERVER_API, strlen(GAME_SERVER_API), DOWN_FILE_NAME, strlen(DOWN_FILE_NAME), CTDXMFrame::GetInstance()->m_ServerPort);

	return 0;
}*/