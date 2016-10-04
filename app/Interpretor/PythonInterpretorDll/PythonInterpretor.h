#pragma once

#ifdef PYTHONINTERPRETORDLL_EXPORTS
#define PYTHONINTERPRETORDLL_API __declspec(dllexport) 
#else
#define PYTHONINTERPRETORDLL_API __declspec(dllimport) 
#endif

#include <memory>

/**
* Interface of CPythonInterpretor callback.
**/
class PYTHONINTERPRETORDLL_API CPythonInterpretorCallback {
public:
	virtual void OnPythonInterpretResult(
		std::shared_ptr<wchar_t> result) = 0;
};

class PYTHONINTERPRETORDLL_API CPythonInterpretor {
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