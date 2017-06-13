#ifndef INI_FILE_IO_H_		//IniFileIO
#define INI_FILE_IO_H_

#pragma once

#include"CommonModulHead.h"
#include<Windows.h>
#include<tchar.h>

#define INI_FILE_NAME_LEN		512
class COMMON_MODUL_CLASS CIniFileIO
{
public:
	CIniFileIO();
	virtual ~CIniFileIO(){}

	//文件路径
public:
	//设置路径(绝对路径并附上文件名)
	BOOL SetFilePath(LPTSTR szFilePath);
	//获取路径
	LPTSTR GetFilePath();

	//文件IO
public:
	//获取字符串
	LPTSTR ReadString(LPTSTR pszItem, LPTSTR pszSubItem, LPTSTR pszDefault, LPTSTR pszString, WORD wMaxCount);
	//获取整型值
	UINT ReadInt(LPTSTR pszItem, LPTSTR pszSubItem, INT nDefault);
	//写入字符串
	BOOL WriteString(LPTSTR lpAppName, LPTSTR lpKeyName, LPTSTR lpString, LPTSTR lpFileName);

	//文件操作
protected:
	//文件不存在则创建
	BOOL CreateFile(LPTSTR lpFileName);
	//删除文件
	BOOL DeleteFile(LPTSTR lpFileName);
	//UnicodeToAnsi
	BOOL UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte = NULL);
	//AnsiToUnicode
	BOOL AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);


protected:
	TCHAR  m_strFilePath[INI_FILE_NAME_LEN];
};
#endif