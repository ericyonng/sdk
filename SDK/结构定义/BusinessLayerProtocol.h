#ifndef BUSINESS_LAYER_PROTOCOL_H_
#define BUSINESS_LAYER_PROTOCOL_H_

#pragma once

//1��2����ȷ�Ϻ�������ռ��
//���ܺ�
enum{ RIGHT_RECV = 0, REG_ID = 0x03, MODIFY_PWD_ID = 0x04, LOGIN_ID = 0x05 };

//��������
#define ACCOUNT_LEN		15
#define PWD_LEN			32
#define EMAIL_LEN		47
#define RET_LEN			64



//ע���
typedef struct stRegisterBodyPack
{
	unsigned int		uFunID;
	char account[ACCOUNT_LEN];
	char pwd[PWD_LEN];
	char email[EMAIL_LEN];
}REGISTER_BODY_PACK_T;
#define S_REGISTER_BODY_PACK_T	sizeof(REGISTER_BODY_PACK_T)

//�޸�����
typedef struct stModifyPwdBodyPack
{
	unsigned int		uFunID;
	char account[ACCOUNT_LEN];
	char oldpwd[PWD_LEN];
	char newpwd[PWD_LEN];
}MODIFY_PWD_BODY_PACK_T;
#define S_MODIFY_PWD_BODY_PACK_T	sizeof(MODIFY_PWD_BODY_PACK_T)

//Ӧ���
typedef struct stRegReturnPack
{
	unsigned int		uFunID;
	char			ret[RET_LEN];
}REG_RETURN_PACK_T;
#define S_REG_RETURN_PACK_T	sizeof(REG_RETURN_PACK_T)

//�޸ķ��ش���
enum
{
	SUCCESS_MOD,
	OLD_PWD_WRONG,
	USER_ID_WRONG,
	UPDATE_ERROR
};


#endif