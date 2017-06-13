#ifndef RSA_H_
#define RSA_H_

#pragma once

/* RSA-DES ��Կ����ϵͳ����
*
* ʹ��rsa-des��ϼ�����Կ����ϵͳ��
*��ʹ��des �ж�������������Ŀռ䣺EncrypteMemoryNeeded ͬʱ�õ�des key
*�ٶ����Ľ��м��ܲ�����õ����ģ�Encrypte
*�ѵõ���des key ʹ�öԷ��ļ��ܹ�Կ rsa���� Encrypte �õ�des key ������
*���õ������� deskey �������������Ĵ���� �Է� 
*�Է�ͨ��rsa˽Կ����Decrypte �õ�des key ������
*��ͨ�����ܺ��des key ����Decrypte ����������������ݵ����� �õ�����
*rsa-des��ϼ�����Կ����ϵͳ�������
*end
*/

//RSAͷ�ļ�֧��
#include<Windows.h>
#include <Wincrypt.h>
#include"CommonModulHead.h"
#include<string>
using namespace std;

//�궨��
#define		PLAIN_TEXT_MAX_LEN					64						//���ĳ������ƣ����÷�����ܣ�117��1024/8-11����rsa1024�����ĵ�����
#define		CIPHER_TEXT_OUT_ENCODE_LEN			256						//���ܲ����������ݳ��ȣ��������Ķ೤ �����������ĵ����ƣ�
#define		CIPHER_TEXT_OUT_DECODE_LEN			128						//Ϊ����ļ��ܺ�����ݳ���
#define		RSA_CONTAINER_NAME_LEN				128						//CSP������
#define		CSP_CONTAINER_NAME				TEXT("FengYanchenCsp")		//����CSP������
#define		PUBLIC_KEY_APPNAME				TEXT("PUBLIC_KEY")			//��Կ�ڵ���
#define		PRIVATE_KEY_APPNAME				TEXT("PRIVATE_KEY")			//˽Կ�ڵ���
#define		RSA_KEY_NAME					TEXT("key")					//����
#define		RSA_KEY_STR_LEN					8192						//��Կ����

//Կ�׽ṹ����Ϣ����
typedef struct stkey{
	unsigned char ucKey[RSA_KEY_STR_LEN];			//key
	unsigned long ulKeyLen;			//key����
}KEY_T;								//����Կ�׽ṹ����Ϣ						
#define S_KEY_T	sizeof(KEY_T)		//Կ�׽ṹ�峤��	

//��Կ����
static	const char const YCRsaPublicKey[] = "";
static const char const YCRsaPrivateKey[] = "";

#define MY_ENCODE_TYPE   X509_ASN_ENCODING | PKCS_7_ASN_ENCODING
#define CRYPT_STRING_NOCRLF                 0x40000000//2003�汾��������֧�ֲ���

// BASE64������ǩ����Կ	20170323	shy
static const char *szYCSignaturePubKey = "MIGJAoGBAI3U3kCD19AINDV3JOuqU5BOw617Nkz2Z6GNyHbCV3aA/HCXoyp6pzPNKo4Ytu2JIlgi+bFBCUD/2QIBjI4qDhL2GOjuZfHMNlpGlrwesp2CLMEunRqU/XKR+2R91gxP5UQMCb8RI4CTFNizndinzjFirYl/Vjgd+JS7zplvZ2FTAgMBAAE=";

//BASE64������ǩ��˽Կ	20170323	shy
static const char *szYCSignaturePrivKey = "MIICXAIBAAKBgQCN1N5Ag9fQCDQ1dyTrqlOQTsOtezZM9mehjch2wld2gPxwl6MqeqczzSqOGLbtiSJYIvmxQQlA/9kCAYyOKg4S9hjo7mXxzDZaRpa8HrKdgizBLp0alP1ykftkfdYMT+VEDAm/ESOAkxTYs53Yp84xYq2Jf1Y4HfiUu86Zb2dhUwIDAQABAoGAA6T4oQfCP+I2FTmehT+3LfoYJRkJaCjcuNX/1K1hufXOhsYetzS3nykicsxHxwrDCyi66ccWCcmJNff8OJr0CZblnhng02NsW4RxY0u6ySsdfH9XwEm1JWauqo3UeT7GRHLcV4+ksfiUsdEqtiNR2LLkp1yIIG7n4IiT3mnwdcECQQDDPv67bGuPVItKNTnPVXenkfPrGPRC7+0Rs6KlhhY+H3U5jBWXnOilwUwN4nFECWjcpHjrK1et99yaJ9JufZOrAkEAufbzZZuwKD8BVaFlj4DjSN73GE4WSe4JeswhPMru4wZhzh03xEe3Wl/0WG0RV5wKFVXymlN4moVddYE63whA+QJANqKTH5lIQ/RxgdOBGM6ezApgiiISgASpfzGYSt9JrBqbb/4OML3csD117dC2x0YmzM9NculPAMSCO7T3AqorPwJBAIDDLzDRph2X0pK6GBpPAhD44Vqg5un1ufae1PzE/rePObV/oZd0mM4o1oVLr9BbqNexFTQqS72iTl4IRv0bN/ECQHdeULI7dZczNWkJXiQTDs0E4+oz6d5yOMkrMYFUJvEzYbvvwf54sWo4ORtSZ11iL9cn1x2s+KIZro2CBT/BRQM=";


/////////////////////////////////////////////////////////

//RSA����
class COMMON_MODUL_CLASS CRsaEngine
{
public:
	CRsaEngine();
	virtual ~CRsaEngine();
	//�ͷŶ���
	virtual void FreeObj(){ try{ delete this; } catch (...){} }

	//�ӿں���
public:
	//���ܲ����� bFinal����ָ���ǲ������һ�����ݣ�ѭ�����ܵ�ʱ�����һ��Ҫ���ܵ�ΪTRUE������ı���ΪFALSE���������û����
	BOOL Encrypte(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut, BOOL bFinal = TRUE);
	//���벢���� bFinal����ָ���ǲ������һ�����ܵ����ݣ�ѭ�����ܵ�ʱ�����һ��Ҫ���ܵ�ΪTRUE������ı���ΪFALSE���������û����
	BOOL Decrypte(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut, BOOL bFinal = TRUE);
	//Csp��ʼ������ʼ��������������Կ����Կ��˽Կ��������������Կ��
	BOOL CSPInit(bool bUseExistKeyPairs = false, LPTSTR szPublicKeyFile = NULL, LPTSTR szPrivateKeyFile = NULL);

	//���ù�Կ ���������
	BOOL SetPublicKey(unsigned char *ucKey);
	
	//����˽Կ ���������
	BOOL SetPrivateKey(unsigned char *ucKey);

	//ǩ����ǩ
public:
	//ǩ��	20170323	shy	128�ֽ�
	string SignatureData(char * strData, long lDataSize);
	//��ǩ	20170323	shy
	bool VerifySignature(char * strUnSignData, long lUnSignDataSize, char * strSignData, long lSignDataSize);
	//����ǩ����Կ��	20170323	shy
	bool GenRsaSignatureKeyPairs(bool bGenerateKeyFile = false);	//���ù���
	//����ǩ����Կ	20170323	shy
	bool ImportRsaSignatureKey(HCRYPTPROV* hProv, HCRYPTKEY* hKey, const char* szRSAKey, bool isPrivKey);


	//��Կ��ʼ��
private:
	//�����������ͷ�key
	BOOL Release();
	//������Կ��
	BOOL GenerateKeyPairs();
	//������Կ�ԣ����Ѿ�������Կ���������ٺ��룩
	BOOL ImportKeyPairs();

	//�ڲ����ܽ���
private:
	//�������� 
	BOOL EncrypteData(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut, BOOL bFinal = TRUE);
	//�������� bFinal����ָ���ǲ������һ�����ܵ����ݣ�ѭ�����ܵ�ʱ�����һ��Ҫ���ܵ�ΪTRUE������ı���ΪFALSE���������û����
	BOOL DecrypteData(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut, BOOL bFinal = TRUE);


	//��������
private:
	//���루16���ƣ�
	BOOL HexEncodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//�����루16���ƣ�
	BOOL HexDecodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//16����תʮ����
	BOOL HexToInt(unsigned char *HexData);
	//UnicodeToAnsi
	BOOL UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte = NULL);
	//AnsiToUnicode
	BOOL AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);
	//���ļ��ж�ȡpublic key ��private key
	BOOL ReadPublicPrivateKey(LPTSTR szPublicKeyFile, LPTSTR szPrivateKeyFile);
	//д�������ļ�
	BOOL WritePublicPrivateKey(LPTSTR szPublicKeyFile, LPTSTR szPrivateKeyFile);

private:
	//���ݳ�Ա
	//CSP, PRIVATEKEY,PUBLICKEY,
	HCRYPTPROV		m_hCryptProv;		//CSP�������
	HCRYPTKEY		m_hPublicKey;		//��Կ���
	HCRYPTKEY		m_hPrivateKey;		//˽Կ���
	TCHAR			m_szCspName[RSA_CONTAINER_NAME_LEN];		//������
	KEY_T			m_stPublicKey;								//��Կ
	KEY_T			m_stPrivateKey;			//˽Կ
};

//�������
class COMMON_MODUL_CLASS CBlockRsa : public CRsaEngine
{
public:
	CBlockRsa(){}
	virtual ~CBlockRsa(){}

public:
	//����ѭ���������� ÿ�����ݹ̶�64�ֽ�����
	BOOL DataBlockCircleEncrypte(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut);
	//�����������
	BOOL DataBlockCircleDecrypte(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut);
};



////////////////////////////////////////////////////////////////////////////////////

#define DES_KEY_BIT			8				//key��λ��

#define		CSP_DES_NAME					TEXT("FengDesCsp")			//des������

//����ⶨ��
#define DES_NUM_TYPE			0			//����
#define DES_CHAR_TYPE			1			//Ӣ��
#define DES_SYMBOL_TYPE			2			//�ַ�
#define DES_KEY_TYPE_NUM		3			//��������
#define DES_NUM_LEN				10
#define DES_CHAR_LEN			26
#define DES_SYMBOL_LEN			14

//#ifdef _UNICODE
typedef struct stKeyBlob
{
	BLOBHEADER hdr;
	DWORD cbKeySize;			//ֻ֧��8λ����
	BYTE rgbKeyData[DES_KEY_BIT];		//ֻ֧��8λ����
} KEY_BLOB_T;    //�ṹ�ο�MSDN - CryptImportKey
#define S_KEY_BLOB_T sizeof(KEY_BLOB_T)
//#endif


//����
static const char DesKeyNumLib[DES_NUM_LEN] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
//��ĸ
static const char DesKeyCharLib[DES_CHAR_LEN] = { 'a', 'b', 'c', 'd', 'e'
, 'f', 'g', 'h', 'i', 'j'
, 'k', 'l', 'm', 'n', 'o'
, 'p', 'q', 'r', 's', 't'
, 'u', 'v', 'w', 'x', 'y'
, 'z' };

//����
static const char DesKeySymbolLib[DES_SYMBOL_LEN] = { '~', '!', '@', '#'
, '$', '%', '^'
, '&', '*', '('
, ')', '_', '+'
, '`' };




//des���ܽ���
/*
DES���ܡ����ܺ�����ʹ��PKCS 5 padding��CBCģʽ

������
pbKey       DES��Կ
cbKeyLen    pbKey�ֽڳ���
pbData      Ҫ���ܡ����ܵ�����
cbDataLen   pbData�ֽڳ���
pbBuf       �����������������ܺ󡢽��ܺ�����ݣ�����ΪNULL
pcbBufLen   pbBuf�ֽڳ���
��pbBuf��ΪNULLʱ������ʵ�ʸ��Ƶ�pbBuf���ֽڳ���
��pbBufΪNULLʱ��������Ҫ��pbBuf�ֽڳ���
bIsDecrypt  ΪTRUEʱִ�н��ܲ���������ִ�м��ܲ���

���أ�
�ɹ�����TRUE�����򷵻�FALSE
*/

#define __in	/*������in*/
#define __out	/*�����out*/
#define __inout	/*�������inout*/

//1.�Ȳ����������ռ��С��new�����Ŀռ� EncrypteMemoryNeeded
//2.���ܲ����� Encrypte
//3.���벢���� Decrypte
class COMMON_MODUL_CLASS CDESEngine
{
public:
	CDESEngine();
	virtual ~CDESEngine();
	//�ͷŶ���
	virtual void FreeObj(){ try{ delete this; } catch (...){} }

	//�ӿں���
public:
	//���ܲ�����
	BOOL Encrypte(__inout const BYTE* pbKey,
		__inout DWORD *pcbKeyLen,
		__in const BYTE* pbPlainText,
		__in DWORD cbPlainDataLen,
		__out BYTE* pbCipherBuf,
		__inout DWORD* pcbCipherBufLen);

	//���벢����
	BOOL Decrypte(__in const BYTE* pbKey,
		__in DWORD cbKeyLen,
		__in const BYTE* pbCipherData,
		__in DWORD cbCipherDataLen,
		__out BYTE* pbPlainTextBuf,
		__inout DWORD* pcbPlainTextBufLen);

	//��������Ҫ�Ŀռ�
	int    EncrypteMemoryNeeded(__inout const BYTE* pbKey,
		__inout DWORD *pcbSizeInKeyLenOut,
		__in const BYTE* pbPlainText,
		__in DWORD cbPlainDataLen);

	//��������Ҫ�Ŀռ�
	int		DecrypteMemoryNeeded(__in const BYTE* pbKey,
		__in DWORD cbKeyLen,
		__in const BYTE* pbCipherData,
		__in DWORD cbCipherDataLen);

	//CSP �ڲ�����
private:
	//������ʼ��
	BOOL	CspInit();

	//�������8λ��Կ
	BOOL	GenerateKey(BYTE * pbKey = NULL, DWORD *pnKeyLen = NULL);

	//������Կ
	BOOL	ImportKey();

	//����
	BOOL	EncrypteData(__inout const BYTE* pbKey,
		__inout DWORD *pcbKeyLen,
		__in const BYTE* pbPlainText,
		__in DWORD cbPlainDataLen,
		__out BYTE* pbCipherBuf,
		__inout DWORD* pcbCipherBufLen);

	//����
	BOOL	DecrypteData(__in const BYTE* pbKey,
		__in DWORD cbKeyLen,
		__in const BYTE* pbCipherData,
		__in DWORD cbCipherDataLen,
		__out BYTE* pbPlainTextBuf,
		__inout DWORD* pcbPlainTextBufLen);

	//�ͷ�����
	BOOL	Release();

	//��������
private:
	//���루16���ƣ�
	BOOL HexEncodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//�����루16���ƣ�
	BOOL HexDecodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//16����תʮ����
	BOOL HexToInt(unsigned char *HexData);


private:
	KEY_BLOB_T		m_stKey;
	HCRYPTKEY		m_hKey;
	HCRYPTPROV		m_hCryptProv;		//CSP�������
	TCHAR			m_szCspName[RSA_CONTAINER_NAME_LEN];		//������

};

#endif