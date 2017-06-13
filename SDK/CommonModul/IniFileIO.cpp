#include"stdafx.h"
#include"IniFileIO.h"
#include<io.h>
#include<stdio.h>

CIniFileIO::CIniFileIO()
{
	memset(m_strFilePath, 0, sizeof(TCHAR)* INI_FILE_NAME_LEN);
}

//设置路径(绝对路径并附上文件名)
BOOL CIniFileIO::SetFilePath(LPTSTR szFilePath)
{
	//设置变量
	memset(m_strFilePath, 0, sizeof(TCHAR)* INI_FILE_NAME_LEN);
	_sntprintf(m_strFilePath, INI_FILE_NAME_LEN, TEXT("%s"), szFilePath);
	m_strFilePath[INI_FILE_NAME_LEN - 1] = 0;

	return TRUE; 
}

//获取路径
LPTSTR CIniFileIO::GetFilePath()
{
	return m_strFilePath;
}

//获取字符串
LPTSTR CIniFileIO::ReadString(LPTSTR pszItem, LPTSTR pszSubItem, LPTSTR pszDefault, LPTSTR pszString, WORD wMaxCount)
{
	if (lstrlen(m_strFilePath) == 0) return NULL;

	//读取字符
	GetPrivateProfileString(pszItem, pszSubItem, pszDefault, pszString, wMaxCount, m_strFilePath);
	return pszString;
}

//获取整型值
UINT CIniFileIO::ReadInt(LPTSTR pszItem, LPTSTR pszSubItem, INT nDefault)
{
	if (lstrlen(m_strFilePath) == 0) return 0;

	//读取数值
	UINT uReadData = GetPrivateProfileInt(pszItem, pszSubItem, nDefault, m_strFilePath);

	return uReadData;
}

//写入字符串
BOOL CIniFileIO::WriteString(LPTSTR lpAppName, LPTSTR lpKeyName, LPTSTR lpString, LPTSTR lpFileName)
{ 
	if(!CreateFile(lpFileName)) return FALSE;
	return WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
}

//文件不存在则创建
BOOL CIniFileIO::CreateFile(LPTSTR lpFileName)
{
	char cFile[INI_FILE_NAME_LEN] = { 0 };

	//字符串转换
	if (!UnicodeToAnsi(lpFileName, cFile, INI_FILE_NAME_LEN)) return FALSE;

	//判断钥匙文件是否存在
	if (_access(cFile, 0) != 0)
	{
		//文件不存在
		FILE *fp = fopen(cFile, "w+");//创建文件
		if (fp)
		{
			fclose(fp);
			return TRUE;
		}
	}

	return TRUE;
}

//删除文件
BOOL CIniFileIO::DeleteFile(LPTSTR lpFileName)
{
	char cFile[INI_FILE_NAME_LEN] = { 0 };

	//字符串转换
	UnicodeToAnsi(lpFileName, cFile, INI_FILE_NAME_LEN);

	char str[INI_FILE_NAME_LEN] = { 0 };
	sprintf(str, "del %s", cFile);
	system(str);//执行删除

	return TRUE;
}

//字符转换
BOOL CIniFileIO::UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte)
{
	//参数校验
	if (szUnicodeSrc == NULL || pAnsiStrOut == NULL || nAnsiStrSize < 0) return FALSE;

	ZeroMemory(pAnsiStrOut, nAnsiStrSize);
	INT nLength = 0;

	//获取转换所需字节数 
	nLength = WideCharToMultiByte(CP_ACP, 0, szUnicodeSrc, -1, NULL, 0, NULL, NULL);

	//转换空间合法性判断
	if (nLength > nAnsiStrSize)
	{
		//所需的字节数
		if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nLength;

		return FALSE;
	}

	INT nTransFormResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)szUnicodeSrc, -1, pAnsiStrOut, nAnsiStrSize, NULL, NULL);//Unicode转ANSI，第一个参数当前系统ANSI,第二个参数是默认0，最后第二个是不能转换用于替换的字符，最后一个是默认true

	if (nTransFormResult == 0) return FALSE;

	//转换字节数返回
	if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nTransFormResult;

	return TRUE;
}


//字符转换
BOOL CIniFileIO::AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail)
{
	//参数校验
	if (pAnsiStrIn == NULL || szUnicodeStrOut == NULL || nUnicodeCacheSize <= 0) return FALSE;

	//清空输出
	ZeroMemory(szUnicodeStrOut, nUnicodeCacheSize);

	//获取转换所需宽字符个数
	INT nLength = 0;
	nLength = MultiByteToWideChar(CP_ACP, 0, pAnsiStrIn, -1, NULL, 0);	//ccWideByte为0计算所需个数

	//转换空间合法性判断
	if (nLength*sizeof(TCHAR) > nUnicodeCacheSize)
	{
		//所需宽字符个数
		if (nUnicodeStrLenOutCalculateZeroTail)	*nUnicodeStrLenOutCalculateZeroTail = nLength;

		return FALSE;
	}

	//转换
	INT nTransFormResult = MultiByteToWideChar(CP_ACP, 0, pAnsiStrIn, -1, szUnicodeStrOut, nUnicodeCacheSize);
	if (nTransFormResult == 0) return FALSE;

	//转换字节数返回
	if (nUnicodeStrLenOutCalculateZeroTail)	*nUnicodeStrLenOutCalculateZeroTail = nTransFormResult;

	return TRUE;
}
