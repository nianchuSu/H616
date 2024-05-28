#include "Python.h"

int main()
{
	Py_Initialize();	// initialization function
	PyRun_SimpleString("print('hello')");	// Calling a python function to output
	Py_Finalize();	// release resource
	return 0;
}
