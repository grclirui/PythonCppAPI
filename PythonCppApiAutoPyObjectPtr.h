/*
 * PythonCppApiAutoPyObjectPtr.h
 *
 * originally form http://code.activestate.com/recipes/528875-automatic-ref-count-
 * management-in-c-using-a-smart-/
 * modified by Rui Li on Apr 30. 2013
 */

#ifndef PYTHONCPPAPIAUTOPYOBJECTPTR_H_
#define PYTHONCPPAPIAUTOPYOBJECTPTR_H_

#include <Python.h>
#include <memory>

namespace py
{
  typedef std::auto_ptr<PyObject> PythonCppApiAutoPyObjectPtrBase;
  
  /**
   * An auto_ptr that, instead of deleting, decrements the reference count of a 
   * PyObject pointer
   *
   * Make sure to only use this when you get a *new* reference (Py_INCREF or 
   * getting the result of any function that says it returns a new reference
   * to a PyObject), NOT for "borrowed" references
   */
  
  class PythonCppApiAutoPyObjectPtr : public PythonCppApiAutoPyObjectPtrBase
  {
    public:
      PythonCppApiAutoPyObjectPtr(PyObject * pyobj = NULL) : PythonCppApiAutoPyObjectPtrBase(pyobj) {}
      ~PythonCppApiAutoPyObjectPtr() 
      {
        reset();
      }
      void reset(PyObject * pyobj = NULL) 
      {
        if (pyobj != get())
          {
          PyObject *old = release(); // avoid the delete call
          Py_XDECREF(old);
          PythonCppApiAutoPyObjectPtrBase::reset(pyobj);
          }
      }
      void inc() 
      {
        PyObject *ptr = get();
        if (ptr)
          Py_INCREF(ptr);
      }

      operator PyObject*() 
      {
        return this->get();
      }
  };
} 
#endif /* PYTHONCPPAPIAUTOPYOBJECTPTR_H_*/
