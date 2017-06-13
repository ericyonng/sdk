#ifndef SDK_TOOLS_TASK_H_
#define SDK_TOOLS_TASK_H_

#pragma once

class CCopyFilesTask :public CTaskBase
{
public:
	CCopyFilesTask(void *pArg){ m_pArg = pArg; }
	virtual ~CCopyFilesTask(){}
	//任务执行体
	virtual int Run();
	//释放
	virtual void Release(){ delete this; }
protected:
	void *		m_pArg;
};

#endif