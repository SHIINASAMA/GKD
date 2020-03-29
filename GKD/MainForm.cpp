#include "MainForm.h"

const std::wstring MainForm::kClassName = L"GKD工具";

std::wstring MainForm::GetSkinFolder() {
	return L"GKD";
}

std::wstring MainForm::GetSkinFile() {
	return L"1.xml";
}

std::wstring MainForm::GetWindowClassName() const{
	return kClassName;
}

void MainForm::InitWindow() {
	//绑定控件
	m_ImagePathEdit = dynamic_cast<ui::RichEdit*>(FindControl(L"ImagePath"));
	m_FilePathEdit = dynamic_cast<ui::RichEdit*>(FindControl(L"FilePath"));
	m_ImageBtn = dynamic_cast<ui::Button*>(FindControl(L"FindIamgeButton"));
	m_FileBtn = dynamic_cast<ui::Button*>(FindControl(L"FindFileButton"));
	m_MakeFileBtn = dynamic_cast<ui::Button*>(FindControl(L"MakeFileButton"));
	m_Progress = dynamic_cast<ui::Progress*>(FindControl(L"Progress"));
	//绑定事件
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&MainForm::MessageLoop, this, std::placeholders::_1));
}

bool MainForm::MessageLoop(ui::EventArgs* msg) {
	std::wstring name = msg->pSender->GetName();
	
	if (name == L"FindIamgeButton")ImageBtnClicked();
	if (name == L"FindFileButton")FileBtnClicked();
	if (name == L"MakeFileButton")MakeFileBtnClicked();

	return true;
}

void MainForm::ImageBtnClicked() {
	char szFileName[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.lpstrTitle = (LPWSTR)L"选择图片文件";
	if (GetOpenFileName(&ofn)) {
		m_ImagePathEdit->SetText(ofn.lpstrFile);
	}
}

void MainForm::FileBtnClicked() {
	char szFileName[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.lpstrTitle = (LPWSTR)L"选择压缩包文件";
	if (GetOpenFileName(&ofn)) {
		m_FilePathEdit->SetText(ofn.lpstrFile);
	}
}

void MainForm::MakeFileBtnClicked() {
	if (m_ImagePathEdit->GetText() == L"" || m_FilePathEdit->GetText() == L"") {
		MessageBox(NULL, L"路径不能为空", L"提示", NULL);
		return;
	}
	char szFileName[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = (LPWSTR)szFileName;
	ofn.lpstrTitle = (LPWSTR)L"保存为";
	if (GetSaveFileName(&ofn)) {
		m_OutputFileName = ofn.lpstrFile;
		nbase::ThreadManager::PostTask(kWorkerThread, ToWeakCallback(Task));
	}
}

void MainForm::ChangeProgress(float Value) {
	m_Progress->SetValue(Value);

}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void MainForm::LockControl() {
	m_ImageBtn->SetEnabled(false);
	m_FileBtn->SetEnabled(false);
	m_MakeFileBtn->SetEnabled(false);
	m_MakeFileBtn->SetText(L"生成中");
}

void MainForm::UnlockControl() {
	m_ImageBtn->SetEnabled(true);
	m_FileBtn->SetEnabled(true);
	m_MakeFileBtn->SetEnabled(true);
	m_MakeFileBtn->SetText(L"生成");
}

long MainForm::GetFileSize(std::wstring FileName) {
	long temp_;
	HANDLE handle = CreateFile(FileName.c_str(), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		temp_ = ::GetFileSize(handle, NULL);
		CloseHandle(handle);
	}
	return temp_;
}