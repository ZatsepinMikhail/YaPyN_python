#pragma once

#include <strsafe.h>

#include "PythonInterpretor.h"
#include "Windows.h"

void CPythonInterpretor::Run(std::shared_ptr<wchar_t> text,
	std::shared_ptr<CPythonInterpretorCallback> callback) {

	//smart code must be here
	callback.get()->OnPythonInterpretResult(text);
}

/**
* Format a readable error message, display a message box,
* and exit from the application.
**/
void ErrorExit(PTSTR lpszFunction) {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL
		);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));

	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}

void CPythonInterpretor::Run() {

	SECURITY_ATTRIBUTES secAttr;
	secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	secAttr.lpSecurityDescriptor = NULL;
	secAttr.bInheritHandle = TRUE;

	HANDLE g_hInputFile = NULL;
	HANDLE g_hOutputFile = NULL;

	g_hInputFile = CreateFile(
		L"C:\\Users\\Mikhail\\Documents\\Visual Studio 2015\\Projects\\YaPyN_python\\YaPyN_python\\input.txt",
		GENERIC_READ,
		0,
		&secAttr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	g_hOutputFile = CreateFile(
		L"C:\\Users\\Mikhail\\Documents\\Visual Studio 2015\\Projects\\YaPyN_python\\YaPyN_python\\output.txt",
		GENERIC_WRITE,
		0,
		&secAttr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if (!g_hInputFile) {
		ErrorExit(TEXT("InputFile"));
	}

	if (!g_hOutputFile) {
		ErrorExit(TEXT("OutputFile"));
	}

	STARTUPINFO siStartInfo;

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdOutput = g_hOutputFile;
	siStartInfo.hStdInput = g_hInputFile;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	PROCESS_INFORMATION piProcInfo;
	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	if (!CreateProcess(
		L"C:\\Users\\Mikhail\\AppData\\Local\\Programs\\Python\\Python35-32\\python.exe",
		0, 0, 0,
		TRUE,
		0, 0, 0,
		&siStartInfo,
		&piProcInfo
		)) {
		ErrorExit(TEXT("CreateProcess"));
	}
}

void CPythonInterpretor::Reset() {
}

CPythonInterpretor::CPythonInterpretor() {
	callback = nullptr;
}