#ifndef TDXM_HEAD_H_
#define TDXM_HEAD_H_

#pragma once
//加解密模块
const TCHAR *const TDXM_DE_ENCRYPT_DLL_MODULE_FILE = TEXT("endll.dll");

//长度参数定义
#define MW_USERID_LEN							16			//账号id字符串长度
#define MW_PASSWORD_DIGEST_LEN					64			//密码长度
#define MONITOR_SEED_LEN						(128/sizeof(DWORD))		//随机数长度

#include "../结构定义/Public.h"
//组件
#include "../DUI窗体/GuiFoundationHead.h"
#include"../ImageMethod/YCImageHead.h"

#include"../Thread/ThreadHead.h"
#include"../NetworkService/NetworkServiceHead.h"

#include"../CommonModul/CommonModulHead.h"
#pragma comment(lib, "../库文件/Unicode/CommonModul.lib")

#pragma comment(lib, "../库文件/Unicode/Thread.lib")
#pragma comment(lib, "../库文件/Unicode/NetworkService.lib")
#endif