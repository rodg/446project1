
/*
 * 
 * main.c
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include "fu.h"
#include "pipeline.h"
#include "output.h"

const char usage[] =
"usage: %s <options>\n"
"\toptions:\n"
"\t-b"    "\t<binary file>\n"
"\t-o"    "\t<functional unit options file>\n";

void parse_args(int, char **);

static char *bin_file_name = NULL;
static char *fu_file_name = NULL;
static FILE *bin_file,*fu_file;
static int wbpi = -1;
static int wbpf = -1;


/************************************************************/
int
main(int argc, char *argv[]) {
  state_t *state;
  int data_count;
  int num_insn, i;

  parse_args(argc, argv);
  state = state_create(&data_count, bin_file, fu_file);

  if (state == NULL) {
    fclose(bin_file);
    fclose(fu_file);
    return -1;
  }

  fclose(bin_file);
  fclose(fu_file);

  /* main sim loop */
  for (i = 0, num_insn = 0; TRUE; i++) {

    printf("\n\n*** CYCLE %d\n", i);
    print_state(state, data_count);

    writeback(state, &num_insn);
    execute(state);
    if (!(state->fetch_lock)) {
      decode(state);
      fetch(state);
    }
  }

  printf("SIMULATION COMPLETE!\n");
  printf("EXECUTED %d INSTRUCTIONS IN %d CYCLES\n", num_insn, i);
  printf("CPI:  %.2f\n", (float)i / (float)num_insn);

  return 0;
}
/************************************************************/


void
parse_args(int argc, char **argv)
{
  int i;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i],"-b") == 0) {
      if (bin_file_name == NULL && i + 1 < argc) {
	bin_file_name = argv[++i];
      } else {
	fprintf(stderr, usage, argv[0]);
        exit(-1);
      }
    } else if (strcmp(argv[i],"-o") == 0) {
      if(fu_file_name == NULL && i + 1 < argc) {
	fu_file_name = argv[++i];
      } else {
	fprintf(stderr, usage, argv[0]);
        exit(-1);
      }
    } else {
      fprintf(stderr, usage, argv[0]);
      exit(-1);
    }
  }

  if (bin_file_name == NULL || fu_file_name == NULL) {
    fprintf(stderr, usage, argv[0]);
    exit(-1);
  }

  bin_file = fopen(bin_file_name,"r");
  if (bin_file == NULL) {
    fprintf(stderr, "error: cannot open binary file '%s'\n", bin_file_name);
    exit(-1);
  }

  fu_file = fopen(fu_file_name,"r");
  if (fu_file == NULL) {
    fclose(bin_file);
    fprintf(stderr, "error: cannot open functional unit options file '%s'\n", fu_file_name);
    exit(-1);
  }
}
