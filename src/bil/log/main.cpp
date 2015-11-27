#include <iostream>
#include <string>
#include "Log.hpp"



int main()
{
	Log Log;

	Log.writeError(__func__,"this is a test");

	return 0;
}