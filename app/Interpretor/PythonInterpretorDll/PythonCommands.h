#pragma once

#include <string>

/**
* Catcher code
**/
const std::string CATCHER_CLASS_CODE =
"import sys\
class StdoutCatcher :\
	def __init__(self) :\
		self.data = ''\
	def write(self, stuff) :\
		self.data = self.data + stuff\
\
catcher = StdoutCatcher()\
sys.stdout = catcher";

/**
* Flush catcher output
**/
const std::string CATCHER_FLUSH_OUTPUT = "catcher.data = ''";

const std::string PYTHON_MAIN = "__main__";

const std::string PYTHON_CATCHER = "catcher";

const std::string PYTHON_CATCHER_DATA = "data";
