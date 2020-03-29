#pragma once
#include "stdAfx.h"
#include "WorkerThread.h"

class MainThread : public nbase::FrameworkThread
{
public:
	MainThread() : nbase::FrameworkThread("MainThread") {}
	virtual ~MainThread(){}

private:
	virtual void Init() override;
	virtual void Cleanup() override;

private:
	std::unique_ptr<WorkerThread> Worker;
};

