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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cw.h>
#include <cstr.h>

struct _cw_t {
    char *start,
         *line_start,
         *end;
    cstr_t buffer;
    int is_started;
};

cw_t *cw_new(const char *start, const char *lstart, const char *end) {
    cw_t *r = calloc(1, sizeof *r);
    if(!r)
        return NULL;

    if(!(r->start = calloc(strlen(start)+1,sizeof *(r->start))) ||
            !(r->line_start = calloc(strlen(lstart)+1,sizeof *(r->line_start))) ||
            !(r->end = calloc(strlen(end)+1,sizeof *(r->end))) ||
            !(r->buffer = cstr_new())) {
        cw_free(r);
        return NULL;
    }
    strcpy(r->start,start);
    strcpy(r->line_start,lstart);
    strcpy(r->end,end);
    return r;
}

void cw_free(cw_t *r) {
    if(!r)
        return;
    if(r->start)
        free(r->start);
    if(r->line_start)
        free(r->line_start);
    if(r->end)
        free(r->end);
    cstr_free(r->buffer);
    free(r);
}

int cw_start(cw_t *r) {
    if(!r || r->is_started)
        return -1;
    r->is_started = 1;
    return cstr_append(r->buffer, r->start);
}

int cw_end(cw_t *r) {
    if(!r || !r->is_started)
        return -1;
    return cstr_append(r->buffer, r->end);
}

int cw_write_line(cw_t *r, const char *line) {
    if(!r || !r->is_started)
        return -1;
    if(*line == '\0')
        return 0;
    if(cstr_append(r->buffer, r->line_start))
        return -1;
    if(cstr_append(r->buffer, line))
        return -1;

    if(line[strlen(line) - 1] != '\n') {
        if(cstr_append(r->buffer, "\n"))
            return -1;
    }
    return 0;
}

const char *cw_digest(cw_t *r) {
    if(!r || !r->is_started)
        return NULL;
    return cstr_digest(r->buffer);
}
