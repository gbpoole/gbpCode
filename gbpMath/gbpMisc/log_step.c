#include <stdio.h>
#include <math.h>

double log_step(double min,
                double max,
                int    n){
  return(pow(max/min,1./((double)(n-1))));
}
