#include "PythonCppApiUtility.h"
#include <iostream>
#include <sstream>
namespace py
{
  void MakeListFromPyTuple(PyObject* tuple, StringList& pyStringList)
  {
    if (tuple)
      {
      Py_ssize_t size = PyTuple_Size(tuple);
      for (Py_ssize_t i = 0; i < size; i ++)
        {
        pyStringList.push_back(ToString(PyTuple_GetItem(tuple, i)));
        }
      }
  }

  void MakeMapFromPyDictionary(PyObject* dictionary, StringMap& pyStringMap)
  {
    if (dictionary)
      {
      PyObject *key;
      PyObject *value;
      Py_ssize_t position = 0;
      while(PyDict_Next(dictionary, &position, &key, &value))
        {
        pyStringMap[ToString(key)] = ToString(value);
        }
      }
  }

  std::string ToString(PyObject* value)
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
        {
        oss << '\"' << PyString_AsString(value) << '\"';
        }  
      else if (PyTuple_Check(value))
        {
        oss << '(';
        StringList pyStringList;
        MakeListFromPyTuple(value, pyStringList);
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
        StringList pyStringList;
        PythonCppApiAutoPyObjectPtr tuple (PyList_AsTuple(value));
        MakeListFromPyTuple(tuple, pyStringList);
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
        StringMap pyStringMap;
        MakeMapFromPyDictionary(value, pyStringMap);
        size_t sz = pyStringMap.size();
        size_t i = 0;
        StringMap::const_iterator it;
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
}
      

