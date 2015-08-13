
#include <stdlib.h>
#include <string.h>

static char* extract_arg(char* flag, int n, int argc, char** argv) {
  char* res = NULL;
  unsigned i = 0;

  while (i < argc) {
    if (!strncmp(flag, argv[i], n)) {
      if (argc <= i+1) exit(1);
      res = argv[i+1];
      break;
    }
    ++i;
  }
  return res;
}

unsigned __arg_length(int argc, char** argv) {
  unsigned length = 1;

  char* l_str = extract_arg("-L", 2, argc, argv);
  length = (l_str == NULL) ? 1 : atoi(l_str);
  return length;
}

unsigned __arg_rep(int argc, char** argv) {
  unsigned rep = 1;

  char* r_str = extract_arg("-R", 2, argc, argv);
  rep = (r_str == NULL) ? 1 : atoi(r_str);
  return rep;
}
