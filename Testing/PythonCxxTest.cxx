#include <iostream>

#include "PythonCxx.h"
using namespace py; // for brevity

// unit test function
static void run_test(int argc, char** argv);

int main(int argc, char** argv)
{
  run_test(argc, argv);
  std::cout << "back from run_test" << "\n";
  return 0;
}

void
run_test(int argc, char** argv)
{
  try
    {
    // Ok. Now we can create an instance of the interprter itself.
    PythonCxx pycxx(argc, argv);

    // to check if the python interpreter is available and working,
    // execute a simple python statement.
    pycxx.ExecuteString("print 'hello'");

    // execute the script in a file
    pycxx.ExecuteFile("hello.py");
      
    // Load the test module into the interpreter
    pycxx.ImportModule("test");

    // Call a simple function, that takes no arguments
    // and returns nothing      
    pycxx.CallFunction("simple");


    ArgMap args; // our argument list that maps string values 
                  // to their python types     

    // Call a function that takes two long arguments and returns
    // a long      
    {
	  long ret = 0; // value returned by python is stored here
	  args["10"] = PythonLong;
	  args["20"] = PythonLong;
	  pycxx.CallFunction("multiply", args, ret);
	  std::cout << ret << '\n';
	  args.clear();
    }      

    // call a function a takes a string and just echoes it
    {
	  std::string ret = "";
	  args["Hello from C++"] = PythonString;
	  pycxx.CallFunction("echo", args, ret);
	  std::cout << ret << '\n';
	  args.clear();
    }

    // call a function that returns a list
    {
    StringList ret;
	  pycxx.CallFunction("names_as_list", args, ret);
	  size_t sz = ret.size();
	  for (size_t i=0; i<sz; i++)
	    std::cout << ret[i] << '\t';
	  std::cout << '\n';
    }

    // call a function that returns a tuple
    {
	  StringList ret;
	  pycxx.CallFunction("names_as_tuple", args, ret);
	  size_t sz = ret.size();
	  for (size_t i=0; i<sz; i++)
	    std::cout << ret[i] << '\t';
	  std::cout << '\n';
    }

    // call a function that returns a dictionary
    {
	  StringMap ret;
	  pycxx.CallFunction("config", args, ret);
	  StringMap::const_iterator it;
	  for (it = ret.begin(); it != ret.end(); it++)
	    std::cout << it->first << ':' << it->second << '\t';
	  std::cout << '\n';
    }
    std::cout << "about to call destructor" << "\n";
    }
  catch (PythonException ex)
    {
    std::cout << ex.Info() << "\n";
    }
  std::cout << "back to main" << "\n";
}
