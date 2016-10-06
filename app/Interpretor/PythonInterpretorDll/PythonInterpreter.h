﻿#pragma once

#ifdef PYTHONINTERPRETORDLL_EXPORTS
#define PYTHONINTERPRETORDLL_API __declspec(dllexport) 
#else
#define PYTHONINTERPRETORDLL_API __declspec(dllimport) 
#endif

#include <memory>
#include <string>
#include <exception>

#include "ReturnResultCallback.h"
#include "PythonCommands.h"

class PYTHONINTERPRETORDLL_API CPythonInterpretor {
public:
	CPythonInterpretor();
	virtual ~CPythonInterpretor();

	/**
	* Run piece of code on Python
	**/
	void Run(
		const std::wstring& text,
		std::shared_ptr<IReturnResultCallback> callback) const;

	/**
	* Reset interpreter state
	**/
	void Reset();

private:
	enum State {
		IDLE, INITIALIZED
	} state = IDLE;

	void CheckState(State needState) const throw(std::logic_error);

	void InitializePython();
	void FinalizePython();

	void WstringToString(const std::wstring& from, std::string& to) const;
};
