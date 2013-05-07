/**
 * defines wrapper functions to the Python C API
 *
 * @author: Rui Li
 * @date: 02/06/2013
 */

#include <iostream>
using std::cout;
#include <sstream>

#include "PythonCppApiUtility.h"
#include "PythonCppApiCallFunction.h"

/// PythonCppApiCallFunction
py::PythonCppApiCallFunction::PythonCppApiCallFunction(int argc, char** argv)
{
  this->_module.reset(PythonCppApiAutoPyObjectPtr(NULL));
  this->_pyinterpreter = new PythonCppApiInterpreter(argc, argv);
}

py::PythonCppApiCallFunction::~PythonCppApiCallFunction()
{
  if (this->_pyinterpreter)
    {
    delete this->_pyinterpreter;
    }
}

bool py::PythonCppApiCallFunction::ExecuteString(const std::string& s)
{
  int flag = PyRun_SimpleString(s.c_str());
  if (flag)
    {
    std::ostringstream oss;
    oss << "There was an error in the script < " << s << " >";
    std::cerr << "Err: " << oss.str() << std::endl;
    return false;
    }
  return true;
}

bool
py::PythonCppApiCallFunction::ExecuteFile(const std::string& fname) 
{
  FILE *fp = fopen(fname.c_str(), "r");

  if (!fp)
    {
    std::ostringstream oss;
    oss << "Failed to open file <" << fname << ">";
    std::cerr << "Err: " << oss.str() << std::endl;
    return false;
    }

  int flag = PyRun_SimpleFile(fp, fname.c_str());
  fclose(fp);
  if (flag)
    {
      std::ostringstream oss;
      oss << "There is a error in file <" << fname << ">";
      std::cerr << "Err: " << oss.str() << std::endl;
      return false;
    }
  return true;
}

bool py::PythonCppApiCallFunction::ImportModule(const std::string& moduleName) 
{
  PythonCppApiAutoPyObjectPtr name (PyString_FromString(moduleName.c_str()));
  this->_module.reset(PyImport_Import(name));

  if (!(this->_module))
    {
    std::ostringstream oss;
    oss << "Failed to load module < " << moduleName << " >";
    std::cerr << "Err: " << oss.str() << std::endl;
    return false;
    }
  return true;
}

py::Variant py::PythonCppApiCallFunction::CallFunction(const std::string& functionName,
    const py::ArgMap& args)
{
  PythonCppApiAutoPyObjectPtr ret (MakeFunctionCall(functionName, args));
  if (PyInt_Check(ret))
    {
    return Variant(PyInt_AsLong(ret));
    }
  else if (PyFloat_Check(ret))
    {
    return Variant(PyFloat_AsDouble(ret));
    }
  else if (PyString_Check(ret))
    {
    return Variant(PyString_AsString(ret));
    }
  else if (PyTuple_Check(ret))
    {
    StringList stringList;
    MakeListFromPyTuple(ret, stringList); 
    return Variant(stringList);
    }
  else if (PyList_Check(ret))
    {
    StringList stringList;
    PythonCppApiAutoPyObjectPtr tuple (PyList_AsTuple(ret));
    MakeListFromPyTuple(tuple, stringList);
    return Variant(stringList);
    }
  else if (PyDict_Check(ret))
    {
    py::StringMap stringMap;
    MakeMapFromPyDictionary(ret, stringMap);
    return Variant(stringMap);
    }

  return Variant();
}

/// private functions
py::PythonCppApiAutoPyObjectPtr 
py::PythonCppApiCallFunction::GetFunctionObject(const std::string& functionName)
{
  if (!_module)
    {
    std::ostringstream oss;
    oss << "No module loaded";
    std::cerr << "Err: " << oss.str() << std::endl;
    return PythonCppApiAutoPyObjectPtr(NULL);
    }
  PythonCppApiAutoPyObjectPtr func(PyObject_GetAttrString(_module, static_cast<const char*>(functionName.c_str())));
  if (!func || !PyCallable_Check(func))
    {
    std::ostringstream oss;
    oss << '<' << functionName << "> is not a function or is not callable.";
    std::cerr << "Err: " << oss.str() << std::endl;
    return PythonCppApiAutoPyObjectPtr(NULL);
    }
  return func;
}

py::PythonCppApiAutoPyObjectPtr
py::PythonCppApiCallFunction::SetFunctionArgs(const py::ArgMap& args)
{
  if (!_module)
    {
    std::ostringstream oss;
    oss << "No module loaded";
    std::cerr << "Err: " << oss.str() << std::endl;
    return PythonCppApiAutoPyObjectPtr(NULL);
    }
  size_t sz = args.size();  
  if (!sz)
    return PythonCppApiAutoPyObjectPtr(NULL);
  PythonCppApiAutoPyObjectPtr tuple (PyTuple_New(sz));
  size_t i = 0;
  PythonCppApiAutoPyObjectPtr val;
  ArgMap::const_iterator it;
  for (it = args.begin(); it != args.end(); it++)
    {
    switch (it->second)
      {
      case PythonLong:
        {
        long l = strtol(it->first.c_str(), 0, 10); 
        val.reset(PyInt_FromLong(l));
        break;
        } 
      case PythonReal: 
        {
        double d = strtod(it->first.c_str(), 0);      
        val.reset(PyFloat_FromDouble(d));
        break; 
        } 
      case PythonString: 
        { 
        val.reset(PyString_FromString(it->first.c_str())); 
        break; 
        } 
      default: 
        { 
        std::ostringstream oss;
        oss << "Not a supported type for argument passing.";
        std::cerr << "Err: " << oss.str() << std::endl;
        return PythonCppApiAutoPyObjectPtr(NULL);
        } 
      } 
    if (!val) 
      { 
      std::ostringstream oss; 
      oss << "Failed to convert <" << it->second << "> to PyObject.";
      std::cerr << "Err: " << oss.str() << std::endl;
      return PythonCppApiAutoPyObjectPtr(NULL);
      }      
    PyTuple_SetItem(tuple, i++, val); 
    }
  return tuple;
}

py::PythonCppApiAutoPyObjectPtr 
py::PythonCppApiCallFunction::MakeFunctionCall(const std::string &functionName, 
    const py::ArgMap &args)
{
  PythonCppApiAutoPyObjectPtr ret(PyObject_CallObject(GetFunctionObject(functionName), 
        SetFunctionArgs(args)));
  return ret;
}

