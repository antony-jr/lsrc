/*
 * MIT License
 * 
 * Copyright (c) 2019 Antony jr
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#ifndef CSTR_LIST_H_INCLUDED
#define CSTR_LIST_H_INCLUDED
#include <cstr.h>
#include <stdlib.h>

struct _cstr_list_t;
typedef struct _cstr_list_t* cstr_list_t;

void *cstr_list_end();

cstr_list_t cstr_list_new_ex();
cstr_list_t cstr_list_new(cstr_t,...);
void cstr_list_free(cstr_list_t, /*free resources for listed cstrs=*/int);

size_t cstr_list_length(cstr_list_t);
cstr_t cstr_list_get(cstr_list_t, size_t);
int cstr_list_set(cstr_list_t, size_t, cstr_t);

int cstr_list_append(cstr_list_t, ...);


#endif /* CSTR_LIST_H_INCLUDED */
