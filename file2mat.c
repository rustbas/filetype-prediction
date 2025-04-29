#include <Python.h>
#include <stdio.h>

#define BUFFER_SIZE 256

PyObject *get_signature(PyObject *self, PyObject *args) {
  char *filename;

  PyArg_ParseTuple(args, "s", &filename);

  printf("%s\n", filename);

  return PyLong_FromLong((long)1);
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
  printf("hello from extension!\n");

  return PyModule_Create(&file2mat);
}
