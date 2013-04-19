/**
 * @file   pyrabin.c
 * @author Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 *
 * Copyright (C) 2013 -  Wei-Ning Huang (AZ) <aitjcize@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <Python.h>

#include "rabin_polynomial.h"

static PyObject* RabinError;

extern uint64_t rabin_polynomial_prime;
extern unsigned int rabin_sliding_window_size;
extern unsigned int rabin_polynomial_max_block_size;
extern unsigned int rabin_polynomial_min_block_size;
extern unsigned int rabin_polynomial_average_block_size;

static PyObject* get_file_fingerprints(PyObject* self, PyObject* args,
    PyObject *keywds) {
  const char *filename;
  static char *kwlist[] = {"filename", "prime", "window_size", "max_block_size",
    "min_block_size", "avg_block_size", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s|KIIII", kwlist,
     &filename, &rabin_polynomial_prime, &rabin_sliding_window_size,
     &rabin_polynomial_max_block_size, &rabin_polynomial_min_block_size,
     &rabin_polynomial_average_block_size)) {
    return NULL;
  }

  FILE* fp = fopen(filename, "r");
  if (!fp) {
    return PyErr_SetFromErrnoWithFilename(RabinError, filename);
  }

  PyObject* list = NULL;
  PyObject* tuple = NULL;
  PyObject* node = NULL;

  if (!(list = PyList_New(0))) {
    return NULL;
  }

  struct rabin_polynomial* head = get_file_rabin_polys(fp);
  struct rabin_polynomial* curr = head;

  if (curr == NULL) {
    PyErr_SetString(RabinError, "get_file_rabin_polys()");
    return NULL;
  } 

  while (curr) {
    if (!(tuple = PyTuple_New(3))) {
      return NULL;
    }
    node = Py_BuildValue("K", curr->start);
    PyTuple_SetItem(tuple, 0, node);
    node = Py_BuildValue("K", curr->length);
    PyTuple_SetItem(tuple, 1, node);
    node = Py_BuildValue("K", curr->polynomial);
    PyTuple_SetItem(tuple, 2, node);
    PyList_Append(list, tuple);
    Py_DECREF(tuple);
    curr = curr->next_polynomial;
  }

  free_rabin_fingerprint_list(head);

  return list;
}

static PyMethodDef PyRabinMethods[] = {
  {"get_file_fingerprints", get_file_fingerprints,
    METH_VARARGS | METH_KEYWORDS, "Get Rabin fingerprint of file"},
  {NULL, NULL, 0, NULL}     
};


PyMODINIT_FUNC initrabin(void) {
  (void) Py_InitModule("rabin", PyRabinMethods);
  RabinError = PyErr_NewException("rabin.error", NULL, NULL);
}
