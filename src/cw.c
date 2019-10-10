#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cw.h>
#define INITIAL_BUFFER_SIZE 1024 /* in bytes. */

struct _cw_t{
	char *start,
	     *line_start,
	     *end;
	char *buffer;
	size_t capacity;
	size_t pos;
	int is_started;
};



static int write_to_buffer(cw_t *r, const char *data){
	char *g = NULL;
	size_t pos = 0;
	if(!r)
		return -1;

	while(data[pos] != '\0'){
		if(r->capacity <= r->pos){
			g = realloc(r->buffer, sizeof *(r->buffer) * (r->capacity + INITIAL_BUFFER_SIZE));
			if(!g){
				return -1;
			}
			r->capacity += INITIAL_BUFFER_SIZE;
			r->buffer = g;
		}
		r->buffer[r->pos++] = data[pos++];
	}
	return 0;
}

cw_t *cw_new(const char *start, const char *lstart, const char *end){
	cw_t *r = calloc(1, sizeof *r);
	if(!r)
		return NULL;

	if(!(r->start = calloc(strlen(start)+1,sizeof *(r->start))) || 
	   !(r->line_start = calloc(strlen(lstart)+1,sizeof *(r->line_start))) ||
	   !(r->end = calloc(strlen(end)+1,sizeof *(r->end)))){
		cw_free(r);
		return NULL;
	}
	strcpy(r->start,start);
	strcpy(r->line_start,lstart);
	strcpy(r->end,end);

	r->buffer = calloc(INITIAL_BUFFER_SIZE, sizeof *(r->buffer));
	if(!r->buffer){
		cw_free(r);
		return NULL;
	}
	r->capacity = INITIAL_BUFFER_SIZE;
	return r;
}

void cw_free(cw_t *r){
	if(!r)
		return;
	if(r->start)
		free(r->start);
	if(r->line_start)
		free(r->line_start);
	if(r->end)
		free(r->end);
	if(r->buffer)
		free(r->buffer);
	free(r);
}

int cw_start(cw_t *r){
	if(!r || r->is_started)
		return -1;
	r->is_started = 1;
	return write_to_buffer(r, r->start);
}

int cw_end(cw_t *r){
	if(!r || !r->is_started)
		return -1;
	return write_to_buffer(r, r->end);
}

int cw_write_char(cw_t *r, char c){
	if(!r || !r->is_started)
		return -1;
}

int cw_write_line(cw_t *r, const char *line){
	if(!r || !r->is_started)
		return -1;
	if(write_to_buffer(r, r->line_start))
		return -1;
	if(write_to_buffer(r, line))
		return -1;

	if(line[strlen(line) - 1] != '\n'){
		if(write_to_buffer(r, "\n"))
			return -1;
	}
	return 0;
}

char *cw_get_string(cw_t *r){
	if(!r || !r->is_started){
		return NULL;
	}
	return r->buffer;
}
