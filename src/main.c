#include <stdio.h>
#include <fwriter.h>
#include <cstr.h>
#include <cstr_list.h>
#include <logger.h>
#include <string.h>

#define True 1
#define False 0

static void print_header();
static void print_usage(const char*);

int main(int ac, char **av){
	int in_place = False;
	int is_replace = False;
	int type = C_FILE;
	size_t iter = 0;
	fwriter_t obj = NULL;
	cstr_t license_file_path = NULL;
	cstr_t output = NULL;
	cstr_t prog_name = cstr_new_ex(*av);
	cstr_list_t files = cstr_list_new_ex();

	/* Print version and other stuff. */
	print_header();
	if(ac == 1){
		print_usage(cstr_digest(prog_name));
		goto prog_end;
	}
	++av; /* go past the program name. */

	while(*av){
		if(strstr(*av, "-v") || strstr(*av, "--version")){
			goto prog_end;
		}else if(strstr(*av, "-t") || strstr(*av, "--type")){
			++av;
			if(!(*av)){
				printl(fatal, "expected a valid type, none given, aborting");
				print_usage(cstr_digest(prog_name));
				goto prog_end;
			}
			
			if(!strcmp(*av, "python")){
				type = PYTHON_FILE;
			}else{
				printl(warning, "invalid comment type given so asusming its a C file.");
				type = C_FILE;
			}
		}else if(strstr(*av, "-o") || strstr(*av, "--output")){
			++av;
			if(!(*av)){
				printl(fatal, "expected a valid output file path, none given, aborting");
				print_usage(cstr_digest(prog_name));
				goto prog_end;
			}
			output = cstr_new_ex(*av);	
		}else if(strstr(*av, "-r") || strstr(*av, "--replace")){
			is_replace = True;
		}else if(strstr(*av, "-i") || strstr(*av, "--in-place")){
			in_place = True;
		}else{
			/* we take the license path first which is not in any of our args options. */

			if(license_file_path == NULL){
				license_file_path = cstr_new_ex(*av);
			}else{
				/* append everything else as files to the list. */
				cstr_list_append(files, cstr_new_ex(*av), cstr_list_end());
			}
		}
		++av;
	}

	if(cstr_digest(license_file_path) == NULL){
		printl(fatal, "no license file path is given, giving up");
		print_usage(cstr_digest(prog_name));
		goto prog_end;
	}

	if(cstr_list_length(files) == 0){
		printl(fatal, "no files given, giving up");
		print_usage(cstr_digest(prog_name));
		goto prog_end;
	}

	while(iter < cstr_list_length(files)){
		obj = fwriter_new(cstr_digest(license_file_path),
				  cstr_digest(cstr_list_get(files, iter)),
				  type,
				  is_replace,
				  in_place,
				  cstr_digest(output));
		if(obj == NULL){
			printl(fatal, "cannot construct file writer");
			goto prog_end;
		}

		if(fwriter_exec(obj) < 0){
			printl(fatal, "file writer failed, giving up");
			fwriter_free(obj);
			goto prog_end;
		}
		
		fwriter_free(obj);
		++iter;
	}	

prog_end:
	/* free all resource, we don't need to worry about
	 * if they are null or not.*/	
	cstr_free(license_file_path);
	cstr_free(output);
	cstr_free(prog_name);
	cstr_list_free(files, True);
	return 0;
}

static void print_header(){
	printf("lsrc git-commit %s, built on %s\n",
#ifdef GIT_COMMIT_STR
           GIT_COMMIT_STR
#else
           "none"
#endif
           ,
           __TIMESTAMP__
          );
    printf("Copyright (C) %s, Antony Jr.\n", __DATE__ + 7);
    putchar('\n');

}

static void print_usage(const char *prog){
	printf(
	"Usage: %s [OPTIONS] [PATH TO LICENSE TEXT] [SOURCE FILE(S)]\n\n"
	"OPTIONS:\n"
	"    -t,--type      type of comment to use(default=c).\n"
	"    -o,--output    file to write the output.(default=stdout).\n"
	"    -i,--in-place  write the output in place.\n"
	"    -v,--version   show version.\n"
	"    -r,--replace   replace the first multiline comment if seen.\n\n"
	"EXAMPLE: \n"
	"    %s -t python -o commented.py -r LICENSE non-commented.py\n"
	"    %s -i LICENSE non-commented.c non-commented.h\n"
	, prog, prog, prog);
}
