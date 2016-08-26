
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>


#include "utils.h"
#include "readinput.h"
#include "corrdata.h"
#include "sample.h"

int ReadSnap(INPUT_DATA *, SNAPSHOT *);
void PrintData(INPUT_DATA *, MSD *, size_t, size_t *);
size_t FindNumberOfBlocks(INPUT_DATA *, size_t );
void StoreBlockData(INPUT_DATA *, SNAPSHOT *, STATE_BLOCK *, size_t, bool, size_t * );
void CalcMSD(INPUT_DATA *, STATE_BLOCK *, MSD *, size_t, size_t *);

void Sample(INPUT_DATA *Input)
{

  size_t SampleTimes = 0;

  SNAPSHOT *Snapshot = ReserveMemorySnapshot(Input);
  STATE_BLOCK *State = ReserveMemoryStateBlock(Input);  
  size_t *BlockLength = (size_t *) calloc(Input->MaxBlocks,sizeof(size_t));
  MSD *SampleMSD = ReserveMemoryMSD(Input);

  // read the first snapshot
  printf("\t Reading first snapshot ...");
  ReadSnap(Input, Snapshot);
  printf(" done\n");
  StoreBlockData(Input,Snapshot,State,SampleTimes,true,BlockLength);
  SampleTimes++;

  printf("\n\t Sampling ...\n");
  while(ReadSnap(Input,Snapshot))
  {
    if((SampleTimes%1000)==0)
    {
      printf(" \t \t snapshot %zu ",SampleTimes);
      printf("\r"); 
      fflush(stdout);
    }

    StoreBlockData(Input,Snapshot,State,SampleTimes,false,BlockLength);
    CalcMSD(Input,State,SampleMSD,SampleTimes,BlockLength);
    SampleTimes++;

  }
  printf("\n\t ... done  \n"); 

  PrintData(Input,SampleMSD,SampleTimes,BlockLength);
  
  ReleaseMemorySnapshot(Input,Snapshot);
  ReleaseMemoryStateBlock(Input,State);
  ReleaseMemoryMSD(Input,SampleMSD);

}


int ReadSnap(INPUT_DATA *Input, SNAPSHOT *snap)
{
  
  int MaxLineLength=2048;
  char line[MaxLineLength];

  size_t ptypes = Input->ParticleTypes;

  for (size_t comp=0;comp<ptypes;comp++)
  {
    size_t chars_read;
  
    // reset the center of mass.
    snap->CentreOfMassX[comp] = 0.0;
    snap->CentreOfMassY[comp] = 0.0;
    snap->CentreOfMassZ[comp] = 0.0;
   
    // skip 9 lines
    for (size_t j=0;j<9;j++) ReadNewLine(line,MaxLineLength,Input->Files[comp]);

    // end of file
    if(chars_read==0)
    {
      return false;
    }

    for (size_t part=0;part<Input->NumberOfParticles[comp];part++)
    {

      chars_read = ReadNewLine(line,MaxLineLength,Input->Files[comp]);
      //printf("Chars_read2 %zu \n",chars_read);

      //if(ReadNewLine(line,MaxLineLength,input->Files[comp])>0)
      if(chars_read!=0)
      {
        double rx,ry,rz;
        int particlenum;

        sscanf(line,"%i%lf%lf%lf%i%i%i",&particlenum,&rx,&ry,&rz);
        
        snap->CentreOfMassX[comp] += rx;
        snap->CentreOfMassY[comp] += ry;
        snap->CentreOfMassZ[comp] += rz;

        snap->PosX[comp][particlenum-1] = rx;
        snap->PosY[comp][particlenum-1] = ry;
        snap->PosZ[comp][particlenum-1] = rz;
      }
      else 
      {
        // premature end for a particle file
        printf("\n Error!!!!\n");
        printf("Premature end of file!!!\n");
        printf("Check input in file for comp. %zu !!!\n",comp);
        return false;
      }
    }

    // find the actual center of mass
    snap->CentreOfMassX[comp] /= Input->NumberOfParticles[comp];
    snap->CentreOfMassY[comp] /= Input->NumberOfParticles[comp];
    snap->CentreOfMassZ[comp] /= Input->NumberOfParticles[comp];
  }

  return true;

}

// calculate the current number of blocks we have
size_t FindNumberOfBlocks(INPUT_DATA *Input, size_t TimesSampled)
{
  size_t NumberOfBlocks=1;
  // times sampled / elements. Finds the floor(times/elem)
  size_t val=TimesSampled/Input->MaxElements;

  while(val!=0)
  {
    NumberOfBlocks++;
    val/=Input->MaxElements;
  }

  NumberOfBlocks = MIN2(NumberOfBlocks,Input->MaxBlocks);

  return NumberOfBlocks;
}


// this function stores the current snapshot, and shifts the position of the 
// data already stored in the system. 
// the initial keyword is called to store data in all blocks 
void StoreBlockData(INPUT_DATA *Input, SNAPSHOT *Snapshot, STATE_BLOCK *State, size_t TimesSampled, bool initial, size_t *BlockLength)
{

  size_t ParticleTypes = Input->ParticleTypes;
  size_t MaxBlocks = Input->MaxBlocks;

  if(initial)
  {
    // we fill in data of the first timestep into the first element in all blocks 
    
    printf("\t Storing first snapshot ...");
    
    for(size_t block=0;block<MaxBlocks;block++)
    {

      BlockLength[block]++;
      //printf("BlockLengt[%zu]=%zu\n",block,BlockLength[block]);
  
      for(size_t comp=0;comp<ParticleTypes;comp++)
      {
        for(size_t part=0;part<Input->NumberOfParticles[comp];part++)
        {
          State->ParticleX[block][0][comp][part] = Snapshot->PosX[comp][part];
          State->ParticleY[block][0][comp][part] = Snapshot->PosY[comp][part];
          State->ParticleZ[block][0][comp][part] = Snapshot->PosZ[comp][part];
        }

        State->CenterOfMassX[block][0][comp] = Snapshot->CentreOfMassX[comp];
        State->CenterOfMassY[block][0][comp] = Snapshot->CentreOfMassY[comp];
        State->CenterOfMassZ[block][0][comp] = Snapshot->CentreOfMassZ[comp];
        
      }
    }
    printf(" done\n");
  }
  else
  {

    size_t NumberOfBlocks=FindNumberOfBlocks(Input,TimesSampled);

    // loop over all the current blocks
    for(size_t CurrBlock=0;CurrBlock<NumberOfBlocks;CurrBlock++)
    {
      // if the current snapshot is divisable by the elements**block
      // it means that we can store information from this snapshot in that block
      if((TimesSampled)%((int)pow((double)Input->MaxElements,CurrBlock))==0)
      {
        
        // how long is the current block, i.e. how many elements do we have here?
        size_t CurrBlockLength = MIN2(BlockLength[CurrBlock]+1,Input->MaxElements);

        // we store data, increment the length of the current block
        BlockLength[CurrBlock]++;

        // loop over all particles, and store the snapshot
        for(size_t comp=0;comp<ParticleTypes;comp++)
        {
          for(size_t part=0;part<Input->NumberOfParticles[comp];part++)
          {
            // move the elements one step over
            for (size_t elem=CurrBlockLength-1;elem>0;elem--)
            {
              State->ParticleX[CurrBlock][elem][comp][part] = State->ParticleX[CurrBlock][elem-1][comp][part];
              State->ParticleY[CurrBlock][elem][comp][part] = State->ParticleY[CurrBlock][elem-1][comp][part];
              State->ParticleZ[CurrBlock][elem][comp][part] = State->ParticleZ[CurrBlock][elem-1][comp][part];
            }

            State->ParticleX[CurrBlock][0][comp][part] = Snapshot->PosX[comp][part];
            State->ParticleY[CurrBlock][0][comp][part] = Snapshot->PosY[comp][part];
            State->ParticleZ[CurrBlock][0][comp][part] = Snapshot->PosZ[comp][part];

          } // end of loop over individual particles

          // move elements
          for(size_t elem=CurrBlockLength-1;elem>0;elem--)
          {
            State->CenterOfMassX[CurrBlock][elem][comp] = State->CenterOfMassX[CurrBlock][elem-1][comp];
            State->CenterOfMassY[CurrBlock][elem][comp] = State->CenterOfMassY[CurrBlock][elem-1][comp];
            State->CenterOfMassZ[CurrBlock][elem][comp] = State->CenterOfMassZ[CurrBlock][elem-1][comp];
          }
          
          State->CenterOfMassX[CurrBlock][0][comp] = Snapshot->CentreOfMassX[comp];
          State->CenterOfMassY[CurrBlock][0][comp] = Snapshot->CentreOfMassY[comp];
          State->CenterOfMassZ[CurrBlock][0][comp] = Snapshot->CentreOfMassZ[comp];

        } // loop over all components
      }
    }
  }
}


// calculate the msd
void CalcMSD(INPUT_DATA *Input, STATE_BLOCK *State, MSD *SampleMSD, size_t TimesSampled, size_t *BlockLength)
{
  
  size_t ParticleTypes = Input->ParticleTypes;
  size_t NumberOfBlocks=FindNumberOfBlocks(Input,TimesSampled);

  // loop over all the current blocks
  for(size_t CurrBlock=0;CurrBlock<NumberOfBlocks;CurrBlock++)
  {
    // if this is true, we should store this snapshot in a block
    if((TimesSampled)%((int)pow((double)Input->MaxElements,CurrBlock))==0)
    {

      // how long is the current block, i.e. how many elements do we have here?
      size_t CurrBlockLength = MIN2(BlockLength[CurrBlock],Input->MaxElements);

      for(size_t comp=0;comp<ParticleTypes;comp++)
      {
        for(size_t part=0;part<Input->NumberOfParticles[comp];part++)
        {
          for(size_t elem=0;elem<CurrBlockLength;elem++)
          {

            double msdx = State->ParticleX[CurrBlock][elem][comp][part] * State->ParticleX[CurrBlock][0][comp][part];
            double msdy = State->ParticleY[CurrBlock][elem][comp][part] * State->ParticleY[CurrBlock][0][comp][part];
            double msdz = State->ParticleZ[CurrBlock][elem][comp][part] * State->ParticleZ[CurrBlock][0][comp][part];
            
            SampleMSD->SelfCounter[CurrBlock][elem][comp]+=1.0;

            SampleMSD->SelfX[CurrBlock][elem][comp]+=msdx;
            SampleMSD->SelfY[CurrBlock][elem][comp]+=msdy;
            SampleMSD->SelfZ[CurrBlock][elem][comp]+=msdz;
            
            SampleMSD->SelfAvg[CurrBlock][elem][comp]+=(msdx + msdy + msdz);
          }
        } // end of loop over individual particles

        // here we do the cross correlation
        
        for(size_t elem=0;elem<CurrBlockLength;elem++)
        {
          for(size_t comp2=0;comp2<ParticleTypes;comp2++)
          {
            double blcx = ((State->CenterOfMassX[CurrBlock][elem][comp] * State->CenterOfMassX[CurrBlock][0][comp2]));
            
            double blcy = ((State->CenterOfMassY[CurrBlock][elem][comp] * State->CenterOfMassY[CurrBlock][0][comp2]));
            
            double blcz = ((State->CenterOfMassZ[CurrBlock][elem][comp] * State->CenterOfMassZ[CurrBlock][0][comp2]));

            SampleMSD->CrossCounter[CurrBlock][elem][comp][comp2]+=1.0;

            SampleMSD->CrossX[CurrBlock][elem][comp][comp2]+=blcx;
            SampleMSD->CrossY[CurrBlock][elem][comp][comp2]+=blcy;
            SampleMSD->CrossZ[CurrBlock][elem][comp][comp2]+=blcz;
            SampleMSD->CrossAvg[CurrBlock][elem][comp][comp2]+=(blcx + blcy + blcz);
                                                             
          }
        }
      } // loop over all components
    }
  }
}

void PrintData(INPUT_DATA *Input, MSD *SampleMSD, size_t TimesSampled, size_t *BlockLength)
{

  FILE *tfileptr=NULL;
  size_t NumberOfBlocks = FindNumberOfBlocks(Input,TimesSampled);
  
  // first we print self-diffusion coefficients for each component
  for (size_t comp=0;comp<Input->ParticleTypes;comp++) 
  {

    FILE *fileptr=NULL;
    char buffer[256];
    
    printf("\t Printing self-VACF for Component %zu to file ...",comp);

    sprintf(buffer,"vacf_self_comp_%zu.dat",comp+1);
    fileptr=fopen(buffer,"w");
    fprintf(fileptr,"# Self diffusion coefficient for component %zu\n",comp+1);
    fprintf(fileptr,"# Time[fs] xdir,ydir,zdir,avg,Counter\n");

    for (size_t CurrBlock=0;CurrBlock<NumberOfBlocks;CurrBlock++)
    {
      size_t CurrentBlockLength = MIN2(BlockLength[CurrBlock],Input->MaxElements);
      double dt =Input->Timestep*pow((double)Input->MaxElements,CurrBlock);

      for(size_t elem=1;elem<CurrentBlockLength;elem++)
      {
        if(SampleMSD->SelfCounter[CurrBlock][elem][comp]>0.0)
        {

          double fac = 1.0/SampleMSD->SelfCounter[CurrBlock][elem][comp];

          fprintf(fileptr,"%10.3f   %10.7e   %10.7e   %10.7e   %10.7e         %zu\n",
              ((double)elem*dt),
              (fac*SampleMSD->SelfX[CurrBlock][elem][comp]),
              (fac*SampleMSD->SelfY[CurrBlock][elem][comp]),
              (fac*SampleMSD->SelfZ[CurrBlock][elem][comp]),
              (fac*SampleMSD->SelfAvg[CurrBlock][elem][comp]),
              (size_t)(SampleMSD->SelfCounter[CurrBlock][elem][comp]));
        }
      }
    }

    fclose(fileptr);
    printf(" done \n");
  }

  
  // print the onsager coefficients (cross-VACF) for each component
  for (size_t comp=0;comp<Input->ParticleTypes;comp++) 
  {
    for (size_t comp2=0;comp2<Input->ParticleTypes;comp2++)
    {
      FILE *fileptr=NULL;
      char buffer[256];
      
      printf("\t Printing Onsager coeff for Component %zu -- %zu to file ...",comp,comp2);

      sprintf(buffer,"vacf_onsager_comp_%zu_%zu.dat",comp+1,comp2+1);
      fileptr=fopen(buffer,"w");
      fprintf(fileptr,"# Onsager coefficient for components %zu--%zu \n",comp+1,comp2+1);
      fprintf(fileptr,"# Time[fs] xdir,ydir,zdir,avg,Counter\n");

      for (size_t CurrBlock=0;CurrBlock<NumberOfBlocks;CurrBlock++)
      {

        size_t CurrentBlockLength = MIN2(BlockLength[CurrBlock],Input->MaxElements);

        double dt =Input->Timestep*pow((double)Input->MaxElements,CurrBlock);

        for(size_t elem=1;elem<CurrentBlockLength;elem++)
        {
          if(SampleMSD->CrossCounter[CurrBlock][elem][comp][comp2]>0.0)
          {

            double fac = 1.0/SampleMSD->CrossCounter[CurrBlock][elem][comp][comp2];

            fprintf(fileptr,"%10.3f   %10.7e   %10.7e   %10.7e   %10.7e         %zu\n",
                ((double)elem*dt),
                (fac*SampleMSD->CrossX[CurrBlock][elem][comp][comp2]),
                (fac*SampleMSD->CrossY[CurrBlock][elem][comp][comp2]),
                (fac*SampleMSD->CrossZ[CurrBlock][elem][comp][comp2]),
                (fac*SampleMSD->CrossAvg[CurrBlock][elem][comp][comp2]),
                (size_t)(SampleMSD->CrossCounter[CurrBlock][elem][comp][comp2]));
          }
        }
      }

      fclose(fileptr);
      printf(" done\n");
    }
  }
  
  // print the total self diffusion coefficient (for single component systems, this should be the normal one)
  
  printf("\t Printing average VACF to file ...");

  tfileptr=fopen("vacf_total_self.dat","w");
  fprintf(tfileptr,"# Total Self diffusion coefficient \n");
  fprintf(tfileptr,"# Time[fs] xdir,ydir,zdir,avg,Counter\n");

  for (size_t CurrBlock=0;CurrBlock<NumberOfBlocks;CurrBlock++)
  {
    size_t CurrentBlockLength = MIN2(BlockLength[CurrBlock],Input->MaxElements);
    double dt =Input->Timestep*pow((double)Input->MaxElements,CurrBlock);

    for(size_t elem=1;elem<CurrentBlockLength;elem++)
    {

      double val_x = 0.0;
      double val_y = 0.0;
      double val_z = 0.0;
      double val_avg = 0.0;
      size_t count = 0;

      for(size_t comp=0;comp<Input->ParticleTypes;comp++)
      {
        val_x += SampleMSD->SelfX[CurrBlock][elem][comp];
        val_y += SampleMSD->SelfY[CurrBlock][elem][comp];
        val_z += SampleMSD->SelfZ[CurrBlock][elem][comp];
        val_avg += SampleMSD->SelfAvg[CurrBlock][elem][comp];
        count += SampleMSD->SelfCounter[CurrBlock][elem][comp];
      }

      if(count>0)
      {
        double fac = 1.0/count;
        fprintf(tfileptr,"%10.3f   %10.7e   %10.7e   %10.7e   %10.7e         %zu\n",
        ((double)elem*dt),
        (fac*val_x),
        (fac*val_y),
        (fac*val_z),
        (fac*val_avg),
        count);
      }
    }
  }

  fclose(tfileptr);
  printf(" done \n");
  
}

