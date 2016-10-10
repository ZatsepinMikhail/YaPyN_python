#include "OverlappedWindow.h"

#include <codecvt>
#include <locale>
#include <vector>
#include <iostream>
#include <Commctrl.h>
#include <Windows.h>

#include "resource.h"
#include "SimplePythonCallback.h"


COverlappedWindow::COverlappedWindow()
{

}

COverlappedWindow::~COverlappedWindow()
{

}

LRESULT __stdcall COverlappedWindow::WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ERASEBKGND) {
		return true;
	}

	if (message == WM_NCCREATE) {
		COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		::SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
	}
	else {
		COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(::GetWindowLongPtr(handle, GWLP_USERDATA));
		switch (message) {
		case WM_CREATE:
			window->OnCreate(handle);
			break;
		case WM_SIZE:
			window->OnSize(lParam);
			break;
		case WM_SETFOCUS:
			window->OnSetFocus();
			break;
		case WM_COMMAND:
			window->OnCommand(wParam, lParam);
			break;
		case WM_DESTROY:
			window->OnDestroy();
			break;
		default:
			break;
		}
	}

	return DefWindowProc(handle, message, wParam, lParam);

}

bool COverlappedWindow::RegisterClass(HINSTANCE instance)
{
	WNDCLASSEX windowClass;

	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = COverlappedWindow::WindowProc;
	windowClass.hIcon = (HICON) ::LoadImage(instance, L"pain.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	windowClass.hInstance = instance;
	windowClass.lpszClassName = L"COverlappedWindow";
	windowClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	windowClass.lpszMenuName = L"MainMenu";

	return ::RegisterClassEx(&windowClass) != 0;
}

bool COverlappedWindow::Create(HINSTANCE instance)
{
	hInst = instance;

	wchar_t windowName[32];
	if (::LoadString(instance, IDS_WINDOW_NAME, windowName, 64)) {
		//pain
	}

	COverlappedWindow::handle = ::CreateWindowEx(0,
		L"COverlappedWindow",
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		instance,
		this);

	return COverlappedWindow::handle != 0;
}

void COverlappedWindow::OnCreate(HWND handle) {
	//creating edit
	RECT rect;
	::GetClientRect(handle, &rect);
	
	hwndEdit = CreateWindowEx(
		0, L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		0, 0, 0, 0,   // set size in WM_SIZE message 
		handle,         // parent window 
		(HMENU)ID_EDITCHILD,   // edit control ID 
		(HINSTANCE)GetWindowLong(handle, GWL_HINSTANCE),
		NULL);        // pointer not needed 

	hwndShow = CreateWindowEx(
		0, L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		0, 0, 0, 0,   // set size in WM_SIZE message 
		handle,         // parent window 
		(HMENU)ID_SHOWCHILD,   // edit control ID 
		(HINSTANCE)GetWindowLong(handle, GWL_HINSTANCE),
		NULL);        // pointer not needed 

	HRSRC res = ::FindResource(hInst, MAKEINTRESOURCE(IDR_DEFAULTEDITTEXT), L"MYRES");
	HGLOBAL loadedRs = ::LoadResource(hInst, res);
	LPVOID pointerToRes = ::LockResource(loadedRs);
	DWORD size = ::SizeofResource(hInst, res);
	wchar_t* text = (wchar_t*)pointerToRes;

	if (!::SetWindowText(hwndEdit, text)) {
		//pain
	}

	if (!::SetWindowText(hwndShow, L"Result will be here")) {
		//pain
	}
	
	haccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR1));
}

void COverlappedWindow::OnSize(LPARAM lParam) {
	// Make the edit control the size of the window's client area. 
	MoveWindow(hwndEdit,
		0, 0,                  // starting x- and y-coordinates 
		LOWORD(lParam),        // width of client area 
		HIWORD(lParam) / 2,        // height of client area 
		TRUE);                 // repaint window

	// Make the edit control the size of the window's client area. 
	MoveWindow(hwndShow,
		0, HIWORD(lParam) / 2, // starting x- and y-coordinates 
		LOWORD(lParam),        // width of client area 
		HIWORD(lParam),        // height of client area 
		TRUE);                 // repaint window 
}

void COverlappedWindow::OnSetFocus() {
	SetFocus(hwndEdit);
}

void COverlappedWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case ID_ACCELERATOR_FAST_QUIT:
		::PostMessage(handle, WM_QUIT, (WPARAM)0, (LPARAM)0);
		return;
	case ID_COMMANDS_RESET:
		ShowText(L"Reset pressed");
		pythonInterpretor.Reset();
		return;
	case ID_COMMANDS_RUN: {
		std::wstring command = GetTextFromInput();
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string stringCommand = converter.to_bytes(command);

		std::shared_ptr<IReturnResultCallback> callback = 
			std::make_shared<SimplePythonCallback>(SimplePythonCallback(hwndShow));

		pythonInterpretor.Run(stringCommand, callback);
		return;
	}
	default:
		//just relax;
		return;
	}
}

void COverlappedWindow::OnDestroy()
{
	::DestroyAcceleratorTable(haccel);
	::PostQuitMessage(0);
}

void COverlappedWindow::Show(int cmdShow)
{
	::ShowWindow(handle, cmdShow);
}

void COverlappedWindow::ShowText(const std::wstring& text) const {
	if (!::SetWindowText(hwndShow, text.c_str())) {
		//pain
	}
}

std::wstring COverlappedWindow::GetTextFromInput() const {
	int textLength = ::GetWindowTextLength(hwndEdit) + 1;
	std::vector<wchar_t> buffer(textLength);
	::GetWindowText(hwndEdit, &buffer[0], textLength);
	
	std::wstring text = &buffer[0];

	return text;
}
