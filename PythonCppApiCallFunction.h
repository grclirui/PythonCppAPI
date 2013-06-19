/**
 * Handle python embedding in c++
 *
 * @author Rui Li
 * @date 02/04/2013
 */

#ifndef PYTHONCPPAPICALLFUNCTION_H
#define PYTHONCPPAPICALLFUNCTION_H

// Undefine macros that Python.h defines to vaoid redefinition warning
#undef _POSIX_C_SOURCE
#undef _POSIX_THREADS
#undef _XOPEN_SOURCE

#include <Python.h>
#include "PythonCppApiInterpreter.h"
#include "PythonCppApiAutoPyObjectPtr.h"
#include "PythonCppApiVariant.h"

#include <string>
#include <map>
#include <vector>

namespace py 
{
  enum Type { PythonLong, PythonReal, PythonString };

  typedef std::vector<std::string> StringList;
  typedef std::map<std::string, std::string> StringMap;
  typedef std::map<std::string, Type> ArgMap;

  /// wrapper class to python function calls 
  class PythonCppApiCallFunction
  {

  public:

    PythonCppApiCallFunction(int argc, char **argv);
    virtual ~PythonCppApiCallFunction();
  
    /// execute a python statement or a script
    bool ExecuteString(const std::string& statement); 
  
    /// execute a python file
    bool ExecuteFile(const std::string& fileName);
  
    /// load a python module
    bool ImportModule(const std::string& moduleName);
  
    /// execute a function in the module
    py::PythonCppApiVariant CallFunction(const std::string& functionName, const ArgMap& args);
  
  private:
    /// grab a function handle from the python module
    PythonCppApiAutoPyObjectPtr GetFunctionObject(const std::string& functionName);
  
    /// convert arguments to a PyObjects tuple so that
    /// they can be passed as arguments to a python function
    PythonCppApiAutoPyObjectPtr SetFunctionArgs(const py::ArgMap& args); 
  
    /// makes the actual function call
    PythonCppApiAutoPyObjectPtr MakeFunctionCall(const std::string& functionName,
        const py::ArgMap& args);
  
  private:
    PythonCppApiInterpreter* _pyinterpreter;
    PythonCppApiAutoPyObjectPtr _module;
    PythonCppApiAutoPyObjectPtr _argstuple;
  }; // PythonCppApiCallFunction class
  
} //namespace py

#endif /* #ifndef PYTHONCPPAPICALLFUNCTION_H */
