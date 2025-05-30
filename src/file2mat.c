#include <Python.h>
#include <stdio.h>
#include <stdint.h>

/* #define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION */
#define NPY_NO_DEPRECATED_API NPY_2_2_API_VERSION
#include <numpy/arrayobject.h>

#define	 BUFFER_SIZE 256
#define	 MSIZE       256

typedef unsigned char u8;

typedef struct {
  u8 *data;
  size_t size;
} raw_data;

int read_file(const char *filepath, raw_data *rd, int verbose) {
  FILE *file = fopen(filepath, "rb");

  if (file == NULL) {
    fprintf(stderr, "[ERR] Can't read file %s. Error: %s", filepath, strerror(errno));
    return 1;
  }
  
  fseek(file, 0, SEEK_END); // Jump to the EOF
  rd->size = ftell(file);   // Get offset of file
  rewind(file);             // Return to begin of file

  rd->data = (u8*) malloc(rd->size * sizeof(u8));
  fread(rd->data, rd->size, 1, file);
  
  if (verbose)
    printf("[INFO] Readed %zu bytes\n", rd->size);

  fclose(file);
  return 0;
}

int build_matrix(raw_data *rd, uint32_t matrix[MSIZE][MSIZE], int verbose) {
  size_t idx1, idx2;
  for (size_t i=0; i<rd->size-1; i++){
    idx1 = rd->data[i];
    idx2 = rd->data[i+1];
    matrix[idx1][idx2]++;
  }

  if (verbose) {
    printf("[INFO] First 4 elements\n    ");
    for (size_t i=0; i<4; i++)
      printf("%08X ", matrix[0][i]);
    printf("\n");
  }
  return 0;
}

PyObject *signature_from_filepath_by2(PyObject *self, PyObject *args) {
  char *filepath;
  int verbose;

  PyArg_ParseTuple(args, "si", &filepath, &verbose);

  raw_data rd = {0};
  uint32_t matrix[MSIZE][MSIZE] = {0};
  
  if (read_file(filepath, &rd, verbose) != 0) {
    char msg_error[1024];
    snprintf(msg_error, sizeof(msg_error), "[ERR] Can't load file \"%s\": %s", filepath, strerror(errno));
    PyErr_SetString(PyExc_FileNotFoundError, msg_error);
    return NULL;
  }
  build_matrix(&rd, matrix, verbose);

  npy_intp dims[] = {MSIZE, MSIZE};
  PyObject *result = PyArray_SimpleNew(2, dims, NPY_UINT32);

  if (PyErr_Occurred()) {
    return NULL;
  }

  uint32_t *result_data = PyArray_DATA((PyArrayObject *)result);

  /* for (size_t i=0; i<MSIZE; i++) */
  /*   for (size_t j=0; j<MSIZE; j++) */
  /*     result_data[i*MSIZE + j] = matrix[i][j]; */

  memcpy(result_data, matrix, MSIZE*MSIZE*sizeof(uint32_t));
  
  free(rd.data);
  return result;
}

static PyMethodDef methods[] = {
  { "signature_from_filepath_by2", signature_from_filepath_by2, METH_VARARGS, "Gets filepath and return filled numpy.2darray"}, 
  { NULL, NULL, 0, NULL },
};

static struct PyModuleDef file2mat = {
  PyModuleDef_HEAD_INIT,
  "file2mat",
  "Calculate file signature matrix by it's name.",
  -1,
  methods,
};

PyMODINIT_FUNC PyInit_file2mat() {
  import_array()
  return PyModule_Create(&file2mat);
}
