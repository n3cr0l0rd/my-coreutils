#include "./wrapped_io.h"
#include "./stdlibs.h"
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>

/*TODO: add -r and -f opts*/

#define PROGRAM_NAME      "cp"
#define MAX_INPUT_STR_LEN 8192
#define MAX_DST_FN_LEN    8192
#define MAX_SRC_LINE_LEN  8192
#define LAST_ARG          argv[argc - 1]

#define _COPY(s_path, d_path)                        \
({                                                   \
  int64_t s_file, d_file;                            \
  char s_file_str[MAX_INPUT_STR_LEN] = {0};          \
                                                     \
  s_file = _OPEN(s_path, O_RDONLY, false);           \
  d_file = _OPEN(d_path, O_CREAT | O_WRONLY, false); \
                                                     \
  _READ(s_file, s_file_str, MAX_INPUT_STR_LEN);      \
  while(*s_file_str) {                               \
    _WRITE(d_file, s_file_str);                      \
    memset(s_file_str, 0, MAX_INPUT_STR_LEN);        \
    _READ(s_file, s_file_str, MAX_INPUT_STR_LEN);    \
  }                                                  \
  close(s_file);                                     \
  close(d_file);                                     \
})                                                   \

#define TRY_HELP                                     \
  printf("Try '%s --help' for more information.\n",  \
          PROGRAM_NAME);                             \

#define DELETE_SLASH(s)                              \
  if(s[strlen(s) - 1] == '/') {                      \
    s[strlen(s) - 1] = '\0';                         \
  }                                                  \

int main(int argc, char **argv)
{
  size_t i = 0;
  struct stat stat_buf;
  char dst_file_name[MAX_DST_FN_LEN] = {0};

  if(argc < 2) {
    fprintf(stderr, "%s: missing file operand\n", PROGRAM_NAME);
    TRY_HELP;
  }

  if(argc == 2) {
    fprintf(stderr, "%s: missing destination file operand after '%s'\n",
            PROGRAM_NAME, argv[1]);
    TRY_HELP;
  }

  stat(LAST_ARG, &stat_buf);
  if(S_ISDIR(stat_buf.st_mode)) {
    DELETE_SLASH(LAST_ARG);
    for(i = 1; i < argc - 1; i++) {
      sprintf(dst_file_name, "%s/%s", argv[argc - 1], argv[i]); 
      _COPY(argv[i], dst_file_name);
      memset(dst_file_name, 0, MAX_DST_FN_LEN);
    }
  }
  else if (argc == 3) {
    _COPY(argv[1], argv[2]);
  }
  else {
    fprintf(stderr, "%s: target '%s': Not a directory\n",
            PROGRAM_NAME, argv[argc - 1]); 
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
