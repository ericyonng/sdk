#include"stdafx.h"
#include"IniFileIO.h"
#include<io.h>
#include<stdio.h>

CIniFileIO::CIniFileIO()
{
	memset(m_strFilePath, 0, sizeof(TCHAR)* INI_FILE_NAME_LEN);
}

//����·��(����·���������ļ���)
BOOL CIniFileIO::SetFilePath(LPTSTR szFilePath)
{
	//���ñ���
	memset(m_strFilePath, 0, sizeof(TCHAR)* INI_FILE_NAME_LEN);
	_sntprintf(m_strFilePath, INI_FILE_NAME_LEN, TEXT("%s"), szFilePath);
	m_strFilePath[INI_FILE_NAME_LEN - 1] = 0;

	return TRUE; 
}

//��ȡ·��
LPTSTR CIniFileIO::GetFilePath()
{
	return m_strFilePath;
}

//��ȡ�ַ���
LPTSTR CIniFileIO::ReadString(LPTSTR pszItem, LPTSTR pszSubItem, LPTSTR pszDefault, LPTSTR pszString, WORD wMaxCount)
{
	if (lstrlen(m_strFilePath) == 0) return NULL;

	//��ȡ�ַ�
	GetPrivateProfileString(pszItem, pszSubItem, pszDefault, pszString, wMaxCount, m_strFilePath);
	return pszString;
}

//��ȡ����ֵ
UINT CIniFileIO::ReadInt(LPTSTR pszItem, LPTSTR pszSubItem, INT nDefault)
{
	if (lstrlen(m_strFilePath) == 0) return 0;

	//��ȡ��ֵ
	UINT uReadData = GetPrivateProfileInt(pszItem, pszSubItem, nDefault, m_strFilePath);

	return uReadData;
}

//д���ַ���
BOOL CIniFileIO::WriteString(LPTSTR lpAppName, LPTSTR lpKeyName, LPTSTR lpString, LPTSTR lpFileName)
{ 
	if(!CreateFile(lpFileName)) return FALSE;
	return WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
}

//�ļ��������򴴽�
BOOL CIniFileIO::CreateFile(LPTSTR lpFileName)
{
	char cFile[INI_FILE_NAME_LEN] = { 0 };

	//�ַ���ת��
	if (!UnicodeToAnsi(lpFileName, cFile, INI_FILE_NAME_LEN)) return FALSE;

	//�ж�Կ���ļ��Ƿ����
	if (_access(cFile, 0) != 0)
	{
		//�ļ�������
		FILE *fp = fopen(cFile, "w+");//�����ļ�
		if (fp)
		{
			fclose(fp);
			return TRUE;
		}
	}

	return TRUE;
}

//ɾ���ļ�
BOOL CIniFileIO::DeleteFile(LPTSTR lpFileName)
{
	char cFile[INI_FILE_NAME_LEN] = { 0 };

	//�ַ���ת��
	UnicodeToAnsi(lpFileName, cFile, INI_FILE_NAME_LEN);

	char str[INI_FILE_NAME_LEN] = { 0 };
	sprintf(str, "del %s", cFile);
	system(str);//ִ��ɾ��

	return TRUE;
}

//�ַ�ת��
BOOL CIniFileIO::UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte)
{
	//����У��
	if (szUnicodeSrc == NULL || pAnsiStrOut == NULL || nAnsiStrSize < 0) return FALSE;

	ZeroMemory(pAnsiStrOut, nAnsiStrSize);
	INT nLength = 0;

	//��ȡת�������ֽ��� 
	nLength = WideCharToMultiByte(CP_ACP, 0, szUnicodeSrc, -1, NULL, 0, NULL, NULL);

	//ת���ռ�Ϸ����ж�
	if (nLength > nAnsiStrSize)
	{
		//������ֽ���
		if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nLength;

		return FALSE;
	}

	INT nTransFormResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)szUnicodeSrc, -1, pAnsiStrOut, nAnsiStrSize, NULL, NULL);//UnicodeתANSI����һ��������ǰϵͳANSI,�ڶ���������Ĭ��0�����ڶ����ǲ���ת�������滻���ַ������һ����Ĭ��true

	if (nTransFormResult == 0) return FALSE;

	//ת���ֽ�������
	if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nTransFormResult;

	return TRUE;
}


//�ַ�ת��
BOOL CIniFileIO::AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail)
{
	//����У��
	if (pAnsiStrIn == NULL || szUnicodeStrOut == NULL || nUnicodeCacheSize <= 0) return FALSE;

	//������
	ZeroMemory(szUnicodeStrOut, nUnicodeCacheSize);

	//��ȡת��������ַ�����
	INT nLength = 0;
	nLength = MultiByteToWideChar(CP_ACP, 0, pAnsiStrIn, -1, NULL, 0);	//ccWideByteΪ0�����������

	//ת���ռ�Ϸ����ж�
	if (nLength*sizeof(TCHAR) > nUnicodeCacheSize)
	{
		//������ַ�����
		if (nUnicodeStrLenOutCalculateZeroTail)	*nUnicodeStrLenOutCalculateZeroTail = nLength;

		return FALSE;
	}

	//ת��
	INT nTransFormResult = MultiByteToWideChar(CP_ACP, 0, pAnsiStrIn, -1, szUnicodeStrOut, nUnicodeCacheSize);
	if (nTransFormResult == 0) return FALSE;

	//ת���ֽ�������
	if (nUnicodeStrLenOutCalculateZeroTail)	*nUnicodeStrLenOutCalculateZeroTail = nTransFormResult;

	return TRUE;
}
