#include <stdio.h>
#include <cw.h>

int main(int ac, char **av){
	if(ac == 1){
		printf("Usage: %s [PATH TO LICENSE TEXT]\n", av[0]);
		return 0;
	}
	cw_t *cw = cw_new("/*\n", " * ", "*/\n");
	FILE *fp = fopen(av[1], "r");
	if(!fp){
		printf("cannot open license text!\n");
		cw_free(cw);
		return -1;
	}

	cw_start(cw);

	cw_end(cw);
	printf("%s", cw_get_string(cw));
	cw_free(cw);
	return 0;
}
