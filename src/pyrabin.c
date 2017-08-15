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
#include <openssl/sha.h>

#include "utils.h"
#include "rabin_polynomial.h"

#define READ_BUF_SIZE 1048576

struct module_state {
    PyObject *RabinError;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

extern PyTypeObject RabinType;

extern uint64_t rabin_polynomial_prime;
extern unsigned int rabin_sliding_window_size;
extern unsigned int rabin_polynomial_max_block_size;
extern unsigned int rabin_polynomial_min_block_size;
extern unsigned int rabin_polynomial_average_block_size;

static PyObject* set_prime(PyObject* self, PyObject* args)
{
  if (!PyArg_ParseTuple(args, "K", &rabin_polynomial_prime)) {
    return NULL;
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* set_window_size(PyObject* self, PyObject* args)
{
  if (!PyArg_ParseTuple(args, "I", &rabin_sliding_window_size)) {
    return NULL;
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* set_max_block_size(PyObject* self, PyObject* args)
{
  if (!PyArg_ParseTuple(args, "I", &rabin_polynomial_max_block_size)) {
    return NULL;
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* set_min_block_size(PyObject* self, PyObject* args)
{
  if (!PyArg_ParseTuple(args, "I", &rabin_polynomial_min_block_size)) {
    return NULL;
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* set_average_block_size(PyObject* self, PyObject* args)
{
  unsigned int prev;
  if (!PyArg_ParseTuple(args, "I", &prev)) {
    return NULL;
  }
  if (prev < rabin_polynomial_min_block_size ||
      prev > rabin_polynomial_max_block_size) {
    struct module_state *st = GETSTATE(self);
    PyErr_SetString(st->RabinError,
        "average block size should between min and max block size");
    return NULL;
  }
  rabin_polynomial_average_block_size = prev;

  Py_INCREF(Py_None);
  return Py_None;
}

static void to_hex_digest(char* digest, char* hex_digest)
{
  int i, j;
  for(i = j = 0; i < SHA_DIGEST_LENGTH; ++i) {
    char c;
    c = (digest[i] >> 4) & 0xf;
    c = (c > 9) ? c + 'a'- 10 : c + '0';
    hex_digest[j++] = c;
    c = (digest[i] & 0xf);
    c = (c > 9) ? c + 'a' - 10 : c + '0';
    hex_digest[j++] = c;
  }
  hex_digest[2 * SHA_DIGEST_LENGTH] = 0;
}

static PyObject* get_file_fingerprints(PyObject* self, PyObject* args)
{
  const char *filename;

  if (!PyArg_ParseTuple(args, "s", &filename)) {
    return NULL;
  }

  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    struct module_state *st = GETSTATE(self);
    return PyErr_SetFromErrnoWithFilename(st->RabinError, filename);
  }

  struct rabin_polynomial* head = get_file_rabin_polys(fp);
  if (head == NULL) {
    return PyList_New(0);
  }
  fclose(fp);

  PyObject* list = rabin_polynomial_to_PyList(head);
  free_rabin_fingerprint_list(head);

  return list;
}

static PyObject* split_file_by_fingerprints(PyObject* self, PyObject* args)
{
  struct module_state *st = GETSTATE(self);
  const char *filename;
  if (!PyArg_ParseTuple(args, "s", &filename)) {
    return NULL;
  }

  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    return PyErr_SetFromErrnoWithFilename(st->RabinError, filename);
  }

  struct rabin_polynomial* head = get_file_rabin_polys(fp);
  if (head == NULL) {
    PyErr_SetString(st->RabinError, "get_file_rabin_polys()");
    return NULL;
  }

  struct rabin_polynomial* curr = head;
  char outfile[BUFSIZ];
  char* buffer = malloc(READ_BUF_SIZE);
  uint64_t offset = 0;
  size_t read_size = 0;
  char digest[SHA_DIGEST_LENGTH];
  char hex_digest[SHA_DIGEST_LENGTH * 2 + 5];
  SHA_CTX ctx;

  PyObject* list = NULL;
  PyObject* tuple = NULL;

  if (!(list = PyList_New(0))) {
    return NULL;
  }

  while (curr) {
    if ((offset = fseek(fp, curr->start, SEEK_SET) == -1)) {
      fclose(fp);
      return PyErr_SetFromErrnoWithFilename(st->RabinError, filename);
    }

    /* Save chunk to temporarily file */
    snprintf(outfile, BUFSIZ, ".%llx.tmp", (unsigned long long) curr->polynomial);
    SHA1_Init(&ctx);
    FILE* ofp = fopen(outfile, "wb");
    if (!ofp) {
      fclose(fp);
      return PyErr_SetFromErrnoWithFilename(st->RabinError, filename);
    }

    size_t remain_read = curr->length;
    size_t bytes_read = 0;

    do {
      read_size = (curr->length < READ_BUF_SIZE)? curr->length: READ_BUF_SIZE;
      bytes_read = fread(buffer, 1, read_size, fp);
      SHA1_Update(&ctx, buffer, bytes_read);

      size_t total_written = 0;
      size_t bytes_written = 0;
      do {
        bytes_written = fwrite(buffer, 1, read_size, ofp);
        if (bytes_written == 0 && ferror(ofp)) {
          fclose(fp);
          fclose(ofp);
          return PyErr_SetFromErrnoWithFilename(st->RabinError, outfile);
        }
        total_written += bytes_written;
      } while (total_written != bytes_read);
      remain_read -= bytes_read;
    } while (remain_read > 0);
    fclose(ofp);

    /* Rename chunk using SHA-1 sum as filename */
    SHA1_Final((unsigned char *) digest, &ctx);
    to_hex_digest(digest, hex_digest);
    strncat(hex_digest, ".blk", SHA_DIGEST_LENGTH * 2 + 5);
    rename(outfile, hex_digest);

    tuple = Py_BuildValue("KiKs", curr->start, curr->length, 
        curr->polynomial, hex_digest);
    /* PyObject_Print(tuple, stdout, 0); printf("\n"); */
    PyList_Append(list, tuple);
    Py_DECREF(tuple);

    curr = curr->next_polynomial;
  }

  free_rabin_fingerprint_list(head);
  fclose(fp);

  return list;
}

static PyMethodDef PyRabinMethods[] = {
  {"set_prime", (PyCFunction)set_prime,
    METH_VARARGS, "Set Rabin polynomial prime"},
  {"set_window_size", (PyCFunction)set_window_size,
    METH_VARARGS, "Set Rabin polynomial sliding window size"},
  {"set_max_block_size", (PyCFunction)set_max_block_size,
    METH_VARARGS, "Set Rabin polynomial max block size"},
  {"set_min_block_size", (PyCFunction)set_min_block_size,
    METH_VARARGS, "Set Rabin polynomial min block size"},
  {"set_average_block_size", (PyCFunction)set_average_block_size,
    METH_VARARGS, "Set Rabin polynomial average block size"},
  {"get_file_fingerprints", (PyCFunction)get_file_fingerprints,
    METH_VARARGS, "Get Rabin fingerprint of a file"},
  {"get_file_fingerprints", (PyCFunction)get_file_fingerprints,
    METH_VARARGS, "Get Rabin fingerprint of a file"},
  {"split_file_by_fingerprints", (PyCFunction)split_file_by_fingerprints,
    METH_VARARGS, "Split a file by fingerprints"},
  {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3

static int rabin_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->RabinError);
    return 0;
}

static int rabin_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->RabinError);
    return 0;
}


static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "rabin",
        NULL,
        sizeof(struct module_state),
        PyRabinMethods,
        NULL,
        rabin_traverse,
        rabin_clear,
        NULL
};

#define INITERROR return NULL

PyMODINIT_FUNC
PyInit_rabin(void)

#else
#define INITERROR return

void
initrabin(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
  PyObject *module = PyModule_Create(&moduledef);
#else
  PyObject *module = Py_InitModule("rabin", PyRabinMethods);
#endif
  if (module == NULL)
    INITERROR;

  struct module_state *st = GETSTATE(module);

  // Initialize defaults
  initialize_rabin_polynomial_defaults();

  // Initialize rabin.Rabin
  if (PyType_Ready(&RabinType) < 0) {
    fprintf(stderr, "Invalid PyTypeObject `RabinType'\n");
    INITERROR;
  }

  Py_INCREF(&RabinType);
  PyModule_AddObject(module, "Rabin", (PyObject*)&RabinType);

  // Initialize RabinError
  st->RabinError = PyErr_NewException("rabin.error", NULL, NULL);
  if (st->RabinError == NULL) {
    Py_DECREF(module);
    INITERROR;
  }

#if PY_MAJOR_VERSION >= 3
  return module;
#endif
}
