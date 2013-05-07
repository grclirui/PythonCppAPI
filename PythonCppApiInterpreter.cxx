/**
 * defines wrapper functions to the PythonCppApiInterpreter
 *
 * @author: Rui Li
 * @date: 02/06/2013
 */

#include "PythonCppApiInterpreter.h"

/// class PythonCppApiInterpreter
py::PythonCppApiInterpreter::PythonCppApiInterpreter(int argc, char** argv)
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
  
  if ((this->_mstate = PyThreadState_Get()))
    PyEval_ReleaseLock();
}

py::PythonCppApiInterpreter::~PythonCppApiInterpreter()
{
  PyEval_ReleaseLock(); // this is to prevent deadlock at PyEval_AcquireLock, ?better solution
  PyEval_AcquireLock();
  PyThreadState_Swap(this->_mstate);
  Py_Finalize();
  PyEval_ReleaseLock();
}

