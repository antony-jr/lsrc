#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstr.h>
#include <cw.h>
#include <fwriter.h>
#include <logger.h>

struct _fwriter_t {
	FILE *license;
	int type;
	int is_replace;
	int inplace;
	cstr_t buffer;
	cstr_t file_path;
	cstr_t outfile_path;
};
#define FWRITER_SZ (sizeof(struct _fwriter_t))

static cstr_t get_line(FILE *fp){
	cstr_t r = cstr_new();
	int c = 0;
	
	if(feof(fp)){
		cstr_free(r);
		return NULL;
	}
	while((c = getc(fp)) != EOF){
		if(cstr_append_char(r, c)){
			cstr_free(r);
			return NULL;
		}
		if(c == '\n'){
			break;
		}
	}
	return r;
}

fwriter_t fwriter_new(const char *license_file_path,
		      const char *file_path,
		      int type,
		      int replace,
		      int inplace,
		      const char *output){
	fwriter_t r = calloc(1,FWRITER_SZ);
	if(r == NULL)
		return NULL;
	r->type = type;
	r->is_replace = replace;
	r->file_path = cstr_new_ex(file_path);
	r->outfile_path = (output == NULL) ? NULL: cstr_new_ex(output);
	r->inplace = inplace;
	if(!r->file_path){
		printl(fatal, "memory error");
		fwriter_free(r);
		return NULL;
	}
	r->license = fopen(license_file_path,"r");
	if(!r->license){
		printl(fatal, "cannot open license file for reading at %s", license_file_path);
		fwriter_free(r);
		return NULL;
	}
	return r;
}

void fwriter_free(fwriter_t obj){
	if(obj == NULL)
		return;
	if(obj->license)
		fclose(obj->license);
	cstr_free(obj->file_path);
	cstr_free(obj->buffer);
	cstr_free(obj->outfile_path);
	free(obj);
}

int fwriter_exec(fwriter_t obj){	
	cstr_t line = NULL;
	cstr_t path = NULL;
	int c = 0;
	FILE *file = NULL;
	FILE *fp = NULL;
	cw_t *writer = NULL;
	const char *start = NULL,
	           *end = NULL;
	if(obj == NULL)
		return -1;

	printl(info, "formating %s", cstr_digest(obj->file_path));

	if(obj->type == C_FILE){
		start = "/*\n";
		end = "*/\n";
		writer = cw_new("/*\n", " * ", "*/\n");
	}else if(obj->type == PYTHON_FILE){
		start = "# ";
		writer = cw_new(NULL, "# ", NULL);
	}

	if(writer == NULL){
		printl(fatal, "cannot construct comment writer");
		return -1;
	}

	if(!(file = fopen(cstr_digest(obj->file_path), "r"))){
		printl(fatal, "cannot open %s for reading", cstr_digest(obj->file_path));
		cw_free(writer);
		return -1;
	}

	printl(info, "warming license file");
	rewind(obj->license);

	if(obj->inplace){
	/* copy original file somewhere. */
	path = cstr_new_ex(cstr_digest(obj->file_path));
	cstr_append(path, ".orig");
	printl(info, "copying %s to %s for backup", cstr_digest(obj->file_path), cstr_digest(path));
	if(!(fp = fopen(cstr_digest(path), "w"))){
		printl(fatal, "copy failed");
		fclose(file);
		cw_free(writer);
		cstr_free(path);
		return -1;
	}	
	while((c = getc(file)) != EOF)
		putc(c, fp);

	printl(info, "copied successfully");	
	fclose(fp);
	fclose(file);
	
	printl(info, "opening copy of original file.");
	if(!(file = fopen(cstr_digest(path), "r"))){
		cw_free(writer);
		cstr_free(path);
		return -1;
	}
	cstr_free(path);
	}
	
	printl(info, "reading license file");
	cw_start(writer);
	while((line = get_line(obj->license)) != NULL){
		if(cw_write_line(writer, cstr_digest(line))){
			printl(fatal, "comment writer failed to write '%s', aborting", cstr_digest(line));
			cw_free(writer);
			cstr_free(path);
			cstr_free(line);
			return -1;
		}
		cstr_free(line);
	}
	cw_end(writer);
	printl(info, "successfully buffered license file to memory");


	if(obj->inplace){
		fp = fopen(cstr_digest(obj->file_path), "w");
	}else if(obj->outfile_path == NULL || strlen(cstr_digest(obj->outfile_path)) == 0){
		fp = stdout;
	}else{
		fp = fopen(cstr_digest(obj->outfile_path), "w");
		printl(info, "opening %s for writing", cstr_digest(obj->outfile_path));
	}

	if(!fp){
		printl(fatal, "cannot open output file");
		fclose(file);
		cw_free(writer);
		return -1;
	}

	/* first copy the copyright stuff from the writer. */
	fprintf(fp, "%s" , cw_digest(writer));

	c = 0; /* is continue */
	while((line = get_line(file)) != NULL){
		if(obj->is_replace){
			if(obj->type == C_FILE){
				if(!strcmp(cstr_digest(line), start)){
					c = 1;
					cstr_free(line);
					continue;
				}
				else if(!strcmp(cstr_digest(line), end)){
					c = 0;
					cstr_free(line);
					continue;
				}
			}
			
		}
		if(c){
			cstr_free(line);
			continue;
		}
		if(fprintf(fp, "%s", cstr_digest(line)) < 0){
			printl(fatal, "cannot write to output file, giving up");
			fclose(file);
			if(fp != stdout){ 
				fclose(fp);
			}
			cw_free(writer);
			cstr_free(line);
			return -1;
		}
		cstr_free(line);
	}
	fclose(file);
	if(fp != stdout){
		fclose(fp);
	}
	cw_free(writer);
	printl(info, "successfully finished writing");
	return 0;
}
