
#include"Stdafx.h"
#include<errno.h>
#include"DailyLog.h"
#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<io.h>//access����
#include <sys/stat.h>  //_stat����
#include<iostream>
#include<string>
#include<direct.h>	//mkdir����
using namespace std;

//�����ļ���֧��
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

/*
//����������λ��
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

//����ת�����ַ���
VOID I_to_TCHAR(INT iNumIn, PTCHAR pNumOut, INT iLengthOut)
{
	INT iNumTmp=iNumIn, iRest=0, iNumCount=0;
	TCHAR tcTmp;
	//FILE *fp;
	//fp=fopen(".\\0.txt","a");
	//char str[100]={0};
	iNumCount=Count_Num(iNumIn);//����λ��
	for(int i=((iNumCount>iLengthOut?iLengthOut:iNumCount)-1);i>=0;i--)
	{
		//ZeroMemory(str, 100);		
		iRest=iNumTmp%10;//ȡ���һλ��ֵ
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
			iNumTmp=iNumTmp/10;//ȡʣ�µ���ֵ
		}
	}
	//fflush(fp);
	//fclose(fp);
	return;
}

//������TCHAR תChar�ַ���

void TCHAR_to_Char(LPCTSTR lpStrIn, char *pStrOut, INT iStrLenOut)
{
	ZeroMemory(pStrOut,iStrLenOut+1);//����
	for(INT i=0;i<iStrLenOut;i++)
	{
		pStrOut[i]=lpStrIn[i];
	}
}

//������TCHARתchar�ַ���
void ZNTCHAR_to_Char(LPCTSTR lpStrIn, char *pStrOut, INT iStrLenOut)
{
	ZeroMemory(pStrOut, iStrLenOut+1);
	WideCharToMultiByte(CP_ACP, 0, lpStrIn, lstrlen(lpStrIn), pStrOut, iStrLenOut, (LPCSTR)TEXT(" "), NULL);//UnicodeתANSI����һ��������ǰϵͳANSI,�ڶ���������Ĭ��0�����ڶ����ǲ���ת�������滻���ַ������һ����Ĭ��true
}*/

/////////////////////////////////////////////////////////


//����

//��ȡ��������ʱ���ַ���
void CWriteDailyLog::GetStandardTime(STANDARD_TIME_T *stExTime)
{
	SYSTEMTIME sys;
	GetLocalTime( &sys );
	memset(stExTime, 0, S_STANDARD_TIME_T);

	//���ʱ��ṹ��
	stExTime->iYear=(int)sys.wYear;
	stExTime->iMonth=(int)sys.wMonth;
	stExTime->iDay=(int)sys.wDay;
	stExTime->iWorkDay=(int)sys.wDayOfWeek;
	stExTime->iHour=(int)sys.wHour;
	stExTime->iMinute=(int)sys.wMinute;
	stExTime->iSecond=(int)sys.wSecond;
	stExTime->iMillisecond=(int)sys.wMilliseconds;

	//ȡ�����ַ���
	char WorkDayStr[3]={0};
	char WorkDayTmp[30]="��һ����������";
	memmove(WorkDayStr, &(WorkDayTmp[2*(stExTime->iWorkDay)]), 2);
	
	//����ַ���
	sprintf(stExTime->TimeStr,"%4d.%02d.%02d %02d:%02d:%02d.%03d",stExTime->iYear, stExTime->iMonth, stExTime->iDay, stExTime->iHour, stExTime->iMinute,stExTime->iSecond, stExTime->iMillisecond);
}
/*
//д��־
void CWriteDailyLog::WriteDialyLog(DAILYLOG_T stExLog)
{
	FILE *fp=NULL;
	char WholePath[256]={0};

	//����־�ļ�
	//·��
	if(stExLog.Path==NULL)
	{
		strcat(WholePath, ".\\");
	}
	else
	{
		strcat(WholePath, stExLog.Path);
	}

	//�ļ���
	if(stExLog.FileName==NULL)
	{
		strcat(WholePath, DEFAULT_LogName);
	}
	else
	{
		strcat(WholePath, stExLog.FileName);
	}
	fp=fopen(WholePath, "a+");

	//��ȡʱ��
	GetStandardTime(&(stExLog.Time));

	fwrite("\n",strlen("\n"), 1, fp);//��ʼ
	
	//д�����ݣ�
	fwrite(stExLog.Time.TimeStr, strlen(stExLog.Time.TimeStr), 1, fp);//ʱ��
	fwrite(": \n",strlen(": \n"), 1, fp);
	fwrite(stExLog.Data, stExLog.iDataLen, 1, fp);//����
	fwrite("\n\n",strlen("\n\n"), 1, fp);//����

	//ˢ�����ݲ��ر���־
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
	//ת���ַ���

	//�����ռ�
	if (!UnicodeToAnsi(szString, CacheTmp, 0, &iNeedSize))
	{
		if (iNeedSize!=0)
			pCache = new char[iNeedSize+1];
		if (pCache) memset(pCache, 0, iNeedSize + 1);
	}

	//����
	int iNeedSizeTmp = iNeedSize + 1;

	//�ַ�ת��
	UnicodeToAnsi(szFileName, WholePath, 1024);
	WholePath[1023] = 0;
	UnicodeToAnsi(szString, pCache, iNeedSize+1, &iNeedSize);

	//���ļ�
	fp = fopen(WholePath, "a+");
	if (!fp) return false;
	
	STANDARD_TIME_T stTime;
	memset(&stTime, 0, sizeof(STANDARD_TIME_T));

	//��ȡʱ��
	GetStandardTime(&stTime);

	//д�����ݣ�
	fwrite(stTime.TimeStr, strlen(stTime.TimeStr), 1, fp);//ʱ��
	fwrite(":", strlen(":"), 1, fp);
	fwrite(pCache, iStrByte, 1, fp);//����
	fwrite("\r\n", strlen("\r\n"), 1, fp);//����

	//ˢ�����ݲ��ر���־
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
	

	//���ļ�
	fp = fopen(FileName, "a+");
	if (!fp) return false;

	STANDARD_TIME_T stTime;
	memset(&stTime, 0, sizeof(STANDARD_TIME_T));

	//��ȡʱ��
	GetStandardTime(&stTime);

	//д�����ݣ�
	fwrite(stTime.TimeStr, strlen(stTime.TimeStr), 1, fp);//ʱ��
	fwrite(":", strlen(":"), 1, fp);
	fwrite(String, iStrByte, 1, fp);//����
	fwrite("\r\n", strlen("\r\n"), 1, fp);//����

	//ˢ�����ݲ��ر���־
	fflush(fp);
	fclose(fp);

	return true;
}

bool CWriteDailyLog::SaveDataAsFile(char *String, int iStrByte, char *FileName)
{
	if (String == NULL) return false;
	if (FileName == NULL) FileName = "DailyLog.txt";

	FILE *fp = NULL;


	//���ļ�
	fp = fopen(FileName, "wb+");
	if (!fp) return false;
	rewind(fp);

	//д�����ݣ�
	fwrite(String, iStrByte, 1, fp);//����

	//ˢ�����ݲ��ر���־
	fflush(fp);
	fclose(fp);

	return true;
}

//ɾ���ļ�
void CWriteDailyLog::delfile(char *path)
{
	//�ļ����������
	if( access(path,0)==-1 )//�ļ����������˳�
	{
		return;
	}
	char file[1024]={0};
	strcat(file, "del ");
	strcat(file, path);
	system(file);
}

//�����ļ���������
bool CWriteDailyLog::SetFileHide(const TCHAR * lpName)
{
	DWORD dwResult = ::GetFileAttributes(lpName);
	if (INVALID_FILE_ATTRIBUTES == dwResult)
	{
		return false;
	}
	if (!(FILE_ATTRIBUTE_HIDDEN & dwResult)) // ������������ص� 
	{
		if (INVALID_FILE_ATTRIBUTES == ::SetFileAttributes(lpName, dwResult | FILE_ATTRIBUTE_HIDDEN))
		{
			return false;
		}
		return true;
	}
	else// ������Ѿ������صģ��͵����óɹ��� 
	{
		return true;
	}

	return true;
}

//�����ļ�����������
bool CWriteDailyLog::SetDirHide(const TCHAR * lpName)
{
	return true;
}

//�����ļ���
bool CWriteDailyLog::CreateDir(CString masterdir, CString subdir)
{
	if (!PathIsDirectory(masterdir))
	{
		//����Ŀ¼,���еĻ���Ӱ�� 
		if (!::CreateDirectory(masterdir, NULL))
			return false;
	}

	masterdir += subdir;

	if (!PathIsDirectory(masterdir))
	{
		//����Ŀ¼,���еĻ���Ӱ�� 
		if (!::CreateDirectory(masterdir, NULL))
			return false;
	}
	//mkdir("./��/");

	return true;
}

//�����ļ���
bool CWriteDailyLog::CreateSubDir(string strDir)
{
	if (strDir.length() == 0) return false;

	if (mkdir(strDir.c_str())) return false;

	return true;
}

//�ݹ鴴��Ŀ¼
bool CWriteDailyLog::CreateRecursiveDir(string strAbsolutePath)
{
	//��ȡ�̷�
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

//�ݹ鴴����Ŀ¼
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

		//������Ŀ¼
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

//���ļ�
FILE *CWriteDailyLog::OpenFile(string strDir, string strFileName, char *pOpenType, bool bCursorRetHead)
{
	//����У��
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

//���ļ�
FILE *CWriteDailyLog::OpenFileWithoutPath(string strFileName, char *pOpenType, bool bCursorRetHead)
{
	//����У��
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

//�����ļ�
bool CWriteDailyLog::CopyFIle(string strSrcFile, string strDestFile)
{
	//����У��
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
		if (iCount != 1) break;	//�ж϶�ȡ�Ƿ������ߵ��ļ���βfeof���ӳ�
		fwrite(&get_c,1,1, fpDest);
		fflush(fpDest);
	}

	clearerr(fpSrc);
	clearerr(fpDest);
	fclose(fpSrc);
	fclose(fpDest);

	return true;
}

//��ȡһ��
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

				if (lReadDataLen <= lCount) break;	//��ֹ���
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
			//�ļ���β
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

//��ȡ�ļ�
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

//д���ļ�
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

//�ر��ļ�
bool CWriteDailyLog::CloseFile(FILE *fpFile)
{
	if (!fpFile) return false;

	if (fclose(fpFile)) return false;

	return true;
}

//��ȡ��ǰĿ¼
bool CWriteDailyLog::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	//ģ��·��
	TCHAR szModulePath[MAX_PATH] = TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(), szModulePath, MAX_PATH);

	//�����ļ�
	for (INT i = lstrlen(szModulePath); i >= 0; i--)
	{
		if (szModulePath[i] == TEXT('\\'))
		{
			szModulePath[i] = 0;
			break;
		}
	}

	//���ý��
	ASSERT(szModulePath[0] != 0);
	lstrcpyn(szWorkDirectory, szModulePath, wBufferCount);

	return true;
}

//�ļ��Ƿ����
bool CWriteDailyLog::IsFileExist(string strFileName)
{
	if (strFileName.length() == 0) return false;

	//�ļ����������
	if (access(strFileName.c_str(), 0) == -1)//�ļ����������˳�
	{
		return false;
	}

	return true;
}

//·���н�ȡ�ļ�
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
//�ַ�ת��
bool CWriteDailyLog::UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte)
{
	//����У��
	if (szUnicodeSrc == NULL || pAnsiStrOut == NULL || nAnsiStrSize < 0) return false;

	ZeroMemory(pAnsiStrOut, nAnsiStrSize);
	INT nLength = 0;

	//��ȡת�������ֽ��� 
	nLength = WideCharToMultiByte(CP_ACP, 0, szUnicodeSrc, -1, NULL, 0, NULL, NULL);

	//ת���ռ�Ϸ����ж�
	if (nLength > nAnsiStrSize)
	{
		//������ֽ���
		if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nLength;

		return false;
	}

	INT nTransFormResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)szUnicodeSrc, -1, pAnsiStrOut, nAnsiStrSize, NULL, NULL);//UnicodeתANSI����һ��������ǰϵͳANSI,�ڶ���������Ĭ��0�����ڶ����ǲ���ת�������滻���ַ������һ����Ĭ��true

	if (nTransFormResult == 0) return false;

	//ת���ֽ�������
	if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nTransFormResult;

	return true;
}

//�ַ�ת��
bool CWriteDailyLog::AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail)
{
	//����У��
	if (pAnsiStrIn == NULL || szUnicodeStrOut == NULL || nUnicodeCacheSize <= 0) return false;

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

		return false;
	}

	//ת��
	INT nTransFormResult = MultiByteToWideChar(CP_ACP, 0, pAnsiStrIn, -1, szUnicodeStrOut, nUnicodeCacheSize);
	if (nTransFormResult == 0) return false;

	//ת���ֽ�������
	if (nUnicodeStrLenOutCalculateZeroTail)	*nUnicodeStrLenOutCalculateZeroTail = nTransFormResult;

	return true;
}

//#endif

/*
void CWriteDailyLog::WriteDialyLog(TCHARDAILYLOG_T stExLog)
{
	FILE *fp=NULL;
	char WholePath[256]={0};

	//����־�ļ�
	//·��
	if(stExLog.Path==NULL)
	{
		strcat(WholePath, ".\\");
	}
	else
	{
		strcat(WholePath, stExLog.Path);
	}

	//�ļ���
	if(stExLog.FileName==NULL)
	{
		strcat(WholePath, DEFAULT_LogName);
	}
	else
	{
		strcat(WholePath, stExLog.FileName);
	}
	fp=fopen(WholePath, "a+");

	//��ȡʱ��
	GetStandardTime(&(stExLog.Time));

	fwrite("\n",strlen("\n"), 1, fp);//��ʼ
	
	//д�����ݣ�
	fwrite(stExLog.Time.TimeStr, strlen(stExLog.Time.TimeStr), 1, fp);//ʱ��
	fwrite(": \n",strlen(": \n"), 1, fp);

	char *pData=new char[stExLog.iDataLen+1];
	ZeroMemory(pData, stExLog.iDataLen+1);
	ZNTCHAR_to_Char(stExLog.Data, pData, stExLog.iDataLen+1);//TCHAR����ת��char����
	fwrite(pData, stExLog.iDataLen+1, 1, fp);//����
	fwrite("\n\n",strlen("\n\n"), 1, fp);//����
	SAFE_ARRAY_DELETE(pData);

	//ˢ�����ݲ��ر���־
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
