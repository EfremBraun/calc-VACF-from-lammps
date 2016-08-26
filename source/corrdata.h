#ifndef CORRDATA_H
#define CORRDATA_H 


// the instantaneous pos. of particles
typedef struct
{
  double **PosX;
  double **PosY;
  double **PosZ;
  double *CentreOfMassX;
  double *CentreOfMassY;
  double *CentreOfMassZ;
} SNAPSHOT;



// position of particles and COM of particle types during simulation
typedef struct 
{
  double ***CenterOfMassX;
  double ***CenterOfMassY;
  double ***CenterOfMassZ;
  double ****ParticleX;
  double ****ParticleY;
  double ****ParticleZ;
} STATE_BLOCK;

// correlation data
typedef struct 
{
  double ***SelfX;
  double ***SelfY;
  double ***SelfZ;
  double ***SelfAvg;
  double ***SelfCounter;
  double ****CrossX;
  double ****CrossY;
  double ****CrossZ;
  double ****CrossAvg;
  double ****CrossCounter;
} MSD;


SNAPSHOT *ReserveMemorySnapshot(INPUT_DATA *);
void ReleaseMemorySnapshot(INPUT_DATA *,SNAPSHOT *);

STATE_BLOCK *ReserveMemoryStateBlock(INPUT_DATA *);
void ReleaseMemoryStateBlock(INPUT_DATA *,STATE_BLOCK *);

MSD *ReserveMemoryMSD(INPUT_DATA *);
void ReleaseMemoryMSD(INPUT_DATA *,MSD *);

#endif
