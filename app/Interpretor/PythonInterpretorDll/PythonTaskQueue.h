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
		std::shared_ptr<IReturnResultCallback>& callback_,
		int queueId_)
		: text(text_)
		, callback(callback_)
		, queueId(queueId_) { }

	std::string text;
	std::shared_ptr<IReturnResultCallback> callback;
	int queueId;
};

class CPythonTaskQueue
{
public:
	CPythonTaskQueue(std::unique_ptr<PyObject> newCatcher) { }
	~CPythonTaskQueue() { }

	// Create new task and run it if nothing is running now
	void AddNewTask(
		const std::string& text,
		std::shared_ptr<IReturnResultCallback>& callback);

	//Reset queue
	void Reset(std::unique_ptr<PyObject>& newCatcher);
private:
	std::queue<CPythonTask> queue;
	std::mutex queueMutex;
	//Helps to get output from python
	std::unique_ptr<PyObject> catcher;
	//Helps to manage tasks
	int queueId;

	// Run task and start next one if it's available
	void Run(const CPythonTask& task);

	void FlushPythonOutput() const;

	const std::string GET_OUTPUT_ERROR_MESSAGE = "Output error";
	std::string GetOutputFromPyObject(std::unique_ptr<PyObject>& output) const;
};
