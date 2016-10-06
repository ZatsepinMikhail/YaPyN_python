#pragma once

#include <string>

class IReturnResultCallback
{
public:
	IReturnResultCallback();
	virtual ~IReturnResultCallback();

	virtual void ReturnResult(std::string) = 0;
};

