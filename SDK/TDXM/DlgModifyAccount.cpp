#include"stdafx.h"
#include"DlgModifyAccount.h"
#include"Resource.h"
//#include"../�ṹ����/BusinessLayerProtocol.h"
#include"Task.h"
#include"TDXMWnd.h"

//�ؼ���
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
	//�رմ��� 
	//if (m_hWnd)
	//	{
	//if (IsWindow(m_hWnd)) 
	//	DestroyWindow();
	//	}
}
//��Ϣ����
BOOL CDlgModifyAccount::PreTranslateMessage(MSG * pMsg)
{

	return __super::PreTranslateMessage(pMsg);
}
//��������
BOOL CDlgModifyAccount::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	m_PaintManager.SetDefaultFont(TEXT("����"), 12, false, false, false);

	//���ô���
	ModifyStyle(0, WS_MINIMIZEBOX);
	ModifyStyle(0, WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("�������-�޸�����"));

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
void CDlgModifyAccount::InitControlUI()
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


	//���볤������
	

	//��ʼ���ؼ�
}

//��Ϣ����
void CDlgModifyAccount::Notify(TNotifyUI &  msg)
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
			else if (lstrcmp(pControlUI->GetName(), szEditNewPwdControlName) == 0)
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
			else if (lstrcmp(pControlUI->GetName(), szEditOldPwdControlName) == 0)
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
			}//end of if
		}//end of for
	}//end of if
}

//ȷ������
VOID CDlgModifyAccount::OnOkModify()
{
	//У������

	//�˻�
	if (lstrlen(GetControlByName(szEditAccountControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("�û�������Ϊ�գ�"));
		return;
	}
	TCHAR szAccount[ACCOUNT_LEN] = { 0 };
	_sntprintf(szAccount, CountArray(szAccount), TEXT("%s"), GetControlByName(szEditAccountControlName)->GetText());

	//ԭ����
	if (lstrlen(GetControlByName(szEditOldPwdControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("ԭ���벻��Ϊ�գ�"));
		return;
	}

	TCHAR szPwd[PWD_LEN] = { 0 };
	_sntprintf(szPwd, CountArray(szPwd), TEXT("%s"), GetControlByName(szEditOldPwdControlName)->GetText());

	//������
	if (lstrlen(GetControlByName(szEditNewPwdControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("�����벻��Ϊ�գ�"));
		return;
	}

	//ȷ������
	if (lstrlen(GetControlByName(szEditConfirmPwdControlName)->GetText()) == 0)
	{
		AfxMessageBox(TEXT("ȷ�����벻��Ϊ�գ�"));
		return;
	}

	//��������Ƚ�
	if (lstrcmp(GetControlByName(szEditConfirmPwdControlName)->GetText(), GetControlByName(szEditNewPwdControlName)->GetText()) != 0)
	{
		AfxMessageBox(TEXT("�������벻��ͬ��"));
		return;
	}

	TCHAR szNewPwd[PWD_LEN] = { 0 };
	_sntprintf(szNewPwd, CountArray(szNewPwd), TEXT("%s"), GetControlByName(szEditNewPwdControlName)->GetText());

	//��װ�޸�����ҵ���
	MODIFY_PWD_BODY_PACK_T stModPwdPack;
	memset(&stModPwdPack, 0, S_MODIFY_PWD_BODY_PACK_T);

	//���ܺ�
	stModPwdPack.uFunID = MODIFY_PWD_ID;

	//�û���
	char strCache[1024] = { 0 };
	CWriteDailyLog::UnicodeToAnsi(szAccount, strCache, 1024);
	memmove(stModPwdPack.account, strCache, ACCOUNT_LEN);
	stModPwdPack.account[ACCOUNT_LEN - 1] = 0;

	//������
	memset(strCache, 0, 1024);
	CWriteDailyLog::UnicodeToAnsi(szPwd, strCache, 1024);
	memmove(stModPwdPack.oldpwd, strCache, PWD_LEN);
	stModPwdPack.oldpwd[PWD_LEN - 1] = 0;

	//������
	memset(strCache, 0, 1024);
	CWriteDailyLog::UnicodeToAnsi(szNewPwd, strCache, 1024);
	memmove(stModPwdPack.newpwd, strCache, PWD_LEN);
	stModPwdPack.newpwd[PWD_LEN - 1] = 0;

	//����޸���������
	CTaskBase *pModPwdTask = new CModPwdTask(stModPwdPack);
	CThreadPool::GetInstance()->AddTask(pModPwdTask);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditNewPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditOldPwdControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);

	//__super::OnOK();
}

//ȡ����Ϣ
VOID CDlgModifyAccount::OnCancel()
{
	//�رմ���
	//PostMessage(WM_CLOSE);

	GetControlByName(szEditAccountControlName)->SetText(TEXT(""));
	GetControlByName(szEditNewPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditConfirmPwdControlName)->SetText(TEXT(""));
	GetControlByName(szEditOldPwdControlName)->SetText(TEXT(""));

	CTDXMFrame::GetInstance()->PostMessage(WM_COMMAND, IDM_LOG_IN, 0);

	//__super::OnCancel();
}