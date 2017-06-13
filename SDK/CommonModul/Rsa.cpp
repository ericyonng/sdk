#include"stdafx.h"
#include"Rsa.h"
#include"IniFileIO.h"
#include<stdio.h>
#pragma comment(lib, "Crypt32")	//�ӽ���ʱ�õ�
#define COMBINE_BYTE(High_Bit, Low_Bit)	((High_Bit&0x0F)<<4)|(Low_Bit&0x0F)		//����λ����λ�ϲ�һ���ֽ�,0x0F��ֻ֤��4λ����


//����
CRsaEngine::CRsaEngine()
{
	m_hCryptProv = NULL;
	m_hPublicKey = NULL;
	m_hPrivateKey = NULL;
	memset(m_szCspName, 0, sizeof(TCHAR)* RSA_CONTAINER_NAME_LEN);
	memmove(m_szCspName, CSP_CONTAINER_NAME, sizeof(TCHAR)* lstrlen(CSP_CONTAINER_NAME));
	memset(&m_stPublicKey, 0, S_KEY_T);
	memset(&m_stPrivateKey, 0, S_KEY_T);
}

CRsaEngine::~CRsaEngine()
{
	Release();
}

//���ܲ�����
BOOL CRsaEngine::Encrypte(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut, BOOL bFinal)
{
	if (!pCipherTextOut || !plaintext || !pCipherSizeInLenOut || (*pCipherSizeInLenOut <= 0)) return FALSE;

	if (!CSPInit(true)) return FALSE;

	if (!m_hPublicKey)
	{
		Release();
		return FALSE;
	}
	unsigned long ulCipherSize = *pCipherSizeInLenOut;

	//����
	if (!EncrypteData(plaintext, ulPlainTextLen, pCipherTextOut, &ulCipherSize, bFinal))
	{
		Release();
		return FALSE;
	}

	//�������
	if (!HexEncodeData(*pCipherTextOut, ulCipherSize, *pCipherTextOut, pCipherSizeInLenOut))
	{
		Release();
		return FALSE;
	}

	Release();
	return TRUE;
}

//���벢����
BOOL CRsaEngine::Decrypte(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut, BOOL bFinal)
{
	if (!HexDecodeData(ciphertext, cipherlen, ciphertext, &cipherlen)) return FALSE;

	if (!CSPInit(true)) return FALSE;

	if (!DecrypteData(ciphertext, cipherlen, plaintext, pPlainSizeInLenOut, bFinal))
	{
		Release();
		return FALSE;
	}
	Release();

	return TRUE;
}



//��������
BOOL CRsaEngine::EncrypteData(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut, BOOL bFinal)
{
	if (!pCipherTextOut || !plaintext || !pCipherSizeInLenOut || (*pCipherSizeInLenOut <= 0)) return FALSE;

	if (!m_hPublicKey) return FALSE;

	DWORD lEncryptLen = ulPlainTextLen;	//���������ĳ��ȣ�

	// ��ü��ܺ�����ݳ���
	if (!CryptEncrypt(m_hPublicKey, NULL, bFinal, 0, NULL, &lEncryptLen, 0))//����lEncryptLen����������ֽ�
	{
		//TRACE("CryptEncrypt error 0x%x\n", GetLastError());
		return FALSE;
	}

	// Create a buffer for encrypted data
	//pbEncrypted = (BYTE*)realloc(pbEncrypted, dwEncryptedLen);
	unsigned char *strCache = new unsigned char[lEncryptLen];
	if (!strCache) return FALSE;
	memset(strCache, 0, lEncryptLen);
	memmove(strCache, plaintext, ulPlainTextLen);

	DWORD dwOutCacheSize = ulPlainTextLen;

	// Encrypt data
	if (!CryptEncrypt(m_hPublicKey, NULL, bFinal, 0, strCache, &dwOutCacheSize, lEncryptLen))//����dwOutCacheSize�ֽ����ݣ���������СlEncryptLen
	{
		//TRACE("CryptEncrypt error 0x%x\n", GetLastError());
		return FALSE;
	}

	if (*pCipherSizeInLenOut < dwOutCacheSize)
	{
		*pCipherSizeInLenOut = dwOutCacheSize;
		delete[]strCache;
		return FALSE;
	}

	memset((char *)(*pCipherTextOut), 0, *pCipherSizeInLenOut);

	memmove((char *)*pCipherTextOut, strCache, dwOutCacheSize);
	*pCipherSizeInLenOut = dwOutCacheSize;

	delete[]strCache;

	return TRUE;
}

//��������
BOOL CRsaEngine::DecrypteData(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut, BOOL bFinal)
{
	if (!ciphertext || !plaintext || !pPlainSizeInLenOut || (*pPlainSizeInLenOut <= 0)) return FALSE;
	if (!m_hPrivateKey) return FALSE;

	//��������
	unsigned char *pCipherText = new unsigned char[cipherlen];
	if (!pCipherText) return FALSE;
	memset(pCipherText, 0, cipherlen);
	memmove(pCipherText, ciphertext, cipherlen);

	// ���ܺ󸲸�ԭ������Ϣ
	if (!CryptDecrypt(m_hPrivateKey, NULL, bFinal, 0, pCipherText, &cipherlen))
	{
		delete[]pCipherText;

		//TRACE("CryptDecrypt error 0x%x\n", GetLastError());
		return FALSE;
	}

	if (*pPlainSizeInLenOut < cipherlen)
	{
		delete[]pCipherText;
		*pPlainSizeInLenOut = cipherlen;
		return FALSE;
	}

	memset(*plaintext, 0, *pPlainSizeInLenOut);
	memmove(*plaintext, pCipherText, cipherlen);

	*pPlainSizeInLenOut = cipherlen;


	//�ָ���������ռ�
	delete[]pCipherText;

	return TRUE;
}

//���루16���ƣ�
BOOL CRsaEngine::HexEncodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut)
{
	//����У��
	if (!pucDataSrc || !pucDataOut || !pulDataSizeInLenOut) return FALSE;
	if (*pulDataSizeInLenOut < 2 * ulDataSrcDataLen)
	{
		*pulDataSizeInLenOut = 2 * ulDataSrcDataLen;
		return FALSE;
	}

	//������
	unsigned long n = 0;
	unsigned char ucCache[3] = { 0 };
	unsigned char *ucDataCache = new unsigned char[2 * ulDataSrcDataLen];
	if (!ucDataCache) return FALSE;
	memset((void *)ucDataCache, 0, 2 * ulDataSrcDataLen);

	unsigned char *pDataCacheTmp = ucDataCache;
	unsigned char *pDataSrcTmp = pucDataSrc;

	//����
	for (n = 0; n < ulDataSrcDataLen; n++)
	{
		memset(ucCache, 0, 3);
		sprintf((char *)ucCache, "%02X", *pDataSrcTmp);
		memmove(pDataCacheTmp, ucCache, 2);
		pDataSrcTmp++;
		pDataCacheTmp += 2;
	}

	//����Ǩ�Ʋ��ͷſռ�
	memset(pucDataOut, 0, *pulDataSizeInLenOut);
	memmove(pucDataOut, ucDataCache, 2 * ulDataSrcDataLen);
	char miwend[8192] = { 0 };
	memmove(miwend, ucDataCache, 2 * ulDataSrcDataLen);
	*pulDataSizeInLenOut = 2 * ulDataSrcDataLen;
	delete[]ucDataCache;

	return TRUE;
}

//�����루16���ƣ�
BOOL CRsaEngine::HexDecodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut)
{
	//����У��
	if (!pucDataSrc || !pucDataOut || !pulDataSizeInLenOut) return FALSE;

	unsigned char strtmp[3] = { 0 };
	int strcount = ((ulDataSrcDataLen) % 2 == 0 ? (ulDataSrcDataLen) / 2 : ((ulDataSrcDataLen) / 2 + 1));
	char *decodetmp = new char[strcount];
	if (!decodetmp) return FALSE;
	memset(decodetmp, 0, strcount);

	if (*pulDataSizeInLenOut <  strcount)
	{
		*pulDataSizeInLenOut = strcount;
		delete[]decodetmp;
		return FALSE;
	}

	//ת��
	for (int i = 0; i<strcount; i++)
	{
		memset(strtmp, 0, 3);
		memmove(strtmp, pucDataSrc + 2 * i, 2);
		if (!HexToInt(&strtmp[0]))
			return false;
		if (!HexToInt(&strtmp[1]))
			return false;
		decodetmp[i] = COMBINE_BYTE(strtmp[0], strtmp[1]);
	}

	//ת������
	memset(pucDataOut, 0, *pulDataSizeInLenOut);
	memmove(pucDataOut, decodetmp, strcount);
	*pulDataSizeInLenOut = strcount;

	delete[]decodetmp;

	return TRUE;
}

//16����תʮ����
BOOL CRsaEngine::HexToInt(unsigned char *HexData)
{
	//����У��
	if (!HexData) return FALSE;

	if (*HexData >= '0'&&*HexData <= '9')
	{
		*HexData = *HexData - '0' + 0;
	}
	else if (*HexData >= 'A'&&*HexData <= 'F')
	{
		*HexData = *HexData - 'A' + 10;
	}
	else if (*HexData >= 'a'&&*HexData <= 'f')
	{
		*HexData = *HexData - 'a' + 10;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

//Csp��ʼ������ʼ��������������Կ����Կ��˽Կ��������������Կ��
BOOL CRsaEngine::CSPInit(bool bUseExistKeyPairs, LPTSTR szPublicKeyFile, LPTSTR szPrivateKeyFile)
{
	//�����Ѵ���
	if (m_hCryptProv) return TRUE;

	//�򿪻򴴽�CSP����
	if (!CryptAcquireContext(&m_hCryptProv, m_szCspName, NULL, PROV_RSA_FULL, 0))//Ĭ������Ϊ0����ȡ�������½�
	{
		if (GetLastError() == NTE_BAD_KEYSET)//�������ܱ���
		{
			if (!CryptAcquireContextW(&m_hCryptProv, m_szCspName, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))//ʹ��ָ���������½�
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	//�Ƿ�ʹ���Ѵ���key
	if (bUseExistKeyPairs)
	{
		//������Կ�Ե���
		if (!ReadPublicPrivateKey(szPublicKeyFile, szPrivateKeyFile)) return FALSE;
	}
	else
	{
		//������Կ��
		if (!GenerateKeyPairs()) return FALSE;

		//д���ļ�
		WritePublicPrivateKey(szPublicKeyFile, szPrivateKeyFile);
	}

	//������Կ
	if (!ImportKeyPairs()) return FALSE;

	return TRUE;
}

//���ù�Կ ���������
BOOL CRsaEngine::SetPublicKey(unsigned char *ucKey)
{
	m_stPublicKey.ulKeyLen = RSA_KEY_STR_LEN;

	//������
	if (!HexDecodeData(ucKey, strlen((char *)ucKey), (m_stPublicKey.ucKey), &m_stPublicKey.ulKeyLen)) return FALSE;

	return TRUE;
}

//����˽Կ ���������
BOOL CRsaEngine::SetPrivateKey(unsigned char *ucKey)
{
	m_stPrivateKey.ulKeyLen = RSA_KEY_STR_LEN;

	//������
	if (!HexDecodeData(ucKey, strlen((char *)ucKey), (m_stPrivateKey.ucKey), &m_stPrivateKey.ulKeyLen)) return FALSE;
}

//ǩ��	20170323	shy	128�ֽ�
string CRsaEngine::SignatureData(char * strData, long lDataSize)
{
	//У�����
	if (!strData || lDataSize <= 0 || lDataSize >= 128) return "";
	//if (strData.empty()) return "";

	string sRet("");

	HCRYPTPROV hProv = NULL;	//����
	HCRYPTKEY hKey = NULL;		//��Կ���
	HCRYPTHASH hHash = NULL;			//��ϣ���

	if (!ImportRsaSignatureKey(&hProv, &hKey, szYCSignaturePubKey, false))
	{
		printf("ImportRsaKey for public key failed with error 0x%.8X\n", GetLastError());
		goto Exit;
	}

	BYTE bData[128];
	ZeroMemory(bData, sizeof(BYTE)* 128);

	//2016-05-05 modified by songhuiya
	//memcpy_s(bData,sizeof(BYTE)*128, strSrc.c_str(), strSrc.size());
	memmove(bData, strData, lDataSize);
	//////////////////////////////////////////////////////////
	DWORD dwDataLen = (DWORD)lDataSize;

	//������ϣ�����ָ����ϣ�㷨���������CALG_SHA1,��sha1�㷨
	if (CryptCreateHash(
		hProv,
		CALG_SHA1,
		0,
		0,
		&hHash))
	{
		printf("������ϣ����ɹ���\n");
	}
	else
	{
		goto Exit;
		//HandleError("����CryptCreateHashʧ��.");
	}

	//--------�����ݼ����ϣ���ժҪ-----------------------------------
	if (CryptHashData(
		hHash,
		bData, //����������ݽ���hash����,����������hashժҪ����
		dwDataLen,
		0))
	{
		printf("�����ϣ�ɹ�\n");
	}
	else
	{
		goto Exit;
		//HandleError("����CryptHashDataʧ�ܡ�");
	}

	//--------��ժҪ����ǩ������һ�ε��û��ǩ�������ݵĳ���-----------
	DWORD dwSigLen = 0;
	BYTE *pbSignature = NULL;  //����ǩ��ֵ�Ļ�����ָ��
	if (CryptSignHash(
		hHash,
		AT_SIGNATURE,
		NULL,
		0,
		NULL,
		&dwSigLen))
	{
		printf("ǩ��ֵ�ĳ���Ϊ%d \n", dwSigLen);
	}
	else
	{
		goto Exit;
		//HandleError("����CryptSignHashʧ�ܡ�");
	}
	if (pbSignature = (BYTE*)malloc(dwSigLen))
	{
		;
	}
	else
	{
		goto Exit;
		//HandleError("�ڴ治������");
	}

	//-----------��hash����ǩ��------------------------------------------
	if (CryptSignHash(
		hHash,
		AT_SIGNATURE,
		NULL,
		0,
		pbSignature,
		&dwSigLen))
	{
		printf("����ǩ���ɹ���\n");
	}
	else
	{
		printf("����cryptSignHashʧ�ܡ���");
		goto Exit;
	}


	//------------���ǩ������-------------------------------------------
	printf("ǩ��ֵ:\n");
	for (int i = 0; i<dwSigLen; i++)
	{
		if ((i == 0) && (i != 0))
		{
			printf("\n");
		}
		printf("%2.2x", pbSignature[i]);
	}
	printf("\n");
	printf("ǩ���ɹ������ȣ�%d\n\n", dwSigLen * 2);

	//---------�ͷ���Դ--------------



	/*
	*  2016-04-21 Added by rinmb
	*  �ֽڷ�ת��Ϊ��������ƽ̨��RSA�㷨��openssl��java��c#��
	*  ��ϵͳֻ�����C++��C#�����Կ�ȥ���������.
	*  ��C#�ˣ�����ʱҲ��Ҫ�ȷ�ת�������������.
	*/
	//char c = 0;
	//   for (int i = 0; i < (dwDataLen / 2); i++) 
	//   {
	//       c = bData[i];
	//       bData[i] = bData[dwDataLen - 1 - i];
	//       bData[dwDataLen - 1 - i] = c;
	//   }

	char *pDataCache = new char[dwSigLen * 2 + 1];
	if (!pDataCache) return "";
	memset(pDataCache, 0, dwSigLen * 2 + 1);
	unsigned long lDataCache = dwSigLen * 2 + 1;
	HexEncodeData(pbSignature, dwSigLen, (unsigned char *)pDataCache, &lDataCache);
	sRet = pDataCache;
	if (pDataCache) delete[]pDataCache;

Exit:
	//�ͷ���Դ
	if (pbSignature) free(pbSignature);
	//���ٹ�ϣ����
	if (hHash)
		CryptDestroyHash(hHash);
	if (hKey) CryptDestroyKey(hKey);
	if (hProv) CryptReleaseContext(hProv, 0);

	return sRet;
}

//��ǩ	20170323	shy 128�ֽ�
bool CRsaEngine::VerifySignature(char * strUnSignData, long lUnSignDataSize, char * strSignData, long lSignDataSize)
{
	//У�����
	if (!strUnSignData || lUnSignDataSize <= 0 || !strSignData || lSignDataSize <= 0 || lUnSignDataSize >= 128 || lSignDataSize >= 128) return false;
	//if (strData.empty()) return "";

	string sRet("");

	HCRYPTPROV hProv = NULL;	//����
	HCRYPTKEY hKey = NULL;		//��Կ���
	HCRYPTHASH hHash = NULL;			//��ϣ���

	if (!ImportRsaSignatureKey(&hProv, &hKey, szYCSignaturePubKey, false))
	{
		printf("ImportRsaKey for private key failed with error 0x%.8X\n", GetLastError());
		goto Exit;
	}

	BYTE bData[128];
	ZeroMemory(bData, sizeof(BYTE)* 128);
	unsigned long lDataSize = 128 * sizeof(BYTE);
	int nRetVal = HexDecodeData((unsigned char *)strSignData, lSignDataSize, bData, &lDataSize);
	if (nRetVal != 0) return false;
	DWORD dwDataLen = lSignDataSize / 2;
	printf("dwDatalen:%d, \r\n%s\r\n", dwDataLen, strSignData);

	BYTE bUnsignData[128] = { 0 };
	memmove(bUnsignData, strUnSignData, lUnSignDataSize);
	//sprintf((char *)bUnsignData, "%s", strUnSignData.c_str());
	DWORD dwUnsignDataLen = lUnSignDataSize;



	//--------------����hash����--------------------------------------------
	if (CryptCreateHash(
		hProv,
		CALG_SHA1,
		0,
		0,
		&hHash))
	{
		printf("������ϣ����ɹ� \n");
	}
	else
	{
		printf("����CryptCreateHashʧ��\r\n");
		goto Exit;
	}

	//--------------�����ϣ-----------------------------------
	if (CryptHashData(
		hHash,
		bUnsignData,
		dwUnsignDataLen,
		0))
	{
		printf("���ݹ�ϣ���.\n");
	}
	else
	{
		printf("����CryptHashDataʧ��\r\n");
		goto Exit;
	}

	//--------------��֤ǩ��-----------------------------------
	bool bRet = false;
	if (bRet = CryptVerifySignature(
		hHash,
		bData,
		dwDataLen,
		hKey,
		NULL,
		0))
	{
		printf("��֤ǩ���ɹ���\n");
	}
	else
	{
		printf("ǩ����֤ʧ�ܣ�ǩ����Ч0x%x\r\n", GetLastError());
		goto Exit;
		//HandleError("ǩ����֤ʧ�ܣ�ǩ����Ч");
	}

Exit:
	if (hHash) CryptDestroyHash(hHash);
	if (hKey) CryptDestroyKey(hKey);
	if (hProv) CryptReleaseContext(hProv, 0);

	return bRet;
}

//����ǩ����Կ��	20170323	shy
bool CRsaEngine::GenRsaSignatureKeyPairs(bool bGenerateKeyFile)	//���ù���
{
	HCRYPTKEY key = NULL;
	HCRYPTPROV provider = NULL;

	DWORD pubKeyLen = 0;
	BYTE* pbPubKey = NULL;

	DWORD privKeyLen = 0;
	BYTE* pbPrivKey = NULL;

	// 1.����RSA���
	if (!CryptAcquireContext(&provider, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		if (GetLastError() == NTE_BAD_KEYSET)
		{
			if (!CryptAcquireContext(&provider, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if (!provider)
		return false;

	// 2.������Կ��
	if (!CryptGenKey(provider, AT_SIGNATURE, RSA1024BIT_KEY | CRYPT_EXPORTABLE, &key))
	{
		return false;
	}

	// 3.������Կ
	if (!CryptExportKey(key, 0, PUBLICKEYBLOB, 0, NULL, &pubKeyLen))
	{
		if (key) CryptDestroyKey(key);
		return false;
	}
	pbPubKey = (unsigned char *)malloc(pubKeyLen * sizeof(unsigned char));
	if (pbPubKey == NULL)
	{
		if (key) CryptDestroyKey(key);
		return false;
	}
	SecureZeroMemory(pbPubKey, pubKeyLen * sizeof(unsigned char));

	if (!CryptExportKey(key, 0, PUBLICKEYBLOB, 0, pbPubKey, &pubKeyLen))
	{
		if (key) CryptDestroyKey(key);
		return false;
	}

	// buffer
	DWORD cbEncoded;
	// length of the encoded string
	DWORD cbDecoded;
	// length of the decoded buffer
	BYTE *pbEncoded = NULL;
	// encoded buffer
	BYTE *pbDecoded = NULL;


	if (CryptEncodeObjectEx(
		MY_ENCODE_TYPE,        // the encoding/decoding type
		RSA_CSP_PUBLICKEYBLOB,
		pbPubKey,
		0,
		NULL,
		NULL,
		&cbEncoded))    // fill in the length needed for
	{
		printf("The number of bytes needed is %d \n", cbEncoded);
	}
	else
	{
		return false;
	}

	if (pbEncoded = (BYTE*)malloc(cbEncoded))
	{
		printf("Memory for pvEncoded has been allocated.\n");
	}
	else
	{
		return false;
	}

	if (CryptEncodeObjectEx(
		MY_ENCODE_TYPE,
		RSA_CSP_PUBLICKEYBLOB,
		pbPubKey,
		0,
		NULL,
		pbEncoded,
		&cbEncoded))
	{
		printf("The public key encoded! \n");
	}
	else
	{
		return false;
	}

	DWORD dwEncodeLen = 0;
	LPSTR szEncodeStr;

	if (!CryptBinaryToStringA(pbEncoded, cbEncoded, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &dwEncodeLen))
		return false;

	szEncodeStr = (char *)malloc(dwEncodeLen * sizeof(char));
	if (szEncodeStr == NULL) return false;
	SecureZeroMemory(szEncodeStr, dwEncodeLen * sizeof(char));

	if (!CryptBinaryToStringA(pbEncoded, cbEncoded, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, szEncodeStr, &dwEncodeLen))
	{
		return false;
	}


	// 4.����˽Կ
	if (!CryptExportKey(key, 0, PRIVATEKEYBLOB, 0, NULL, &privKeyLen))
	{
		if (key) CryptDestroyKey(key);
		return false;
	}

	pbPrivKey = (unsigned char *)malloc(privKeyLen * sizeof(unsigned char));
	if (pbPrivKey == NULL)
	{
		if (key) CryptDestroyKey(key);
		return false;
	}
	SecureZeroMemory(pbPrivKey, privKeyLen * sizeof(unsigned char));

	if (!CryptExportKey(key, 0, PRIVATEKEYBLOB, 0, pbPrivKey, &privKeyLen))
	{
		if (key) CryptDestroyKey(key);
		return false;
	}

	// buffer
	DWORD cbEncoded2;
	// length of the encoded string
	DWORD cbDecoded2;
	// length of the decoded buffer
	BYTE *pbEncoded2;
	// encoded buffer
	BYTE *pbDecoded2;

	if (CryptEncodeObjectEx(
		MY_ENCODE_TYPE,        // the encoding/decoding type
		PKCS_RSA_PRIVATE_KEY,
		pbPrivKey,
		0,
		NULL,
		NULL,
		&cbEncoded2))    // fill in the length needed for
	{
		printf("The number of bytes needed is %d \n", cbEncoded2);
	}
	else
	{
		return false;
	}

	if (pbEncoded2 = (BYTE*)malloc(cbEncoded2))
	{
		printf("Memory for pvEncoded has been allocated.\n");
	}
	else
	{
		return false;
	}

	if (CryptEncodeObjectEx(
		MY_ENCODE_TYPE,
		PKCS_RSA_PRIVATE_KEY,
		pbPrivKey,
		0,
		NULL,
		pbEncoded2,
		&cbEncoded2))
	{
		printf("The private key encoded! \n");
	}
	else
	{
		return false;
	}

	DWORD dwEncodeLen2 = 0;
	LPSTR szEncodeStr2;

	if (!CryptBinaryToStringA(pbEncoded2, cbEncoded2, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &dwEncodeLen2))
		return false;

	szEncodeStr2 = (char *)malloc(dwEncodeLen2 * sizeof(char));
	if (szEncodeStr2 == NULL) return false;
	SecureZeroMemory(szEncodeStr2, dwEncodeLen2 * sizeof(char));

	if (!CryptBinaryToStringA(pbEncoded2, cbEncoded2, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, szEncodeStr2, &dwEncodeLen2))
	{
		return false;
	}

	if (bGenerateKeyFile)
	{
		printf("��Կ��\r\n");
		printf("%s\r\n", szEncodeStr);
		FILE *fpPub = fopen("Pub.txt", "w+");
		if (fpPub)
		{
			fwrite(szEncodeStr, strlen(szEncodeStr), 1, fpPub);
			fflush(fpPub);
			fclose(fpPub);
		}
		printf("˽Կ��\r\n");
		printf("%s\r\n", szEncodeStr2);
		FILE *fpPriv = fopen("Priv.txt", "w+");
		if (fpPriv)
		{
			fwrite(szEncodeStr2, strlen(szEncodeStr2), 1, fpPriv);
			fflush(fpPriv);
			fclose(fpPriv);
		}
	}

	return true;
}

//����ǩ����Կ	20170323	shy
bool CRsaEngine::ImportRsaSignatureKey(HCRYPTPROV* hProv, HCRYPTKEY* hKey, const char* szRSAKey, bool isPrivKey)
{
	DWORD dwBufferLen = 0, cbKeyBlob = 0;
	LPBYTE pbBuffer = NULL, pbKeyBlob = NULL;
	BOOL hasError = FALSE;
	LPCSTR  keyType;

	if (!CryptStringToBinaryA(szRSAKey, 0, CRYPT_STRING_BASE64, NULL, &dwBufferLen, NULL, NULL))
	{
		printf("Failed to convert BASE64 public key. Error 0x%.8X\n", GetLastError());
		hasError = TRUE;
		goto Exit;
	}

	pbBuffer = (LPBYTE)LocalAlloc(0, dwBufferLen);
	if (!CryptStringToBinaryA(szRSAKey, 0, CRYPT_STRING_BASE64, pbBuffer, &dwBufferLen, NULL, NULL))
	{
		printf("Failed to convert BASE64 public key. Error 0x%.8X\n", GetLastError());
		hasError = TRUE;
		goto Exit;
	}

	// �жϵ������˽Կ�����ǹ�Կ
	if (isPrivKey)
	{
		keyType = PKCS_RSA_PRIVATE_KEY;
	}
	else
	{
		keyType = RSA_CSP_PUBLICKEYBLOB;
	}

	if (!CryptDecodeObjectEx(MY_ENCODE_TYPE, keyType, pbBuffer, dwBufferLen, 0, NULL, NULL, &cbKeyBlob))
	{
		printf("Failed to parse public key. Error 0x%.8X\n", GetLastError());
		hasError = TRUE;
		goto Exit;
	}

	pbKeyBlob = (LPBYTE)LocalAlloc(0, cbKeyBlob);
	if (!CryptDecodeObjectEx(MY_ENCODE_TYPE, keyType, pbBuffer, dwBufferLen, 0, NULL, pbKeyBlob, &cbKeyBlob))
	{
		printf("Failed to parse public key. Error 0x%.8X\n", GetLastError());
		hasError = TRUE;
		goto Exit;
	}

	if (!CryptAcquireContext(hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		printf("CryptAcquireContext failed with error 0x%.8X\n", GetLastError());
		hasError = TRUE;
		goto Exit;
	}

	if (!CryptImportKey(*hProv, pbKeyBlob, cbKeyBlob, NULL, 0, hKey))
	{
		printf("CryptImportKey for public key failed with error 0x%.8X\n", GetLastError());
		hasError = TRUE;
		goto Exit;
	}

	printf("ImportRsaKey successfully!\n\n");

Exit:
	if (pbBuffer) LocalFree(pbBuffer);
	if (pbKeyBlob) LocalFree(pbKeyBlob);

	if (hasError) {
		return FALSE;
	}
	return TRUE;
}

//�����������ͷ�key
BOOL CRsaEngine::Release()
{
	int iAbort = 0;
	try
	{
		//�쳣1
		iAbort++;

		if (m_hPublicKey) CryptDestroyKey(m_hPublicKey);// printf("Error: %d\n", GetLastError());

		//�쳣2
		iAbort++;

		if (m_hPrivateKey) CryptDestroyKey(m_hPrivateKey);// printf("Error: %d\n", GetLastError());

		//�쳣3
		iAbort++;

		if (m_hCryptProv) CryptReleaseContext(m_hCryptProv, 0); //printf("Error: %d\n", GetLastError());//������������Ҫ���ñ�������޷�����ɹ�

		//�쳣4
		iAbort++;
	}
	catch (...)
	{
		//TCHAR yichang[200] = { 0 };
		//_sntprintf(yichang,200, TEXT("�쳣��%d\r\n"), iAbort);

		//m_IniFileIO.WriteString(TEXT("�쳣"), TEXT("yichang"), yichang, TEXT(".\\yichang.txt"));
		//	printf("%s", yichang);
		//getchar();
	}

	m_hPublicKey = NULL;
	m_hPrivateKey = NULL;
	m_hCryptProv = NULL;

	return TRUE;
}

//������Կ��
BOOL CRsaEngine::GenerateKeyPairs()
{
	if (!m_hCryptProv) return FALSE;

	HCRYPTKEY hKey = NULL;

	// Generate new key pair
	if (!CryptGenKey(m_hCryptProv, AT_KEYEXCHANGE, RSA1024BIT_KEY | CRYPT_EXPORTABLE, &hKey))
	{
		//TRACE(_T("CryptGenKey error 0x%x\n"), GetLastError());
		return FALSE;
	}

	// Get public key size
	if (!CryptExportKey(hKey, NULL, PUBLICKEYBLOB, 0, NULL, &m_stPublicKey.ulKeyLen))
	{
		//TRACE(_T("CryptExportKey error 0x%x\n"), GetLastError());
		return FALSE;
	}

	// Create a buffer for the public key
	//pbPublicKey = (BYTE *)malloc(dwPublicKeyLen);

	// Get public key
	if (!CryptExportKey(hKey, NULL, PUBLICKEYBLOB, 0, m_stPublicKey.ucKey, &m_stPublicKey.ulKeyLen))
	{
		//TRACE(_T("CryptExportKey error 0x%x\n"), GetLastError());
		return FALSE;
	}

	// Get private key size
	if (!CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, 0, NULL, &m_stPrivateKey.ulKeyLen))
	{
		//TRACE(_T("CryptExportKey error 0x%x\n"), GetLastError());
		return FALSE;
	}

	// Create a buffer for the private key
	//pbPrivateKey = (BYTE *)malloc(dwPrivateKeyLen);

	// Get private key
	//TRACE(_T("CryptExportKey...\n"));
	if (!CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, 0, m_stPrivateKey.ucKey, &m_stPrivateKey.ulKeyLen))
	{
		//TRACE(_T("CryptExportKey error 0x%x\n"), GetLastError());
		return FALSE;
	}

	if (hKey)
	{
		CryptDestroyKey(hKey);
	}

	return TRUE;
}

//������Կ�ԣ����Ѿ�������Կ���������ٺ��룩
BOOL CRsaEngine::ImportKeyPairs()
{
	if (!m_hCryptProv) return FALSE;

	BOOL bImportSuccess = FALSE;

	//����public key
	if (m_stPublicKey.ulKeyLen != 0)
	{
		if (!CryptImportKey(m_hCryptProv, (BYTE *)m_stPublicKey.ucKey, m_stPublicKey.ulKeyLen, 0, 0, &m_hPublicKey))
		{
			//TRACE("CryptImportKey error 0x%x\n", GetLastError());
			return FALSE;
		}

		bImportSuccess = TRUE;
	}

	//����Private key
	if (m_stPrivateKey.ulKeyLen != 0)
	{
		if (!CryptImportKey(m_hCryptProv, (BYTE *)m_stPrivateKey.ucKey, m_stPrivateKey.ulKeyLen, 0, 0, &m_hPrivateKey))
		{
			//TRACE("CryptImportKey error 0x%x\n", GetLastError());
			return FALSE;
		}

		bImportSuccess = TRUE;
	}

	return bImportSuccess;
}

//�ַ�ת��
BOOL CRsaEngine::UnicodeToAnsi(LPTSTR szUnicodeSrc, char *pAnsiStrOut, INT nAnsiStrSize, INT *pAnsiOutStrlenByte)
{
	//����У��
	if (szUnicodeSrc == NULL || pAnsiStrOut == NULL || nAnsiStrSize < 0) return FALSE;

	ZeroMemory(pAnsiStrOut, nAnsiStrSize);
	INT nLength = 0;

	//��ȡת�������ֽ��� 
	nLength = WideCharToMultiByte(CP_ACP, 0, szUnicodeSrc, -1, NULL, 0, NULL, NULL);

	//ת���ռ�Ϸ����ж�
	if (nLength > nAnsiStrSize)
	{
		//������ֽ���
		if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nLength;

		return FALSE;
	}

	INT nTransFormResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)szUnicodeSrc, -1, pAnsiStrOut, nAnsiStrSize, NULL, NULL);//UnicodeתANSI����һ��������ǰϵͳANSI,�ڶ���������Ĭ��0�����ڶ����ǲ���ת�������滻���ַ������һ����Ĭ��true

	if (nTransFormResult == 0) return FALSE;

	//ת���ֽ�������
	if (pAnsiOutStrlenByte)	*pAnsiOutStrlenByte = nTransFormResult;

	return TRUE;
}


//�ַ�ת��
BOOL CRsaEngine::AnsiToUnicode(char *pAnsiStrIn, LPTSTR szUnicodeStrOut, INT nUnicodeCacheSize, INT *nUnicodeStrLenOutCalculateZeroTail)
{
	//����У��
	if (pAnsiStrIn == NULL || szUnicodeStrOut == NULL || nUnicodeCacheSize <= 0) return FALSE;

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

		return FALSE;
	}

	//ת��
	INT nTransFormResult = MultiByteToWideChar(CP_ACP, 0, pAnsiStrIn, -1, szUnicodeStrOut, nUnicodeCacheSize);
	if (nTransFormResult == 0) return FALSE;

	//ת���ֽ�������
	if (nUnicodeStrLenOutCalculateZeroTail)	*nUnicodeStrLenOutCalculateZeroTail = nTransFormResult;

	return TRUE;
}

//���ļ��ж�ȡpublic key ��private key
BOOL CRsaEngine::ReadPublicPrivateKey(LPTSTR szPublicKeyFile, LPTSTR szPrivateKeyFile)
{
	CIniFileIO      IniFileIO;		//��д�����ļ�	

	//��ȡpublic	key
	TCHAR szKey[RSA_KEY_STR_LEN] = { 0 };
	unsigned char ucKey[RSA_KEY_STR_LEN] = { 0 };

	BOOL bReadSuccess = FALSE;

	if (szPublicKeyFile)
	{
		IniFileIO.SetFilePath(szPublicKeyFile);
		IniFileIO.ReadString(PUBLIC_KEY_APPNAME, RSA_KEY_NAME, TEXT(""), szKey, RSA_KEY_STR_LEN);
		if (lstrlen(szKey) == 0)
		{
			//����ʧ��
			return FALSE;
		}
		//m_szPublicKey[RSA_KEY_STR_LEN - 1] = 0;

		//TCHAR ת��Ϊ char*
		if (!UnicodeToAnsi(szKey, (char *)ucKey, sizeof(char)*RSA_KEY_STR_LEN)) return FALSE;

		m_stPublicKey.ulKeyLen = RSA_KEY_STR_LEN;

		//������
		if (!HexDecodeData(ucKey, strlen((char *)ucKey), (m_stPublicKey.ucKey), &m_stPublicKey.ulKeyLen)) return FALSE;
		bReadSuccess = TRUE;
	}

	if (szPrivateKeyFile)
	{
		//˽Կ
		memset(szKey, 0, sizeof(TCHAR)*RSA_KEY_STR_LEN);
		memset(ucKey, 0, sizeof(char)*RSA_KEY_STR_LEN);
		IniFileIO.SetFilePath(szPrivateKeyFile);
		IniFileIO.ReadString(PRIVATE_KEY_APPNAME, RSA_KEY_NAME, TEXT(""), szKey, RSA_KEY_STR_LEN);
		if (lstrlen(szKey) == 0)
		{
			//����ʧ��
			return FALSE;
		}
		//m_szPublicKey[RSA_KEY_STR_LEN - 1] = 0;

		//TCHAR ת��Ϊ char*
		if (!UnicodeToAnsi(szKey, (char *)ucKey, sizeof(char)*RSA_KEY_STR_LEN)) return FALSE;

		//�ռ��С
		m_stPrivateKey.ulKeyLen = RSA_KEY_STR_LEN;

		//������
		if (!HexDecodeData(ucKey, strlen((char *)ucKey), (m_stPrivateKey.ucKey), &m_stPrivateKey.ulKeyLen)) return FALSE;

		bReadSuccess = TRUE;
	}

	//���ڳ���
	if (strlen(YCRsaPublicKey) != 0)
	{
		memset(ucKey, 0, sizeof(char)*RSA_KEY_STR_LEN);
		memmove(ucKey, YCRsaPublicKey, strlen(YCRsaPublicKey));

		m_stPublicKey.ulKeyLen = RSA_KEY_STR_LEN;

		//������
		if (!HexDecodeData(ucKey, strlen((char *)ucKey), (m_stPublicKey.ucKey), &m_stPublicKey.ulKeyLen)) return FALSE;

		bReadSuccess = TRUE;
	}

	//���ڳ���
	if (strlen(YCRsaPrivateKey) != 0)
	{
		memset(ucKey, 0, sizeof(char)*RSA_KEY_STR_LEN);
		memmove(ucKey, YCRsaPrivateKey, strlen(YCRsaPrivateKey));

		m_stPrivateKey.ulKeyLen = RSA_KEY_STR_LEN;

		//������
		if (!HexDecodeData(ucKey, strlen((char *)ucKey), (m_stPrivateKey.ucKey), &m_stPrivateKey.ulKeyLen)) return FALSE;

		bReadSuccess = TRUE;
	}

	//��Կ����
	if (strlen((char *)m_stPublicKey.ucKey)!=0||strlen((char *)m_stPrivateKey.ucKey)!=0) bReadSuccess = TRUE;

	return bReadSuccess;
}

//д�������ļ�
BOOL CRsaEngine::WritePublicPrivateKey(LPTSTR szPublicKeyFile, LPTSTR szPrivateKeyFile)
{
	BOOL bWriteSuccess = FALSE;
	CIniFileIO      IniFileIO;		//��д�����ļ�	

	//����
	unsigned char szKey[RSA_KEY_STR_LEN] = { 0 };
	TCHAR szUnicodeKey[RSA_KEY_STR_LEN] = { 0 };
	unsigned long ulKeyLen = 0;

	if (szPublicKeyFile)
	{
		memset(szKey, 0, sizeof(char)*RSA_KEY_STR_LEN);
		memset(szUnicodeKey, 0, sizeof(TCHAR)*RSA_KEY_STR_LEN);
		ulKeyLen = RSA_KEY_STR_LEN;

		//����
		if (!HexEncodeData(m_stPublicKey.ucKey, m_stPublicKey.ulKeyLen, szKey, &ulKeyLen)) return FALSE;

		//�ַ���ת��
		if (!AnsiToUnicode((char *)szKey, szUnicodeKey, sizeof(TCHAR)*RSA_KEY_STR_LEN)) return FALSE;

		//д�������ļ�
		if (!IniFileIO.WriteString(PUBLIC_KEY_APPNAME, RSA_KEY_NAME, szUnicodeKey, szPublicKeyFile)) return FALSE;

		bWriteSuccess = TRUE;
	}

	ulKeyLen = 0;

	if (szPrivateKeyFile)
	{
		memset(szKey, 0, sizeof(char)*RSA_KEY_STR_LEN);
		memset(szUnicodeKey, 0, sizeof(TCHAR)*RSA_KEY_STR_LEN);
		ulKeyLen = RSA_KEY_STR_LEN;

		//����
		if (!HexEncodeData(m_stPrivateKey.ucKey, m_stPrivateKey.ulKeyLen, szKey, &ulKeyLen)) return FALSE;

		//�ַ���ת��
		if (!AnsiToUnicode((char *)szKey, szUnicodeKey, sizeof(TCHAR)*RSA_KEY_STR_LEN)) return FALSE;

		//д�������ļ�
		if (!IniFileIO.WriteString(PRIVATE_KEY_APPNAME, RSA_KEY_NAME, szUnicodeKey, szPrivateKeyFile)) return FALSE;

		bWriteSuccess = TRUE;
	}

	return bWriteSuccess;
}

///////////////////////////////////////////////////////////////////////////////

//������ܽ���

//����ѭ���������� ÿ�����ݹ̶�64�ֽ�����
BOOL CBlockRsa::DataBlockCircleEncrypte(unsigned char *plaintext, unsigned long ulPlainTextLen, unsigned char **pCipherTextOut, unsigned long *pCipherSizeInLenOut)
{
	if (!pCipherTextOut || !plaintext || !pCipherSizeInLenOut || (*pCipherSizeInLenOut <= 0)) return FALSE;

	//���ĳ��ȷ���
	int iCircleCount = (ulPlainTextLen%PLAIN_TEXT_MAX_LEN == 0) ? (ulPlainTextLen / PLAIN_TEXT_MAX_LEN) : (ulPlainTextLen / PLAIN_TEXT_MAX_LEN + 1);

	//�������Ŀռ�
	int iCipherMemoryNeeded = iCircleCount*CIPHER_TEXT_OUT_ENCODE_LEN;

	//����ָ��	���Ĳ���PLAIN_TEXT_MAX_LEN����������0
	unsigned char *pPlainTextTmp = new unsigned char[iCipherMemoryNeeded];
	if (!pPlainTextTmp) return FALSE;
	memset(pPlainTextTmp, 0, iCipherMemoryNeeded);
	memmove(pPlainTextTmp, plaintext, ulPlainTextLen);

	//��������ĳ��ȷ�������������ռ��Ƿ��㹻��
	if (*pCipherSizeInLenOut < iCipherMemoryNeeded)
	{
		*pCipherSizeInLenOut = iCipherMemoryNeeded;
		return FALSE;
	}

	//���ָ��
	unsigned char *pCipherOutTmp = *pCipherTextOut;
	unsigned long ulCipherOut = *pCipherSizeInLenOut;
	unsigned long	ulCipherLenOutTmp = 0;

	BOOL bFinal = FALSE;
	//�������ѭ����
	for (int i = 0; i < iCircleCount; i++)
	{
		if (i == iCircleCount - 1) bFinal = TRUE;

		if (!Encrypte(pPlainTextTmp, PLAIN_TEXT_MAX_LEN, &pCipherOutTmp, &ulCipherOut, bFinal))
		{
			delete[]pPlainTextTmp;
			*pCipherSizeInLenOut = ulCipherLenOutTmp;
			return FALSE;
		}
		pCipherOutTmp += ulCipherOut;
		pPlainTextTmp += PLAIN_TEXT_MAX_LEN;
		ulCipherLenOutTmp += ulCipherOut;
		ulCipherOut = *pCipherSizeInLenOut - ulCipherOut;
	}

	//������ģ����ĳ���
	*pCipherSizeInLenOut = ulCipherLenOutTmp;

	//����ռ�
	delete[]pPlainTextTmp;

	return TRUE;

}

//�����������
BOOL CBlockRsa::DataBlockCircleDecrypte(unsigned char *ciphertext, unsigned long cipherlen, unsigned char **plaintext, unsigned long *pPlainSizeInLenOut)
{
	if (!ciphertext || !plaintext || !pPlainSizeInLenOut || (*pPlainSizeInLenOut <= 0)) return FALSE;

	//��������ѭ������
	int iCircleCount = (cipherlen%CIPHER_TEXT_OUT_ENCODE_LEN == 0) ? (cipherlen / CIPHER_TEXT_OUT_ENCODE_LEN) : (cipherlen / CIPHER_TEXT_OUT_ENCODE_LEN + 1);

	//����ռ�
	int iDecrypteSpaceNeeded = iCircleCount*PLAIN_TEXT_MAX_LEN;

	//�жϿռ��ͷ��㹻
	if (*pPlainSizeInLenOut < iDecrypteSpaceNeeded)
	{
		*pPlainSizeInLenOut = iDecrypteSpaceNeeded;
	}

	//����ָ��
	unsigned char *pCipherData = ciphertext;

	//���ָ��
	unsigned char *pPlainTextOut = *plaintext;
	unsigned long	ulPlainLenOut = *pPlainSizeInLenOut;
	unsigned long	ulPlainLenOutTmp = 0;

	BOOL bFinal = FALSE;

	//����ѭ����
	for (int i = 0; i < iCircleCount; i++)
	{
		if (i == iCircleCount - 1) bFinal = TRUE;

		if (!Decrypte(pCipherData, CIPHER_TEXT_OUT_ENCODE_LEN, &pPlainTextOut, &ulPlainLenOut, bFinal))
		{
			*pPlainSizeInLenOut = ulPlainLenOutTmp;
			return FALSE;
		}

		pCipherData += CIPHER_TEXT_OUT_ENCODE_LEN;
		pPlainTextOut += ulPlainLenOut;

		ulPlainLenOutTmp += ulPlainLenOut;
		ulPlainLenOut = *pPlainSizeInLenOut - ulPlainLenOut;

	}

	//����������ݴ�С
	*pPlainSizeInLenOut = ulPlainLenOutTmp;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////

//des���ܽ���
#include<time.h>

CDESEngine::CDESEngine()
{
	memset(&m_stKey, 0, S_KEY_BLOB_T);
	m_stKey.hdr.bType = PLAINTEXTKEYBLOB;
	m_stKey.hdr.bVersion = CUR_BLOB_VERSION;
	m_stKey.hdr.reserved = 0;
	m_stKey.hdr.aiKeyAlg = CALG_DES;
	m_stKey.cbKeySize = DES_KEY_BIT;
	ZeroMemory(m_stKey.rgbKeyData, DES_KEY_BIT);

	m_hCryptProv = NULL;
	m_hKey = NULL;
	memset(m_szCspName, 0, sizeof(TCHAR)* RSA_CONTAINER_NAME_LEN);
	memmove(m_szCspName, CSP_DES_NAME, sizeof(TCHAR)* lstrlen(CSP_DES_NAME));
}


CDESEngine::~CDESEngine()
{
	Release();
}

//������ʼ��
BOOL	CDESEngine::CspInit()
{
	//�򿪻򴴽�CSP����
	if (!CryptAcquireContext(&m_hCryptProv, m_szCspName, MS_DEF_PROV, PROV_RSA_FULL, 0))//Ĭ������Ϊ0����ȡ�������½�
	{
		if (GetLastError() == NTE_BAD_KEYSET)//�������ܱ���
		{
			if (!CryptAcquireContextW(&m_hCryptProv, m_szCspName, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))//ʹ��ָ���������½�
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;

}

//������Կ
BOOL CDESEngine::GenerateKey(BYTE * pbKey, DWORD *pnKeyLen)
{
	//	if (nKeySize < DES_KEY_BIT+1) return FALSE;

	BYTE key[DES_KEY_BIT + 1] = { 0 };

	//ѡȡ3λ����

	if (!pbKey || strlen((char *)pbKey) == 0)
	{
		srand(time(NULL));
		int m = 0;
		for (int i = 0; i < DES_KEY_BIT; i++)
		{
			//ѡ������
			m = rand() % DES_KEY_TYPE_NUM;
			switch (m)
			{
			case DES_NUM_TYPE:
			{
								 //ѡȡ����
								 key[i] = DesKeyNumLib[rand() % DES_NUM_LEN];
								 break;
			}
			case DES_CHAR_TYPE:
			{
								  //ѡȡӢ��
								  key[i] = DesKeyCharLib[rand() % DES_CHAR_LEN];
								  break;
			}
			case DES_SYMBOL_TYPE:
			{
									//ѡȡ�ַ�
									key[i] = DesKeySymbolLib[rand() % DES_SYMBOL_LEN];
									//key[i] = DesKeyNumLib[rand() % DES_NUM_LEN];
									break;
			}
			}
		}
	}
	else
	{
		if (pnKeyLen)
			memmove(key, pbKey, *pnKeyLen);
	}

	int iKeyLenCount = 0;

	//�������
	if (pbKey&&strlen((char *)pbKey) == 0)
	{

		if (pnKeyLen)
		{
			iKeyLenCount = (*pnKeyLen > DES_KEY_BIT ? DES_KEY_BIT : *pnKeyLen);
			memmove(pbKey, key, iKeyLenCount);
		}
		else
		{
			iKeyLenCount = DES_KEY_BIT;
			memmove(pbKey, key, DES_KEY_BIT);
		}
	}
	else
	{
		iKeyLenCount = strlen((char *)key) > DES_KEY_BIT ? DES_KEY_BIT : strlen((char *)key);
	}

	//	memset(pbKey, 0, nKeySize);
	//memmove(pbKey, key, DES_KEY_BIT);
	memmove(m_stKey.rgbKeyData, key, iKeyLenCount);
	*pnKeyLen = strlen((char *)m_stKey.rgbKeyData);
	//printf("GenerateKey ����key:%s\r\n", (char *)m_stKey.rgbKeyData);

	return TRUE;
}

//������Կ
BOOL	CDESEngine::ImportKey()
{
	if (!m_hCryptProv) return FALSE;

	if (!CryptImportKey(m_hCryptProv, (BYTE*)&m_stKey, sizeof(m_stKey), 0, 0, &m_hKey))
	{
		CryptReleaseContext(m_hCryptProv, 0);
		m_hCryptProv = NULL;
		return FALSE;
	}

	return TRUE;
}

//�ͷ�����
BOOL	CDESEngine::Release()
{
	if (m_hKey) CryptDestroyKey(m_hKey);
	if (m_hCryptProv) CryptReleaseContext(m_hCryptProv, 0);

	m_hKey = NULL;
	m_hCryptProv = NULL;

	memset(&m_stKey, 0, S_KEY_BLOB_T);
	m_stKey.hdr.bType = PLAINTEXTKEYBLOB;
	m_stKey.hdr.bVersion = CUR_BLOB_VERSION;
	m_stKey.hdr.reserved = 0;
	m_stKey.hdr.aiKeyAlg = CALG_DES;
	m_stKey.cbKeySize = DES_KEY_BIT;
	ZeroMemory(m_stKey.rgbKeyData, DES_KEY_BIT);
	memset(m_szCspName, 0, sizeof(TCHAR)* RSA_CONTAINER_NAME_LEN);
	memmove(m_szCspName, CSP_DES_NAME, sizeof(TCHAR)* lstrlen(CSP_DES_NAME));

	return TRUE;
}

//����
BOOL	CDESEngine::EncrypteData(__inout const BYTE* pbKey,
	__inout DWORD *pcbKeyLen,
	__in const BYTE* pbPlainText,
	__in DWORD cbPlainDataLen,
	__out BYTE* pbCipherBuf,
	__inout DWORD* pcbCipherBufLen)
{
	//��ʼ��
	if (!CspInit()) return FALSE;

	//������Կ
	BYTE cbKey[DES_KEY_BIT] = { 0 };
	if (!GenerateKey((BYTE *)pbKey, pcbKeyLen)) return FALSE;

	//������Կ
	ImportKey();

	//����
	BOOL bRet;
	BYTE pbBlock[2 * DES_KEY_BIT];
	DWORD dwBlock, dwOut = 0;
	BOOL bEOF;
	for (DWORD i = 0; i < cbPlainDataLen; i += DES_KEY_BIT)
	{
		bEOF = cbPlainDataLen - i <= DES_KEY_BIT;    //�Ƿ�Ϊ���һ��
		dwBlock = bEOF ? cbPlainDataLen - i : DES_KEY_BIT;

		CopyMemory(pbBlock, pbPlainText + i, dwBlock);    //������ܡ����ܣ�ÿ�� DES_KEY_BIT �ֽ�

		bRet = CryptEncrypt(m_hKey, NULL, bEOF, 0, pbBlock, &dwBlock, sizeof(pbBlock));

		if (!bRet)
			break;

		if (pbCipherBuf)
		{
			if (dwOut + dwBlock > *pcbCipherBufLen)
			{
				bRet = FALSE;
				break;
			}
			else
			{
				CopyMemory(pbCipherBuf + dwOut, pbBlock, dwBlock);
				dwOut += dwBlock;
			}
		}
		else    //��pbBufΪNULLʱ����������Ҫ����pbBuf
			dwOut += dwBlock;
	}

	*pcbCipherBufLen = dwOut;

	//�ͷ�
	Release();

	return TRUE;
}

//����
BOOL	CDESEngine::DecrypteData(__in const BYTE* pbKey,
	__in DWORD cbKeyLen,
	__in const BYTE* pbCipherData,
	__in DWORD cbCipherDataLen,
	__out BYTE* pbPlainTextBuf,
	__inout DWORD* pcbPlainTextBufLen)
{
	//��ʼ��
	if (!CspInit()) return FALSE;

	//������Կ
	BYTE cbKey[DES_KEY_BIT] = { 0 };
	if (!GenerateKey((BYTE *)pbKey, &cbKeyLen)) return FALSE;

	//������Կ
	if (!ImportKey()) return FALSE;

	//����
	BOOL bRet;
	BYTE pbBlock[2 * DES_KEY_BIT];
	DWORD dwBlock, dwOut = 0;
	BOOL bEOF;
	for (DWORD i = 0; i < cbCipherDataLen; i += DES_KEY_BIT)
	{
		bEOF = cbCipherDataLen - i <= DES_KEY_BIT;    //�Ƿ�Ϊ���һ��
		dwBlock = bEOF ? cbCipherDataLen - i : DES_KEY_BIT;

		CopyMemory(pbBlock, pbCipherData + i, dwBlock);    //������ܡ����ܣ�ÿ��8�ֽ�

		bRet = CryptDecrypt(m_hKey, NULL, bEOF, 0, pbBlock, &dwBlock);

		if (!bRet)
			break;

		if (pbPlainTextBuf)
		{
			if (dwOut + dwBlock > *pcbPlainTextBufLen)
			{
				bRet = FALSE;
				break;
			}
			else
			{
				CopyMemory(pbPlainTextBuf + dwOut, pbBlock, dwBlock);
				dwOut += dwBlock;
			}
		}
		else    //��pbBufΪNULLʱ����������Ҫ����pbBuf
			dwOut += dwBlock;
	}

	*pcbPlainTextBufLen = dwOut;

	//�ͷ�
	Release();

	return TRUE;
}

//��������Ҫ�Ŀռ�
int  CDESEngine::EncrypteMemoryNeeded(__inout const BYTE* pbKey,
	__inout DWORD *pcbSizeInKeyLenOut,
	__in const BYTE* pbPlainText,
	__in DWORD cbPlainDataLen)
{
	DWORD dwNeeded = 0;

	if (!EncrypteData(pbKey, pcbSizeInKeyLenOut, pbPlainText, cbPlainDataLen, NULL, &dwNeeded)) return -1;

	return dwNeeded * 2;/*��������*/
}

//��������Ҫ�Ŀռ�
int		CDESEngine::DecrypteMemoryNeeded(__in const BYTE* pbKey,
	__in DWORD cbKeyLen,
	__in const BYTE* pbCipherData,
	__in DWORD cbCipherDataLen)
{
	DWORD dwNeeded = 0;

	if (!DecrypteData(pbKey, cbKeyLen, pbCipherData, cbCipherDataLen, NULL, &dwNeeded)) return -1;

	return dwNeeded;/*��������*/
}


//���루16���ƣ�
BOOL CDESEngine::HexEncodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut)
{
	//����У��
	if (!pucDataSrc || !pucDataOut || !pulDataSizeInLenOut) return FALSE;
	if (*pulDataSizeInLenOut < 2 * ulDataSrcDataLen)
	{
		*pulDataSizeInLenOut = 2 * ulDataSrcDataLen;
		return FALSE;
	}

	//������
	unsigned long n = 0;
	unsigned char ucCache[3] = { 0 };
	unsigned char *ucDataCache = new unsigned char[2 * ulDataSrcDataLen];
	if (!ucDataCache) return FALSE;
	memset((void *)ucDataCache, 0, 2 * ulDataSrcDataLen);

	unsigned char *pDataCacheTmp = ucDataCache;
	unsigned char *pDataSrcTmp = pucDataSrc;

	//����
	for (n = 0; n < ulDataSrcDataLen; n++)
	{
		memset(ucCache, 0, 3);
		sprintf((char *)ucCache, "%02X", *pDataSrcTmp);
		memmove(pDataCacheTmp, ucCache, 2);
		pDataSrcTmp++;
		pDataCacheTmp += 2;
	}

	//����Ǩ�Ʋ��ͷſռ�
	memset(pucDataOut, 0, *pulDataSizeInLenOut);
	memmove(pucDataOut, ucDataCache, 2 * ulDataSrcDataLen);
	char miwend[8192] = { 0 };
	memmove(miwend, ucDataCache, 2 * ulDataSrcDataLen);
	*pulDataSizeInLenOut = 2 * ulDataSrcDataLen;
	delete[]ucDataCache;

	return TRUE;
}

//�����루16���ƣ�
BOOL CDESEngine::HexDecodeData(unsigned char *pucDataSrc, unsigned long ulDataSrcDataLen, unsigned char *pucDataOut, unsigned long *pulDataSizeInLenOut)
{
	//����У��
	if (!pucDataSrc || !pucDataOut || !pulDataSizeInLenOut) return FALSE;

	unsigned char strtmp[3] = { 0 };
	int strcount = ((ulDataSrcDataLen) % 2 == 0 ? (ulDataSrcDataLen) / 2 : ((ulDataSrcDataLen) / 2 + 1));
	char *decodetmp = new char[strcount];
	if (!decodetmp) return FALSE;
	memset(decodetmp, 0, strcount);

	if (*pulDataSizeInLenOut <  strcount)
	{
		*pulDataSizeInLenOut = strcount;
		delete[]decodetmp;
		return FALSE;
	}

	//ת��
	for (int i = 0; i<strcount; i++)
	{
		memset(strtmp, 0, 3);
		memmove(strtmp, pucDataSrc + 2 * i, 2);
		if (!HexToInt(&strtmp[0]))
			return false;
		if (!HexToInt(&strtmp[1]))
			return false;
		decodetmp[i] = COMBINE_BYTE(strtmp[0], strtmp[1]);
	}

	//ת������
	memset(pucDataOut, 0, *pulDataSizeInLenOut);
	memmove(pucDataOut, decodetmp, strcount);
	*pulDataSizeInLenOut = strcount;

	delete[]decodetmp;

	return TRUE;
}

//16����תʮ����
BOOL CDESEngine::HexToInt(unsigned char *HexData)
{
	//����У��
	if (!HexData) return FALSE;

	if (*HexData >= '0'&&*HexData <= '9')
	{
		*HexData = *HexData - '0' + 0;
	}
	else if (*HexData >= 'A'&&*HexData <= 'F')
	{
		*HexData = *HexData - 'A' + 10;
	}
	else if (*HexData >= 'a'&&*HexData <= 'f')
	{
		*HexData = *HexData - 'a' + 10;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


//���ܲ�����
BOOL CDESEngine::Encrypte(__inout const BYTE* pbKey,
	__inout DWORD *pcbKeyLen,
	__in const BYTE* pbPlainText,
	__in DWORD cbPlainDataLen,
	__out BYTE* pbCipherBuf,
	__inout DWORD* pcbCipherBufLen)
{
	DWORD dwCipherSize = *pcbCipherBufLen;

	//����
	if (!EncrypteData(pbKey, pcbKeyLen, pbPlainText, cbPlainDataLen, pbCipherBuf, pcbCipherBufLen)) return FALSE;

	//�������
	if (!HexEncodeData(pbCipherBuf, *pcbCipherBufLen, pbCipherBuf, &dwCipherSize)) return FALSE;

	*pcbCipherBufLen = dwCipherSize;

	return TRUE;
}

//���벢����
BOOL CDESEngine::Decrypte(__in const BYTE* pbKey,
	__in DWORD cbKeyLen,
	__in const BYTE* pbCipherData,
	__in DWORD cbCipherDataLen,
	__out BYTE* pbPlainTextBuf,
	__inout DWORD* pcbPlainTextBufLen)
{
	//����
	if (!HexDecodeData((unsigned char *)pbCipherData, cbCipherDataLen, (unsigned char *)pbCipherData, &cbCipherDataLen)) return FALSE;

	//����
	if (!DecrypteData(pbKey, cbKeyLen, pbCipherData, cbCipherDataLen, pbPlainTextBuf, pcbPlainTextBufLen)) return FALSE;

	return TRUE;
}
