#pragma once

#include <memory>

/**
* Interface of CPythonInterpretor callback.
**/
class CPythonInterpretorCallback {
public:
	virtual void OnPythonInterpretResult(std::shared_ptr<wchar_t> result) = 0;
};

class CPythonInterpretor {
public:
	/**
	* Run piece of code on Python
	**/
	void Run(std::shared_ptr<wchar_t> text, std::shared_ptr<CPythonInterpretorCallback> callback);

	void Run();

	/**
	* Reset Python Interpretor session
	**/
	void Reset();

	CPythonInterpretor();

private:
	CPythonInterpretorCallback* callback;
};
