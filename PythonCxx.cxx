/**
 * defines wrapper functions to the Python C API
 *
 * @author: Rui Li
 * @date: 02/06/2013
 */

#include <iostream>
using std::cout;
#include <sstream>

#include "PythonCxx.h"

/// static utility functions
static void raiseFunctionExceptions(const std::string& functionName)
  throw (py::PythonException);

static void makeListFromPyTuple(PyObject* tuple, py::StringList& pyStringList);
static void makeListFromPyList(PyObject* list, py::StringList& pyStringList);
static void makeMapFromPyDictionary(PyObject* dict, py::StringMap& pyStringList);
static std::string toString(PyObject* val);

/// class PythonInterpreter
py::PythonInterpreter::PythonInterpreter(int argc, char** argv)
{
  if (PyEval_ThreadsInitialized())
    PyEval_AcquireLock();
  else
    PyEval_InitThreads();

  Py_Initialize();
  PyEval_ReleaseLock();

  // if this is not the first time we initialize Python, ther interpreter
  // lock already exists and we need to lock it as PyEval_InitThreads would
  // not do that in that case
  PyEval_AcquireLock();
  PySys_SetArgv(argc, argv);
  
  if (!(this->_mstate = PyThreadState_Get()))
    throw PythonException("Python threadstate is NULL.");
  PyEval_ReleaseLock();
}

py::PythonInterpreter::~PythonInterpreter()
{
  PyEval_ReleaseLock(); // this is to prevent deadlock at PyEval_AcquireLock, ?better solution
  PyEval_AcquireLock();
  PyThreadState_Swap(this->_mstate);
  Py_Finalize();
  PyEval_ReleaseLock();
}

/// PythonException
py::PythonException::PythonException(const std::string& m)
{
  this->_message = m; 
}

py::PythonException::~PythonException() throw()
{
}

const char* 
py::PythonException::Info() throw()
{
  return this->_message.c_str();
}

/// PythonCxx
py::PythonCxx::PythonCxx(int argc, char** argv)
{
  this->_pyinterpreter = new PythonInterpreter(argc, argv);
  this->_module = 0;
}

py::PythonCxx::~PythonCxx()
{
  if (this->_module)
    {
    Py_DECREF(this->_module);
    }

  if (this->_pyinterpreter)
    {
    delete this->_pyinterpreter;
    }
}

void py::PythonCxx::ExecuteString(const std::string& s)
  throw (py::PythonException)
{
  int flag = PyRun_SimpleString(s.c_str());
  if (flag)
    {
    throw PythonException("There was a error in the script.");
    }
}

void
py::PythonCxx::ExecuteFile(const std::string& fname) 
  throw (py::PythonException)
{
  FILE *fp = fopen(fname.c_str(), "r");

  if (!fp)
    {
    std::ostringstream oss;
    oss << "Failed to open file <" << fname << ">";
    throw PythonException(oss.str());
    }

  int flag = PyRun_SimpleFile(fp, fname.c_str());
  fclose(fp);
  if (flag)
    {
      std::ostringstream oss;
      oss << "There is a error in file <" << fname << ">";
      throw PythonException(oss.str());
    }
}

void py::PythonCxx::ImportModule(const std::string& moduleName) 
  throw (py::PythonException)
{
  PyObject* name = PyString_FromString(moduleName.c_str());
  this->_module = PyImport_Import(name);
  Py_DECREF(name);

  if (!(this->_module))
    {
    std::ostringstream oss;
    oss << "Failed to load module <" << moduleName << ">";
    throw PythonException(oss.str());
    }
}

void py::PythonCxx::CallFunction(const std::string& functionName)
  throw (py::PythonException)
{
  py::ArgMap args;
  PyObject* ret = MakeFunctionCall(functionName, args);
  if (ret)
    {
    Py_DECREF(ret);
    }
}

void py::PythonCxx::CallFunction(const std::string& functionName,
    const py::ArgMap& args)
  throw (py::PythonException)
{
  PyObject* ret = MakeFunctionCall(functionName, args);
  if (ret)
    {
    Py_DECREF(ret);
    }
}

void
py::PythonCxx::CallFunction(const std::string& functionName, 
    const py::ArgMap& args,
    long& ret
    ) 
  throw (py::PythonException)
{
  PyObject* retVal = MakeFunctionCall(functionName, args);
  if (retVal)
    {
    if (!PyInt_Check(retVal))
      {
      Py_DECREF(retVal);
      throw PythonException("returned value is not integer.");
      }
    ret = PyInt_AsLong(retVal);
    Py_DECREF(retVal);
    }
  else
    {
    raiseFunctionExceptions(functionName);
    }
}


void
py::PythonCxx::CallFunction(const std::string& functionName, 
    const py::ArgMap& args,
    double& ret
    ) 
  throw (py::PythonException)
{
  PyObject* retVal = MakeFunctionCall(functionName, args);
  if (retVal)
    {
    if (!PyFloat_Check(retVal))
      {
      Py_DECREF(retVal);
      throw PythonException("returned value is not real.");
      }
    ret = PyFloat_AsDouble(retVal);
    Py_DECREF(retVal);
    }
  else
    {
    raiseFunctionExceptions(functionName);
    }
}

void
py::PythonCxx::CallFunction(const std::string& functionName, 
    const py::ArgMap& args,
    std::string& ret
    ) 
  throw (py::PythonException)
{
  PyObject* retVal = MakeFunctionCall(functionName, args);
  if (retVal)
    {
    if (!PyString_Check(retVal))
      {
      Py_DECREF(retVal);
      throw PythonException("returned value is not string.");
      }
    ret = PyString_AsString(retVal);
    Py_DECREF(retVal);
    }
  else
    {
    raiseFunctionExceptions(functionName);
    }
}

void
py::PythonCxx::CallFunction(const std::string& functionName, 
    const py::ArgMap& args,
    py::StringList& ret
    ) 
  throw (py::PythonException)
{
  PyObject* retVal = MakeFunctionCall(functionName, args);
  if (retVal)
    {
    if (PyTuple_Check(retVal))
      {
      makeListFromPyTuple(retVal, ret);
      }
    else if (PyList_Check(retVal))
      {
      makeListFromPyList(retVal, ret);
      }
    else
      {
      Py_DECREF(retVal);
      throw PythonException("returned is neither a list or tuple.");
      }
    Py_DECREF(retVal);
    }
  else
    {
    raiseFunctionExceptions(functionName);
    }
}

void
py::PythonCxx::CallFunction(const std::string& functionName, 
    const py::ArgMap& args,
    py::StringMap& ret
    ) 
  throw (py::PythonException)
{
  PyObject* retVal = MakeFunctionCall(functionName, args);
  if (retVal)
    {
    if (!PyDict_Check(retVal))
      {
      Py_DECREF(retVal);
      throw PythonException("returned value is not dictionary object.");
      }
    makeMapFromPyDictionary(retVal, ret);
    Py_DECREF(retVal);
    }
  else
    {
    raiseFunctionExceptions(functionName);
    }
}

/// private functions
PyObject* py::PythonCxx::GetFunctionObject(const std::string& functionName)
  throw(py::PythonException)
{
  if (!_module)
    {
    throw PythonException("No module loaded.");
    }
  PyObject* func = PyObject_GetAttrString(_module, static_cast<const char*>(functionName.c_str()));
  if (!func || !PyCallable_Check(func))
    {
      std::ostringstream oss;
      oss << '<' << functionName << "> is not a function or is not callable.";
      throw PythonException(oss.str());
    }
  return func;
}

PyObject*
py::PythonCxx::SetFunctionArgs(const py::ArgMap& args)
  throw(py::PythonException)
{
  if (!_module)
    {
    throw PythonException("No module loaded.");
    }
  size_t sz = args.size();  
  if (!sz)
    return 0;
  PyObject* tuple = PyTuple_New(sz);
  size_t i = 0;
  PyObject* val = 0;
  ArgMap::const_iterator it;
  for (it = args.begin(); it != args.end(); it++)
    {
    switch (it->second)
      {
      case PythonLong:
        {
        long l = strtol(it->first.c_str(), 0, 10); 
        val = PyInt_FromLong(l);
        break;
        } 
      case PythonReal: 
        {
        double d = strtod(it->first.c_str(), 0);      
        val = PyFloat_FromDouble(d);
        break; 
        } 
      case PythonString: 
        { 
        val = PyString_FromString(it->first.c_str()); 
        break; 
        } 
      default: 
        { 
        Py_DECREF(tuple);     
        throw PythonException("Not a supported type for argument passing."); 
        } 
      } 
    if (!val) 
      { 
      Py_DECREF(tuple);   
      std::ostringstream oss; 
      oss << "Failed to convert <" << it->second << "> to PyObject.";
      throw PythonException(oss.str()); 
      }      
    PyTuple_SetItem(tuple, i++, val); 
    Py_DECREF(val);      
    }
  return tuple;
}

PyObject* 
py::PythonCxx::MakeFunctionCall(const std::string &functionName, 
    const py::ArgMap &args)
  throw (py::PythonException)
{
  PyObject* function = GetFunctionObject(functionName);
  PyObject* argsTuple = SetFunctionArgs(args);
  Py_XINCREF(argsTuple);
  PyObject* ret = PyObject_CallObject(function, argsTuple);
  Py_XDECREF(argsTuple);
  Py_DECREF(function);
  return ret;
}

void raiseFunctionExceptions(const std::string& functionName)
  throw (py::PythonException)
{
  std::ostringstream oss;
  oss << "Call to function <" << functionName <<">failed!";
  throw py::PythonException(oss.str());
}

void makeListFromPyTuple(PyObject* tuple, py::StringList& pyStringList)
{
  if (tuple)
    {
    int size = PyTuple_Size(tuple);
    for (int i = 0; i < size; i ++)
      {
      pyStringList.push_back(toString(PyTuple_GetItem(tuple, i)));
      }
    }
}

void makeListFromPyList(PyObject* list, py::StringList& pyStringList)
{
  if (list)
    {
    int size = PyList_Size(list);
    for (int i = 0; i < size; i ++) 
      {
      pyStringList.push_back(toString(PyList_GetItem(list, i)));
      }
    }
}

void makeMapFromPyDictionary(PyObject* dictionary, py::StringMap& pyStringMap)
{
  if (dictionary)
    {
    PyObject *key;
    PyObject *value;
    Py_ssize_t position = 0;
    while(PyDict_Next(dictionary, &position, &key, &value))
      {
      pyStringMap[toString(key)] = toString(value);
      }
    }
}

std::string toString(PyObject* value)
{
  if (value)
    {
    std::ostringstream oss;
    if (PyInt_Check(value))
      oss << PyInt_AsLong(value);
    else if (PyLong_Check(value))
      oss << PyLong_AsLong(value);
    else if (PyFloat_Check(value))
      oss << PyFloat_AsDouble(value);
    else if (PyString_Check(value))
      oss << '\"' << PyString_AsString(value) << '\"';
    else if (PyTuple_Check(value))
      {
      oss << '(';
      py::StringList pyStringList;
      makeListFromPyTuple(value, pyStringList);
      size_t sz = pyStringList.size();
      for (size_t i = 0; i < sz; i++)
        {
        oss << pyStringList[i];
        if (i != (sz - 1))
          oss << ',';
        }
      oss << ')';
      }
    else if (PyList_Check(value))
      {
      oss << '[';
      py::StringList pyStringList;
      makeListFromPyList(value, pyStringList);
      size_t sz = pyStringList.size();
      for (size_t i = 0; i < sz; i++)
        {
        oss << pyStringList[i];
        if (i != (sz - 1))
          oss << ',';
        }
        oss << ']';
      }
    else if (PyDict_Check(value))
      {
      oss << '{';
      py::StringMap pyStringMap;
      makeMapFromPyDictionary(value, pyStringMap);
      size_t sz = pyStringMap.size();
      size_t i = 0;
      py::StringMap::const_iterator it;
      for (it=pyStringMap.begin(); it != pyStringMap.end(); it++)
        {
        oss << it->first << ':' << it->second;
        if (i != (sz -1))
          oss << ',';
        i++;
        }
        oss << '}';
      }
    else
      return "";
    
    return oss.str();
    }
  else
    return "";
}
