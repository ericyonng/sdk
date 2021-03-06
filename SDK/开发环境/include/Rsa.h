#ifndef RSA_H_
#define RSA_H_

#pragma once

/* RSA-DES 密钥交换系统介绍
*
* 使用rsa-des混合加密密钥交换系统：
*先使用des 判断明文所需的密文空间：EncrypteMemoryNeeded 同时得到des key
*再对明文进行加密并编码得到密文：Encrypte
*把得到的des key 使用对方的加密公钥 rsa加密 Encrypte 得到des key 的密文
*将得到的密文 deskey 的密文两个密文传输给 对方 
*对方通过rsa私钥解密Decrypte 得到des key 的明文
*再通过解密后的des key 解密Decrypte 传输过来的有用数据的密文 得到明文
*rsa-des混合加密密钥交换系统介绍完毕
*end
*/

//RSA头文件支持
#include<Windows.h>
#include <Wincrypt.h>
#include"CommonModulHead.h"
#include<string>
using namespace std;

//宏定义
#define		PLAIN_TEXT_MAX_LEN					64						//明文长度限制（采用分组加密）117（1024/8-11）是rsa1024对明文的限制
#define		CIPHER_TEXT_OUT_ENCODE_LEN			256						//加密并编码后的数据长度（无论明文多长 不超过对明文的限制）
#define		CIPHER_TEXT_OUT_DECODE_LEN			128						//为编码的加密后的数据长度
#define		RSA_CONTAINER_NAME_LEN				128						//CSP名长度
#define		CSP_CONTAINER_NAME				TEXT("FengYanchenCsp")		//定义CSP容器名
#define		PUBLIC_KEY_APPNAME				TEXT("PUBLIC_KEY")			//公钥节点名
#define		PRIVATE_KEY_APPNAME				TEXT("PRIVATE_KEY")			//私钥节点名
#define		RSA_KEY_NAME					TEXT("key")					//键名
#define		RSA_KEY_STR_LEN					8192						//公钥长度

//钥匙结构体信息定义
typedef struct stkey{
	unsigned char ucKey[RSA_KEY_STR_LEN];			//key
	unsigned long ulKeyLen;			//key长度
}KEY_T;								//定义钥匙结构体信息						
#define S_KEY_T	sizeof(KEY_T)		//钥匙结构体长度	

//密钥定义
static	const char const YCRsaPublicKey[] = "";
static const char const YCRsaPrivateKey[] = "";

#define MY_ENCODE_TYPE   X509_ASN_ENCODING | PKCS_7_ASN_ENCODING
#define CRYPT_STRING_NOCRLF                 0x40000000//2003版本编译器不支持补上

// BASE64编码后的签名公钥	20170323	shy
static const char *szYCSignaturePubKey = "MIGJAoGBAI3U3kCD19AINDV3JOuqU5BOw617Nkz2Z6GNyHbCV3aA/HCXoyp6pzPNKo4Ytu2JIlgi+bFBCUD/2QIBjI4qDhL2GOjuZfHMNlpGlrwesp2CLMEunRqU/XKR+2R91gxP5UQMCb8RI4CTFNizndinzjFirYl/Vjgd+JS7zplvZ2FTAgMBAAE=";

//BASE64编码后的签名私钥	20170323	shy
static const char *szYCSignaturePrivKey = "MIICXAIBAAKBgQCN1N5Ag9fQCDQ1dyTrqlOQTsOtezZM9mehjch2wld2gPxwl6MqeqczzSqOGLbtiSJYIvmxQQlA/9kCAYyOKg4S9hjo7mXxzDZaRpa8HrKdgizBLp0alP1ykftkfdYMT+VEDAm/ESOAkxTYs53Yp84xYq2Jf1Y4HfiUu86Zb2dhUwIDAQABAoGAA6T4oQfCP+I2FTmehT+3LfoYJRkJaCjcuNX/1K1hufXOhsYetzS3nykicsxHxwrDCyi66ccWCcmJNff8OJr0CZblnhng02NsW4RxY0u6ySsdfH9XwEm1JWauqo3UeT7GRHLcV4+ksfiUsdEqtiNR2LLkp1yIIG7n4IiT3mnwdcECQQDDPv67bGuPVItKNTnPVXenkfPrGPRC7+0Rs6KlhhY+H3U5jBWXnOilwUwN4nFECWjcpHjrK1et99yaJ9JufZOrAkEAufbzZZuwKD8BVaFlj4DjSN73GE4WSe4JeswhPMru4wZhzh03xEe3Wl/0WG0RV5wKFVXymlN4moVddYE63whA+QJANqKTH5lIQ/RxgdOBGM6ezApgiiISgASpfzGYSt9JrBqbb/4OML3csD117dC2x0YmzM9NculPAMSCO7T3AqorPwJBAIDDLzDRph2X0pK6GBpPAhD44Vqg5un1ufae1PzE/rePObV/oZd0mM4o1oVLr9BbqNexFTQqS72iTl4IRv0bN/ECQHdeULI7dZczNWkJXiQTDs0E4+oz6d5yOMkrMYFUJvEzYbvvwf54sWo4ORtSZ11iL9cn1x2s+KIZro2CBT/BRQM=";


/////////////////////////////////////////////////////////

//RSA引擎
class COMMON_MODUL_CLASS CRsaEngine
{
public:
	CRsaEngine();
	virtual ~CRsaEngine();
	//释放对象
	virtual void FreeObj(){ try{ delete this; } catch (...){} }

	//接口函数
public:
	//加密并编码 bFinal用于指定是不是最后一个数据，循环加密的时候最后一个要加密的为TRUE，其余的必须为FALSE否则程序最好会崩溃
	BOOL Encrypte(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut, BOOL bFinal = TRUE);
	//解码并解密 bFinal用于指定是不是最后一个解密的数据，循环解密的时候最后一个要解密的为TRUE，其余的必须为FALSE否则程序最好会崩溃
	BOOL Decrypte(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut, BOOL bFinal = TRUE);
	//Csp初始化（初始化容器，导入密钥（公钥和私钥），或者生成密钥）
	BOOL CSPInit(bool bUseExistKeyPairs = false, LPTSTR szPublicKeyFile = NULL, LPTSTR szPrivateKeyFile = NULL);

	//设置公钥 经过编码的
	BOOL SetPublicKey(unsigned char *ucKey);
	
	//设置私钥 经过编码的
	BOOL SetPrivateKey(unsigned char *ucKey);

	//签名验签
public:
	//签名	20170323	shy	128字节
	string SignatureData(char * strData, long lDataSize);
	//验签	20170323	shy
	bool VerifySignature(char * strUnSignData, long lUnSignDataSize, char * strSignData, long lSignDataSize);
	//生成签名密钥对	20170323	shy
	bool GenRsaSignatureKeyPairs(bool bGenerateKeyFile = false);	//禁用功能
	//导入签名密钥	20170323	shy
	bool ImportRsaSignatureKey(HCRYPTPROV* hProv, HCRYPTKEY* hKey, const char* szRSAKey, bool isPrivKey);


	//密钥初始化
private:
	//销毁容器和释放key
	BOOL Release();
	//生成密钥对
	BOOL GenerateKeyPairs();
	//导入密钥对（若已经存在密钥对则先销毁后导入）
	BOOL ImportKeyPairs();

	//内部加密解密
private:
	//加密数据 
	BOOL EncrypteData(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut, BOOL bFinal = TRUE);
	//解密数据 bFinal用于指定是不是最后一个解密的数据，循环解密的时候最后一个要解密的为TRUE，其余的必须为FALSE否则程序最好会崩溃
	BOOL DecrypteData(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut, BOOL bFinal = TRUE);


	//辅助函数
private:
	//编码（16进制）
	BOOL HexEncodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//反编码（16进制）
	BOOL HexDecodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//16进制转十进制
	BOOL HexToInt(unsigned char *HexData);
	//UnicodeToAnsi
	BOOL UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte = NULL);
	//AnsiToUnicode
	BOOL AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail = NULL);
	//从文件中读取public key 和private key
	BOOL ReadPublicPrivateKey(LPTSTR szPublicKeyFile, LPTSTR szPrivateKeyFile);
	//写入配置文件
	BOOL WritePublicPrivateKey(LPTSTR szPublicKeyFile, LPTSTR szPrivateKeyFile);

private:
	//数据成员
	//CSP, PRIVATEKEY,PUBLICKEY,
	HCRYPTPROV		m_hCryptProv;		//CSP容器句柄
	HCRYPTKEY		m_hPublicKey;		//公钥句柄
	HCRYPTKEY		m_hPrivateKey;		//私钥句柄
	TCHAR			m_szCspName[RSA_CONTAINER_NAME_LEN];		//容器名
	KEY_T			m_stPublicKey;								//公钥
	KEY_T			m_stPrivateKey;			//私钥
};

//分组加密
class COMMON_MODUL_CLASS CBlockRsa : public CRsaEngine
{
public:
	CBlockRsa(){}
	virtual ~CBlockRsa(){}

public:
	//分组循环加密数据 每组数据固定64字节明文
	BOOL DataBlockCircleEncrypte(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut);
	//分组解密数据
	BOOL DataBlockCircleDecrypte(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut);
};



////////////////////////////////////////////////////////////////////////////////////

#define DES_KEY_BIT			8				//key的位数

#define		CSP_DES_NAME					TEXT("FengDesCsp")			//des容器名

//密码库定义
#define DES_NUM_TYPE			0			//数字
#define DES_CHAR_TYPE			1			//英文
#define DES_SYMBOL_TYPE			2			//字符
#define DES_KEY_TYPE_NUM		3			//种类总数
#define DES_NUM_LEN				10
#define DES_CHAR_LEN			26
#define DES_SYMBOL_LEN			14

//#ifdef _UNICODE
typedef struct stKeyBlob
{
	BLOBHEADER hdr;
	DWORD cbKeySize;			//只支持8位密码
	BYTE rgbKeyData[DES_KEY_BIT];		//只支持8位密码
} KEY_BLOB_T;    //结构参考MSDN - CryptImportKey
#define S_KEY_BLOB_T sizeof(KEY_BLOB_T)
//#endif


//数字
static const char DesKeyNumLib[DES_NUM_LEN] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
//字母
static const char DesKeyCharLib[DES_CHAR_LEN] = { 'a', 'b', 'c', 'd', 'e'
, 'f', 'g', 'h', 'i', 'j'
, 'k', 'l', 'm', 'n', 'o'
, 'p', 'q', 'r', 's', 't'
, 'u', 'v', 'w', 'x', 'y'
, 'z' };

//符号
static const char DesKeySymbolLib[DES_SYMBOL_LEN] = { '~', '!', '@', '#'
, '$', '%', '^'
, '&', '*', '('
, ')', '_', '+'
, '`' };




//des加密解密
/*
DES加密、解密函数，使用PKCS 5 padding，CBC模式

参数：
pbKey       DES密钥
cbKeyLen    pbKey字节长度
pbData      要加密、解密的数据
cbDataLen   pbData字节长度
pbBuf       输出缓冲区，输出加密后、解密后的数据，可以为NULL
pcbBufLen   pbBuf字节长度
当pbBuf不为NULL时，返回实际复制到pbBuf的字节长度
当pbBuf为NULL时，返回需要的pbBuf字节长度
bIsDecrypt  为TRUE时执行解密操作，否则执行加密操作

返回：
成功返回TRUE，否则返回FALSE
*/

#define __in	/*仅输入in*/
#define __out	/*仅输出out*/
#define __inout	/*输入输出inout*/

//1.先测算加密所需空间大小，new出密文空间 EncrypteMemoryNeeded
//2.加密并编码 Encrypte
//3.解码并解密 Decrypte
class COMMON_MODUL_CLASS CDESEngine
{
public:
	CDESEngine();
	virtual ~CDESEngine();
	//释放对象
	virtual void FreeObj(){ try{ delete this; } catch (...){} }

	//接口函数
public:
	//加密并编码
	BOOL Encrypte(__inout const BYTE* pbKey,
		__inout DWORD *pcbKeyLen,
		__in const BYTE* pbPlainText,
		__in DWORD cbPlainDataLen,
		__out BYTE* pbCipherBuf,
		__inout DWORD* pcbCipherBufLen);

	//解码并解密
	BOOL Decrypte(__in const BYTE* pbKey,
		__in DWORD cbKeyLen,
		__in const BYTE* pbCipherData,
		__in DWORD cbCipherDataLen,
		__out BYTE* pbPlainTextBuf,
		__inout DWORD* pcbPlainTextBufLen);

	//加密所需要的空间
	int    EncrypteMemoryNeeded(__inout const BYTE* pbKey,
		__inout DWORD *pcbSizeInKeyLenOut,
		__in const BYTE* pbPlainText,
		__in DWORD cbPlainDataLen);

	//解密所需要的空间
	int		DecrypteMemoryNeeded(__in const BYTE* pbKey,
		__in DWORD cbKeyLen,
		__in const BYTE* pbCipherData,
		__in DWORD cbCipherDataLen);

	//CSP 内部操作
private:
	//容器初始化
	BOOL	CspInit();

	//产生随机8位密钥
	BOOL	GenerateKey(BYTE * pbKey = NULL, DWORD *pnKeyLen = NULL);

	//导入密钥
	BOOL	ImportKey();

	//加密
	BOOL	EncrypteData(__inout const BYTE* pbKey,
		__inout DWORD *pcbKeyLen,
		__in const BYTE* pbPlainText,
		__in DWORD cbPlainDataLen,
		__out BYTE* pbCipherBuf,
		__inout DWORD* pcbCipherBufLen);

	//解密
	BOOL	DecrypteData(__in const BYTE* pbKey,
		__in DWORD cbKeyLen,
		__in const BYTE* pbCipherData,
		__in DWORD cbCipherDataLen,
		__out BYTE* pbPlainTextBuf,
		__inout DWORD* pcbPlainTextBufLen);

	//释放清理
	BOOL	Release();

	//辅助函数
private:
	//编码（16进制）
	BOOL HexEncodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//反编码（16进制）
	BOOL HexDecodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut);
	//16进制转十进制
	BOOL HexToInt(unsigned char *HexData);


private:
	KEY_BLOB_T		m_stKey;
	HCRYPTKEY		m_hKey;
	HCRYPTPROV		m_hCryptProv;		//CSP容器句柄
	TCHAR			m_szCspName[RSA_CONTAINER_NAME_LEN];		//容器名

};

#endif