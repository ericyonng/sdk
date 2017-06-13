#include"stdafx.h"
#include"Task.h"
#include"HttpDownLoaderWnd.h"



//����ƽ̨΢�Žӿ�����ת����΢��ƽ̨�����ά�����ӣ�
//����ִ����
int CHttpWxApiAnalysisTask::Run()
{
	//����
	CHttpDownLoaderDlg *pDownLoaderDlg = (CHttpDownLoaderDlg *)m_pArg;
	if (!pDownLoaderDlg) return 0;

	//��ʾ
	CDlgPopUp DlgStatus(pDownLoaderDlg);
	DlgStatus.m_strViewText = "���ض�ά���У����Ե�...";
	DlgStatus.Create(106, pDownLoaderDlg);
	DlgStatus.ShowWindow(SW_SHOW);
	DlgStatus.UpdateWindow();

	//�����ļ���
	//CWriteDailyLog::CreateDirectory(".\\tmp\\");
	//system("mkdir .\\tmp\\");

	//����api��Ϣ
	CHttpClient client;
	string strpath = ".\\tmp\\";
	strpath += "jasdfsdfk.html";
	if (client.DownLoadFile((char *)strWxApiLink.c_str(), strWxApiLink.length()*sizeof(char), (char *)strpath.c_str(), strpath.length(), 80, true))
	{
		//CWriteDailyLog::WriteLog("���سɹ�", strlen("���سɹ�"), "wxapi.txt");
		//CWriteDailyLog::WriteLog((char *)client.m_strHttpData.c_str(), client.m_strHttpData.length(), "wxapi.txt");
		client.ReleaseRecvDataOut();
	}

	//�������
	string str;
	int iKeyRet = string::npos, iValueStart = string::npos, iValueEnd = string::npos;

	//���������ά������ m_strQrCode strQrCode
	m_strQrCode = "http://open.weixin.qq.com/connect/qrconnect?";
	string strRedirect_uri;
	int istrWxLoginRet = client.m_strHttpData.find("var obj = new WxLogin", 0);
	if (istrWxLoginRet != string::npos)
	{
		//����
		
		//��id
		iKeyRet=client.m_strHttpData.find("id:", istrWxLoginRet + strlen("var obj = new WxLogin"));
		if (iKeyRet == string::npos) return 0;
		iValueStart = client.m_strHttpData.find("\"", iKeyRet + strlen("id:"));
		if (iValueStart == string::npos) return 0;
		iValueEnd = client.m_strHttpData.find("\"", iValueStart + strlen("\""));
		if (iValueEnd == string::npos) return 0;
		//CWriteDailyLog::WriteLog("id:", strlen("id:"), "wxapi.txt");
	//	CWriteDailyLog::WriteLog((char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str(), iValueEnd - iValueStart - 1, "wxapi.txt");//��ӡid

		//��appid
		iKeyRet = client.m_strHttpData.find("appid:", iValueEnd + strlen("\""));
		if (iKeyRet == string::npos) return 0;
		iValueStart = client.m_strHttpData.find("\"", iKeyRet + strlen("appid:"));
		if (iValueStart == string::npos) return 0;
		iValueEnd = client.m_strHttpData.find("\"", iValueStart + strlen("\""));
		if (iValueEnd == string::npos) return 0;
		//CWriteDailyLog::WriteLog("appid:", strlen("appid:"), "wxapi.txt");
		//CWriteDailyLog::WriteLog((char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str(), iValueEnd - iValueStart - 1, "wxapi.txt");//��ӡappid
		m_strQrCode += "appid=";
		m_strQrCode += (char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str();

		//��scope
		iKeyRet = client.m_strHttpData.find("scope:", iValueEnd + strlen("\""));
		if (iKeyRet == string::npos) return 0;
		iValueStart = client.m_strHttpData.find("\"", iKeyRet + strlen("scope:"));
		if (iValueStart == string::npos) return 0;
		iValueEnd = client.m_strHttpData.find("\"", iValueStart + strlen("\""));
		if (iValueEnd == string::npos) return 0;
		//CWriteDailyLog::WriteLog("scope:", strlen("scope:"), "wxapi.txt");
	//	CWriteDailyLog::WriteLog((char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str(), iValueEnd - iValueStart - 1, "wxapi.txt");//��ӡappid
		m_strQrCode += "&scope=";
		m_strQrCode += (char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str();

		//��redirect_uri
		iKeyRet = client.m_strHttpData.find("redirect_uri:", iValueEnd + strlen("\""));
		if (iKeyRet == string::npos) return 0;
		iValueStart = client.m_strHttpData.find("\"", iKeyRet + strlen("redirect_uri:"));
		if (iValueStart == string::npos) return 0;
		iValueEnd = client.m_strHttpData.find("\"", iValueStart + strlen("\""));
		if (iValueEnd == string::npos) return 0;
		//CWriteDailyLog::WriteLog("redirect_uri:", strlen("redirect_uri:"), "wxapi.txt");
		//CWriteDailyLog::WriteLog((char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str(), iValueEnd - iValueStart - 1, "wxapi.txt");//��ӡappid
		strRedirect_uri = (char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str();
		m_strQrCode += "&redirect_uri=";
		m_strQrCode += (char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str();

		//��state
		iKeyRet = client.m_strHttpData.find("state:", iValueEnd + strlen("\""));
		if (iKeyRet == string::npos) return 0;
		iValueStart = client.m_strHttpData.find("\"", iKeyRet + strlen("state:"));
		if (iValueStart == string::npos) return 0;
		iValueEnd = client.m_strHttpData.find("\"", iValueStart + strlen("\""));
		if (iValueEnd == string::npos) return 0;
		//CWriteDailyLog::WriteLog("state:", strlen("state:"), "wxapi.txt");
		//CWriteDailyLog::WriteLog((char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str(), iValueEnd - iValueStart - 1, "wxapi.txt");//��ӡappid
		m_strQrCode += "&state=";
		m_strQrCode += (char *)client.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str();

		//login_type
		m_strQrCode += "&login_type=jssdk";
		//m_strQrCode += (char *)m_strLoginType.c_str();
	}

	client.ClearAllDataCache();
	//CWriteDailyLog::WriteLog((char *)m_strQrCode.c_str(), m_strQrCode.length()*sizeof(char), "strCode.txt");

	//����΢��ƽ̨��ά��apiҳ��
	CHttpClient httpWxPlatformGetCode;
	iKeyRet = string::npos, iValueStart = string::npos, iValueEnd = string::npos;
	if (httpWxPlatformGetCode.DownLoadFile((char *)m_strQrCode.c_str(), m_strQrCode.length()*sizeof(char), (char *)strpath.c_str(), strpath.length()*sizeof(char), 80, true))
	{
		//CWriteDailyLog::WriteLog("���سɹ�", strlen("���سɹ�"), "wxapi0.txt");
		//CWriteDailyLog::WriteLog((char *)client.m_strHttpData.c_str(), client.m_strHttpData.length(), "wxapi0.txt");

		//��ȡ��ά������
		string strCodeImageLink="http://open.weixin.qq.com";
		iKeyRet = httpWxPlatformGetCode.m_strHttpData.find("<div class=\"wrp_code\">", 0);
		if (iKeyRet == string::npos) return 0;
		iKeyRet = httpWxPlatformGetCode.m_strHttpData.find("class=\"qrcode lightBorder\"", iKeyRet + strlen("<div class=\"wrp_code\">"));
		if (iKeyRet == string::npos) return 0;
		iKeyRet = httpWxPlatformGetCode.m_strHttpData.find("src=", iKeyRet + strlen("class=\"qrcode lightBorder\""));
		if (iKeyRet == string::npos) return 0;
		iValueStart = httpWxPlatformGetCode.m_strHttpData.find("\"", iKeyRet + strlen("src="));
		if (iValueStart == string::npos) return 0;
		iValueEnd = httpWxPlatformGetCode.m_strHttpData.find("\"", iValueStart + strlen("\""));
		if (iValueEnd == string::npos) return 0;
		//CWriteDailyLog::WriteLog("��ά������:", strlen("��ά������:"), "wxapi.txt");
		//CWriteDailyLog::WriteLog((char *)httpWxPlatformGetCode.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str(), iValueEnd - iValueStart-1, "wxapi.txt");//��ӡid
		strCodeImageLink += (char *)httpWxPlatformGetCode.m_strHttpData.substr(iValueStart + 1, iValueEnd - iValueStart - 1).c_str();
		
		//��ȡ��ά������
		int iCodeNameStart = string::npos, iCodeNameEnd = string::npos;
		iCodeNameStart = strCodeImageLink.find("/connect/qrcode/", 0);
		if (iCodeNameStart == string::npos) return 0;
		iCodeNameStart += strlen("/connect/qrcode/");
		iCodeNameEnd = strCodeImageLink.length() - 1;
		//CWriteDailyLog::WriteLog("��ά������:", strlen("��ά������:"), "wxapi.txt");
		//CWriteDailyLog::WriteLog((char *)strCodeImageLink.substr(iCodeNameStart, iCodeNameEnd - iCodeNameStart+1).c_str(), iCodeNameEnd - iCodeNameStart+1, "wxapi.txt");//��ӡid
		


		//���ض�ά��
		CHttpClient CodeImageClient;
		CodeImageClient.UseDataCache();
		CodeImageClient.DownLoadFile((char *)strCodeImageLink.c_str(), strCodeImageLink.length()*sizeof(char), "code.png", strlen("code.png"),80, true);

		//�ر���ʾ
		DlgStatus.SendMessage(WM_CLOSE);

		/*CFile f;
		f.Open(L"code.png", CFile::modeRead);
		f.SeekToBegin();
		f.SeekToEnd();
		int size = f.GetPosition();
		f.SeekToBegin();
		BYTE *buf = new BYTE[size];
		f.Read(buf, size);
		f.Close();

		CString strname;
		RECT r;
		int w = 470;
		int h = 470;

		r.left = 10;
		r.top = 10;*/
		

		//��ʾ��ά��
		CDlgPopUp DlgCode(pDownLoaderDlg);
		
		//DlgCode.m_strCodeBitImageStream.clear(); 
		//DlgCode.m_strCodeBitImageStream = (char *)CodeImageClient.m_strHttpData.c_str();
	//	CxImage *pOrgimage = new CxImage((BYTE *)buf, CodeImageClient.m_ulHttpDataSize, 0);   // ����һ��CxImage���� 
		
		//char rizhid[100] = { 0 };
		//sprintf(rizhid, "���ݴ�С��%d", CodeImageClient.m_ulHttpDataSize);
	//	CWriteDailyLog::WriteLog(rizhid, strlen(rizhid), "pngshuju.txt");
		//if (buf) delete[]buf;
		//DlgCode.RefreshCodeBitmap((char *)DlgCode.m_strCodeBitImageStream.c_str());
		//DlgCode.LoadMemImage(pOrgimage);
		DlgCode.LoadMemImage((BYTE *)CodeImageClient.m_pHttpDataCache, CodeImageClient.m_ulHttpDataSize, CXIMAGE_FORMAT_UNKNOWN);
		//DlgCode.LoadMemImage((char *)DlgCode.m_strCodeBitImageStream.c_str(), DlgCode.m_strCodeBitImageStream.length(), CXIMAGE_FORMAT_PNG);
		//DlgCode.LoadBackImg(TEXT("code.png"));
		CRect rcClient;
		pDownLoaderDlg->GetWindowRect(&rcClient);
		DlgCode.SetWindowPos(NULL, rcClient.left, rcClient.top, 500, 500, SWP_HIDEWINDOW);
		
		//��������code���� m_strCodeBack strCodeBack
		//string strCodeBack = "http://long.open.weixin.qq.com/connect/l/qrconnect?";
		m_strCodeBack = "http://long.open.weixin.qq.com/connect/l/qrconnect?";
		m_strCodeBack += "uuid=";
		m_strCodeBack += (char *)strCodeImageLink.substr(iCodeNameStart, iCodeNameEnd - iCodeNameStart + 1).c_str();
	//	CWriteDailyLog::WriteLog((char *)m_strCodeBack.c_str(), m_strCodeBack.length(), "strCodeBack.txt");
		
		//����ɨ������ȡ������������
		CTaskBase *pListenTask = new CHttpGetPersonalInfoTask((char *)m_strCodeBack.c_str(), (char *)strRedirect_uri.c_str(), m_pArg, &DlgCode);
		GlobalThreadPool.AddTask(pListenTask);

		//��ʾ��ά��
		DlgCode.DoModal();
		//CWriteDailyLog::WriteLog("�ر�DlgCode", strlen("�ر�DlgCode"), "�ر�.txt");

		//httpWxPlatformGetCode.ReleaseRecvDataOut();
		
	}



	return 0;
}

//��ȡ������Ϣ����
//#define HTTP_GET_API_STR				("GET %s HTTP/1.1\r\nHost:%s\r\nConnection: keep-alive\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"\
//	"Upgrade - Insecure - Requests: 1\r\nUser - Agent: Mozilla / 5.0 (Windows NT 6.1; WOW64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 45.0.2454.101 Safari / 537.36\r\n"\
//	"Accept - Encoding: gzip, deflate, sdch\r\nAccept - Language: zh - CN, zh; q = 0.8\r\nCookie: ts_uid = 6780437480; pac_uid = 1_120453674; tvfe_boss_uuid = 5baf41df610bc6d9; mobileUV = 1_158b2c1648d_2c649; gaduid = 584667ea1116e; eas_sid = E1a498q1P8C5X9Y1l8f048T9r6; RK = OdlfEha + dP; pgv_pvi = 3162536960; LW_uid = B1W4E9S3H2H7I4C7F9Y6p9Z6H8; LW_sid = w1L469V3r2d7e4T7z9G9A2d8o9; pgv_pvid = 9023754364; o_cookie = 120453674; ptcz = dcdc379ec16b02dc6804a98c42766b016b5f5ed4a2f2e715c408a4b67e82a163; pt2gguin = o0120453674\r\n\r\n")

#define HTTP_GET_API_STR				("GET %s HTTP/1.1\r\nHost:%s\r\nConnection: keep-alive\r\nAccept: */*\r\n"\
	"Upgrade - Insecure - Requests: 1\r\nUser - Agent: Mozilla / 5.0 (Windows NT 6.1; WOW64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 45.0.2454.101 Safari / 537.36\r\n"\
	"Accept - Encoding: gzip, deflate, sdch\r\nAccept - Language: zh - CN, zh; q = 0.8\r\nCookie: ts_uid = 6780437480; pac_uid = 1_120453674; tvfe_boss_uuid = 5baf41df610bc6d9; mobileUV = 1_158b2c1648d_2c649; gaduid = 584667ea1116e; eas_sid = E1a498q1P8C5X9Y1l8f048T9r6; RK = OdlfEha + dP; pgv_pvi = 3162536960; LW_uid = B1W4E9S3H2H7I4C7F9Y6p9Z6H8; LW_sid = w1L469V3r2d7e4T7z9G9A2d8o9; pgv_pvid = 9023754364; o_cookie = 120453674; ptcz = dcdc379ec16b02dc6804a98c42766b016b5f5ed4a2f2e715c408a4b67e82a163; pt2gguin = o0120453674\r\n\r\n")


int CHttpGetPersonalInfoTask::Run()
{
	//����
	CHttpDownLoaderDlg *pDownLoaderDlg = (CHttpDownLoaderDlg *)m_pArg;
	if (!pDownLoaderDlg) return 0;

	CDlgPopUp *pCodeWnd = (CDlgPopUp *)m_pCodeWnd;
	if (!pCodeWnd) return 0;

	//��������
	
	int iRet = string::npos, iCodeStart = string::npos, iCodeEnd = string::npos;
	long lLinkHeadPos = string::npos, lHostEndPos = string::npos, lRecvDataLen = 0;
	char *pRecvData = NULL;
	struct hostent* stpRemote = NULL;
	struct in_addr addr;

	CHttpClient client;
	unsigned long lTimeEscape = 1494302477619;
	char szTimeEscape[128] = { 0 };
	char szGetRequestStr[8096] = { 0 };
	string strLink, strHost, strWxApiLink;
	bool bSuccessScan=false;
	bool bFirstConnect = true;
	bool bFirstGetInfo = true;
	bool bFinishMonitor = false;
	while (!bFinishMonitor)
	{
		iRet = string::npos, iCodeStart = string::npos, iCodeEnd = string::npos;
		
	//	client.CloseLink();

		//client.ClearAllDataCache();
		strLink.clear();
		strHost.clear();
		strWxApiLink.clear();
		strWxApiLink = m_strWxApiLink;
		strLink = (char *)strWxApiLink.c_str();
		strHost;


		//ȡ������
		lLinkHeadPos = string::npos, lHostEndPos = string::npos;
		lLinkHeadPos = strLink.find("http://", 0);			//��httpͷ
		if (lLinkHeadPos == string::npos) return FALSE;

		lHostEndPos = strLink.find("/", lLinkHeadPos + strlen("http://"));		//��������βб��/
		if (lHostEndPos == string::npos) return FALSE;

		//��ȡ����
		//strHost = (char *)strLink.substr(lLinkHeadPos, lHostEndPos - lLinkHeadPos).c_str();
		strHost.assign(strLink, lLinkHeadPos + strlen("http://"), lHostEndPos - (lLinkHeadPos + strlen("http://")));
		//CWriteDailyLog::WriteLog((char *)strHost.c_str(), strHost.length(), "InfoTask.txt");

		memset(szGetRequestStr, 0, 8096);
		memset(szTimeEscape, 0, 128);
		sprintf(szTimeEscape, "&_=%lu", lTimeEscape);
		szTimeEscape[127] = 0;
		strWxApiLink += szTimeEscape;	//����ʱ
		lTimeEscape++;
		//sprintf(szGetRequestStr, HTTP_GET_API_STR, (char *)strWxApiLink.c_str(), "long.open.weixin.qq.com");
		sprintf(szGetRequestStr, HTTP_GET_API_STR, (char *)strWxApiLink.substr(strWxApiLink.find("/connect/", 0), strWxApiLink.length() - strWxApiLink.find("/connect/", 0)).c_str(), (char *)strHost.c_str());
		//CWriteDailyLog::WriteLog(szGetRequestStr, strlen(szGetRequestStr), "InfoTask.txt");

		//����
		pRecvData = NULL;
		lRecvDataLen = 0;

		stpRemote = NULL;
		memset(&addr, 0, sizeof(struct in_addr));
		//stpRemote = gethostbyname("long.open.weixin.qq.com");			//��ȡԶ��������Ϣ
		stpRemote = gethostbyname((char *)strHost.c_str());			//��ȡԶ��������Ϣ
		addr.s_addr = *(u_long *)stpRemote->h_addr_list[0];			//ip��ַ
		//CWriteDailyLog::WriteLog((char *)"ip:", strlen("ip:"), "InfoTask.txt");
	//	CWriteDailyLog::WriteLog((char *)inet_ntoa(addr), strlen(inet_ntoa(addr)), "InfoTask.txt");

		//���õ�ַ�˿ڵ�
		//client.RefreshConfig((char *)strHost.c_str(), strlen((char *)strHost.c_str()), 80, true);
		client.RefreshConfig((char *)strHost.c_str(), strlen((char *)strHost.c_str()), 80, true);

		//����
		if (bFirstConnect || bSuccessScan)
		{
			//if (bFirstGetInfo)
			{
				//if (bSuccessScan) bFirstGetInfo = false;
				//bFirstConnect = false;
				client.ConnectServer();
			}
		}
		//client.ConnectServer();

		//����ᵼ�»�ȡʧ�� ��Ѷ����������ˮ��������
		if (bSuccessScan)
		{
			Sleep(1000);	//����CODEʱ����
		}
		else
		{
			Sleep(100);		//���ɨ�����ʱ����
		}

		//��������
		if (client.SendDataToServer(szGetRequestStr, strlen(szGetRequestStr)))
		{
			//CWriteDailyLog::WriteLog("��ǰʱ��1", strlen("��ǰʱ��1"), "time.txt");
			//BOOL bRet = client.RecvDataFromServer((void **)&pRecvData, &lRecvDataLen, bSuccessScan?10:2, 0, 1);
			BOOL bRet = client.RecvDataFromServer((void **)&pRecvData, &lRecvDataLen, 1, 0, 1);
			//CWriteDailyLog::WriteLog("��ǰʱ��2", strlen("��ǰʱ��2"), "time.txt");
		//	char rizhi1[100] = { 0 };
		//	sprintf(rizhi1, "get���ص�״̬��:%ld", client.m_HttpBackStatusCode);
		//	CWriteDailyLog::WriteLog(rizhi1, strlen(rizhi1), "InfoTask.txt");

			//if (bRet)
			//{
			//	if (!m_strHttpReturnHeader.empty())+
			//	{
			//		CWriteDailyLog::WriteLog((char *)m_strHttpReturnHeader.c_str(), m_strHttpReturnHeader.length(), "httpHeadInfo.txt");
			//	}
			//}

			if (pRecvData&&client.m_HttpBackStatusCode == HTTP_REQUEST_SUCCESS_RET_CODE)
			{
					//�����ļ�
					//CWriteDailyLog::SaveDataAsFile(pRecvData, lRecvDataLen, "InfoTask.txt");
					//���ص��ڴ�
					if (!client.m_strHttpData.empty()) client.m_strHttpData.clear();

					client.m_strHttpData = pRecvData;

				//�ͷŻ���
					client.ReleaseRecvDataOut();

					if (!client.m_strHttpData.empty())
					{
						iRet = string::npos, iCodeStart = string::npos, iCodeEnd = string::npos;
						//CWriteDailyLog::WriteLog("code����", strlen("code����"), "strCodeBack.txt");
						//CWriteDailyLog::WriteLog((char *)client.m_strHttpData.c_str(), client.m_strHttpData.length(), "strCodeBack.txt");

						//�ж�ɨ��״̬ 408��ʾδɨ�裬404��ʾɨ�����
						if (!bSuccessScan)
						{
							if ((iRet = client.m_strHttpData.find("window.wx_errcode=", 0)) != string::npos)
							{
								iCodeStart = client.m_strHttpData.find(";", iRet + strlen("window.wx_errcode="));
								if ((iRet + strlen("window.wx_errcode="))<iCodeStart)
								{
								//	CWriteDailyLog::WriteLog((char *)client.m_strHttpData.substr(iRet + strlen("window.wx_errcode="), iCodeStart - (iRet + strlen("window.wx_errcode="))).c_str(), strlen((char *)client.m_strHttpData.substr(iRet + strlen("window.wx_errcode="), iCodeStart - (iRet + strlen("window.wx_errcode="))).c_str()), "strCodeBack.txt");
									//ɨ��ɹ�״̬�ı�
									if (strcmp("404", (char *)client.m_strHttpData.substr(iRet + strlen("window.wx_errcode="), iCodeStart - (iRet + strlen("window.wx_errcode="))).c_str()) == 0)
									{
									//	CWriteDailyLog::WriteLog("404", strlen("404"), "404.txt");
										pCodeWnd->SetScanResult(true, &bFinishMonitor);
										bSuccessScan = true;
										//pCodeWnd->Invalidate(FALSE);
										//pCodeWnd->ShowWindow(SW_SHOW);
										//pCodeWnd->UpdateWindow();
									}
									else
									{
										//pCodeWnd->ReleaseScanStatusCacheBuffer();
									}
								}
							}
						}
	

						iRet = string::npos, iCodeStart = string::npos, iCodeEnd = string::npos;

						if ((iRet = client.m_strHttpData.find("window.wx_code=", 0)) != string::npos)
						{
						//	CWriteDailyLog::WriteLog("�ҵ�wx_code", strlen("�ҵ�wx_code"), "wx_code.txt");
						//	CWriteDailyLog::WriteLog((char *)client.m_strHttpData.c_str(), client.m_strHttpData.length(), "wx_code.txt");

							iCodeStart = client.m_strHttpData.find("\'", iRet + strlen("window.wx_code=")) + 1;
							iCodeEnd = client.m_strHttpData.find("\'", iCodeStart);
							if (iCodeEnd - iCodeStart > 0)
							{
								//CWriteDailyLog::WriteLog("׼����ȡ��������", strlen("׼����ȡ��������"), "strCodeBack.txt");

							//	CWriteDailyLog::WriteLog((char *)client.m_strHttpData.substr(iCodeStart, iCodeEnd - iCodeStart).c_str(), client.m_strHttpData.substr(iCodeStart, iCodeEnd - iCodeStart).length(), "strCodeBack.txt");
								
								//string strPersonalInfoLink = "http://www.7879game.com/IntApi/WxApi.aspx?action=client";
								m_strRedirect_uri += "&code=";
								m_strRedirect_uri += (char *)client.m_strHttpData.substr(iCodeStart, iCodeEnd - iCodeStart).c_str();
								m_strRedirect_uri += "&state=wx@7879Game";

								//CWriteDailyLog::WriteLog((char *)m_strRedirect_uri.c_str(), m_strRedirect_uri.length(), "strCodeBack.txt");

								CHttpClient PersonalInfoClient;
							//	PersonalInfoClient.CloseLink();
								PersonalInfoClient.DownLoadFile((char *)m_strRedirect_uri.c_str(), m_strRedirect_uri.length()*sizeof(char), "codetext2.txt", strlen("codetext2.txt"), 80, true);

								//CWriteDailyLog::WriteLog((char *)PersonalInfoClient.m_strHttpData.c_str(), PersonalInfoClient.m_strHttpData.length(), "strCodeBack.txt");

								//�������ϴ�
								CDlgPopUp DlgPersonalInfo;

								TCHAR *pszText = new TCHAR[PersonalInfoClient.m_strHttpData.length() * 2 + 2];
								if (pszText)
								{
									//CWriteDailyLog::WriteLog("PersonalInfoClient", strlen("PersonalInfoClient"), "strCodeBack.txt");
									memset(pszText, 0, PersonalInfoClient.m_strHttpData.length() * 2 + 2);
									CWriteDailyLog::AnsiToUnicode((char *)PersonalInfoClient.m_strHttpData.c_str(), pszText, PersonalInfoClient.m_strHttpData.length() * 2 + 2);
									DlgPersonalInfo.m_strViewText = pszText;								
									delete[]pszText;
									
								}
								


								//��ά�봰�ڹر�
								pCodeWnd->SendMessage(WM_CLOSE);

								DlgPersonalInfo.Create(106);
								CRect rcRect;
								pDownLoaderDlg->GetWindowRect(&rcRect);
								DlgPersonalInfo.SetWindowPos(NULL, rcRect.left, rcRect.top, 600, 600, SWP_HIDEWINDOW);
								DlgPersonalInfo.DoModal();
								//pDownLoaderDlg->m_PopupPersonalInfo.ShowWindow(SW_SHOW);
								//pDownLoaderDlg->m_PopupPersonalInfo.UpdateWindow();
								//pDownLoaderDlg->m_PopupPersonalInfo.SetForegroundWindow();
								//pDownLoaderDlg->m_PopupPersonalInfo.BringWindowToTop();
								//pDownLoaderDlg->m_PopupPersonalInfo.DoModal();
								//CWriteDailyLog::WriteLog("�ر�m_PopupPersonalInfo", strlen("�ر�m_PopupPersonalInfo"), "�ر�.txt");
								return 0;
								//http://www.7879game.com/IntApi/WxApi.aspx?action=client&state=wx@7879Game
							}
						}
					}
			}
			else
			{
				//if (pRecvData) 			//����ʧ��
				//	CWriteDailyLog::WriteLog("������", strlen("������"), "log.txt");
				//�ͷŻ���
				client.ReleaseRecvDataOut();

			}
		}
		else
		{

		}

	

			
	}
	//1.����get����
	//2.���շ���ҳ
	//3.����wx_code����
	//4.��wx_code�����������code����������ϣ���������ʾ������ѭ����������

	return 0;
}

//����http��������
int CHttpListenTask::Run()
{
	//ʵ������
	CHttpDownLoaderDlg *pDownLoaderDlg = (CHttpDownLoaderDlg *)m_pArg;
	if (!pDownLoaderDlg) return 0;

	char *pData = NULL;
	long lDataLen = 0;
	BOOL bRet = FALSE;
/*	while (true)
	{
		pData = NULL;
		lDataLen = 0;
		pDownLoaderDlg->m_httpdown.ClearAllDataCache();
		pDownLoaderDlg->m_httpdown.RecvDataFromServer((void **)&pData, &lDataLen);
		if (bRet)
		{
			CWriteDailyLog::WriteLog("�ɹ�����", strlen("�ɹ�����"), "HttpDown.txt");
		}
		else
		{
			CWriteDailyLog::WriteLog("����ʧ��", strlen("����ʧ��"), "HttpDown.txt");
		}
		
		if (pData)
		{
			CWriteDailyLog::WriteLog((char *)pData, lDataLen, "HttpDown.txt");
			pDownLoaderDlg->m_httpdown.ReleaseRecvDataOut();
		}
		Sleep(50);
	}*/

	return 0;
}


int CHttpDownloadTask::Run()
{
	try
	{
		//return 0;
		//CTcpSocket Client2;
		//return 0;
		CHttpClient Client;

		bool bFinishFlowCount = false;
		CTaskBase *pDataFlowCountTask = new CDataFlowCount(
			&(Client.GetEffectDataFlowCount()->lTotalFlowSize),
			&(Client.GetEffectDataFlowCount()->lCurrentFlowCount), &bFinishFlowCount, m_pArg);
		CThreadPool::GetInstance()->AddTask(pDataFlowCountTask);

		//return 0;
		BOOL bRet = FALSE;
		bRet = Client.DownLoadFile((char *)m_strLink.c_str(), m_strLink.length(), (char *)m_strPath.c_str(), m_strPath.length(), m_port);
		
		//����
		bFinishFlowCount = true;
		Sleep(1000);

		//return 0;

		//httpͷ��Ϣ
		if (m_bSaveHttpHeaderInfo)
		{
			if (!Client.m_strHttpReturnHeader.empty())
			{
				m_strPath += ".header";
				CWriteDailyLog::SaveDataAsFile((char *)Client.m_strHttpReturnHeader.c_str(), Client.m_strHttpReturnHeader.length(), (char *)m_strPath.c_str());
			}
		}

		if (!bRet)
		{
			AfxMessageBox(TEXT("����ʧ�ܣ�"));
			return 0;
		}
		AfxMessageBox(TEXT("���سɹ���"));

	}
	catch (...)
	{
		char rizhi[100] = { 0 };
		sprintf(rizhi, "CHttpDownloadTask::Run() �쳣�� %d", GetLastError());
		CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "�쳣.txt");
	}

	return 0;
}

//����ִ����
int CDataFlowCount::Run()
{
	//����У��
	if (!m_pArg || !m_plTotalSize || !m_plCurrentRecvByte || !m_pbFinish) return 0;
	CHttpDownLoaderDlg *pLoaderDlg = (CHttpDownLoaderDlg *)m_pArg;

	try
	{
		while (!(*m_pbFinish))
		{
			Sleep(1000);
			pLoaderDlg->RefreshFlowCount(*m_plCurrentRecvByte, *m_plTotalSize);
		}
	}
	catch (...)
	{
		char rizhi[100] = { 0 };
		sprintf(rizhi, "CDataFlowCount::Run() �쳣�� %d", GetLastError());
		CWriteDailyLog::WriteLog(rizhi, strlen(rizhi), "�쳣.txt");
	}


	return 0;
}