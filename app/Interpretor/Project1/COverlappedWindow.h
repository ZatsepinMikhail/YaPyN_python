#pragma once
#include <Windows.h>
#include "CPythonInterpretor.h"

//для поля ввода
#define ID_EDITCHILD 100

//для поля вывода
#define ID_SHOWCHILD 101

class COverlappedWindow : public CPythonInterpretorCallback {
public:
	COverlappedWindow();
	~COverlappedWindow();

	static bool RegisterClass(HINSTANCE instance);

	bool Create(HINSTANCE instance);

	void Show(int cmdShow);

	void ShowText(wchar_t* text);

	HACCEL haccel;
	HWND handle;

	void OnPythonInterpretResult(wchar_t* result);
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
};
