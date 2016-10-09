#include "SimplePythonCallback.h"

#include <locale>
#include <codecvt>
#include <string>

SimplePythonCallback::SimplePythonCallback(HWND hwndShow) {
	this->hwndShow = hwndShow;
}

SimplePythonCallback::~SimplePythonCallback() {}

void SimplePythonCallback::ReturnResult(const std::string& result) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wstring = converter.from_bytes(result.c_str());

	if (!::SetWindowText(hwndShow, wstring.c_str())) {
		//pain
	}
}
