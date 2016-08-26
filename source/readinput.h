#ifndef READINPUT_H
#define READINPUT_H

/* Read and store all input-data to calculate a correlation function
 */

typedef struct
{
  FILE **Files;
  size_t MaxBlocks;
  size_t MaxElements;
  size_t ParticleTypes;
  size_t *NumberOfParticles;
  char Filenames[10][256]; // fixme, make this better
  double *Mass;
  double Timestep;
  double xlo;
  double xhi;
  double ylo;
  double yhi;
  double zlo;
  double zhi;
  double sidex;
  double sidey;
  double sidez;
  int cdrift;
  int indpart;
} INPUT_DATA;

INPUT_DATA *ReadInput(void);
void ReleaseInput(INPUT_DATA *);

#endif
