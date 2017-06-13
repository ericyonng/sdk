#ifndef SOCKET_PROTOCOL_H_
#define SOCKET_PROTOCOL_H_

#pragma once
#include<windows.h>


////////////////////////////////

//心跳

///////////////////////////////////////////////



//宏定义
#define INVALID_SOCKET_ID			-1
#define INVALID_DATA_FLOW_COUNT		0
#define HOST_ADDR_LEN				1024

//逻辑层数据长度限制
#define BUSINESS_DATA_LIMIT			1024	//功能号4字节	数据1004字节
#define TIME_OUT_COUNT				3		//超时次数（每次一秒，一秒等待后重复操作）
#define HTTP_MAX_DATA_LIMIT			1024	//http请求消息最大字节数

//最大包尺寸
#define SOCKET_PACK_SIZE_MAX_LIMIT	1024

//listen队列支持的最多监听连接个数
#define LISTEN_MAX_LIMIT			200

//流量统计
typedef struct stDataFlowCount
{
	long	lRecvTotal;				//总的接收量	byte
	long	lSendTotal;				//总的发送量	byte
}DATA_FLOW_COUNT_T;
#define S_DATA_FLOW_COUNT_T		sizeof(DATA_FLOW_COUNT_T)

//有效数据动态流量
typedef struct stEffectDataFlowCount
{
	long	lTotalFlowSize;					//总的大小
	long	lCurrentFlowCount;				//当前流量
}EFFECT_DATA_FLOW_COUNT_T;
#define S_EFFECT_DATA_FLOW_COUNT_T	sizeof(EFFECT_DATA_FLOW_COUNT_T)


//定义缓冲区结构
typedef struct stCache
{
	void *pData;
	long lDataLen;
}CACHE_T;
#define S_CACHE_T	sizeof(CACHE_T)

///////////////////////////////////////////////////////////////////////////////////


//socket 数据包 约定一个包不能超过1024Byte
enum
{
	enPackHead=0,
	enAccessLayer,
	enBusiness,
	enPackTail
};


//包头
typedef struct stSocketPackHead
{
	enum{HEAD_ID=0x0404};
	unsigned short usHead;
	//0x0404
}SOCKET_PACK_HEAD_T;
#define S_SOCKET_PACK_HEAD_T sizeof(SOCKET_PACK_HEAD_T)

//接入层
typedef struct stAccessLayer
{
	unsigned long ulPacketSize;	//包含包头包尾的数据长度
	unsigned long ulPacketSeq;		//包序号
	unsigned long ulPacketNum;		//包总数
	unsigned long	ulTransID;		//包流水
}ACCESS_LAYER_T;
#define S_ACCESS_LAYER_T sizeof(ACCESS_LAYER_T)


//包尾
typedef struct stSocketPackTail
{
	enum{TAIL_ID=0x0505};
	unsigned short  usTail;
}SOCKET_PACK_TAIL;
#define S_SOCKET_PACK_TAIL sizeof(SOCKET_PACK_TAIL)

//接收缓冲区守护参数	(尺寸限制加超时刷新防止因内存占用过大儿崩溃)
#define		SOCKET_RECV_CACHE_LIMIT_MAX			1073741824		//最大缓冲限制	1024*1024*1024	1MB	
#define		SOCKET_RECV_CACHE_FLUSH_PERIOD		100				//单位ms 每隔100ms刷新接收缓存

//socket包顺序：
//包头
//接入层
//业务逻辑层
//固定的 CIPHER_TEXT_OUT_ENCODE_LEN（256）长度的密钥空间
//包尾

//接收正确返回包:ID:RIGHT_RECV;数据为空
//参数定义
//#define FUNID_RIGHT_RECV			0x0909		//成功接收功能号

//包确认
typedef struct stRecvConfirmPack
{
	enum{ SUCCESS_RECV = 0x01 };
	unsigned int uFunID;			//包确认功能号
	unsigned long ulPacketSeq;		//成功接收的包序号（发送者发送包的序号非真正包序号）
}RECV_CONFIRM_PACK_T;
#define S_RECV_CONFIRM_PACK_T sizeof(RECV_CONFIRM_PACK_T)

//心跳包
typedef struct stHeartBeatingPack
{
	enum{HEART_BEATING=0x02};
	unsigned int		uFunID;
}HEART_BEATING_PACK_T;
#define S_HEART_BEATING_PACK_T sizeof(HEART_BEATING_PACK_T)

//可自定义的功能号
#define FUNID_CUSTOM_START	(stHeartBeatingPack::HEART_BEATING+1)



//////////////////////////////////////////////////////////

//其他业务层协议



#endif