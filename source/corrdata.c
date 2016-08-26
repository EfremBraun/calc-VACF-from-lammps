#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "readinput.h"
#include "corrdata.h"


SNAPSHOT *ReserveMemorySnapshot(INPUT_DATA *Input)
{

  SNAPSHOT *Snap = NULL;

  Snap = (SNAPSHOT *) calloc(1,sizeof(SNAPSHOT));

  printf("\t Reserving memory for snapshots ...");
    
  Snap->PosX = (double**)calloc(Input->ParticleTypes,sizeof(double*));
  Snap->PosY = (double**)calloc(Input->ParticleTypes,sizeof(double*));
  Snap->PosZ = (double**)calloc(Input->ParticleTypes,sizeof(double*));

  Snap->CentreOfMassX = (double*)calloc(Input->ParticleTypes,sizeof(double));
  Snap->CentreOfMassY = (double*)calloc(Input->ParticleTypes,sizeof(double));
  Snap->CentreOfMassZ = (double*)calloc(Input->ParticleTypes,sizeof(double));

  for (size_t i=0;i<Input->ParticleTypes;i++)
  {
    Snap->PosX[i] = (double*)calloc(Input->NumberOfParticles[i],sizeof(double));
    Snap->PosY[i] = (double*)calloc(Input->NumberOfParticles[i],sizeof(double));
    Snap->PosZ[i] = (double*)calloc(Input->NumberOfParticles[i],sizeof(double));
  }
  
  printf(" done\n");

  return Snap;
}

void ReleaseMemorySnapshot(INPUT_DATA *Input, SNAPSHOT *data)
{

  printf("\t Releasing memory for snapshot ...");

  for (size_t i=0;i<Input->ParticleTypes;i++)
  {
    free(data->PosX[i]);
    free(data->PosY[i]);
    free(data->PosZ[i]);
  }

  free(data->PosX);
  free(data->PosY);
  free(data->PosZ);

  free(data->CentreOfMassX);
  free(data->CentreOfMassY);
  free(data->CentreOfMassZ);

  printf(" done \n");
}


STATE_BLOCK *ReserveMemoryStateBlock(INPUT_DATA *Input)
{

  STATE_BLOCK *State = NULL;
  
  size_t maxblock = Input->MaxBlocks;
  size_t maxelems = Input->MaxElements;
  size_t ptypes = Input->ParticleTypes;

  State = (STATE_BLOCK *)malloc(1*sizeof(SNAPSHOT));

  printf("\t Reserving memory for stateblock ...");

  State->ParticleX = (double****)calloc(maxblock,sizeof(double***));
  State->ParticleY = (double****)calloc(maxblock,sizeof(double***));
  State->ParticleZ = (double****)calloc(maxblock,sizeof(double***));

  State->CenterOfMassX = (double***)calloc(maxblock,sizeof(double**));
  State->CenterOfMassY = (double***)calloc(maxblock,sizeof(double**));
  State->CenterOfMassZ = (double***)calloc(maxblock,sizeof(double**));

  for (size_t i=0;i<maxblock;i++)
  {
    State->ParticleX[i] = (double***)calloc(maxelems,sizeof(double**));
    State->ParticleY[i] = (double***)calloc(maxelems,sizeof(double**));
    State->ParticleZ[i] = (double***)calloc(maxelems,sizeof(double**));
  
    State->CenterOfMassX[i] = (double**)calloc(maxelems,sizeof(double*));
    State->CenterOfMassY[i] = (double**)calloc(maxelems,sizeof(double*));
    State->CenterOfMassZ[i] = (double**)calloc(maxelems,sizeof(double*));

    for (size_t j=0;j<maxelems;j++)
    {
      State->ParticleX[i][j] = (double**)calloc(ptypes,sizeof(double*));
      State->ParticleY[i][j] = (double**)calloc(ptypes,sizeof(double*));
      State->ParticleZ[i][j] = (double**)calloc(ptypes,sizeof(double*));
    
      State->CenterOfMassX[i][j] = (double*)calloc(ptypes,sizeof(double));
      State->CenterOfMassY[i][j] = (double*)calloc(ptypes,sizeof(double));
      State->CenterOfMassZ[i][j] = (double*)calloc(ptypes,sizeof(double));
    
      if(State->CenterOfMassZ[i][j] == NULL)
      {
        printf("Memory allocation failed");
        exit(1);
      }

      for (size_t k=0;k<ptypes;k++)
      {
        State->ParticleX[i][j][k] = (double*)calloc(Input->NumberOfParticles[k],sizeof(double));
        State->ParticleY[i][j][k] = (double*)calloc(Input->NumberOfParticles[k],sizeof(double));
        State->ParticleZ[i][j][k] = (double*)calloc(Input->NumberOfParticles[k],sizeof(double));
      }
    }
  }
  
  printf(" done\n");

  return State;
}

void ReleaseMemoryStateBlock(INPUT_DATA *Input, STATE_BLOCK *data)
{
  size_t maxblock = Input->MaxBlocks;
  size_t maxelem = Input->MaxElements;
  size_t ptypes = Input->ParticleTypes;
  
  printf("\t Releasing memory for stateblock ...");

  for (size_t i=0;i<maxblock;i++)
  {
    for (size_t j=0;j<maxelem;j++)
    {
      for (size_t k=0;k<ptypes;k++)
      {
        free(data->ParticleX[i][j][k]);
        free(data->ParticleY[i][j][k]);
        free(data->ParticleZ[i][j][k]);
      }

      free(data->ParticleX[i][j]);
      free(data->ParticleY[i][j]);
      free(data->ParticleZ[i][j]);
    
      free(data->CenterOfMassX[i][j]);
      free(data->CenterOfMassY[i][j]);
      free(data->CenterOfMassZ[i][j]);
    }

    free(data->ParticleX[i]);
    free(data->ParticleY[i]);
    free(data->ParticleZ[i]);
  
    free(data->CenterOfMassX[i]);
    free(data->CenterOfMassY[i]);
    free(data->CenterOfMassZ[i]);
  }
    
  free(data->ParticleX);
  free(data->ParticleY);
  free(data->ParticleZ);

  free(data->CenterOfMassX);
  free(data->CenterOfMassY);
  free(data->CenterOfMassZ);

  printf(" done \n");
}



MSD *ReserveMemoryMSD(INPUT_DATA *Input)
{

  MSD *State = NULL;
  
  // local copy of some variables (short names)
  size_t maxblock = Input->MaxBlocks;
  size_t maxelems = Input->MaxElements;
  size_t ptypes = Input->ParticleTypes;

  State = (MSD *)calloc(1,sizeof(MSD));

  printf("\t Reserving memory for msd data ...");

  State->SelfX = (double***)calloc(maxblock,sizeof(double**));
  State->SelfY = (double***)calloc(maxblock,sizeof(double**));
  State->SelfZ = (double***)calloc(maxblock,sizeof(double**));
  State->SelfAvg = (double***)calloc(maxblock,sizeof(double**));
  State->SelfCounter = (double***)calloc(maxblock,sizeof(double**));

  State->CrossX = (double****)calloc(maxblock,sizeof(double***));
  State->CrossY = (double****)calloc(maxblock,sizeof(double***));
  State->CrossZ = (double****)calloc(maxblock,sizeof(double***));
  State->CrossAvg = (double****)calloc(maxblock,sizeof(double***));
  State->CrossCounter = (double****)calloc(maxblock,sizeof(double***));

  for (size_t i=0;i<maxblock;i++)
  {
  
    State->SelfX[i] = (double**)calloc(maxelems,sizeof(double*));
    State->SelfY[i] = (double**)calloc(maxelems,sizeof(double*));
    State->SelfZ[i] = (double**)calloc(maxelems,sizeof(double*));
    State->SelfAvg[i] = (double**)calloc(maxelems,sizeof(double*));
    State->SelfCounter[i] = (double**)calloc(maxelems,sizeof(double*));

    State->CrossX[i] = (double***)calloc(maxelems,sizeof(double**));
    State->CrossY[i] = (double***)calloc(maxelems,sizeof(double**));
    State->CrossZ[i] = (double***)calloc(maxelems,sizeof(double**));
    State->CrossAvg[i] = (double***)calloc(maxelems,sizeof(double**));
    State->CrossCounter[i] = (double***)calloc(maxelems,sizeof(double**));

    for (size_t j=0;j<maxelems;j++)
    {
    
      State->SelfX[i][j] = (double*)calloc(ptypes,sizeof(double));
      State->SelfY[i][j] = (double*)calloc(ptypes,sizeof(double));
      State->SelfZ[i][j] = (double*)calloc(ptypes,sizeof(double));
      State->SelfAvg[i][j] = (double*)calloc(ptypes,sizeof(double));
      State->SelfCounter[i][j] = (double*)calloc(ptypes,sizeof(double));

      State->CrossX[i][j] = (double**)calloc(ptypes,sizeof(double*));
      State->CrossY[i][j] = (double**)calloc(ptypes,sizeof(double*));
      State->CrossZ[i][j] = (double**)calloc(ptypes,sizeof(double*));
      State->CrossAvg[i][j] = (double**)calloc(ptypes,sizeof(double*));
      State->CrossCounter[i][j] = (double**)calloc(ptypes,sizeof(double*));


      for (size_t k=0;k<ptypes;k++)
      {
        State->CrossX[i][j][k] = (double*)calloc(ptypes,sizeof(double));
        State->CrossY[i][j][k] = (double*)calloc(ptypes,sizeof(double));
        State->CrossZ[i][j][k] = (double*)calloc(ptypes,sizeof(double));
        State->CrossAvg[i][j][k] = (double*)calloc(ptypes,sizeof(double));
        State->CrossCounter[i][j][k] = (double*)calloc(ptypes,sizeof(double));

      }
    }
  }

  printf(" done\n");
  return State;
}


void ReleaseMemoryMSD(INPUT_DATA *Input, MSD *data)
{
  
  size_t maxblock = Input->MaxBlocks;
  size_t maxelem = Input->MaxElements;
  size_t ptypes = Input->ParticleTypes;

  printf("\t Releasing memory for MSD ...");

  for (size_t i=0;i<maxblock;i++)
  {
    for (size_t j=0;j<maxelem;j++)
    {
      for (size_t k=0;k<ptypes;k++)
      {
        free(data->CrossX[i][j][k]);
        free(data->CrossY[i][j][k]);
        free(data->CrossZ[i][j][k]);
        free(data->CrossAvg[i][j][k]);
        free(data->CrossCounter[i][j][k]);
      }
    
      free(data->SelfX[i][j]);
      free(data->SelfY[i][j]);
      free(data->SelfZ[i][j]);
      free(data->SelfAvg[i][j]);
      free(data->SelfCounter[i][j]);

      free(data->CrossX[i][j]);
      free(data->CrossY[i][j]);
      free(data->CrossZ[i][j]);
      free(data->CrossAvg[i][j]);
      free(data->CrossCounter[i][j]);
    }
      
    free(data->SelfX[i]);
    free(data->SelfY[i]);
    free(data->SelfZ[i]);
    free(data->SelfAvg[i]);
    free(data->SelfCounter[i]);

    free(data->CrossX[i]);
    free(data->CrossY[i]);
    free(data->CrossZ[i]);
    free(data->CrossAvg[i]);
    free(data->CrossCounter[i]);
  }

  free(data->SelfX);
  free(data->SelfY);
  free(data->SelfZ);
  free(data->SelfAvg);
  free(data->SelfCounter);

  free(data->CrossX);
  free(data->CrossY);
  free(data->CrossZ);
  free(data->CrossAvg);
  free(data->CrossCounter);
  
  printf(" done \n");

}

