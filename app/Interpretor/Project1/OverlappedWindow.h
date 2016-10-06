#pragma once

#include <Windows.h>

#include "PythonInterpreter.h"

//ID of input element
#define ID_EDITCHILD 100

//ID of output element
#define ID_SHOWCHILD 101

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	static bool RegisterClass(HINSTANCE instance);

	bool Create(HINSTANCE instance);

	void Show(int cmdShow);


	HACCEL haccel;
	HWND handle;

protected:
	void OnCreate(HWND handle);
	void OnSize(LPARAM lParam);
	void OnSetFocus();
	void OnDestroy();
	void OnCommand(WPARAM wParam, LPARAM lParam);
	void OnSettings();
	LRESULT OnCtlColorEdit(WPARAM wParam);

private:
	CPythonInterpretor pythonInterpretor;
	HWND hwndEdit; 
	HWND hwndShow;
	HINSTANCE hInst;
	static LRESULT __stdcall WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	HANDLE iconHandle;

	std::wstring GetTextFromInput() const;
	void ShowText(const std::wstring& text) const;
};
