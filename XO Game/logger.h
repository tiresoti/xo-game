#pragma once
#include <fstream>

// A simple logger to let user know why the game didn't launch
namespace Logger
{
	void WriteToLog(std::string message);
}
