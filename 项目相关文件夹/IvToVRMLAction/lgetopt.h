#ifndef LGETOPT_H
#define LGETOPT_H

#ifdef LGETOPT_EXT
#define EXTERN
int optind = 1;

#else
#define EXTERN extern
EXTERN int optind;

#endif

EXTERN char *optarg;


int getopt(int argc, char **argv, char *argf);

#endif
