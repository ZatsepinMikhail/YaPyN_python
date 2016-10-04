#pragma once

#include <Python.h>

#include "PythonInterpretor.h"

#include <codecvt>
#include <locale>
#include <strsafe.h>
#include <Windows.h>

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

CPythonInterpretor::CPythonInterpretor() {
	Py_Initialize();
}

CPythonInterpretor::~CPythonInterpretor() {
	Py_Finalize();
}

void CPythonInterpretor::Run(
	const std::wstring& text,
	std::wstring& result) const {
	std::wstring tempFilename;
	CreateTempFilename(tempFilename);
	SetPythonStdoutToTempfile(tempFilename);

	std::string text_string;
	WstringToString(text, text_string);
	PyRun_SimpleString(text_string.c_str());

	FlushPythonOutput();

	HANDLE hFile = CreateFile(
		tempFilename.c_str(), 
		GENERIC_READ,         
		0,                     
		NULL,                  
		OPEN_EXISTING,        
		FILE_ATTRIBUTE_NORMAL, 
		NULL);            

	const int BUFFER_SIZE = 1000;
	wchar_t buffer[BUFFER_SIZE + 1];
	DWORD read_size;
	ReadFile(hFile, buffer, BUFFER_SIZE, &read_size, NULL);
	buffer[read_size] = 0;
	result = std::wstring(buffer);
}

void CPythonInterpretor::Reset() const {
	Py_Finalize();
	Py_Initialize();
}

void CPythonInterpretor::CreateTempFilename(std::wstring& result) const {
	WCHAR buffer[MAX_PATH];
	::GetTempPath(MAX_PATH, buffer);
	std::wstring tempPath(buffer);
	::GetTempFileName(
		tempPath.c_str(),
		L"OUT",
		0,
		buffer);
	result = std::wstring(buffer);
}

void CPythonInterpretor::SetPythonStdoutToTempfile(
	const std::wstring& tempFilename) const {
	std::wstring command = L"import os\n";
	command += L"sys.stdout = open('";
	command += tempFilename;
	command += L"', 'w')\n";
	std::string commandString;
	WstringToString(command, commandString);
	PyRun_SimpleString(commandString.c_str());
}

void CPythonInterpretor::FlushPythonOutput() const {
	PyRun_SimpleString("sys.stdout.flush()");
}

void CPythonInterpretor::WstringToString(const std::wstring& from, std::string& to) const {
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	to = converter.to_bytes(from);
}