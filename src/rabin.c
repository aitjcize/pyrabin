/**
 * @file   rabin.c
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

#include <string.h>
#include <Python.h>

#include "rabin_polynomial.h"
#include "utils.h"

/* rabin.Rabin definition */
typedef struct {
  PyObject_HEAD
  struct rab_block_info* block;
} Rabin;

static int Rabin_init(Rabin* self, PyObject* args, PyObject* kwds)
{
  self->block = NULL;
  return 0;
}

static PyObject*
Rabin_update(Rabin* self, PyObject* args)
{
  const char* src_data;
  int src_len;

  if (!PyArg_ParseTuple(args, "s#", &src_data, &src_len)) {
    return NULL;
  }
  self->block = read_rabin_block((void*)src_data, src_len, self->block);

  return Py_None;
}

static PyObject*
Rabin_clear(Rabin* self)
{
  if (self->block != NULL) {
    free(self->block);
    free_rabin_fingerprint_list(self->block->head);
    self->block = NULL;
  }
  return Py_None;
}

static PyObject*
Rabin_fingerprints(Rabin* self)
{
  return rabin_polynomial_to_PyList(self->block->head);
}

static PyMethodDef Rabin_methods[] = {
  {"update", (PyCFunction)Rabin_update, METH_VARARGS,
    "Update Rabin fingerprint list by adding a block of data"
  },
  {"clear", (PyCFunction)Rabin_clear, METH_NOARGS,
    "Clear fingerprints and reset status"
  },
  {"fingerprints", (PyCFunction)Rabin_fingerprints, METH_NOARGS,
    "Return fingerprints of all data passed by the update method"
  },
  {NULL}  /* Sentinel */
};

PyTypeObject RabinType = {
  PyObject_HEAD_INIT(NULL)
  0,                         /*ob_size*/
  "rabin.Rabin",             /*tp_name*/
  sizeof(Rabin),             /*tp_basicsize*/
  0,                         /*tp_itemsize*/
  0,                         /*tp_dealloc*/
  0,                         /*tp_print*/
  0,                         /*tp_getattr*/
  0,                         /*tp_setattr*/
  0,                         /*tp_compare*/
  0,                         /*tp_repr*/
  0,                         /*tp_as_number*/
  0,                         /*tp_as_sequence*/
  0,                         /*tp_as_mapping*/
  0,                         /*tp_hash */
  0,                         /*tp_call*/
  0,                         /*tp_str*/
  0,                         /*tp_getattro*/
  0,                         /*tp_setattro*/
  0,                         /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
  "Rabin object",            /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  Rabin_methods,             /* tp_methods */
  0,                         /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)Rabin_init,      /* tp_init */
  0,                         /* tp_alloc */
  PyType_GenericNew,         /* tp_new */
};
