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
#include <logger.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

int printl(log_class_t class, const char *fmt, ...) {
    va_list ap;
    time_t current_time;
    struct tm * time_info;
    char time_string[9];
    va_start(ap, NULL);
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
    fprintf(stderr,"[ %s ] ", time_string);
    if(class == info) {
        fprintf(stderr,"    \e[0;34mINFO\e[0m ");
    } else if(class == warning) {
        fprintf(stderr," \e[0;33mWARNING\e[0m ");
    } else {
        fprintf(stderr,"   \e[0;31mFATAL\e[0m ");
    }
    fprintf(stderr, " : ");
    vfprintf(stderr, fmt, ap);
    putc('.',stderr);
    putc('\n',stderr);
    va_end(ap);
    return 0;
}
