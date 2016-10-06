#pragma once

#include <Python.h> // should be the first include

#include "PythonInterpretor.h"

#include <codecvt>
#include <locale>
#include <strsafe.h>
#include <Windows.h>

#include "ReturnResultCallback.h"

CPythonInterpretor::CPythonInterpretor() {
	Py_Initialize();
}

CPythonInterpretor::~CPythonInterpretor() {
	Py_Finalize();
}

void CPythonInterpretor::Run(
	const std::wstring& text,
	std::shared_ptr<IReturnResultCallback> callback) const {
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
	std::string result;
	WstringToString(std::wstring(buffer), result);
	callback->ReturnResult(result);
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
	command += L"', 'w+')\n";
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