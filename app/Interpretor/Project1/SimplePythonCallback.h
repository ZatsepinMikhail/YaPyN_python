#pragma once

#include "ReturnResultCallback.h"

#include <string>
#include <Windows.h>

class SimplePythonCallback : public IReturnResultCallback {
public:
	SimplePythonCallback(HWND hwndShow);

	void ReturnResult(const std::string& resultOutput, const std::string& resultErrors);

	~SimplePythonCallback();
private:
	HWND hwndShow;
};
