#include <stdio.h>
#include <fwriter.h>
#include <cstr.h>
#include <string.h>

int main(int ac, char **av){
	fwriter_t obj = NULL;
	cstr_t license_file_path;
	printf("License Source v0.0.1-alpha, A Simple and lightweight source licensor.\n"
	       "Copyright (C) 2019, Antony Jr.\n");
	if(ac < 3){
		printf("Usage: %s [PATH TO LICENSE TEXT] [PATH(S) TO SOURCE FILE(S) TO WRITE LICENSE]\n", av[0]);
		return 0;
	}

	license_file_path = cstr_new_ex(av[1]);
	av += 2;

	printf("warning: the top multi-line comment in any c source file will be removed.\n");
	while(*av){
		/* guess file type.
		 * TODO: better way to resolve this?
		*/
		printf("info: formatting %s\n", *av);
		if(strstr(*av, ".c") || strstr(*av, ".cc") ||
		   strstr(*av, ".cpp") || strstr(*av, ".cxx") ||
		   strstr(*av, ".h") || strstr(*av, ".hpp") ||
		   strstr(*av, ".hxx")){
			obj = fwriter_new(cstr_digest(license_file_path), *av, C_FILE,1 );
		}else if(strstr(*av, ".py")){
			obj = fwriter_new(cstr_digest(license_file_path), *av, PYTHON_FILE,1);
		}

		if(obj == NULL){
			printf("fatal error: cannot start the writer\n");
			cstr_free(license_file_path);
			return -1;
		}
		if(fwriter_exec(obj)){
			printf("fatal error: write failed for %s\n" , *av);
			fwriter_free(obj);
			cstr_free(license_file_path);
			return -1;
		}
		printf("info: original file moved to %s.orig\n", *av);
		printf("info: formated %s in place\n", *av);

		fwriter_free(obj);
		++av;
	}
	printf("Everything done!\n");
	cstr_free(license_file_path);
	return 0;
}
