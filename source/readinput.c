
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

  if(!(fileptr=fopen("input.vacf","r")))
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
    printf(" done\n");

  }

  printf("Done reading Inputfile\n");

  // close file
  fclose(fileptr);

  return Input;
}

