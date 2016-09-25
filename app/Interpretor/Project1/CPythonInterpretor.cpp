#include "CPythonInterpretor.h"

void CPythonInterpretor::Run(wchar_t* text, CPythonInterpretorCallback* callback) {
	callback->OnPythonInterpretResult(text);
}
