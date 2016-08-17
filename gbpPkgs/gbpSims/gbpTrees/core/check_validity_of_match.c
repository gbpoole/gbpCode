#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>

int check_validity_of_match(int n_particles_halo,int n_particles_match,float match_score){
   double f_0    =(double)n_particles_match/(double)n_particles_halo;
   double f_m1   =match_score_f_goodness(match_score,n_particles_halo);
   double f_diff =f_m1-f_0;
   return(f_diff>=F_MATCH_MOMENT_DIFF_MIN && n_particles_match>N_P_MATCH_MIN);
}
