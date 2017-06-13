#ifndef SOCKET_PROTOCOL_H_
#define SOCKET_PROTOCOL_H_

#pragma once
#include<windows.h>


////////////////////////////////

//����

///////////////////////////////////////////////



//�궨��
#define INVALID_SOCKET_ID			-1
#define INVALID_DATA_FLOW_COUNT		0
#define HOST_ADDR_LEN				1024

//�߼������ݳ�������
#define BUSINESS_DATA_LIMIT			1024	//���ܺ�4�ֽ�	����1004�ֽ�
#define TIME_OUT_COUNT				3		//��ʱ������ÿ��һ�룬һ��ȴ����ظ�������
#define HTTP_MAX_DATA_LIMIT			1024	//http������Ϣ����ֽ���

//�����ߴ�
#define SOCKET_PACK_SIZE_MAX_LIMIT	1024

//listen����֧�ֵ����������Ӹ���
#define LISTEN_MAX_LIMIT			200

//����ͳ��
typedef struct stDataFlowCount
{
	long	lRecvTotal;				//�ܵĽ�����	byte
	long	lSendTotal;				//�ܵķ�����	byte
}DATA_FLOW_COUNT_T;
#define S_DATA_FLOW_COUNT_T		sizeof(DATA_FLOW_COUNT_T)

//��Ч���ݶ�̬����
typedef struct stEffectDataFlowCount
{
	long	lTotalFlowSize;					//�ܵĴ�С
	long	lCurrentFlowCount;				//��ǰ����
}EFFECT_DATA_FLOW_COUNT_T;
#define S_EFFECT_DATA_FLOW_COUNT_T	sizeof(EFFECT_DATA_FLOW_COUNT_T)


//���建�����ṹ
typedef struct stCache
{
	void *pData;
	long lDataLen;
}CACHE_T;
#define S_CACHE_T	sizeof(CACHE_T)

///////////////////////////////////////////////////////////////////////////////////


//socket ���ݰ� Լ��һ�������ܳ���1024Byte
enum
{
	enPackHead=0,
	enAccessLayer,
	enBusiness,
	enPackTail
};


//��ͷ
typedef struct stSocketPackHead
{
	enum{HEAD_ID=0x0404};
	unsigned short usHead;
	//0x0404
}SOCKET_PACK_HEAD_T;
#define S_SOCKET_PACK_HEAD_T sizeof(SOCKET_PACK_HEAD_T)

//�����
typedef struct stAccessLayer
{
	unsigned long ulPacketSize;	//������ͷ��β�����ݳ���
	unsigned long ulPacketSeq;		//�����
	unsigned long ulPacketNum;		//������
	unsigned long	ulTransID;		//����ˮ
}ACCESS_LAYER_T;
#define S_ACCESS_LAYER_T sizeof(ACCESS_LAYER_T)


//��β
typedef struct stSocketPackTail
{
	enum{TAIL_ID=0x0505};
	unsigned short  usTail;
}SOCKET_PACK_TAIL;
#define S_SOCKET_PACK_TAIL sizeof(SOCKET_PACK_TAIL)

//���ջ������ػ�����	(�ߴ����Ƽӳ�ʱˢ�·�ֹ���ڴ�ռ�ù��������)
#define		SOCKET_RECV_CACHE_LIMIT_MAX			1073741824		//��󻺳�����	1024*1024*1024	1MB	
#define		SOCKET_RECV_CACHE_FLUSH_PERIOD		100				//��λms ÿ��100msˢ�½��ջ���

//socket��˳��
//��ͷ
//�����
//ҵ���߼���
//�̶��� CIPHER_TEXT_OUT_ENCODE_LEN��256�����ȵ���Կ�ռ�
//��β

//������ȷ���ذ�:ID:RIGHT_RECV;����Ϊ��
//��������
//#define FUNID_RIGHT_RECV			0x0909		//�ɹ����չ��ܺ�

//��ȷ��
typedef struct stRecvConfirmPack
{
	enum{ SUCCESS_RECV = 0x01 };
	unsigned int uFunID;			//��ȷ�Ϲ��ܺ�
	unsigned long ulPacketSeq;		//�ɹ����յİ���ţ������߷��Ͱ�����ŷ���������ţ�
}RECV_CONFIRM_PACK_T;
#define S_RECV_CONFIRM_PACK_T sizeof(RECV_CONFIRM_PACK_T)

//������
typedef struct stHeartBeatingPack
{
	enum{HEART_BEATING=0x02};
	unsigned int		uFunID;
}HEART_BEATING_PACK_T;
#define S_HEART_BEATING_PACK_T sizeof(HEART_BEATING_PACK_T)

//���Զ���Ĺ��ܺ�
#define FUNID_CUSTOM_START	(stHeartBeatingPack::HEART_BEATING+1)



//////////////////////////////////////////////////////////

//����ҵ���Э��



#endif