#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <cstr.h>
#include <cstr_list.h>

#define True 1
#define False 0

static char *end_mark_ptr = ""; /* we don't need the data but the address. */
static int cstr_list_append_ex(cstr_list_t,va_list);

struct _cstr_list_t {
	cstr_t *list;
	size_t capacity,length;

};

#define CSTR_LIST_SZ (sizeof(struct _cstr_list_t))
#define BUFFER_CAP 10

void *cstr_list_end(){
	return end_mark_ptr; 
}

cstr_list_t cstr_list_new_ex(){
	cstr_list_t r = calloc(1, CSTR_LIST_SZ);
	size_t iter = 0;
	va_list ap;
	if(r == NULL)
		return NULL;	
	r->list = calloc(BUFFER_CAP, sizeof *(r->list));
	if(!r->list){
		cstr_list_free(r, False);
	}
	r->capacity = BUFFER_CAP;
	return r;
}

cstr_list_t cstr_list_new(cstr_t first, ...){
	va_list ap;
	cstr_t arg = NULL;
	cstr_list_t r = cstr_list_new_ex();
	if(r == NULL || first == (cstr_t)end_mark_ptr || first == NULL){
		va_end(ap);
		return (r == NULL) ? NULL : r;
	}	
	if(cstr_list_append(r, first, end_mark_ptr)){
		va_end(ap);
		cstr_list_free(r, True);
		return NULL;
	}
	va_start(ap, NULL);
	if(cstr_list_append_ex(r, ap)){
		va_end(ap);
		cstr_list_free(r, True);
		return NULL;	
	}
	va_end(ap);
	return r;
}

void cstr_list_free(cstr_list_t obj, int free_res){
	size_t iter = 0;
	if(obj == NULL)
		return;

	if(free_res){
		while(iter < obj->length){
			cstr_free(*(obj->list + iter));
			++iter;
		}
	}
	if(obj->list)
		free(obj->list);
	free(obj);
}

size_t cstr_list_length(cstr_list_t obj){
	return (obj == NULL) ? 0 : obj->length;
}

cstr_t cstr_list_get(cstr_list_t obj, size_t index){
	if(obj == NULL || index > obj->length)
		return NULL;
	return *(obj->list + index);
}

int cstr_list_set(cstr_list_t obj, size_t index, cstr_t str){
	cstr_t *guard = NULL;
	if(obj == NULL)
		return -1;

	if(index >= obj->capacity){ /* have to reallocate. */
		guard = realloc(obj->list, (sizeof(*guard)) * (obj->capacity + index + BUFFER_CAP));
		memset(obj->list + obj->capacity, 0, sizeof(*guard) * (index + BUFFER_CAP));
		if(!guard){
			return -1;
		}
		obj->capacity += index + BUFFER_CAP;	
		obj->list = guard;
	}
	obj->list[index] = str;
	return 0;
}

static int cstr_list_append_ex(cstr_list_t obj, va_list ap){
	cstr_t arg = NULL;
	if(obj == NULL)
		return -1;
	while(True){
		arg = va_arg(ap, cstr_t);
		if(arg == (cstr_t)end_mark_ptr){
			break;
		}
		if(cstr_list_set(obj, obj->length++, arg)){
			return -1;
		}
	}
	return 0;
}

int cstr_list_append(cstr_list_t obj, ...){
	va_list ap;
	cstr_t arg = NULL;
	if(obj == NULL){
		return -1;
	}

	va_start(ap, NULL);
	cstr_list_append_ex(obj, ap);	
	va_end(ap);
	return 0;
}
