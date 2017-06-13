
#include"Stdafx.h"
#include<errno.h>
#include"DailyLog.h"
#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<io.h>//access函数
#include <sys/stat.h>  //_stat函数
#include<iostream>
#include<string>
#include<direct.h>	//mkdir函数
using namespace std;

//创建文件夹支持
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

/*
//计算整数的位数
INT Count_Num(INT iNumIn)
{
	INT iCount=0;
	while(1)
	{
		if((iNumIn/=10)!=0)
		{
			iCount++;
		}
		else
		{
			iCount++;
			return iCount;
		}
	}
	return 0;
}

//整型转规则字符串
VOID I_to_TCHAR(INT iNumIn, PTCHAR pNumOut, INT iLengthOut)
{
	INT iNumTmp=iNumIn, iRest=0, iNumCount=0;
	TCHAR tcTmp;
	//FILE *fp;
	//fp=fopen(".\\0.txt","a");
	//char str[100]={0};
	iNumCount=Count_Num(iNumIn);//正数位数
	for(int i=((iNumCount>iLengthOut?iLengthOut:iNumCount)-1);i>=0;i--)
	{
		//ZeroMemory(str, 100);		
		iRest=iNumTmp%10;//取最后一位数值
		tcTmp=INT_TO_TCHAR(iRest);
		//sprintf(str,"\n y/10: %d rest num: %d Count(x):%d iLength:%d X: %d y: %d\n", y/10, rest, Count_Num(x),iLength, x, y);
		//fwrite("begin rest:\n", 13, 1, fp);
		//fwrite(&tmp, sizeof(TCHAR), 1,fp);
		//fwrite(str, 100, 1,fp);
		//fwrite("end\n", 5, 1, fp);		
		CopyMemory((pNumOut+i), &tcTmp, sizeof(TCHAR));
		if((iNumTmp/10)==0){
			tcTmp=INT_TO_TCHAR(iNumTmp);
			CopyMemory((pNumOut+i), &tcTmp, sizeof(TCHAR));
			break;
		}
		else
		{
			iNumTmp=iNumTmp/10;//取剩下的数值
		}
	}
	//fflush(fp);
	//fclose(fp);
	return;
}

//非中文TCHAR 转Char字符串

void TCHAR_to_Char(LPCTSTR lpStrIn, char *pStrOut, INT iStrLenOut)
{
	ZeroMemory(pStrOut,iStrLenOut+1);//清零
	for(INT i=0;i<iStrLenOut;i++)
	{
		pStrOut[i]=lpStrIn[i];
	}
}

//中文型TCHAR转char字符串
void ZNTCHAR_to_Char(LPCTSTR lpStrIn, char *pStrOut, INT iStrLenOut)
{
	ZeroMemory(pStrOut, iStrLenOut+1);
	WideCharToMultiByte(CP_ACP, 0, lpStrIn, lstrlen(lpStrIn), pStrOut, iStrLenOut, (LPCSTR)TEXT(" "), NULL);//Unicode转ANSI，第一个参数当前系统ANSI,第二个参数是默认0，最后第二个是不能转换用于替换的字符，最后一个是默认true
}*/

/////////////////////////////////////////////////////////


//构造

//获取年月日秒时间字符串
void CWriteDailyLog::GetStandardTime(STANDARD_TIME_T *stExTime)
{
	SYSTEMTIME sys;
	GetLocalTime( &sys );
	memset(stExTime, 0, S_STANDARD_TIME_T);

	//填充时间结构体
	stExTime->iYear=(int)sys.wYear;
	stExTime->iMonth=(int)sys.wMonth;
	stExTime->iDay=(int)sys.wDay;
	stExTime->iWorkDay=(int)sys.wDayOfWeek;
	stExTime->iHour=(int)sys.wHour;
	stExTime->iMinute=(int)sys.wMinute;
	stExTime->iSecond=(int)sys.wSecond;
	stExTime->iMillisecond=(int)sys.wMilliseconds;

	//取星期字符串
	char WorkDayStr[3]={0};
	char WorkDayTmp[30]="日一二三四五六";
	memmove(WorkDayStr, &(WorkDayTmp[2*(stExTime->iWorkDay)]), 2);
	
	//填充字符串
	sprintf(stExTime->TimeStr,"%4d.%02d.%02d %02d:%02d:%02d.%03d",stExTime->iYear, stExTime->iMonth, stExTime->iDay, stExTime->iHour, stExTime->iMinute,stExTime->iSecond, stExTime->iMillisecond);
}
/*
//写日志
void CWriteDailyLog::WriteDialyLog(DAILYLOG_T stExLog)
{
	FILE *fp=NULL;
	char WholePath[256]={0};

	//打开日志文件
	//路径
	if(stExLog.Path==NULL)
	{
		strcat(WholePath, ".\\");
	}
	else
	{
		strcat(WholePath, stExLog.Path);
	}

	//文件名
	if(stExLog.FileName==NULL)
	{
		strcat(WholePath, DEFAULT_LogName);
	}
	else
	{
		strcat(WholePath, stExLog.FileName);
	}
	fp=fopen(WholePath, "a+");

	//获取时间
	GetStandardTime(&(stExLog.Time));

	fwrite("\n",strlen("\n"), 1, fp);//开始
	
	//写入数据：
	fwrite(stExLog.Time.TimeStr, strlen(stExLog.Time.TimeStr), 1, fp);//时间
	fwrite(": \n",strlen(": \n"), 1, fp);
	fwrite(stExLog.Data, stExLog.iDataLen, 1, fp);//数据
	fwrite("\n\n",strlen("\n\n"), 1, fp);//结束

	//刷新数据并关闭日志
	fflush(fp);
	fclose(fp);
}

void CWriteDailyLog::WriteDialyLog(char *strLog)
{
	if (!strLog) return;

	try
	{
		DAILYLOG_T stExLog;
		stExLog.Data = strLog;
		stExLog.iDataLen = strlen(strLog);
		WriteDialyLog(stExLog);

	}
	catch (...)
	{
	}

}*/

//#ifdef _UNICODE
bool CWriteDailyLog::WriteLog(LPTSTR szString, int iStrByte, LPTSTR szFileName, bool iDefrence)
{
	if (szString == NULL) return false;
	if (szFileName == NULL) szFileName = TEXT("DailyLog.txt");

	FILE *fp = NULL;
	char WholePath[1024] = { 0 };

	char CacheTmp[2] = { 0 };
	char *pCache = NULL;
	int iNeedSize = 0;
	//转换字符串

	//创建空间
	if (!UnicodeToAnsi(szString, CacheTmp, 0, &iNeedSize))
	{
		if (iNeedSize!=0)
			pCache = new char[iNeedSize+1];
		if (pCache) memset(pCache, 0, iNeedSize + 1);
	}

	//长度
	int iNeedSizeTmp = iNeedSize + 1;

	//字符转换
	UnicodeToAnsi(szFileName, WholePath, 1024);
	WholePath[1023] = 0;
	UnicodeToAnsi(szString, pCache, iNeedSize+1, &iNeedSize);

	//打开文件
	fp = fopen(WholePath, "a+");
	if (!fp) return false;
	
	STANDARD_TIME_T stTime;
	memset(&stTime, 0, sizeof(STANDARD_TIME_T));

	//获取时间
	GetStandardTime(&stTime);

	//写入数据：
	fwrite(stTime.TimeStr, strlen(stTime.TimeStr), 1, fp);//时间
	fwrite(":", strlen(":"), 1, fp);
	fwrite(pCache, iStrByte, 1, fp);//数据
	fwrite("\r\n", strlen("\r\n"), 1, fp);//结束

	//刷新数据并关闭日志
	fflush(fp);
	fclose(fp);

	if (pCache) delete[]pCache;

	return true;
}
//#endif

bool CWriteDailyLog::WriteLog(char *String, int iStrByte, char *FileName)
{
	if (String == NULL) return false;
	if (FileName == NULL) FileName = "DailyLog.txt";

	FILE *fp = NULL;
	

	//打开文件
	fp = fopen(FileName, "a+");
	if (!fp) return false;

	STANDARD_TIME_T stTime;
	memset(&stTime, 0, sizeof(STANDARD_TIME_T));

	//获取时间
	GetStandardTime(&stTime);

	//写入数据：
	fwrite(stTime.TimeStr, strlen(stTime.TimeStr), 1, fp);//时间
	fwrite(":", strlen(":"), 1, fp);
	fwrite(String, iStrByte, 1, fp);//数据
	fwrite("\r\n", strlen("\r\n"), 1, fp);//结束

	//刷新数据并关闭日志
	fflush(fp);
	fclose(fp);

	return true;
}

bool CWriteDailyLog::SaveDataAsFile(char *String, int iStrByte, char *FileName)
{
	if (String == NULL) return false;
	if (FileName == NULL) FileName = "DailyLog.txt";

	FILE *fp = NULL;


	//打开文件
	fp = fopen(FileName, "wb+");
	if (!fp) return false;
	rewind(fp);

	//写入数据：
	fwrite(String, iStrByte, 1, fp);//数据

	//刷新数据并关闭日志
	fflush(fp);
	fclose(fp);

	return true;
}

//删除文件
void CWriteDailyLog::delfile(char *path)
{
	//文件存在则清除
	if( access(path,0)==-1 )//文件不存在则退出
	{
		return;
	}
	char file[1024]={0};
	strcat(file, "del ");
	strcat(file, path);
	system(file);
}

//设置文件隐藏属性
bool CWriteDailyLog::SetFileHide(const TCHAR * lpName)
{
	DWORD dwResult = ::GetFileAttributes(lpName);
	if (INVALID_FILE_ATTRIBUTES == dwResult)
	{
		return false;
	}
	if (!(FILE_ATTRIBUTE_HIDDEN & dwResult)) // 如果它不是隐藏的 
	{
		if (INVALID_FILE_ATTRIBUTES == ::SetFileAttributes(lpName, dwResult | FILE_ATTRIBUTE_HIDDEN))
		{
			return false;
		}
		return true;
	}
	else// 如果它已经是隐藏的，就当设置成功了 
	{
		return true;
	}

	return true;
}

//设置文件夹隐藏属性
bool CWriteDailyLog::SetDirHide(const TCHAR * lpName)
{
	return true;
}

//创建文件夹
bool CWriteDailyLog::CreateDir(CString masterdir, CString subdir)
{
	if (!PathIsDirectory(masterdir))
	{
		//创建目录,已有的话不影响 
		if (!::CreateDirectory(masterdir, NULL))
			return false;
	}

	masterdir += subdir;

	if (!PathIsDirectory(masterdir))
	{
		//创建目录,已有的话不影响 
		if (!::CreateDirectory(masterdir, NULL))
			return false;
	}
	//mkdir("./我/");

	return true;
}

//创建文件夹
bool CWriteDailyLog::CreateSubDir(string strDir)
{
	if (strDir.length() == 0) return false;

	if (mkdir(strDir.c_str())) return false;

	return true;
}

//递归创建目录
bool CWriteDailyLog::CreateRecursiveDir(string strAbsolutePath)
{
	//提取盘符
	string strRoot, strSubPath;
	int iStartPos=strAbsolutePath.find(":", 0);
	if (iStartPos == string::npos)
	{
		strRoot = "";
		strSubPath += "\\";
		strSubPath += strAbsolutePath;
	}
	else
	{
		strRoot = strAbsolutePath.substr(iStartPos - 1, 1);
		strRoot += ":\\";
		iStartPos = strAbsolutePath.find("\\", iStartPos + strlen(":"));
		if (iStartPos == string::npos) return false;
		strSubPath = strAbsolutePath.substr(iStartPos, strAbsolutePath.length() - iStartPos);
		
	}

	return CreateRecursiveSubDir(strRoot, strSubPath);
}

//递归创建子目录
bool CWriteDailyLog::CreateRecursiveSubDir(string strMasterDir, string strSubDir)
{
	string dir = "";
	string strtocreate="";
	string strMasterPath = strMasterDir;
	int iStartPos = 0;
	int iStartPosTmp = 0;
	int iEndPos = 0;
	int iEndPosTmp = 0;
	CString strMasterUnicode, strSubUnicode;
	TCHAR szMasterUnicode[MAX_PATH] = { 0 }, szSubUnicode[MAX_PATH] = { 0 };

	int iFinalStartPos = 0, iFinalCount = 0;

	while (true)
	{
		iStartPos=strSubDir.find("\\", iStartPos);
		iStartPosTmp = strSubDir.find("/", iStartPosTmp);
		if (iStartPos == string::npos&&iStartPosTmp == string::npos) break;

		if (iStartPos == string::npos || iStartPosTmp == string::npos)
		{
			if (iStartPos != string::npos)
			{
				iEndPos = strSubDir.find("\\", iStartPos + strlen("\\"));
				if (iEndPos == string::npos) break;
				iFinalStartPos = iStartPos+strlen("\\");
				iFinalCount = iEndPos - iStartPos - strlen("\\");
				iStartPos += strlen("\\");
			}

			if (iStartPosTmp != string::npos)
			{
				iEndPosTmp = strSubDir.find("/", iStartPosTmp + strlen("/"));
				if (iEndPosTmp == string::npos) break;
				iFinalStartPos = iStartPosTmp + strlen("/");
				iFinalCount = iEndPosTmp - iStartPosTmp - strlen("/");
				iStartPosTmp += strlen("/");
			}
		}
		else if (iStartPos < iStartPosTmp)
		{
			//"\\"

			iEndPos = strSubDir.find("\\", iStartPos+strlen("\\"));
			iEndPosTmp = strSubDir.find("/", iStartPos + strlen("\\"));
			if (iEndPos == string::npos&&iEndPosTmp == string::npos) break;
			dir.clear();
			if (iEndPos == string::npos || iEndPosTmp == string::npos)
			{
				if (iEndPos != string::npos)
				{
					iFinalStartPos = iStartPos + strlen("\\");
					iFinalCount = iEndPos - iStartPos - strlen("\\");
				}
				if (iEndPosTmp != string::npos)
				{
					iFinalStartPos = iStartPos + strlen("\\");
					iFinalCount = iEndPosTmp - iStartPos - strlen("\\");
				}
			}
			else if (iEndPos < iEndPosTmp)
			{
				//"\\"
				iFinalStartPos = iStartPos + strlen("\\");
				iFinalCount = iEndPos - iStartPos - strlen("\\");
				//dir = strMasterPath + strSubDir.substr(iStartPos + strlen("\\"), iEndPos - iStartPos - strlen("\\"));
			}
			else
			{
				//"/"
				iFinalStartPos = iStartPos + strlen("\\");
				iFinalCount = iEndPosTmp - iStartPos - strlen("\\");
				//dir = strMasterPath + strSubDir.substr(iStartPos + strlen("\\"), iEndPos - iStartPos - strlen("\\"));
			}

			iStartPos += strlen("\\");
			
		}
		else
		{
			//"/"
			iFinalStartPos = iStartPosTmp + strlen("/");
			iEndPos = strSubDir.find("\\", iStartPosTmp + strlen("/"));
			iEndPosTmp = strSubDir.find("/", iStartPosTmp + strlen("/"));
			if (iEndPos == string::npos&&iEndPosTmp == string::npos) break;
			if (iEndPos == string::npos || iEndPosTmp == string::npos)
			{
				if (iEndPos != string::npos)
				{					
					iFinalCount = iEndPos - iFinalStartPos;
				}
				if (iEndPosTmp != string::npos)
				{
					iFinalCount = iEndPosTmp - iFinalStartPos;
				}
			}
			else if (iEndPos < iEndPosTmp)
			{
				//"\\"
				iFinalCount = iEndPos - iFinalStartPos;
				//dir = strMasterPath + strSubDir.substr(iStartPos + strlen("\\"), iEndPos - iStartPos - strlen("\\"));
			}
			else
			{
				//"/"
				iFinalCount = iEndPosTmp - iFinalStartPos;
				//dir = strMasterPath + strSubDir.substr(iStartPos + strlen("\\"), iEndPos - iStartPos - strlen("\\"));
			}

			iStartPosTmp += strlen("/");
		}

		//创建子目录
		dir.clear();
		strtocreate.clear();
		dir = strMasterPath; 
		if (dir.length()!=0) dir += "/";
		dir+=strSubDir.substr(iFinalStartPos, iFinalCount);
		strtocreate = dir;
		strtocreate += "/";
		memset(szMasterUnicode, 0, sizeof(TCHAR)*MAX_PATH);
		memset(szSubUnicode, 0, sizeof(TCHAR)*MAX_PATH);
		CWriteDailyLog::AnsiToUnicode((char *)strMasterPath.c_str(), szMasterUnicode, sizeof(TCHAR)*MAX_PATH);
		CWriteDailyLog::AnsiToUnicode((char *)strSubDir.substr(iFinalStartPos, iFinalCount).c_str(), szSubUnicode, sizeof(TCHAR)*MAX_PATH);
		//if (!CreateDir(szMasterUnicode, szSubUnicode))
		CreateSubDir(strtocreate);
		strMasterPath.clear();
			strMasterPath = dir;
	}

	return true;
}

//打开文件
FILE *CWriteDailyLog::OpenFile(string strDir, string strFileName, char *pOpenType, bool bCursorRetHead)
{
	//参数校验
	if (strDir.length() == 0 || strFileName.length() == 0 || pOpenType == NULL) return NULL;

	FILE *fp = NULL;
	string strFile = strDir + strFileName;
	while (fp == NULL)
	{
		fp = fopen(strFile.c_str(), pOpenType);
		if (fp == NULL)
		{
			mkdir(strDir.c_str());
			FILE *fpTmp = fopen(strFile.c_str(), "w");
			if (fpTmp) fclose(fpTmp);
		}
	}
	if (fp == NULL)
	{
		printf("Opening failed!");
		return NULL;
	}

	clearerr(fp);

	if (bCursorRetHead)
	{
		rewind(fp);
	}

		return fp;
}

//打开文件
FILE *CWriteDailyLog::OpenFileWithoutPath(string strFileName, char *pOpenType, bool bCursorRetHead)
{
	//参数校验
	if (strFileName.length() == 0 || pOpenType == NULL) return NULL;

	FILE *fp = NULL;
	fp = fopen(strFileName.c_str(), pOpenType);
	if (fp == NULL)
	{
		FILE *fpTmp = fopen(strFileName.c_str(), "w");
		if (fpTmp) fclose(fpTmp);
		fp = fopen(strFileName.c_str(), pOpenType);
	}	

	if (fp)
	{
		clearerr(fp);

		if (bCursorRetHead)
		{
			rewind(fp);
		}
	}

	return fp;
}

//拷贝文件
bool CWriteDailyLog::CopyFIle(string strSrcFile, string strDestFile)
{
	//参数校验
	if (strSrcFile.length() == 0 || strDestFile.length() == 0) return false;
	FILE *fpSrc = NULL, *fpDest = NULL;
	if (NULL == (fpSrc = fopen(strSrcFile.c_str(), "rb"))) return false;
	if (NULL == (fpDest = fopen(strDestFile.c_str(), "wb+"))) return false;

	clearerr(fpSrc);
	clearerr(fpDest);
	rewind(fpSrc);
	rewind(fpDest);
	fseek(fpSrc, 0, SEEK_SET);
	fseek(fpDest, 0, SEEK_SET);
	char get_c = 0;
	int iCount = 0;
	while (!feof(fpSrc))
	{
		memset(&get_c, 0, sizeof(get_c));
		iCount=fread(&get_c, 1, 1, fpSrc);
		if (iCount != 1) break;	//判断读取是否出错或者到文件结尾feof会延迟
		fwrite(&get_c,1,1, fpDest);
		fflush(fpDest);
	}

	clearerr(fpSrc);
	clearerr(fpDest);
	fclose(fpSrc);
	fclose(fpDest);

	return true;
}

//读取一行
int CWriteDailyLog::ReadOneLine(void *pBuffer, long lReadDataLen, string strFileName, FILE **fpFileOut)
{
	if (!pBuffer || lReadDataLen <= 0) return -1;

	char get_c = 0;
	char *pBufferTmp = (char *)pBuffer;
	memset(pBufferTmp, 0, lReadDataLen);
	long lCount = 0;
	FILE *fpOutCache = NULL;
	
	if (fpFileOut && (*fpFileOut == NULL) || fpFileOut == NULL)
	{
		fpOutCache = OpenFileWithoutPath(strFileName, "r+", true);
	}

	if (fpFileOut && (*fpFileOut != NULL)) fpOutCache = *fpFileOut;
	
	if (fpOutCache)	clearerr(fpOutCache);

	if (!fpOutCache) return -1;
	while (true)
	{
		memset(&get_c, 0, sizeof(get_c));
		if (fread(&get_c, sizeof(get_c), 1, fpOutCache) == 1)
		{
			if (get_c != '\n')
			{
				memmove(pBufferTmp, &get_c, sizeof(get_c));
				pBufferTmp++;
				lCount++;

				if (lReadDataLen <= lCount) break;	//防止溢出
			}
			else
			{
				//fread(&get_c, sizeof(get_c), 1, fpOutCache);
				break;
			}
		}
		else
		{
			break;
			//文件结尾
		}
	}


	if (fpFileOut == NULL)
	{
		if (fpOutCache)
		{
			fclose(fpOutCache);
		}
		
	}
	else
	{
		*fpFileOut = fpOutCache;
	}

	return lCount;

}

//读取文件
int CWriteDailyLog::ReadFile(void *pBuffer, long lReadDataLen, string strFileName, FILE **fpFileOut, bool bCursorRetHead)
{
	if (!pBuffer || strFileName.length() == 0) return false;

	FILE *fpFile = NULL;
	if (fpFileOut&&(*fpFileOut == NULL))
	{
		fpFile = OpenFileWithoutPath(strFileName, "rb+", true);
	}
	else
	{
		fpFile = OpenFileWithoutPath(strFileName, "rb+", false);
	}
	
	if (!fpFile) return false;

	clearerr(fpFile);

	if (bCursorRetHead)
	{
		rewind(fpFile);
	}


	int lRead = fread(pBuffer, lReadDataLen, 1, fpFile);

	if (fpFileOut == NULL)
	{
		fclose(fpFile);
	}
	else
	{
		*fpFileOut = fpFile;
	}

	return lRead;
}

//写入文件
int CWriteDailyLog::WriteFile(void *pBuffer, long lWriteDataLen, string strFileName, FILE **fpFileOut, bool bCursorRetHead)
{
	if (!pBuffer || strFileName.length() == 0) return false;

	FILE *fpFile = NULL;
	if (fpFileOut&&*fpFileOut == NULL)
	{
		fpFile = OpenFileWithoutPath(strFileName, "rb+", true);
	}
	else
	{
		fpFile = OpenFileWithoutPath(strFileName, "rb+", false);
	}
	if (!fpFile) return false;

	clearerr(fpFile);

	if (bCursorRetHead)
	{
		rewind(fpFile);
	}

	int iCount = fwrite(pBuffer, lWriteDataLen, 1, fpFile);

	fflush(fpFile);

	if (fpFileOut == NULL)
	{
		fclose(fpFile);
	}
	else
	{
		*fpFileOut = fpFile;
	}

	return iCount;
}

//关闭文件
bool CWriteDailyLog::CloseFile(FILE *fpFile)
{
	if (!fpFile) return false;

	if (fclose(fpFile)) return false;

	return true;
}

//获取当前目录
bool CWriteDailyLog::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	//模块路径
	TCHAR szModulePath[MAX_PATH] = TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(), szModulePath, MAX_PATH);

	//分析文件
	for (INT i = lstrlen(szModulePath); i >= 0; i--)
	{
		if (szModulePath[i] == TEXT('\\'))
		{
			szModulePath[i] = 0;
			break;
		}
	}

	//设置结果
	ASSERT(szModulePath[0] != 0);
	lstrcpyn(szWorkDirectory, szModulePath, wBufferCount);

	return true;
}

//文件是否存在
bool CWriteDailyLog::IsFileExist(string strFileName)
{
	if (strFileName.length() == 0) return false;

	//文件存在则清除
	if (access(strFileName.c_str(), 0) == -1)//文件不存在则退出
	{
		return false;
	}

	return true;
}

//路径中截取文件
string CWriteDailyLog::GetFileNameInPath(string strPath)
{
	if (strPath.length() == 0) return "";

	char c = 0;
	int i = 0;
	for (i = strPath.length() - 1; i >= 0; i--)
	{
		c = strPath.at(i);
		if (c == '\\')
		{
			i++;
			break;
		}
	}
	if (i < 0) return "";

	return strPath.substr(i, strPath.length() - i);


}

//#ifdef _UNICODE
//字符转换
bool CWriteDailyLog::UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte)
{
	//参数校验
	if (szUnicodeSrc == NULL || pAnsiStrOut == NULL || nAnsiStrSize < 0) return false;

	ZeroMemory(pAnsiStrOut, nAnsiStrSize);
	INT nLength = 0;

	//获取转换所需字节数 
	nLength = WideCharToMultiByte(CP_ACP, 0, szUnicodeSrc, -1, NULL, 0, NULL, NULL);

	//转换空间合法性判断
	if (nLength > nAnsiStrSize)
	{
		//所需的字节数
		if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nLength;

		return false;
	}

	INT nTransFormResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)szUnicodeSrc, -1, pAnsiStrOut, nAnsiStrSize, NULL, NULL);//Unicode转ANSI，第一个参数当前系统ANSI,第二个参数是默认0，最后第二个是不能转换用于替换的字符，最后一个是默认true

	if (nTransFormResult == 0) return false;

	//转换字节数返回
	if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nTransFormResult;

	return true;
}

//字符转换
bool CWriteDailyLog::AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail)
{
	//参数校验
	if (pAnsiStrIn == NULL || szUnicodeStrOut == NULL || nUnicodeCacheSize <= 0) return false;

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

		return false;
	}

	//转换
	INT nTransFormResult = MultiByteToWideChar(CP_ACP, 0, pAnsiStrIn, -1, szUnicodeStrOut, nUnicodeCacheSize);
	if (nTransFormResult == 0) return false;

	//转换字节数返回
	if (nUnicodeStrLenOutCalculateZeroTail)	*nUnicodeStrLenOutCalculateZeroTail = nTransFormResult;

	return true;
}

//#endif

/*
void CWriteDailyLog::WriteDialyLog(TCHARDAILYLOG_T stExLog)
{
	FILE *fp=NULL;
	char WholePath[256]={0};

	//打开日志文件
	//路径
	if(stExLog.Path==NULL)
	{
		strcat(WholePath, ".\\");
	}
	else
	{
		strcat(WholePath, stExLog.Path);
	}

	//文件名
	if(stExLog.FileName==NULL)
	{
		strcat(WholePath, DEFAULT_LogName);
	}
	else
	{
		strcat(WholePath, stExLog.FileName);
	}
	fp=fopen(WholePath, "a+");

	//获取时间
	GetStandardTime(&(stExLog.Time));

	fwrite("\n",strlen("\n"), 1, fp);//开始
	
	//写入数据：
	fwrite(stExLog.Time.TimeStr, strlen(stExLog.Time.TimeStr), 1, fp);//时间
	fwrite(": \n",strlen(": \n"), 1, fp);

	char *pData=new char[stExLog.iDataLen+1];
	ZeroMemory(pData, stExLog.iDataLen+1);
	ZNTCHAR_to_Char(stExLog.Data, pData, stExLog.iDataLen+1);//TCHAR数据转成char数据
	fwrite(pData, stExLog.iDataLen+1, 1, fp);//数据
	fwrite("\n\n",strlen("\n\n"), 1, fp);//结束
	SAFE_ARRAY_DELETE(pData);

	//刷新数据并关闭日志
	fflush(fp);
	fclose(fp);
}*/
/*
bool IsWordExitsInStr(LPCTSTR lpStr, TCHAR tcWord)
{
	int iStrlen=lstrlen(lpStr);
	for(int i=0;i<(iStrlen)/2;i++)
	{
		if(lpStr[i]==tcWord)
		{
			return true;
		}
	}
	return false;
}*/
//////////////////////////////////////////////////////////////
