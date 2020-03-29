#pragma once
#include "stdAfx.h"
class WorkerThread : public nbase::FrameworkThread
{
public:
	WorkerThread(enum ThreadId thread_id, const char* name)
		: nbase::FrameworkThread(name)
		,thread_id_(thread_id){}
	virtual ~WorkerThread() {};

private:
	virtual void Init() override;
	virtual void Cleanup() override;

private:
	enum ThreadId thread_id_;
};

