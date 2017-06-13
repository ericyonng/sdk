/*********************************
/*	FileName: Public.h
/*	Auther:	Feng Yanchen
/*	Creation Date: 2016-05-04
/*	Date Updated : 2016-05-04
*********************************/

#ifndef DAILY_LOG_H_
#define DAILY_LOG_H_

#pragma once 
#include"CommonModulHead.h"

//#include <tchar.h>
//#include<stdio.h>

//#include<windows.h>
//#include<WinNT.h>

//#ifndef _TCHAR_DEFINED
//typedef WCHAR TCHAR, *PTCHAR;
//#endif

//Function: int to TCHAR


//��׼ʱ��ṹ��
#define TIME_STR_LEN	256
typedef struct stStandardTime{
	char TimeStr[TIME_STR_LEN];		//��ʽ��2016.10.29 ����1 24:50:11
	int iYear;						//��
	int iMonth;						//��
	int	iDay;						//��
	int iWorkDay;					//���ڼ�
	int iHour;						//ʱ
	int iMinute;					//��
	int iSecond;					//��
	int iMillisecond;				//����
}STANDARD_TIME_T;
#define S_STANDARD_TIME_T	sizeof(STANDARD_TIME_T)

#define DEFAULT_LogName		"UsrDeafaultLog.txt"		//Ĭ�ϵ���־��
//#define DEFAULT_CURRENT_PATH			".\\"					//Ĭ�ϵ�·��
///////////////////////////////////////////////////////////////////////

//���ݲ���
#define BYTE_UNIT_BITS		0x8											//���ֽ�����λ��
#define GET_WDATA_HIGH_BYTE(x)	((x&0x0FFFF)>>BYTE_UNIT_BITS)&0x0FF		//ȡWORD���ݵĸ��ֽ�
#define GET_WDATA_LOW_BYTE(x)	(x&0x0FFFF)&0x0FF						//ȡWORD���ݵĵ��ֽ�

///////////////////////////////////////////


//д��־
class  COMMON_MODUL_CLASS CWriteDailyLog
{
public:
	CWriteDailyLog(){}
	~CWriteDailyLog(){}

	//��ȡ��������ʱ���ַ���
	static void GetStandardTime(STANDARD_TIME_T *stExTime);

//#ifdef _UNICODE
	static bool WriteLog(LPTSTR szString,int iStrByte, LPTSTR szFileName, bool iDefrence);
//#endif // _UNICODE
		
	static bool WriteLog(char *String, int iStrByte, char *FileName);

	static bool SaveDataAsFile(char *String, int iStrByte, char *FileName);

//#ifdef _UNICODE
	//�ַ�ת��
	static bool UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte = NULL);

	//�ַ�ת��
	static bool AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);
//#endif
	static bool UTF8toUnicode(char *pUTF8StrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);
	
	//�ļ�api
public:
	//ɾ���ļ�
	static void delfile(char *path);

	//�����ļ���������
	static bool SetFileHide(const TCHAR * lpName);

	//�����ļ�����������
	static bool SetDirHide(const TCHAR * lpName);

	//�����ļ���
	static bool CreateDir(CString masterdir, CString subdir);

	//�����ļ���
	static bool CreateSubDir(string strDir);

	//�ݹ鴴����Ŀ¼
	static bool CreateRecursiveSubDir(string strMasterDir, string strSubDir);

	//�ݹ鴴��Ŀ¼
	static bool CreateRecursiveDir(string strAbsolutePath);

	//���ļ�
	static FILE *OpenFile(string strDir, string strFileName, char *pOpenType = "rb+", bool bCursorRetHead = false);
	
	//���ļ�
	static FILE *OpenFileWithoutPath(string strFileName, char *pOpenType = "rb+", bool bCursorRetHead = false);

	//�����ļ�
	static bool CopyFIle(string strSrcFile, string strDestFile);

	//��ȡһ��
	static int ReadOneLine(void *pBuffer, long lReadDataLen, string strFileName, FILE **fpFileOut = NULL);

	//��ȡ�ļ�
	static int ReadFile(void *pBuffer, long lReadDataLen, string strFileName, FILE **fpFileOut = NULL, bool bCursorRetHead=false);

	//д���ļ�
	static int WriteFile(void *pBuffer, long lWriteDataLen, string strFileName, FILE **fpFileOut = NULL, bool bCursorRetHead=false);

	//�ر��ļ�
	static bool CloseFile(FILE *fpFile);

	//��ȡ��ǰĿ¼
	static bool GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount);

	//�ļ��Ƿ����
	static bool IsFileExist(string strFileName);

	//·���н�ȡ�ļ�
	static  string GetFileNameInPath(string strPath);


};

#endif