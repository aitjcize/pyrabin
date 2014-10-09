/**
 * @file   utils.c
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

#include "utils.h"

PyObject* rabin_polynomial_to_PyList(struct rabin_polynomial* head)
{
  PyObject* list = NULL;
  PyObject* tuple = NULL;

  if (!(list = PyList_New(0))) {
    return NULL;
  }

  struct rabin_polynomial* curr = head;
  while (curr) {
    if (curr->length == 0) {
      curr = curr->next_polynomial;
      continue;
    }
    tuple = Py_BuildValue("KiK", curr->start, curr->length,
        curr->polynomial);
    PyList_Append(list, tuple);
    Py_DECREF(tuple);

    curr = curr->next_polynomial;
  }

  return list;
}
