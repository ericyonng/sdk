#include"stdafx.h"
#include "DlgRegAccount.h"
#include"Resource.h"
//#include"../�ṹ����/BusinessLayerProtocol.h"
#include"Task.h"
#include"TDXMWnd.h"

//�ؼ���
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
	//�رմ��� 
	//if (m_hWnd)
//	{
		//if (IsWindow(m_hWnd)) 
		//	DestroyWindow();
//	}
}
//��Ϣ����
BOOL CDlgRegAccount::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}
//��������
BOOL CDlgRegAccount::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	m_PaintManager.SetDefaultFont(TEXT("����"), 12, false, false, false);

	//���ô���
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�������-ע��"));

	//���ô�С
	SetWindowPos(NULL, 0, 0, m_PngDlgBack.GetWidth(), m_PngDlgBack.GetHeight(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);

	//���д���
	CenterWindow(this);

	CRgn rgnWind;
	CRect rcClient;
	GetWindowRect(&rcClient);
	rgnWind.CreateRoundRectRgn(8, 0, m_PngDlgBack.GetWidth(), m_PngDlgBack.GetHeight() - 8 + 1, 12, 12);

	//��ʾ����
	SetWindowRgn(rgnWind, FALSE);

	return TRUE;
}

//��ʼ�ؼ�
void CDlgRegAccount::InitControlUI()
{
	__super::InitControlUI();
	 
	//�˻���������
	CControlUI *pControl = NULL;

	//�û���
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

	//��ʼ���ؼ�
}

//��Ϣ����
void CDlgRegAccount::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
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
						pLabelControl->SetText(TEXT("���û���ֻ��������,Ӣ�ĺ��»��ߵ����"));
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
						pLabelControl->SetText(TEXT("������ֻ��������,Ӣ�ĺ��»��ߵ����"));
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
						pLabelControl->SetText(TEXT("������ֻ��������,Ӣ�ĺ��»��ߵ����"));
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
						pLabelControl->SetText(TEXT("����ʽ����Email��ʽ�磺abc123@tiandixuanmen.com"));
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

//ȷ������
VOID CDlgRegAccount::OnOkReg()
{
	CLabelUI *pLabelControl = NULL;
	pLabelControl = (CLabelUI *)GetControlByName(szLabelWarnControlName);
	if (!pLabelControl) return;
	//ע��

	//����У��

	//�˻�
	if (lstrlen(GetControlByName(szEditAccountControlName)->GetText()) == 0)
	{
		pLabelControl->SetText(TEXT("�û�������Ϊ��!"));
		return;
	}
	TCHAR szAccount[ACCOUNT_LEN] = { 0 };
	_sntprintf(szAccount, CountArray(szAccount), TEXT("%s"), GetControlByName(szEditAccountControlName)->GetText());

	//����
	if (lstrlen(GetControlByName(szEditPwdControlName)->GetText()) == 0
		|| lstrlen(GetControlByName(szEditConfirmPwdControlName)->GetText()) == 0)
	{
		pLabelControl->SetText(TEXT("���벻��Ϊ��!"));
		return;
	}

	//email
	if (lstrlen(GetControlByName(szEditEmailControlName)->GetText()) == 0)
	{
		pLabelControl->SetText(TEXT("email����Ϊ��!"));
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
			pLabelControl->SetText(TEXT("����ʽ����Email��ʽ�磺abc123@tiandixuanmen.com"));
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
			pLabelControl->SetText(TEXT("����ʽ����Email��ʽ�磺abc123@tiandixuanmen.com"));
			pLabelControl->SetTextColor(ARGB(0, 255, 0, 0));
		}
		((CEditUI *)pEmailControl)->SetSel(0, pEmailControl->GetText().GetLength() - 1);
		return;
	}

	TCHAR szEmail[EMAIL_LEN] = { 0 };
	_sntprintf(szEmail, CountArray(szEmail), TEXT("%s"), GetControlByName(szEditEmailControlName)->GetText());

	//У���ʺźϷ���
//	TCHAR szTemp[1024] = { 0 };
	//memmove(szTemp, GetControlByName(szEditAccountControlName)->GetText(), lstrlen(GetControlByName(szEditAccountControlName)->GetText())*sizeof(TCHAR));
	//for (INT i = 0; i < lstrlen(szTemp); i++)
	//{
	//	if ()
	//}

	//������
	if (lstrcmp(GetControlByName(szEditPwdControlName)->GetText(), GetControlByName(szEditConfirmPwdControlName)->GetText()) != 0)
	{
		pLabelControl->SetText(TEXT("ȷ�����������벻��ͬ!"));
		return;
	}
	//

	TCHAR szPwd[PWD_LEN] = { 0 };
	_sntprintf(szPwd, CountArray(szPwd), TEXT("%s"), GetControlByName(szEditPwdControlName)->GetText());



	//ע���û�����
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

	//ע������
	CTaskBase *pRegTask = new CRegTask(stRegPack);
	CThreadPool::GetInstance()->AddTask(pRegTask);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditEmailControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);

	//ע������
	//__super::OnOK();
}

//ȡ����Ϣ
VOID CDlgRegAccount::OnCancel()
{
	//�رմ���
	//PostMessage(WM_CLOSE);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditEmailControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);
	//__super::OnCancel();
}

//ִ��ע��
BOOL CDlgRegAccount::OnExcuteReg()
{
	return TRUE;
}
