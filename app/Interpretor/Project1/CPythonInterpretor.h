#pragma once

/**
* ��������� callback � ������������ ������ ��������������.
**/
class CPythonInterpretorCallback {
public:
	virtual void OnPythonInterpretResult(wchar_t* result) = 0;
};

class CPythonInterpretor {
public:
	/**
	* ��������� ����� ���� �� Python
	**/
	void Run(wchar_t* text, CPythonInterpretorCallback* callback);

	/**
	* ������� ��������� �������������� Python
	**/
	void Reset();

	CPythonInterpretor();

private:
	CPythonInterpretorCallback* callback;
};
