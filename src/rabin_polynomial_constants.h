/*
 * rabin_polynomial_constants.h
 *
 * Created by Joel Lawrence Tucci on 09-May-2011.
 *
 * Copyright (c) 2011 Joel Lawrence Tucci
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

//List of constants, mostly constraints and defaults for various parameters
//to the Rabin Fingerprinting algorithm

#ifndef RAB_MAX_BLOCK_SIZE
#define RAB_MAX_BLOCK_SIZE (64 * 1024)
#endif

#ifndef RAB_MIN_BLOCK_SIZE
#define RAB_MIN_BLOCK_SIZE (2 * 1024)
#endif

#ifndef RAB_POLYNOMIAL_REM
#define RAB_POLYNOMIAL_REM 3
#endif

#ifndef RAB_POLYNOMIAL_AVG_BLOCK_SIZE
#define RAB_POLYNOMIAL_AVG_BLOCK_SIZE  (8 * 1024)
#endif

#ifndef RAB_POLYNOMIAL_WIN_SIZE
#define RAB_POLYNOMIAL_WIN_SIZE 48
#endif

#ifndef RAB_POLYNOMIAL_MIN_WIN_SIZE
#define RAB_POLYNOMIAL_MIN_WIN_SIZE 17
#endif

#ifndef RAB_POLYNOMIAL_MAX_WIN_SIZE
#define RAB_POLYNOMIAL_MAX_WIN_SIZE 63
#endif

//# of bytes to read at a time when reading through files
#ifndef RAB_FILE_READ_BUF_SIZE
#define RAB_FILE_READ_BUF_SIZE 1048576
#endif
