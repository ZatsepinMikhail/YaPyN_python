#pragma once

#include <Python.h> // should be the first include

#include "PythonInterpreter.h"

#include <codecvt>
#include <locale>
#include <strsafe.h>
#include <Windows.h>
#include <memory>

#include "ReturnResultCallback.h"

void CPythonInterpretor::InitializePython() {
	Py_Initialize();
	std::unique_ptr<PyObject> mainModule(PyImport_AddModule(PYTHON_MAIN.c_str()));
	PyRun_SimpleString(CATCHER_CLASS_CODE.c_str());

	std::unique_ptr<PyObject> catcher(
		PyObject_GetAttrString(mainModule.get(), PYTHON_CATCHER.c_str()));

	queue.Reset(catcher);

	Py_XDECREF(mainModule.get());
}

void CPythonInterpretor::FinalizePython() {
	Py_Finalize();
}

void CPythonInterpretor::Reset() {
	FinalizePython();
	InitializePython();
}

CPythonInterpretor::CPythonInterpretor() {
	InitializePython();
}

CPythonInterpretor::~CPythonInterpretor() {
	FinalizePython();
}

void CPythonInterpretor::Run(
	const std::wstring& text,
	std::shared_ptr<IReturnResultCallback> callback) const {


}

void CPythonInterpretor::WstringToString(const std::wstring& from, std::string& to) const {
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	to = converter.to_bytes(from);
}
