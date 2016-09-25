#pragma once

/**
* Interface of CPythonInterpretor callback.
**/
class CPythonInterpretorCallback {
public:
	virtual void OnPythonInterpretResult(wchar_t* result) = 0;
};

class CPythonInterpretor {
public:
	/**
	* Run piece of code on Python
	**/
	void Run(wchar_t* text, CPythonInterpretorCallback* callback);

  void Run();

	/**
	* Reset Python Interpretor session
	**/
	void Reset();

	CPythonInterpretor();

private:
	CPythonInterpretorCallback* callback;
};
