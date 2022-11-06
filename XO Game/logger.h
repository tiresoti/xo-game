#pragma once
#include <fstream>

// A simple logger to let user know why the game didn't launch
namespace Logger
{
	void WriteToLog(std::string message) {
		std::ofstream errorfile;
		try
		{
			errorfile.open("errors.txt", std::ios::out | std::ios::trunc);
			errorfile << message << '\n';
			errorfile.close();
		}
		catch (std::ofstream::failure e)
		{
			exit(23);
		}
	}
}
