#ifndef BASE_TASK_H_
#define BASE_TASK_H_
#pragma once

#include "ThreadHead.h"
//任务基类
class THREAD_CLASS CTaskBase
{
	//函数定义
public:
	//构造函数
	CTaskBase(){}
	//析构函数
	virtual ~CTaskBase(){}
	//任务执行体
	virtual int Run()=0;
	//释放
	virtual void Release() = 0;
};


#endif