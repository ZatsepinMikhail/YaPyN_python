#pragma once

#ifdef PYTHONINTERPRETORDLL_EXPORTS
#define PYTHONINTERPRETORDLL_API __declspec(dllexport) 
#else
#define PYTHONINTERPRETORDLL_API __declspec(dllimport) 
#endif

#include <memory>
#include <string>

#include "ReturnResultCallback.h"

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
	void Reset() const;

private:
	void CreateTempFilename(std::wstring& result) const;

	void SetPythonStdoutToTempfile(const std::wstring& tempFilename) const;
	void FlushPythonOutput() const;

	void WstringToString(const std::wstring& from, std::string& to) const;
};