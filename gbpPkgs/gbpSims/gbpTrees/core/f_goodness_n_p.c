#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>

#define F_GOODNESS_N8 1673.69  // n_p corresponding to S_max=8, assuming n_rank=-1
#define F_GOODNESS_N4   30.65  // n_p corresponding to S_max=4, assuming n_rank=-1
#define F_GOODNESS_M     0.125 // slope     of the f(S) relation between f=0.7 at S=8 and f=0.2 at S=4
#define F_GOODNESS_B    -0.3   // intercept of the f(S) relation between f=0.7 at S=8 and f=0.2 at S=4

double f_goodness_n_p(double n_particles){
   double r_val=0.; 
   if(n_particles>=F_GOODNESS_N8)
      r_val=0.7;
   else if (n_particles<=F_GOODNESS_N4)
      r_val=0.2;
   else{
      double s_n=maximum_match_score(n_particles);
      r_val=F_GOODNESS_M*s_n+F_GOODNESS_B;
   }  
   return(r_val);
}

