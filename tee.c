#include "./wrapped_io.h"
#include "./stdlibs.h"
#include <stdbool.h>
#include <getopt.h>

/*TODO: add -i and -p options*/

#define PROGRAM_NAME      "tee"
#define MAX_FILENAME_LEN  255
#define MAX_INPUT_STR_LEN 8192

typedef struct {
	uint64_t descr;
	char name[MAX_FILENAME_LEN];
} output_file;

#define USAGE(s)                                                    \
	puts("Usage:");                                                   \
	printf("%s [OPTIONS] [FILE]\n\n", PROGRAM_NAME);                  \
	puts("-a, --append\tappend to the given FILE, do not overwrite"); \
	puts("-h, --help\tprint this menu and exit");                     \
	exit(s);                                                      

#define GET_STDIN_STR(b)                                            \
	memset(b, 0, MAX_INPUT_STR_LEN);                                  \
  _READ(STDIN_FILENO, b, MAX_INPUT_STR_LEN)                         \

#define IS_APPEND_OPT(opt)                                          \
  strncmp(opt, "-a", strlen("-a") != 0) &&                          \
  strncmp(opt, "--append", strlen("--append") != 0)                 \

int main(int argc, char **argv)
{
	bool append = false;
	size_t output_files_n;
	int8_t option, options_n = 0;
	const char *SHORT_OPTIONS = "ha";
	char *input_buf = (char*)calloc(MAX_INPUT_STR_LEN, sizeof(char*));
	char *output_filename = (char*)calloc(MAX_FILENAME_LEN, sizeof(char*));
	const struct option LONG_OPTIONS[] = {
		{"help",    no_argument,  NULL, 'h'},
		{"append",  no_argument,  NULL, 'a'},
		{NULL,      0,            NULL,  0}
	};

	opterr = 0;
	while((option = getopt_long(argc, argv, SHORT_OPTIONS, LONG_OPTIONS, NULL)) != -1) {
		switch(option) {
			case 'a':
				append = true;
				break;
			case 'h':
				USAGE(EXIT_SUCCESS);
				break;
			default:
				USAGE(EXIT_FAILURE);
				break;
		}
		options_n++;
	}

	output_files_n = argc - options_n;
	output_file output_f[output_files_n];
	output_f[0].descr = STDOUT_FILENO;

  /*opening all given files*/
	for(size_t i = 1, j = 1; i < output_files_n; j++, i++) {
			memset(output_f[i].name, 0, MAX_FILENAME_LEN);
			if(IS_APPEND_OPT(argv[j])) {
				strncpy(output_f[i].name, argv[j], strlen(argv[j]));
				output_f[i].descr = _OPEN(output_f[i].name, (append ? O_APPEND : 0) | O_WRONLY | O_CREAT, 1);
			}
			else i--;
	}

	GET_STDIN_STR(input_buf)
	while(*input_buf) { 
		for(size_t i = 0; i < output_files_n; i++) {
			_WRITE(output_f[i].descr, input_buf);
		}
		GET_STDIN_STR(input_buf);
	}

	for(size_t i = 1; i < output_files_n && output_f[i].descr; i++) {
		_CLOSE(output_f[i].descr);
	}

	free(input_buf);
	free(output_filename);
	return EXIT_SUCCESS;
}

