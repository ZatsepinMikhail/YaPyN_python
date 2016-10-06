#include "PythonTaskQueue.h"

#include <future>
#include <memory>
#include <mutex>
#include <string>

void CPythonTaskQueue::AddNewTask(
	const std::string& text,
	std::shared_ptr<IReturnResultCallback>& callback) {
	std::lock_guard<std::mutex> lock(queueMutex);
	queue.emplace(text, callback);
	if (queue.size() == 1) {
		std::async(&CPythonTaskQueue::Run, this, queue.front());
	}
}

void CPythonTaskQueue::Run(const CPythonTask& task) {
	// actual run starts here
	//
	// actual run ends here
	std::lock_guard<std::mutex> lock(queueMutex);
	queue.pop();
	if (!queue.empty()) {
		std::async(&CPythonTaskQueue::Run, this, queue.front());
	}
}