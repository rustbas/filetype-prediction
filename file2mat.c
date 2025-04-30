#include <Python.h>
#include <stdio.h>
#include <stdint.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#define	 BUFFER_SIZE 256
#define	 MSIZE       256

PyObject *get_signature(PyObject *self, PyObject *args) {
  char *filename;

  PyArg_ParseTuple(args, "s", &filename);

  printf("%s\n", filename);

  npy_intp dims[] = {MSIZE, MSIZE};
  PyObject *result = PyArray_SimpleNew(2, dims, NPY_UINT32);

  if (PyErr_Occurred()) {
    return NULL;
  }

  uint32_t *result_data = PyArray_DATA((PyArrayObject *)result);

  for (size_t i=0; i<MSIZE*MSIZE; i++)
    result_data[i] = i;

  return result;
}

static PyMethodDef methods[] = {
  { "get_signature", get_signature, METH_VARARGS, "Prints filename"}, 
  { NULL, NULL, 0, NULL },
};

static struct PyModuleDef file2mat = {
  PyModuleDef_HEAD_INIT,
  "file2mat",
  "Get file signatures in C and return NumPy-array.",
  -1,
  methods,
};

PyMODINIT_FUNC PyInit_file2mat() {
  import_array()
  printf("hello from extension!\n");

  return PyModule_Create(&file2mat);
}
