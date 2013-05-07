/**
 * utility functions for PythonCppApi
 *
 * @author Rui Li
 * date: 04/10/2013
 */
#ifndef PYTHONCPPAPIUTILITY_H
#define PYTHONCPPAPIUTILITY_H

#include "PythonCppApiCallFunction.h"

namespace py
{
  void MakeListFromPyTuple(PyObject* tuple, StringList& stringList);
  void MakeMapFromPyDictionary(PyObject* dict, StringMap& stringMap);
  std::string ToString(PyObject *val);
}

#endif
