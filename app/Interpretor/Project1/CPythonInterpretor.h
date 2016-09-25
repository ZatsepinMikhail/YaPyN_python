#pragma once

/**
* Интерфейс callback с результатами работы интерпретатора.
**/
class CPythonInterpretorCallback {
public:
	virtual void OnPythonInterpretResult(wchar_t* result) = 0;
};

class CPythonInterpretor {
public:
	/**
	* Запускает кусок кода на Python
	**/
	void Run(wchar_t* text, CPythonInterpretorCallback* callback);

	/**
	* Очищает состояние интерпретатора Python
	**/
	void Reset();

	CPythonInterpretor();

private:
	CPythonInterpretorCallback* callback;
};
