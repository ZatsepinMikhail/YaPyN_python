#include "PythonTaskQueue.h"

#include <future>
#include <memory>
#include <mutex>
#include <string>

#include "PythonCommands.h"

void CPythonTaskQueue::AddNewTask(
	const std::string& text,
	std::shared_ptr<IReturnResultCallback>& callback) {
	std::lock_guard<std::mutex> lock(queueMutex);
	queue.emplace(text, callback);
	if (queue.size() == 1) {
		std::async(&CPythonTaskQueue::Run, this, queue.front());
	}
}

void CPythonTaskQueue::ClearQueue() {
	std::lock_guard<std::mutex> lock(queueMutex);
	
	std::queue<CPythonTask> empty;
	queue.swap(empty);
}

void CPythonTaskQueue::Run(const CPythonTask& task) {
	// actual run starts here
	PyRun_SimpleString(task.text.c_str());

	std::unique_ptr<PyObject> output(
		PyObject_GetAttrString(catcher.get(), PYTHON_CATCHER_DATA.c_str()));

	std::string stringOutput;//TODO: get it
	task.callback->ReturnResult(stringOutput);

	// actual run ends here
	std::lock_guard<std::mutex> lock(queueMutex);
	queue.pop();
	if (!queue.empty()) {
		std::async(&CPythonTaskQueue::Run, this, queue.front());
	}
}

void CPythonTaskQueue::FlushPythonOutput() const {
	PyRun_SimpleString(CATCHER_FLUSH_OUTPUT.c_str());
}

void CPythonTaskQueue::SetCatcher(std::unique_ptr<PyObject> catcher) {
	this->catcher = std::move(catcher);
}
