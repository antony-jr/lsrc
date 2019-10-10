#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cw.h>
#include <cstr.h>

struct _cw_t{
	char *start,
	     *line_start,
	     *end;
	cstr_t buffer;	
	int is_started;
};

cw_t *cw_new(const char *start, const char *lstart, const char *end){
	cw_t *r = calloc(1, sizeof *r);
	if(!r)
		return NULL;

	if(!(r->start = calloc(strlen(start)+1,sizeof *(r->start))) || 
	   !(r->line_start = calloc(strlen(lstart)+1,sizeof *(r->line_start))) ||
	   !(r->end = calloc(strlen(end)+1,sizeof *(r->end))) ||
	   !(r->buffer = cstr_new())){
		cw_free(r);
		return NULL;
	}
	strcpy(r->start,start);
	strcpy(r->line_start,lstart);
	strcpy(r->end,end);
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
	cstr_free(r->buffer);
	free(r);
}

int cw_start(cw_t *r){
	if(!r || r->is_started)
		return -1;
	r->is_started = 1;
	return cstr_append(r->buffer, r->start);
}

int cw_end(cw_t *r){
	if(!r || !r->is_started)
		return -1;
	return cstr_append(r->buffer, r->end);
}

int cw_write_line(cw_t *r, const char *line){
	if(!r || !r->is_started)
		return -1;
	if(cstr_append(r->buffer, r->line_start))
		return -1;
	if(cstr_append(r->buffer, line))
		return -1;

	if(line[strlen(line) - 1] != '\n'){
		if(cstr_append(r->buffer, "\n"))
			return -1;
	}
	return 0;
}

const char *cw_digest(cw_t *r){
	if(!r || !r->is_started)
		return NULL;
	return cstr_digest(r->buffer);
}
