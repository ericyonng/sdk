#include"stdafx.h"
#include"Task.h"
#include"DlgSdkTools.h"

//拷贝文件任务
int CCopyFilesTask::Run()
{
	CDlgSdkTools *pTools = (CDlgSdkTools *)m_pArg;
	if (!pTools) return 0;
	pTools->m_btnEnvBuild.EnableWindow(FALSE);
	pTools->m_btnCancel.EnableWindow(FALSE);
	pTools->m_btnRefreshList.EnableWindow(FALSE);

	try
	{
		bool bCopySuccess = true;
		bCopySuccess &= pTools->CopyHeaderFiles();
		bCopySuccess &= pTools->CopyLibFiles();

		if (bCopySuccess) MessageBox(pTools->GetSafeHwnd(), TEXT("拷贝成功!"), 0, 0);
	}
	catch (...)
	{
	}


	pTools->m_btnEnvBuild.EnableWindow(TRUE);
	pTools->m_btnCancel.EnableWindow(TRUE);
	pTools->m_btnRefreshList.EnableWindow(TRUE);


	return 0;
}