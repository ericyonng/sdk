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


//标准时间结构体
#define TIME_STR_LEN	256
typedef struct stStandardTime{
	char TimeStr[TIME_STR_LEN];		//格式：2016.10.29 星期1 24:50:11
	int iYear;						//年
	int iMonth;						//月
	int	iDay;						//日
	int iWorkDay;					//星期几
	int iHour;						//时
	int iMinute;					//分
	int iSecond;					//秒
	int iMillisecond;				//毫秒
}STANDARD_TIME_T;
#define S_STANDARD_TIME_T	sizeof(STANDARD_TIME_T)

#define DEFAULT_LogName		"UsrDeafaultLog.txt"		//默认的日志名
//#define DEFAULT_CURRENT_PATH			".\\"					//默认的路径
///////////////////////////////////////////////////////////////////////

//数据操作
#define BYTE_UNIT_BITS		0x8											//单字节数据位数
#define GET_WDATA_HIGH_BYTE(x)	((x&0x0FFFF)>>BYTE_UNIT_BITS)&0x0FF		//取WORD数据的高字节
#define GET_WDATA_LOW_BYTE(x)	(x&0x0FFFF)&0x0FF						//取WORD数据的低字节

///////////////////////////////////////////


//写日志
class  COMMON_MODUL_CLASS CWriteDailyLog
{
public:
	CWriteDailyLog(){}
	~CWriteDailyLog(){}

	//获取年月日秒时间字符串
	static void GetStandardTime(STANDARD_TIME_T *stExTime);

//#ifdef _UNICODE
	static bool WriteLog(LPTSTR szString,int iStrByte, LPTSTR szFileName, bool iDefrence);
//#endif // _UNICODE
		
	static bool WriteLog(char *String, int iStrByte, char *FileName);

	static bool SaveDataAsFile(char *String, int iStrByte, char *FileName);

//#ifdef _UNICODE
	//字符转换
	static bool UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte = NULL);

	//字符转换
	static bool AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);
//#endif
	static bool UTF8toUnicode(char *pUTF8StrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);
	
	//文件api
public:
	//删除文件
	static void delfile(char *path);

	//设置文件隐藏属性
	static bool SetFileHide(const TCHAR * lpName);

	//设置文件夹隐藏属性
	static bool SetDirHide(const TCHAR * lpName);

	//创建文件夹
	static bool CreateDir(CString masterdir, CString subdir);

	//创建文件夹
	static bool CreateSubDir(string strDir);

	//递归创建子目录
	static bool CreateRecursiveSubDir(string strMasterDir, string strSubDir);

	//递归创建目录
	static bool CreateRecursiveDir(string strAbsolutePath);

	//打开文件
	static FILE *OpenFile(string strDir, string strFileName, char *pOpenType = "rb+", bool bCursorRetHead = false);
	
	//打开文件
	static FILE *OpenFileWithoutPath(string strFileName, char *pOpenType = "rb+", bool bCursorRetHead = false);

	//拷贝文件
	static bool CopyFIle(string strSrcFile, string strDestFile);

	//读取一行
	static int ReadOneLine(void *pBuffer, long lReadDataLen, string strFileName, FILE **fpFileOut = NULL);

	//读取文件
	static int ReadFile(void *pBuffer, long lReadDataLen, string strFileName, FILE **fpFileOut = NULL, bool bCursorRetHead=false);

	//写入文件
	static int WriteFile(void *pBuffer, long lWriteDataLen, string strFileName, FILE **fpFileOut = NULL, bool bCursorRetHead=false);

	//关闭文件
	static bool CloseFile(FILE *fpFile);

	//获取当前目录
	static bool GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount);

	//文件是否存在
	static bool IsFileExist(string strFileName);

	//路径中截取文件
	static  string GetFileNameInPath(string strPath);


};

#endif