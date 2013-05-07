#if defined(_WIN32) || defined(__NUTC__)
#  pragma optimize("", off)
#endif

//  lgetopt.cxx --
//  synthesized getopt function - not available in win
//
#define LGETOPT_EXT
#include "lgetopt.h"
#include <string.h>

int 
getopt(int argc, char **argv, char *argf)
{
  char *opt_ptr = NULL;
  int cc;

  if (optind < argc && argv[optind][0] == '-') {
    for (opt_ptr = argf; (cc = *opt_ptr) != 0; opt_ptr++) {
      if (cc == ':') 
        continue;
      if ((optarg = strchr(argv[optind], cc)) != NULL) {
        optind++;
        if (*++opt_ptr == ':')
        optarg = argv[optind++];

        return(cc);
      }
    }
  }
  return -1;
}

#if defined(_WIN32) || defined(__NUTC__)
#  pragma optimize("", on)
#endif
