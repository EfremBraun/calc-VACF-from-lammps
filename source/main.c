#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "readinput.h"
#include "utils.h"
#include "corrdata.h"
#include "sample.h"



/* main function in corrfunc
 */


int main(int argc, char **argv)
{

  int c;

  INPUT_DATA *InputData=NULL;
  
  // parse input arguments

  while((c=getopt(argc,argv,"hpv"))!=-1)
  {
    switch(c)
    {
      case 'h':
        printf("using corrfunc:\n");
        printf("\t -h:  Print this message.\n");
        printf("\t -p:  Print a generic input-file.\n");
        printf("\t -v:  Print version.\n");
        return 0;
        break;
      case 'v':
        printf("version 0.1b\n");
        return 0;
        break;
      case 'p':
        PrintDefaultInput();
        return 0;
        break;
      default:
        ;
    }
  }

  // read input
  InputData = ReadInput();

  Sample(InputData);

  // release memory
  ReleaseInput(InputData);

  printf("\t Done for now!\n");

  return 0;
}
