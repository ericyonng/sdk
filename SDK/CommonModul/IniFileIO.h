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

	//�ļ�·��
public:
	//����·��(����·���������ļ���)
	BOOL SetFilePath(LPTSTR szFilePath);
	//��ȡ·��
	LPTSTR GetFilePath();

	//�ļ�IO
public:
	//��ȡ�ַ���
	LPTSTR ReadString(LPTSTR pszItem, LPTSTR pszSubItem, LPTSTR pszDefault, LPTSTR pszString, WORD wMaxCount);
	//��ȡ����ֵ
	UINT ReadInt(LPTSTR pszItem, LPTSTR pszSubItem, INT nDefault);
	//д���ַ���
	BOOL WriteString(LPTSTR lpAppName, LPTSTR lpKeyName, LPTSTR lpString, LPTSTR lpFileName);

	//�ļ�����
protected:
	//�ļ��������򴴽�
	BOOL CreateFile(LPTSTR lpFileName);
	//ɾ���ļ�
	BOOL DeleteFile(LPTSTR lpFileName);
	//UnicodeToAnsi
	BOOL UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte = NULL);
	//AnsiToUnicode
	BOOL AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);


protected:
	TCHAR  m_strFilePath[INI_FILE_NAME_LEN];
};
#endif