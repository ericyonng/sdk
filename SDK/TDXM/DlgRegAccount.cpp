#include"stdafx.h"
#include "DlgRegAccount.h"
#include"Resource.h"
//#include"../结构定义/BusinessLayerProtocol.h"
#include"Task.h"
#include"TDXMWnd.h"

//控件名
const TCHAR * const szBtnMinControlName = TEXT("BtnMin");
const TCHAR * const szBtnCloseControlName = TEXT("BtnClose");
const TCHAR * const szBtnOkControlName = TEXT("BtnOk");
const TCHAR * const szEditAccountControlName = TEXT("EditAccount");
const TCHAR * const szEditPwdControlName = TEXT("EditPwd");
const TCHAR * const szEditConfirmPwdControlName = TEXT("EditConfirmPwd");
const TCHAR * const szEditEmailControlName = TEXT("EditEmail");
const TCHAR * const szBtnCancelControlName = TEXT("BtnCancel");
const TCHAR * const szLabelWarnControlName = TEXT("LabelWarn");


//////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgRegAccount, CYCDuiDialog)
END_MESSAGE_MAP()
CDlgRegAccount::CDlgRegAccount() :CYCDuiDialog(IDD_DLG_LOGIN)
{
	m_PngDlgBack.LoadImage(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
}

CDlgRegAccount:: ~CDlgRegAccount()
{
	//关闭窗口 
	//if (m_hWnd)
//	{
		//if (IsWindow(m_hWnd)) 
		//	DestroyWindow();
//	}
}
//消息解释
BOOL CDlgRegAccount::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}
//创建函数
BOOL CDlgRegAccount::OnInitDialog()
{
	__super::OnInitDialog();

	//设置字体
	m_PaintManager.SetDefaultFont(TEXT("宋体"), 12, false, false, false);

	//设置窗口
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("天地玄门-注册"));

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

	return TRUE;
}

//初始控件
void CDlgRegAccount::InitControlUI()
{
	__super::InitControlUI();
	 
	//账户长度限制
	CControlUI *pControl = NULL;

	//用户名
	pControl = GetControlByName(szEditAccountControlName);
	if (pControl)
	{
		((CEditUI *)pControl)->SetMaxChar(ACCOUNT_LEN - 1);
	}

	pControl = GetControlByName(szEditPwdControlName);
	if (pControl)
	{
		((CEditUI *)pControl)->SetMaxChar(PWD_LEN - 10);
	}

	pControl = GetControlByName(szEditConfirmPwdControlName);
	if (pControl)
	{
		((CEditUI *)pControl)->SetMaxChar(PWD_LEN - 10);
	}

	pControl = GetControlByName(szEditEmailControlName);
	if (pControl)
	{
		((CEditUI *)pControl)->SetMaxChar(EMAIL_LEN - 1);
	}

	//初始化控件
}

//消息提醒
void CDlgRegAccount::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (lstrcmp(pControlUI->GetName(), szBtnCloseControlName) == 0
			|| lstrcmp(pControlUI->GetName(), szBtnCancelControlName) == 0)
		{

			return OnCancel();
		}
		else if (lstrcmp(pControlUI->GetName(), szBtnOkControlName) == 0)
		{
			return OnOkReg();
		}
	}
	else if (lstrcmp(msg.sType, TEXT("textchanged")) == 0)
	{
		TCHAR szString[512] = { 0 };
		TCHAR szCache[512] = { 0 };
		CLabelUI *pLabelControl = NULL;
		pLabelControl = (CLabelUI *)GetControlByName(szLabelWarnControlName);
		memmove(szString, ((CEditUI *)pControlUI)->GetText(), ((CEditUI *)pControlUI)->GetText().GetLength()*sizeof(TCHAR));
		for (INT i = 0; i < lstrlen(szString); i++)
		{
			if (lstrcmp(pControlUI->GetName(), szEditAccountControlName) == 0)
			{				
				if (!(szString[i] >= _T('a') && szString[i] <= _T('z')
					|| szString[i] >= _T('A') && szString[i] <= _T('Z')
					|| szString[i] >= _T('0') && szString[i] <= _T('9')
					|| szString[i] == _T('_')))
				{					
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT("×用户名只能是数字,英文和下划线的组合"));
						pLabelControl->SetTextColor(ARGB(0, 255, 0, 0));
					}
					memset(szCache, 0, sizeof(TCHAR)*CountArray(szCache));
					memmove(szCache, szString, i*sizeof(TCHAR));
					pControlUI->SetText(szCache);
					((CEditUI *)pControlUI)->SetSel(0, i);
					return;
				}
				else
				{
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT(""));
					}
				}
			}
			else if (lstrcmp(pControlUI->GetName(), szEditPwdControlName) == 0)
			{
				if (!(szString[i] >= _T('a') && szString[i] <= _T('z')
					|| szString[i] >= _T('A') && szString[i] <= _T('Z')
					|| szString[i] >= _T('0') && szString[i] <= _T('9')
					|| szString[i] == _T('_')))
				{
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT("×密码只能是数字,英文和下划线的组合"));
						pLabelControl->SetTextColor(ARGB(0, 255, 0, 0));
					}
					memset(szCache, 0, sizeof(TCHAR)*CountArray(szCache));
					memmove(szCache, szString, i*sizeof(TCHAR));
					pControlUI->SetText(szCache);
					((CEditUI *)pControlUI)->SetSel(0, i);
					return;
				}
				else
				{
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT(""));
					}
				}
			}
			else if (lstrcmp(pControlUI->GetName(), szEditConfirmPwdControlName) == 0)
			{
				if (!(szString[i] >= _T('a') && szString[i] <= _T('z')
					|| szString[i] >= _T('A') && szString[i] <= _T('Z')
					|| szString[i] >= _T('0') && szString[i] <= _T('9')
					|| szString[i] == _T('_')))
				{
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT("×密码只能是数字,英文和下划线的组合"));
						pLabelControl->SetTextColor(ARGB(0, 255, 0, 0));
					}
					memset(szCache, 0, sizeof(TCHAR)*CountArray(szCache));
					memmove(szCache, szString, i*sizeof(TCHAR));
					pControlUI->SetText(szCache);
					((CEditUI *)pControlUI)->SetSel(0, i);
					return;
				}
				else
				{
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT(""));
					}
				}
			}
			else if (lstrcmp(pControlUI->GetName(), szEditEmailControlName) == 0)
			{
				if (!(szString[i] >= _T('a') && szString[i] <= _T('z')
					|| szString[i] >= _T('A') && szString[i] <= _T('Z')
					|| szString[i] >= _T('0') && szString[i] <= _T('9')
					|| szString[i] == _T('@') || szString[i] == _T('.')))
				{
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT("×格式错误！Email格式如：abc123@tiandixuanmen.com"));
						pLabelControl->SetTextColor(ARGB(0, 255, 0, 0));
					}
					memset(szCache, 0, sizeof(TCHAR)*CountArray(szCache));
					memmove(szCache, szString, i*sizeof(TCHAR));
					pControlUI->SetText(szCache);
					((CEditUI *)pControlUI)->SetSel(0, i);
					return;
				}
				else
				{
					if (pLabelControl)
					{
						pLabelControl->SetText(TEXT(""));
					}
				}
			}//end of if
		}//end of for
	}//end of if

	return;
}

//确定函数
VOID CDlgRegAccount::OnOkReg()
{
	CLabelUI *pLabelControl = NULL;
	pLabelControl = (CLabelUI *)GetControlByName(szLabelWarnControlName);
	if (!pLabelControl) return;
	//注册

	//数据校验

	//账户
	if (lstrlen(GetControlByName(szEditAccountControlName)->GetText()) == 0)
	{
		pLabelControl->SetText(TEXT("用户名不能为空!"));
		return;
	}
	TCHAR szAccount[ACCOUNT_LEN] = { 0 };
	_sntprintf(szAccount, CountArray(szAccount), TEXT("%s"), GetControlByName(szEditAccountControlName)->GetText());

	//密码
	if (lstrlen(GetControlByName(szEditPwdControlName)->GetText()) == 0
		|| lstrlen(GetControlByName(szEditConfirmPwdControlName)->GetText()) == 0)
	{
		pLabelControl->SetText(TEXT("密码不能为空!"));
		return;
	}

	//email
	if (lstrlen(GetControlByName(szEditEmailControlName)->GetText()) == 0)
	{
		pLabelControl->SetText(TEXT("email不能为空!"));
		return;
	}

	TCHAR szString[512] = { 0 };
	TCHAR szCache[512] = { 0 };

	CEditUI *pEmailControl = (CEditUI *)GetControlByName(szEditEmailControlName);
	if (!pEmailControl) return;

	memmove(szString, ((CEditUI *)pEmailControl)->GetText().GetData(), ((CEditUI *)pEmailControl)->GetText().GetLength()*sizeof(TCHAR));
	if (szString[lstrlen(szString) - 1] == _T('.') || szString[lstrlen(szString) - 1] == _T('@'))
	{
		if (pLabelControl)
		{
			pLabelControl->SetText(TEXT("×格式错误！Email格式如：abc123@tiandixuanmen.com"));
			pLabelControl->SetTextColor(ARGB(0, 255, 0, 0));
		}
		memset(szCache, 0, sizeof(TCHAR)*CountArray(szCache));
		memmove(szCache, szString, (lstrlen(szString) - 1)*sizeof(TCHAR));
		pEmailControl->SetText(szCache);
		((CEditUI *)pEmailControl)->SetSel(0, lstrlen(szCache) - 1);
		return;
	}
	else if (((CEditUI *)pEmailControl)->GetText().Find(TEXT("@")) == -1)
	{
		if (pLabelControl)
		{
			pLabelControl->SetText(TEXT("×格式错误！Email格式如：abc123@tiandixuanmen.com"));
			pLabelControl->SetTextColor(ARGB(0, 255, 0, 0));
		}
		((CEditUI *)pEmailControl)->SetSel(0, pEmailControl->GetText().GetLength() - 1);
		return;
	}

	TCHAR szEmail[EMAIL_LEN] = { 0 };
	_sntprintf(szEmail, CountArray(szEmail), TEXT("%s"), GetControlByName(szEditEmailControlName)->GetText());

	//校验帐号合法性
//	TCHAR szTemp[1024] = { 0 };
	//memmove(szTemp, GetControlByName(szEditAccountControlName)->GetText(), lstrlen(GetControlByName(szEditAccountControlName)->GetText())*sizeof(TCHAR));
	//for (INT i = 0; i < lstrlen(szTemp); i++)
	//{
	//	if ()
	//}

	//两密码
	if (lstrcmp(GetControlByName(szEditPwdControlName)->GetText(), GetControlByName(szEditConfirmPwdControlName)->GetText()) != 0)
	{
		pLabelControl->SetText(TEXT("确认密码与密码不相同!"));
		return;
	}
	//

	TCHAR szPwd[PWD_LEN] = { 0 };
	_sntprintf(szPwd, CountArray(szPwd), TEXT("%s"), GetControlByName(szEditPwdControlName)->GetText());



	//注册用户任务
	REGISTER_BODY_PACK_T stRegPack;
	memset(&stRegPack, 0, sizeof(REGISTER_BODY_PACK_T));
	char strCache[1024] = { 0 };
	CWriteDailyLog::UnicodeToAnsi(szAccount, strCache, 1024);
	memmove(stRegPack.account, strCache, ACCOUNT_LEN);
	stRegPack.account[ACCOUNT_LEN - 1] = 0;

	memset(strCache, 0, 1024);
	CWriteDailyLog::UnicodeToAnsi(szEmail, strCache, 1024);
	memmove(stRegPack.email, strCache, EMAIL_LEN);
	stRegPack.email[EMAIL_LEN - 1] = 0;

	memset(strCache, 0, 1024);
	CWriteDailyLog::UnicodeToAnsi(szPwd, strCache, 1024);
	memmove(stRegPack.pwd, strCache, PWD_LEN);
	stRegPack.pwd[PWD_LEN - 1] = 0;

	stRegPack.uFunID = REG_ID;

	//注册任务
	CTaskBase *pRegTask = new CRegTask(stRegPack);
	CThreadPool::GetInstance()->AddTask(pRegTask);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditEmailControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);

	//注册任务
	//__super::OnOK();
}

//取消消息
VOID CDlgRegAccount::OnCancel()
{
	//关闭窗口
	//PostMessage(WM_CLOSE);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditEmailControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);
	//__super::OnCancel();
}

//执行注册
BOOL CDlgRegAccount::OnExcuteReg()
{
	return TRUE;
}
