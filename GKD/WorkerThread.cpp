#include "WorkerThread.h"

void WorkerThread::Init() {
	nbase::ThreadManager::RegisterThread(thread_id_);
}

void WorkerThread::Cleanup() {
	nbase::ThreadManager::UnregisterThread();
}