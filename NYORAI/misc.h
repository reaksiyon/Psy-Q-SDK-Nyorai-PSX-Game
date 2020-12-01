#include <STDLIB.H>
#include <STDIO.H>

int absRangeX;
int absRangeY;

int dropID;


void wait()
{
  int i;

  for (i=0; i<3; i++) VSync(0);
}

void wait100()
{
  int i;

  for (i=0; i<100; i++) VSync(0);
}
