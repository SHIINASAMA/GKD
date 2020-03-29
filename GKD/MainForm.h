#pragma once
#include "stdAfx.h"
#include "fstream"

class MainForm : public ui::WindowImplBase
{
public:
	MainForm() {}
	~MainForm() {}

public:
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

public:
	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;

public:
	ui::RichEdit* m_ImagePathEdit;
	ui::RichEdit* m_FilePathEdit;
	ui::Button* m_ImageBtn;
	ui::Button* m_FileBtn;
	ui::Button* m_MakeFileBtn;
	ui::Progress* m_Progress;

private:
	bool MessageLoop(ui::EventArgs* msg);
	void ImageBtnClicked();
	void FileBtnClicked();
	void MakeFileBtnClicked();
	void ChangeProgress(float Value);
	void LockControl();
	void UnlockControl();
	long GetFileSize(std::wstring FileName);

private:
	nbase::WeakCallbackFlag MyCallback;
	std::wstring m_OutputFileName;

	StdClosure Task = [this]() {
		LockControl();
		std::ofstream out(m_OutputFileName, std::ios::binary);
		if (!out.is_open()) {
			MessageBox(NULL, L"打开写入流失败", L"错误", NULL);
			return;
		}
		std::ifstream in(m_ImagePathEdit->GetText(), std::ios::binary);
		if (!in.is_open()) {
			MessageBox(NULL, L"打开读取流失败", L"错误", NULL);
			return;
		}

		char buf[1024];
		int len = 0;
		while (in) {
			in.read(buf, 1024);
			len = in.gcount();
			out.write(buf, len);
		}

		in.close();
		in.clear();
		in.open(m_FilePathEdit->GetText(), std::ios::binary);
		if (!in.is_open()) {
			MessageBox(NULL, L"打开读取流失败", L"错误", NULL);
			return;
		}
		while (in) {
			in.read(buf, 1024);
			len = in.gcount();
			out.write(buf, len);
		}

		in.close();
		out.close();
		MessageBox(NULL, L"生成完成", L"信息", NULL);
		UnlockControl();
	};
};