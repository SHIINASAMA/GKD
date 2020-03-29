#include "stdAfx.h"
#include "MainThread.h"
#include "MainForm.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MainThread thread;
    thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

    return 0;
}

void MainThread::Init() {
    nbase::ThreadManager::RegisterThread(kThreadUI);

    //启动工作者线程
    Worker.reset(new WorkerThread(kWorkerThread, "Worker"));
    Worker->Start();


    std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
#ifdef DEUBG
    ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);
#else
    ui::GlobalManager::OpenResZip(MAKEINTRESOURCE(IDR_ZIP1), L"ZIP", "");
    //ui::GlobalManager::OpenResZip(L"resources.zip", "");
    ui::GlobalManager::Startup(L"resources\\", ui::CreateControlCallback(), false);
#endif

    

    MainForm* Form = new MainForm;
    Form->Create(NULL, MainForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
    Form->CenterWindow();
    Form->ShowWindow();
}

void MainThread::Cleanup() {
    ui::GlobalManager::Shutdown();

    Worker->Stop();
    Worker.reset(nullptr);

    SetThreadWasQuitProperly(true);
    nbase::ThreadManager::UnregisterThread();
}