#pragma once

#include "Python.h"

#include <memory>
#include <mutex>
#include <string>
#include <queue>
#include <exception>

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

	//Clear queue
	void ClearQueue();

	void SetCatcher(std::unique_ptr<PyObject> catcher);
private:
	std::queue<CPythonTask> queue;
	std::mutex queueMutex;
	
	//Helps to get output from python
	std::unique_ptr<PyObject> catcher;

	// Run task and start next one if it's available
	void Run(const CPythonTask& task);

	void FlushPythonOutput() const;
};
