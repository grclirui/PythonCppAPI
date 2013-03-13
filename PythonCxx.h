/**
 * Handle python embedding in c++
 *
 * @author Rui Li
 * @date 02/04/2013
 */

#ifndef PYTHONCXX_H
#define PYTHONCXX_H

#include <string>
#include <map>
#include <vector>
#include <exception>

#include <Python.h>

namespace py
{
  class PythonInterpreter
  {

  public:

    PythonInterpreter(int argc, char **argv);
    ~PythonInterpreter();

  private:
    PyThreadState* _mstate;
  }; //PythonInterpreter

  class PythonException : public std::exception
  {
  
  public:
    PythonException (const std::string& m);
    virtual ~PythonException() throw();
    
    virtual const char *Info() throw();

  private:
    std::string _message;
  
  }; //pyException

  enum Type { PythonLong, PythonReal, PythonString };

  typedef std::map<std::string, Type> ArgMap;
  typedef std::map<std::string, std::string> StringMap;
  typedef std::vector<std::string> StringList;

  /// wrapper class to python function calls 
  class PythonCxx
  {

  public:

    PythonCxx(int argc, char **argv);
    virtual ~PythonCxx();

    /// execute a python statement or a script
    void ExecuteString(const std::string& statement) 
      throw (PythonException);

    /// execute a python file
    void ExecuteFile(const std::string& fileName)
      throw (PythonException);

    /// load a python module
    void ImportModule(const std::string& moduleName)
      throw (PythonException);

    /// execute a function in the module, nothing is returned
    void CallFunction(const std::string& functionName)
      throw (PythonException);
    void CallFunction(const std::string& functionName,
        const ArgMap& args)
      throw (PythonException);

    /// execute a function in the module, the returned integer
    /// copied to ret
    void CallFunction(const std::string& functionName, 
        const ArgMap& args,
        long& ret
        )
      throw (PythonException);

    /// execute a function in the module, the returned real number
    /// is copied to ret
    void CallFunction(const std::string& functionName,
        const ArgMap& args,
        double& ret
        )
      throw (PythonException);


    /// execute a function in the module, returned list or tuple 
    /// is copied to ret
    void CallFunction(const std::string& functionName,
        const ArgMap& args,
        StringList & ret
        )
      throw (PythonException);

    
    /// execute a functhe module, returned dictionary 
    /// is copied to ret
    void CallFunction(const std::string& functionName,
        const ArgMap& args,
        StringMap& ret
        )
      throw (PythonException);

    /// execute a function in the module, any returned value
    /// is copied to ret
    void CallFunction(const std::string& functionName,
        const ArgMap& args,
        std::string& ret
        )
      throw (PythonException);

  private:
    /// grab a function handle from the python module
    PyObject* GetFunctionObject(const std::string& functionName)
      throw (PythonException);

    /// convert arguments to a PyObjects tuple so that
    /// they can be passed as arguments to a python function
    PyObject* SetFunctionArgs(const py::ArgMap& args)
      throw (PythonException);

    /// makes the actual function call
    PyObject* MakeFunctionCall(const std::string& functionName,
        const py::ArgMap& args)
      throw (PythonException);

  private:
    PythonInterpreter* _pyinterpreter;
    PyObject* _module;
    PyObject* _argstuple;
  }; // PythonCxx class

} //namespace py

#endif /* #ifndef PYTHONCXX_H */
