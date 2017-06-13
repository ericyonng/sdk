#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#pragma once

//#include"TcpSocket.h"
#include"TdxmHead.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�ͻ���
class CTcpClient :public CNetworkService
{
public:
	//��ʼ�����л���
	CTcpClient();
	virtual ~CTcpClient();

	//�������
public:
	//���ӷ�����
	BOOL Link(char *pServerAddr, unsigned short port, bool bClearDataCache = true);
	//���ӷ�����
	BOOL ConnectServer();
	//�ر�����
	BOOL CloseLink();
	//���������������
	BOOL SendDataToServer(void *pData, long lDataLen);
	//�ӷ�������ȡ���� �ǵ��ͷſռ�
	BOOL RecvDataFromServer(void **pData, long *lDataLen);
	//�������ã�����Ͽ����ӣ�
	BOOL RefreshConfig();

	//����
protected:
	//��ȡ���ӵ�ַ
	BOOL	ReadServerAddr(){ return TRUE; }
	//��ȡ�ӽ�����Կ
	BOOL	ReadRsaKey(){ return TRUE; }
	//����Զ��
	virtual BOOL Connect();

	//���ݳ�Ա
protected:	
	string				m_strServerAddr;			//��������ַ
	unsigned short		m_ServerPort;				//�������˿�
	bool				m_bIsDomainName;			//�Ƿ�����
	//rsa	key :���ر���server�˿��Խ��ܵļ��������Լ�һ��ר���ڿͻ��˵Ľ�����������˱���ͻ��˿��Խ��ܵļ������Լ�ר���ڷ���˵Ľ�����
};


#endif