#include <stdio.h>
#include <math.h>

double linear_step(double min,
                   double max,
                   int    n){
  return((max-min)/((double)(n-1)));
}
