#include"stdafx.h"
#include"DlgModifyAccount.h"
#include"Resource.h"
//#include"../结构定义/BusinessLayerProtocol.h"
#include"Task.h"
#include"TDXMWnd.h"

//控件名
const TCHAR * const szBtnMinControlName = TEXT("BtnMin");
const TCHAR * const szBtnCloseControlName = TEXT("BtnClose");
const TCHAR * const szBtnOkControlName = TEXT("BtnOk");
const TCHAR * const szBtnCancelControlName = TEXT("BtnCancel");
const TCHAR * const szEditAccountControlName = TEXT("EditAccount");
const TCHAR * const szEditNewPwdControlName = TEXT("EditNewPwd");
const TCHAR * const szEditConfirmPwdControlName = TEXT("EditConfirmPwd");
const TCHAR * const szEditOldPwdControlName = TEXT("EditOldPwd");
const TCHAR * const szLabelWarnControlName = TEXT("LabelWarn");
//////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgModifyAccount, CYCDuiDialog)
END_MESSAGE_MAP()
CDlgModifyAccount::CDlgModifyAccount() :CYCDuiDialog(IDD_DLG_LOGIN)
{
	m_PngDlgBack.LoadImage(AfxGetInstanceHandle(), TEXT("BACK_GROUND"));
}

CDlgModifyAccount:: ~CDlgModifyAccount()
{
	//关闭窗口 
	//if (m_hWnd)
	//	{
	//if (IsWindow(m_hWnd)) 
	//	DestroyWindow();
	//	}
}
//消息解释
BOOL CDlgModifyAccount::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}
//创建函数
BOOL CDlgModifyAccount::OnInitDialog()
{
	__super::OnInitDialog();

	//设置字体
	m_PaintManager.SetDefaultFont(TEXT("宋体"), 12, false, false, false);

	//设置窗口
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("天地玄门-修改密码"));

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
void CDlgModifyAccount::InitControlUI()
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

	pControl = GetControlByName(szEditNewPwdControlName);
	if (pControl)
	{
		((CEditUI *)pControl)->SetMaxChar(PWD_LEN - 10);
	}

	pControl = GetControlByName(szEditConfirmPwdControlName);
	if (pControl)
	{
		((CEditUI *)pControl)->SetMaxChar(PWD_LEN - 10);
	}

	pControl = GetControlByName(szEditOldPwdControlName);
	if (pControl)
	{
		((CEditUI *)pControl)->SetMaxChar(PWD_LEN - 10);
	}


	//密码长度限制
	

	//初始化控件
}

//消息提醒
void CDlgModifyAccount::Notify(TNotifyUI &  msg)
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
			return OnOkModify();
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
			else if (lstrcmp(pControlUI->GetName(), szEditNewPwdControlName) == 0)
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
			else if (lstrcmp(pControlUI->GetName(), szEditOldPwdControlName) == 0)
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
			}//end of if
		}//end of for
	}//end of if
}

//确定函数
VOID CDlgModifyAccount::OnOkModify()
{
	//校验输入

	//账户
	if (lstrlen(GetControlByName(szEditAccountControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("用户名不能为空！"));
		return;
	}
	TCHAR szAccount[ACCOUNT_LEN] = { 0 };
	_sntprintf(szAccount, CountArray(szAccount), TEXT("%s"), GetControlByName(szEditAccountControlName)->GetText());

	//原密码
	if (lstrlen(GetControlByName(szEditOldPwdControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("原密码不能为空！"));
		return;
	}

	TCHAR szPwd[PWD_LEN] = { 0 };
	_sntprintf(szPwd, CountArray(szPwd), TEXT("%s"), GetControlByName(szEditOldPwdControlName)->GetText());

	//新密码
	if (lstrlen(GetControlByName(szEditNewPwdControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("新密码不能为空！"));
		return;
	}

	//确认密码
	if (lstrlen(GetControlByName(szEditConfirmPwdControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("确认密码不能为空！"));
		return;
	}

	//两次密码比较
	if (lstrcmp(GetControlByName(szEditConfirmPwdControlName)->GetText(), GetControlByName(szEditNewPwdControlName)->GetText()) != 0)
	{
		AfxMessageBox(TEXT("两次密码不相同！"));
		return;
	}

	TCHAR szNewPwd[PWD_LEN] = { 0 };
	_sntprintf(szNewPwd, CountArray(szNewPwd), TEXT("%s"), GetControlByName(szEditNewPwdControlName)->GetText());

	//组装修改密码业务包
	MODIFY_PWD_BODY_PACK_T stModPwdPack;
	memset(&stModPwdPack, 0, S_MODIFY_PWD_BODY_PACK_T);

	//功能号
	stModPwdPack.uFunID = MODIFY_PWD_ID;

	//用户名
	char strCache[1024] = { 0 };
	CWriteDailyLog::UnicodeToAnsi(szAccount, strCache, 1024);
	memmove(stModPwdPack.account, strCache, ACCOUNT_LEN);
	stModPwdPack.account[ACCOUNT_LEN - 1] = 0;

	//旧密码
	memset(strCache, 0, 1024);
	CWriteDailyLog::UnicodeToAnsi(szPwd, strCache, 1024);
	memmove(stModPwdPack.oldpwd, strCache, PWD_LEN);
	stModPwdPack.oldpwd[PWD_LEN - 1] = 0;

	//新密码
	memset(strCache, 0, 1024);
	CWriteDailyLog::UnicodeToAnsi(szNewPwd, strCache, 1024);
	memmove(stModPwdPack.newpwd, strCache, PWD_LEN);
	stModPwdPack.newpwd[PWD_LEN - 1] = 0;

	//添加修改密码任务
	CTaskBase *pModPwdTask = new CModPwdTask(stModPwdPack);
	CThreadPool::GetInstance()->AddTask(pModPwdTask);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditNewPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditOldPwdControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);

	//__super::OnOK();
}

//取消消息
VOID CDlgModifyAccount::OnCancel()
{
	//关闭窗口
	//PostMessage(WM_CLOSE);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditNewPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditOldPwdControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);

	//__super::OnCancel();
}