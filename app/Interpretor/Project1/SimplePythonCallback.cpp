#include "SimplePythonCallback.h"

#include <locale>
#include <codecvt>
#include <string>

SimplePythonCallback::SimplePythonCallback(HWND hwndShow) {
	this->hwndShow = hwndShow;
}

SimplePythonCallback::~SimplePythonCallback() {}

void SimplePythonCallback::ReturnResult(const std::string& resultOutput, const std::string& resultErrors) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring output = converter.from_bytes(resultOutput.c_str());
	std::wstring errors = converter.from_bytes(resultErrors.c_str());
	std::wstring delimeter(L"[ERRORS]");

	if (errors.size() > 0) {
		output.append(delimeter).append(errors);
	}

	if (!::SetWindowText(hwndShow, output.c_str())) {
		//pain
	}
}
