#ifndef BASE_TASK_H_
#define BASE_TASK_H_
#pragma once

#include "ThreadHead.h"
//�������
class THREAD_CLASS CTaskBase
{
	//��������
public:
	//���캯��
	CTaskBase(){}
	//��������
	virtual ~CTaskBase(){}
	//����ִ����
	virtual int Run()=0;
	//�ͷ�
	virtual void Release() = 0;
};


#endif