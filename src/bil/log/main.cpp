#include <iostream>
#include <string>
#include "Log.hpp"



int main()
{
	Log Log("test.log");

	Log.writeError("main","this is a test");

	return 0;
}