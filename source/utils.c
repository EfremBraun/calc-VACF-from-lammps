
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"


void PrintDefaultInput(void)
{
  FILE *fptr;

  if(!(fptr=fopen("input.vacf","w")))
  {
    printf(" Problems opening file: 'input.vacf' \n");
    printf(" Hint: Check rights, can we write to folder?\n");
    exit(5);
  }
  else
  {
    fprintf(fptr,"#blocks   elements\n");
    fprintf(fptr,"1         1000000\n");
    fprintf(fptr,"# particle types\n");
    fprintf(fptr,"1\n");
    fprintf(fptr,"# timestep [ps] \n");
    fprintf(fptr,"0.0010\n");
    fprintf(fptr,"# for part 1   number of parts.   mass\n");
    fprintf(fptr,"               2000               32.0\n");
    fprintf(fptr,"filename.dat\n");
    fclose(fptr);
  }
}



int ReadNFloat(int numbers, char *line, double data[], int length)
{
  for(int i=0;i<numbers;i++)
  {
    double num;
    int nargs = sscanf(line,"%lf%[^\n]",&num,line);

    if(nargs==EOF) return false;
    if(i==length) 
    {
      printf("Reading line failed.\n");
      exit(1);
    }
    data[i] = num;
  }

  return true;
}


int ReadNInt(int numbers, char *line, int data[], int length)
{
  for(int i=0;i<numbers;i++)
  {
    int num;
    int nargs = sscanf(line,"%i%[^\n]",&num,line);

    if(nargs==EOF) return false;
    if(i==length) 
    {
      printf("Reading line failed.\n");
      exit(1);
    }
    data[i] = num;
  }

  return true;
}

/* Read a line in to array s, return length
 * From "the C programming language"
 */

int ReadNewLine(char s[], int lim, FILE *file)
{
  int c,i;

  //for (i=0;i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
  for (i=0;i<lim-1 && (c=fgetc(file))!=EOF && c!='\n'; ++i)
    s[i] = c;
  if(c=='\n')
  {
    s[i] = c;
    i++;
  }
  s[i] = '\0';
  return i;
}

size_t LinesInFile(FILE *fileptr)
{
  char c;
  size_t newline_count = 0;

  while((c=fgetc(fileptr))!=EOF)
  {
    if (c=='\n') newline_count+=1;
  }

  // go back to the start of the file
  rewind(fileptr);

  return newline_count;
}

