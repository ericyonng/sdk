#ifndef TDXM_HEAD_H_
#define TDXM_HEAD_H_

#pragma once
//�ӽ���ģ��
const TCHAR *const TDXM_DE_ENCRYPT_DLL_MODULE_FILE = TEXT("endll.dll");

//���Ȳ�������
#define MW_USERID_LEN							16			//�˺�id�ַ�������
#define MW_PASSWORD_DIGEST_LEN					64			//���볤��
#define MONITOR_SEED_LEN						(128/sizeof(DWORD))		//���������

#include "../�ṹ����/Public.h"
//���
#include "../DUI����/GuiFoundationHead.h"
#include"../ImageMethod/YCImageHead.h"

#include"../Thread/ThreadHead.h"
#include"../NetworkService/NetworkServiceHead.h"

#include"../CommonModul/CommonModulHead.h"
#pragma comment(lib, "../���ļ�/Unicode/CommonModul.lib")

#pragma comment(lib, "../���ļ�/Unicode/Thread.lib")
#pragma comment(lib, "../���ļ�/Unicode/NetworkService.lib")
#endif