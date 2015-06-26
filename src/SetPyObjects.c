/* --------------------------------------------------------------------------  \

    SetPyObjects

    Provides functions to set Python objects from within R.

\  -------------------------------------------------------------------------- */

#include "SetPyObjects.h"

/*  --------------------------------------------------------------------
	r_set_py
	
	Is a high level function which can be used on dictionaries and
	object attributes! 

	Write a new set method since the other one is to messy.
	The object should be retrieved by string. The key and value should
	be Python Objects.
	 
	There are several Python functions which can be used to set variables
	here I will compare the properties. PyDict_SetItem, PyObject_SetItem
	
	// PyObject *py_get_py_obj(const char *c_obj_name)
	
  --------------------------------------------------------------------*/
SEXP r_set_py(SEXP obj_name, SEXP key, SEXP value){
	SEXP success;
	PyObject *py_value;
	const char *c_key = R_TO_C_STRING(key);
	const char *c_obj_name = R_TO_C_STRING(obj_name);
	
	//Rprintf("r_to_py");
	py_value = r_to_py(value);
	
	success = c_to_r_integer(py_set_py(c_obj_name, c_key, py_value));
	Py_XDECREF(py_value);
	
	return success;
}

// Returns -1 on failure.
int py_set_py(const char *obj_name, const char *c_key, PyObject *value){
	PyObject *py_object;
	int success = -1;
	
	if (strcmp(obj_name, "__main__") == 0){ // set something in global
		py_object = PyModule_GetDict(PyImport_AddModule("__main__"));
		Py_INCREF(py_object); // Incref is not necessary here but it allows me to be consistent!
	}else{
		py_object = py_get_py_obj(obj_name);
	}
		
	if (PyDict_Check(py_object)){
		success = PyDict_SetItemString(py_object, c_key, value); // Return 0 on success or -1 on failure.
	}
	
	if (PyObject_HasAttrString(py_object, c_key)){
		success = PyObject_SetAttrString(py_object, c_key, value); // Returns -1 on failure.
	}
	
	Py_XDECREF(py_object);
	return success;
}
