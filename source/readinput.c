
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "readinput.h"
#include "utils.h"

void ReleaseInput(INPUT_DATA *Input)
{

  printf("\t Releasing input-data ...");

  free(Input->Mass);
  free(Input->NumberOfParticles);
  free(Input);

  printf(" done \n");

}


INPUT_DATA *ReadInput(void)
{
  FILE *fileptr=NULL;
  int MaxLineLength=2048;
  int MaxN = 200; 
  int Ints[MaxN];
  double Floats[MaxN];
  char keyword[2048];

  char line[MaxLineLength];

  INPUT_DATA *Input = (INPUT_DATA *) calloc(1,sizeof(INPUT_DATA));

  if(!(fileptr=fopen("input.msd","r")))
  {
    printf("ReadInput: Input-file not opened correctly.\n");
    exit(1);
  }

  // parse input-file   
  ReadNewLine(line,MaxLineLength,fileptr);

  // Number of blocks, and elements
  ReadNewLine(line,MaxLineLength,fileptr);
  if(!ReadNInt(2,line,Ints,MaxN))
  {
    printf("Unable to read all input on line. MaxBlocks and MaxElements not defined.\n");
  }
  else
  {
    Input->MaxBlocks = (size_t) Ints[0];
    Input->MaxElements = (size_t) Ints[1];
    printf("Variable MaxBlocks:     %zu\n",Input->MaxBlocks);
    printf("Variable MaxElements:   %zu\n",Input->MaxElements);
  }

  
  // empty line
  ReadNewLine(line,MaxLineLength,fileptr);

  // box-xdir
  ReadNewLine(line,MaxLineLength,fileptr);
  if(!ReadNFloat(2,line,Floats,MaxN))
  {
    printf("Unable to read all input on line. xlo and xhi not defined.\n");
  }
  else
  {
    Input->xlo = Floats[0];
    Input->xhi = Floats[1];
    Input->sidex = Input->xhi - Input->xlo;
    printf("Box with bounds:  xlo   %f\n",Input->xlo);
    printf("                  xhi   %f\n",Input->xhi);
    printf("                  sidex %f\n",Input->sidex);
  }

  // box-ydir
  ReadNewLine(line,MaxLineLength,fileptr);
  if(!ReadNFloat(2,line,Floats,MaxN))
  {
    printf("Unable to read all input on line. ylo and yhi not defined.\n");
  }
  else
  {
    Input->ylo = Floats[0];
    Input->yhi = Floats[1];
    Input->sidey = Input->yhi - Input->ylo;
    printf("Box with bounds:  ylo   %f\n",Input->ylo);
    printf("                  yhi   %f\n",Input->yhi);
    printf("                  sidey %f\n",Input->sidey);
  }
  
  // box-zdir
  ReadNewLine(line,MaxLineLength,fileptr);
  if(!ReadNFloat(2,line,Floats,MaxN))
  {
    printf("Unable to read all input on line. zlo and zhi not defined.\n");
  }
  else
  {
    Input->zlo = Floats[0];
    Input->zhi = Floats[1];
    Input->sidez = Input->zhi - Input->zlo;
    printf("Box with bounds:  zlo   %f\n",Input->zlo);
    printf("                  zhi   %f\n",Input->zhi);
    printf("                  sidez %f\n",Input->sidez);
  }


  // discard an empty lline
  ReadNewLine(line,MaxLineLength,fileptr);
  // Number of particles
  ReadNewLine(line,MaxLineLength,fileptr);
  if(!ReadNInt(1,line,Ints,MaxN))
  {
    printf("Unable to read all input on line. Number of particle types not defined.\n");
  }
  else
  {
    if(Ints[0]==0)
    {
      printf("Exit. no particle types");
    }
    Input->ParticleTypes = (size_t) Ints[0];
    printf("ParticleTypes:          %zu\n",Input->ParticleTypes);
    Input->NumberOfParticles = (size_t *)calloc(Input->ParticleTypes,sizeof(size_t));
    Input->Mass = (double *)calloc(Input->ParticleTypes,sizeof(double));
    Input->Files = (FILE **) calloc(Input->ParticleTypes,sizeof(FILE*));
    //Input->Filenames = (char *) calloc(Input->ParticleTypes,sizeof(char));
  }


  // discard an empty line
  ReadNewLine(line,MaxLineLength,fileptr);
  // logical parameters
  ReadNewLine(line,MaxLineLength,fileptr);
  sscanf(line,"%s%[^\n]",keyword,line);
  if(strcasecmp("yes",keyword)==0)
  {
    Input->cdrift = true;
    printf("Correcting for drift ... true\n");
  }
  else
  {
    Input->cdrift = false;
    printf("Correcting for drift ... false\n");
  }

  sscanf(line,"%s%[^\n]",keyword,line);
  if(strcasecmp("yes",keyword)==0)
  {
    Input->indpart = true;
    printf("... true\n");
  }
  else
  {
    Input->indpart = false;
    printf("... false\n");
  }



  
  // Timestep
  ReadNewLine(line,MaxLineLength,fileptr);

  ReadNewLine(line,MaxLineLength,fileptr);
  if(!ReadNFloat(1,line,Floats,MaxN))
  {
    printf("Unable to read all input on line. Timestep not defined.\n");
  }
  else
  {
    Input->Timestep = Floats[0];
    printf("Timestep:               %f\n\n",Input->Timestep);
  }

  // print the block ranges
  for (size_t i=0;i<Input->MaxBlocks;i++)
  {
    double inf = Input->Timestep*pow((double)Input->MaxElements,i);
    double sup = inf * Input->MaxElements;
    printf("Block %zu: Start: %20.4f [ps]  End: %20.4f [ps] \n",i,inf,sup);
  }
  printf("\n");
  
  // loop over all particle types
  for(size_t i=0;i<Input->ParticleTypes;i++)
  {
    char dummy[256];

    ReadNewLine(line,MaxLineLength,fileptr);
    // read comp. comp. 
    ReadNewLine(line,MaxLineLength,fileptr);
    sscanf(line,"%zu%lf",&Input->NumberOfParticles[i],&Input->Mass[i]);
    printf(" Found %zu particles of type %zu, with mass %f.\n",Input->NumberOfParticles[i],i,Input->Mass[i]);
    // read empty line
    ReadNewLine(line,MaxLineLength,fileptr);
    sscanf(line,"%s",dummy);
    strcpy(Input->Filenames[i],dummy);
    
    printf("Opening file: %s ...",Input->Filenames[i]);
    if(!(Input->Files[i]=fopen(Input->Filenames[i],"r")))
    {
      printf("\n Problems\n");
      printf(" Check if file is good.\n");
      exit(1);
    }
    else
    {
      // read and discard the first three lines of the file
      for (size_t j=0;j<3;j++) ReadNewLine(line,MaxLineLength,Input->Files[i]);
    }
    printf(" done\n");

  }

  printf("Done reading Inputfile\n");

  // close file
  fclose(fileptr);

  return Input;
}

