/**
 * Handle python embedding in c++
 *
 * @author Rui Li
 * @date 02/04/2013
 */

#ifndef PYTHONCPPAPIINTERPRETER_H
#define PYTHONCPPAPIINTERPRETER_H


#include <Python.h>

namespace py
{

  class PythonCppApiInterpreter
  {
  public:

    PythonCppApiInterpreter(int argc, char **argv);
    ~PythonCppApiInterpreter();

  private:
    PyThreadState* _mstate;
  }; //PythonCppApiInterpreter

} //namespace py

#endif /* #ifndef PYTHONCPPAPIINTERPRETER_H */
