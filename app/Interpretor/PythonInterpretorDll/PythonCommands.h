#pragma once

#include <string>

/**
* Catcher code
**/
const std::string CATCHER_CLASS_CODE =
"import sys\n\
class StdoutCatcher :\n\
	def __init__(self) :\n\
		self.data = ''\n\
	def write(self, stuff) :\n\
		self.data = self.data + stuff\n\
\n\
catcher = StdoutCatcher()\n\
sys.stdout = catcher\n";

/**
* Flush catcher output
**/
const std::string CATCHER_FLUSH_OUTPUT = "catcher.data = ''";

const std::string PYTHON_MAIN = "__main__";

const std::string PYTHON_CATCHER = "catcher";

const std::string PYTHON_CATCHER_DATA = "data";
