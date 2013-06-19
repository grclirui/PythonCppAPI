#include <iostream>

#include "PythonCppApiCallFunction.h"
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
  // Ok. Now we can create an instance of the interprter itself.
  PythonCppApiCallFunction pyCppApiCallFunction(argc, argv);

  // to check if the python interpreter is available and working,
  // execute a simple python statement.
  pyCppApiCallFunction.ExecuteString("print 'hello'");

  // execute the script in a file
  pyCppApiCallFunction.ExecuteFile("hello.py");
      
  // Load the test module into the interpreter
  pyCppApiCallFunction.ImportModule("test");

    
  ArgMap args; // our argument list that maps string values 
  bool ok;
    
  // Call a simple function, that takes no arguments
  // and returns nothing     
  args.clear();
    
  pyCppApiCallFunction.CallFunction("simple", args);

  // Call a function that takes two long arguments and returns
  // a long      
  {
  PythonCppApiVariant ret(0); // value returned by python is stored here
  args["10"] = PythonLong;
  args["20"] = PythonLong;
  ret = pyCppApiCallFunction.CallFunction("multiply", args);
  std::cout << ret.toLong(&ok, 0) << '\n';
  args.clear();
  }      
    
    // call a function a takes a string and just echoes it
  {
  PythonCppApiVariant ret("");
  args["Hello from C++"] = PythonString;
  ret = pyCppApiCallFunction.CallFunction("echo", args);
  std::cout << ret.toString(&ok, "") << '\n';
  args.clear();
  }

  // call a function that returns a tuple
  {
  PythonCppApiVariant ret;
	ret = pyCppApiCallFunction.CallFunction("names_as_tuple", args);
  if (ret.getType() == py::PythonCppApiVariant::StringList)
    {
    std::cout << "returned a string list (vector of strings)" << std::endl;
    std::vector<std::string> retStringList = ret.toVector(&ok);
	  size_t sz = retStringList.size();
	  for (size_t i=0; i<sz; i++)
	    std::cout << retStringList[i] << '\t';
	  std::cout << '\n';
    }
  }
    
  // call a function that returns a dictionary
  {
  PythonCppApiVariant ret;
	ret = pyCppApiCallFunction.CallFunction("config", args);
  if (ret.getType() == PythonCppApiVariant::StringMap)
    {
    std::cout << "returned a string map (dictionary)" << std::endl;
    std::map<std::string,std::string> retMap = ret.toMap(&ok);
    std::map<std::string,std::string>::const_iterator it;
	  for (it = retMap.begin(); it != retMap.end(); it++)
      {
	    std::cout << it->first << ':' << it->second << '\t';
      }
	  std::cout << '\n';
    }
  }

  // call a function that returns a list
  {
  PythonCppApiVariant ret;
	ret = pyCppApiCallFunction.CallFunction("names_as_list", args);
  if (ret.getType() == PythonCppApiVariant::StringList)
    {
    std::cout << "returned a string list (vector of strings)" << std::endl;
    std::vector<std::string> retStringList = ret.toVector(&ok);
	  size_t sz = retStringList.size();
	  for (size_t i=0; i<sz; i++)
      {
      std::cout << retStringList[i] << "\t";
      }
	  std::cout << '\n';
    }
  } 
}
