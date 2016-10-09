#pragma once

#include <Python.h> // should be the first include

#include "PythonTaskQueue.h"

#include <future>
#include <memory>
#include <mutex>
#include <string>

#include "PythonCommands.h"

CPythonTaskQueue::~CPythonTaskQueue() {
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

void CPythonTaskQueue::Reset(PyObject* newCatcher) {
	std::lock_guard<std::mutex> lock(queueMutex);
	
	catcher = newCatcher;

	++queueId;

	std::queue<CPythonTask> empty;
	queue.swap(empty);
}

void CPythonTaskQueue::Run(const CPythonTask& task) {
	if (task.queueId != this->queueId) {
		//not relevant task
		return;
	}

	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();

	PyRun_SimpleString(task.text.c_str());

	PyObject* output =
		PyObject_GetAttrString(catcher, PYTHON_CATCHER_DATA.c_str());

	std::string stringOutput = GetOutputFromPyObject(output);
	task.callback->ReturnResult(stringOutput);

	PyGILState_Release(gstate);

	std::lock_guard<std::mutex> lock(queueMutex);
	queue.pop();
	if (!queue.empty()) {
		std::async(&CPythonTaskQueue::Run, this, queue.front());
	}
}

void CPythonTaskQueue::FlushPythonOutput() const {
	PyRun_SimpleString(CATCHER_FLUSH_OUTPUT.c_str());
}

std::string CPythonTaskQueue::GetOutputFromPyObject(PyObject* output) const {
	if (PyUnicode_Check(output)) {
		PyObject* bytes = PyUnicode_AsEncodedString(output, "ASCII", "strict"); // Owned reference
		if (bytes != NULL) {
			std::string result = PyBytes_AS_STRING(bytes); // Borrowed pointer
			result = strdup(result.c_str());

			return result;
		} else {
			return GET_OUTPUT_ERROR_MESSAGE;
		}
	} else {
		return GET_OUTPUT_ERROR_MESSAGE;
	}
}
