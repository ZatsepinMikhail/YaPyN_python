#pragma once

#include "ReturnResultCallback.h"

#include <string>
#include <Windows.h>

class SimplePythonCallback : public IReturnResultCallback {
public:
	SimplePythonCallback(HWND hwndShow);

	void ReturnResult(const std::string& result);

	~SimplePythonCallback();
private:
	HWND hwndShow;
};
