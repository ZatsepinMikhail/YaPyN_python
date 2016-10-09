#pragma once

#include <Python.h> // should be the first include

#include "PythonTaskQueue.h"

#include <future>
#include <memory>
#include <mutex>
#include <string>

#include "PythonCommands.h"

CPythonTaskQueue::CPythonTaskQueue() {
	queueId = 0;
}

CPythonTaskQueue::~CPythonTaskQueue() {
	Py_XDECREF(catcher.get());
}

void CPythonTaskQueue::AddNewTask(
	const std::string& text,
	std::shared_ptr<IReturnResultCallback>& callback) {

	std::lock_guard<std::mutex> lock(queueMutex);
	queue.emplace(text, callback, queueId);
	if (queue.size() == 1) {
		std::async(&CPythonTaskQueue::Run, this, queue.front());
	}
}

void CPythonTaskQueue::Reset(std::unique_ptr<PyObject>& newCatcher) {
	std::lock_guard<std::mutex> lock(queueMutex);
	
	Py_XDECREF(catcher.get());
	catcher = std::move(newCatcher);

	++queueId;

	std::queue<CPythonTask> empty;
	queue.swap(empty);
}

void CPythonTaskQueue::Run(const CPythonTask& task) {
	if (task.queueId != this->queueId) {
		//not relevant task
		return;
	}

	PyRun_SimpleString(task.text.c_str());

	std::unique_ptr<PyObject> output(
		PyObject_GetAttrString(catcher.get(), PYTHON_CATCHER_DATA.c_str()));

	std::string stringOutput = GetOutputFromPyObject(output);
	task.callback->ReturnResult(stringOutput);

	Py_XDECREF(output.get());

	std::lock_guard<std::mutex> lock(queueMutex);
	queue.pop();
	if (!queue.empty()) {
		std::async(&CPythonTaskQueue::Run, this, queue.front());
	}
}

void CPythonTaskQueue::FlushPythonOutput() const {
	PyRun_SimpleString(CATCHER_FLUSH_OUTPUT.c_str());
}

std::string CPythonTaskQueue::GetOutputFromPyObject(std::unique_ptr<PyObject>& output) const {
	if (PyUnicode_Check(output.get())) {
		PyObject * bytes = PyUnicode_AsEncodedString(output.get(), "ASCII", "strict"); // Owned reference
		if (bytes != NULL) {
			std::string result = PyBytes_AS_STRING(bytes); // Borrowed pointer
			result = strdup(result.c_str());
			Py_DECREF(bytes);

			return result;
		} else {
			return GET_OUTPUT_ERROR_MESSAGE;
		}
	} else {
		return GET_OUTPUT_ERROR_MESSAGE;
	}
}
