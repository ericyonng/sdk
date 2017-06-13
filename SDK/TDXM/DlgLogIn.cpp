#include "stdafx.h"
#include"DlgLogIn.h"
#include "Resource.h"
#include"DlgRegAccount.h"
#include"DlgModifyAccount.h"
//#include"writelog.h"
//#include"SocketProtocol.h"
#include<iostream>
#include<string>
#include"TDXMWnd.h"

#include"Task.h"
//#include"ThreadPool.h"
using namespace std;

#include"./endecrypt/tdxmdll.h"
#include"./endecrypt/jni_md.h"
#include"./endecrypt/jni.h"
#include"./endecrypt/game_dlltool_TdxmDllImpl.h"
//#include"TcpSocket.h"


//控件名
const TCHAR * const szBtnMinControlName = TEXT("BtnMin");
const TCHAR * const szBtnCloseControlName = TEXT("BtnClose");
const TCHAR * const szBtnLongInControlName = TEXT("BtnLogIn");
const TCHAR * const szBtnRegisterControlName = TEXT("BtnRegister");
const TCHAR * const szBtnModifyControlName = TEXT("BtnModifyPwd");
const TCHAR * const szLabelSvrStatusControlName = TEXT("labelstatus");

typedef jstring (JNICALL *comm_encrypt_data_fun)(JNIEnv *, jobject, jint, jstring, jstring, jlong);
//////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgLogIn, CYCDuiDialog)
END_MESSAGE_MAP()

CDlgLogIn *CDlgLogIn::m_pLogIn = NULL;

CDlgLogIn::CDlgLogIn() :CYCDuiDialog(IDD_DLG_LOGIN)
{
	m_PngDlgBack.LoadImage(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
	m_hEncryptDll = NULL;
	ZeroMemory(m_Seed_NO, MONITOR_SEED_LEN*sizeof(DWORD));

	m_pLogIn = this;

}

CDlgLogIn:: ~CDlgLogIn()
{ 
}
	//消息解释
BOOL CDlgLogIn::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}
	//创建函数
BOOL CDlgLogIn::OnInitDialog()
{
	__super::OnInitDialog();

	//设置字体
	m_PaintManager.SetDefaultFont(TEXT("宋体"),12, false, false, false);

	//设置窗口
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("天地玄门-登陆"));

	//设置大小
	SetWindowPos(NULL, 0, 0, m_PngDlgBack.GetWidth(), m_PngDlgBack.GetHeight(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);

	//居中窗口
	CenterWindow(this);

	CRgn rgnWind;
	CRect rcClient;
	GetWindowRect(&rcClient);
	rgnWind.CreateRoundRectRgn(8, 0, m_PngDlgBack.GetWidth(), m_PngDlgBack.GetHeight() - 8 + 1, 12, 12);

	//显示窗口
	SetWindowRgn(rgnWind, FALSE);

	//链接服务器
	OnLinkServer();

	return TRUE;
}

	//初始控件
void CDlgLogIn::InitControlUI()
{
	__super::InitControlUI();

	//初始化控件
}

//消息提醒
void CDlgLogIn::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(), szBtnMinControlName) == 0)
		{
			ShowWindow(SW_MINIMIZE);
			return;
		}
		else if (lstrcmp(pControlUI->GetName(), szBtnCloseControlName) == 0)
		{

			return OnCancel();
		}
		else if (lstrcmp(pControlUI->GetName(), szBtnLongInControlName) == 0)
		{
			return OnOK();
		}
		else if (lstrcmp(pControlUI->GetName(), szBtnRegisterControlName) == 0)
		{
			return OnRegister();
		}
		else if (lstrcmp(pControlUI->GetName(), szBtnModifyControlName) == 0)
		{
			return OnModifyAccount();
		}
	}
}

//确定函数
VOID CDlgLogIn::OnOK()
{
	//登陆
	OnLogIn();
}

//取消消息
VOID CDlgLogIn::OnCancel()
{
	//关闭窗口
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);
}

VOID CDlgLogIn::OnRegister()
{
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_HIDEWINDOW);
	 
	//CDlgRegAccount DlgReg;
	//DlgReg.DoModal();
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_REGISTER, 0);
}

VOID CDlgLogIn::OnModifyAccount()
{
	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_HIDEWINDOW);
	//CDlgModifyAccount DlgModify;
	//DlgModify.DoModal();

	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_MODIFY_PWD, 0);
}

VOID CDlgLogIn::OnLogIn()
{
	//打包数据
	/*MonitorMsgMoncliLogin stPacketLogin;
	ZeroMemory(&stPacketLogin, sizeof(stPacketLogin));
	stPacketLogin.msg_id = MonitorMsgMoncliLogin::MSG_ID;
	//数据加密
	//发送数据
	//等待服务器返回数据


	//打包数据包
	MonitorMsgTellSeed stTellSeed;
	ZeroMemory(&stTellSeed, sizeof(stTellSeed));
	stTellSeed.msg_id = MonitorMsgTellSeed::MSG_ID;
	GetSeedNo();
	memmove(stTellSeed.seed_no, m_Seed_NO, sizeof(DWORD)*MONITOR_SEED_LEN);
	unsigned long lDataLen = sizeof(MonitorMsgTellSeed);

	char EncrypData[2048] = { 0 };
	unsigned long EnDataLen = 2048;
	char Data[1024] = "我爱你";
	lDataLen = strlen(Data);*/


	//任务
	CTaskBase *pClient = new CSendDataToServerTask;
	CThreadPool::GetInstance()->AddTask(pClient);


	//加密传输
	//m_TcpSocket.EncryptSendData(&Data, &lDataLen, EncrypData, &EnDataLen, EncryptData);
}

//链接服务器
VOID CDlgLogIn::OnLinkServer()
{
	//链接服务器
	//m_TcpSocket.StartNetService();

	//链接任务
	CTaskBase *pConnectTask = new CConnectServerTask(this, &m_TcpSocket);
	CThreadPool::GetInstance()->AddTask(pConnectTask);

	/*
	if (m_TcpSocket.InitSocketEnvironment())
	{
		m_TcpSocket.CreateSocket();
		unsigned short port = 2762;
		m_TcpSocket.SetAddrInfo("td.wkens.com", strlen("td.wkens.com"), port, AF_INET);
		BOOL bConnected = FALSE;
		CLabelUI *pLabelSvrStatus = (CLabelUI *)GetControlByName(szLabelSvrStatusControlName);
		try
		{
			bConnected = m_TcpSocket.Connect();
			if (bConnected)
			{
				if (pLabelSvrStatus)
				{
					CString strPacket;
					char rd[8] = { 0x04, 0x05, 0x02,0x01, 0x11, 0x12, 0x13, 0x14 };
					ACCESS_LAYER_T *pAccess = NULL;
					pAccess = (ACCESS_LAYER_T *)rd;
					unsigned short sizePacket = pAccess->usPacketSize;
					unsigned char PacketCount = pAccess->ucPacketNum;
					unsigned char PacketSeq = pAccess->ucPacketSeq;
					unsigned int iTransID = pAccess->uTransID;
				//	strPacket.Format(TEXT("total：%d, head:%d, access:%d,business:%d, tail:%d rd 16进制：%x"), S_SOCKET_PACKET_T
				//		, S_SOCKET_PACK_HEAD_T, S_ACCESS_LAYER_T, S_BUSINESS_LOGIC_LAYER_T, S_SOCKET_PACK_TAIL, dd);
					strPacket.Format(TEXT("sizePacket：%hu, PacketCount:%u, PacketSeq:%u,iTransID:%u, ACCESS_LAYER_T:%d"), sizePacket
						, PacketCount, PacketSeq, iTransID, S_ACCESS_LAYER_T);
					AfxMessageBox(strPacket);
					pLabelSvrStatus->SetText(TEXT("服务器链接成功！"));
				}
			}
			else
			{
				if (pLabelSvrStatus)
				{
					pLabelSvrStatus->SetText(TEXT("服务器链接失败！"));
				}
			}
		}
		catch (...)
		{
			if (pLabelSvrStatus)
			{
				pLabelSvrStatus->SetText(TEXT("程序异常！"));
			}
		}
	}
	*/
	//发送程序核实

}

//产生seed_no
VOID CDlgLogIn::GetSeedNo()
{
	ZeroMemory(m_Seed_NO, MONITOR_SEED_LEN*sizeof(DWORD));

	DWORD dwSerilNO[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	srand((time(NULL) % 1000) * 100);

	for (int i = 0; i < MONITOR_SEED_LEN; i++)
	{
		m_Seed_NO[i] = dwSerilNO[rand() % 10];
	}	
}

//加密方法
BOOL CDlgLogIn::EncryptData(VOID *pData, unsigned long *lDataLen, VOID *pEncryptData, unsigned long *lEnDataLen)
{
	try
	{
		//加载dll
		//HINSTANCE hTdxmDll = LoadLibrary(TEXT("tdxmdll.dll")); 
		//HINSTANCE hTdxmDll = LoadLibrary(TDXM_DE_ENCRYPT_DLL_MODULE_FILE);
		HINSTANCE hTdxmDll = LoadLibrary(TEXT("Qt5Network.dll"));
		if (!hTdxmDll)
		{
			AfxMessageBox(TEXT("加载 tdxmdll.dll 失败"));
			return FALSE;
		}
		
		comm_encrypt_data_fun enDataFun = (comm_encrypt_data_fun)::GetProcAddress(hTdxmDll, ("Jstr_encrypt2_reversible"));
		 
		
		if (!enDataFun)
		{
			AfxMessageBox(TEXT("查询 comm_encrypt_data_fun失败"));
			return FALSE;
		}
		AfxMessageBox(TEXT("查询 comm_encrypt_data_fun323342"));
		enDataFun(NULL, NULL, *lDataLen, (jstring)pData, (jstring)pEncryptData, *lEnDataLen);
		string strView = "原文：";
		strView += (char *)pData;
		TCHAR szRizhi[1024] = { 0 };
		CWriteDailyLog::AnsiToUnicode((char *)strView.c_str(), szRizhi, 2048);
		AfxMessageBox(szRizhi);

		strView.clear();
		strView += "密文：";
		strView += (char *)pEncryptData;
		CWriteDailyLog::AnsiToUnicode((char *)strView.c_str(), szRizhi, 2048);
		AfxMessageBox(szRizhi);
		return TRUE;
		
	}
	catch (...)
	{

		AfxMessageBox(TEXT("程序异常"));
		return FALSE;
	}

	return TRUE;
}

//链接结果
VOID CDlgLogIn::SetLinkResult(LPTSTR lpStr)
{
	CLabelUI *pLabelSvrStatus = (CLabelUI *)GetControlByName(szLabelSvrStatusControlName);
	if (pLabelSvrStatus)
	{
		pLabelSvrStatus->SetText(lpStr);
	}
}

