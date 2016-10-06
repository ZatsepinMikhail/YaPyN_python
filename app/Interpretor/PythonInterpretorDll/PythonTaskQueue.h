#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <queue>

#include "ReturnResultCallback.h"

struct CPythonTask {
	CPythonTask(
		const std::string& text_,
		std::shared_ptr<IReturnResultCallback>& callback_)
		: text(text_)
		, callback(callback_) { }

	std::string text;
	std::shared_ptr<IReturnResultCallback> callback;
};

class CPythonTaskQueue
{
public:
	CPythonTaskQueue() { }
	~CPythonTaskQueue() { }

	// Create new task and run it if nothing is running now
	void AddNewTask(
		const std::string& text,
		std::shared_ptr<IReturnResultCallback>& callback);

private:
	std::queue<CPythonTask> queue;
	std::mutex queueMutex;

	// Run task and start next one if it's available
	void Run(const CPythonTask& task);
};

