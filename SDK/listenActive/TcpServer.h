#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#pragma once

//#include"../NetworkService/NetworkServiceHead.h"
#include"../NetworkService/TcpSocket.h"


//����� �����첽IOģ�ͣ�������չ��iocpģ��
class CTcpServer :public CNetworkService
{
public:
	CTcpServer();
	virtual ~CTcpServer();

public:
	//����������
	BOOL StartServer();
	//�رշ�����
	BOOL CloseServer();
	//��������	(�����ȹرշ�����)
	BOOL RefreshConfig();
	//����������
	void SetMainWind(void *pMainWnd){ m_pMainWind = pMainWnd; }
	//����ip
	void SetIp(string ip);
	//���ö˿�
	void SetPort(unsigned short port){ m_ServerPort = port; }

public:
	//�����˿� ֧��200�����Ӷ��еȴ� //���֧��250������
	BOOL Listen(int iBacklog=200);
	//ȷ�����Ӳ���ȡ�ͻ�����Ϣ
	BOOL Accept(SOCKET *pSocketFDForClient, CAddr *pRemoteAddr=NULL);
	//�����첽IOģ��
	BOOL SetAsynchronousServer();
	//����IO�¼�
	BOOL WaitForEvent();
	//��Ӽ���¼�
	BOOL AddEvent(SOCKET hClntSock);
	//�󶨵�ַ
	BOOL Bind(char *ip, int iplen, unsigned short port, unsigned short family);			//�д���ȶ

	//��ȡ����
	BOOL ReadDataFromClient(void **pData, long *lDataLen);
	//��������
	BOOL SendDataToClient(void *pData, long lDataLen);

	//��������
protected:
	//����socket����
	void CompressSockets(SOCKET hSockArr[], int idx, int total);
	//�����¼�����
	void CompressEvents(WSAEVENT hEventArr[], int idx, int total);

	//��������
protected:
	BOOL	ImportAddrConfig(){ return TRUE; }
	BOOL	ReadRsaKey(){ return TRUE; }

	

protected:
	void	*			m_pMainWind;				//������

protected:
	//��ַ����
	string				m_strServerAddr;			//��������ַ
	unsigned short		m_ServerPort;				//�������˿�
	bool				m_bIsDomainName;			//�Ƿ�����
	bool				m_bServerStart;				//�������Ƿ�����
	//rsa	key :���ر���server�˿��Խ��ܵļ��������Լ�һ��ר���ڿͻ��˵Ľ�����������˱���ͻ��˿��Խ��ܵļ������Լ�ר���ڷ���˵Ľ�����

	//�첽IO����
protected:
	SOCKET				m_hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			m_hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT			m_newEvent;
	WSANETWORKEVENTS	m_netEvents;
	int					m_NumOfClntSock;
};
#endif