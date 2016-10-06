#pragma once

#include <string>

class IReturnResultCallback
{
public:
	IReturnResultCallback();
	virtual ~IReturnResultCallback();

	virtual void ReturnResult(const std::string&) = 0;
};

