#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>

#define READ_MATCHES_GOODNESS_C   ONE_THIRD
// 10 Percent values
//#define READ_MATCHES_GOODNESS_A   1.4421650782960167E+01
//#define READ_MATCHES_GOODNESS_B   2.7010890377990435E+00
//#define READ_MATCHES_GOODNESS_D  -2.4514674194264687E+00
// 20 Percent values
//#define READ_MATCHES_GOODNESS_A   1.4517784940714872E+01
//#define READ_MATCHES_GOODNESS_B   5.4013332444434088E+00
//#define READ_MATCHES_GOODNESS_D  -2.4505339973666742E+00
// 40 Percent values
#define READ_MATCHES_GOODNESS_A   1.4806123257012386E+01
#define READ_MATCHES_GOODNESS_B   1.0840337997980678E+01
#define READ_MATCHES_GOODNESS_D  -2.4577976443027350E+00
// 80 Percent values
//#define READ_MATCHES_GOODNESS_A   1.4641188518362066E+01
//#define READ_MATCHES_GOODNESS_B   2.1601666080698095E+01
//#define READ_MATCHES_GOODNESS_D  -2.4490239433470165E+00

int check_goodness_of_match(int n_particles_in,float match_score){
   double n_particles=(double)n_particles_in;
   float  min_score  =(float)pow(READ_MATCHES_GOODNESS_A+READ_MATCHES_GOODNESS_B*n_particles,READ_MATCHES_GOODNESS_C)+READ_MATCHES_GOODNESS_D;
   if(match_score<min_score)
      return(FALSE);
   else
      return(TRUE);
}

