#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpMath.h>
#include <gbpHalos.h>
#include <gbpTrees_build.h>

#define F_GOODNESS_S_HI 9.
#define F_GOODNESS_HI 0.65
#define F_GOODNESS_N_HI 4549.55 // n_p corresponding to S_max=F_GOODNESS_S_HI, assuming n_rank=-1
#define F_GOODNESS_S_LO 7.5
#define F_GOODNESS_LO 0.1
#define F_GOODNESS_N_LO 1015.14 // n_p corresponding to S_max=F_GOODNESS_S_LO, assuming n_rank=-1
#define F_GOODNESS_M \
    ((F_GOODNESS_HI - F_GOODNESS_LO) / (F_GOODNESS_S_HI - F_GOODNESS_S_LO)) // slope of the f(S) relation between f= at S=8 and f=0.2 at S=4
#define F_GOODNESS_B F_GOODNESS_HI - F_GOODNESS_M *F_GOODNESS_S_HI          // intercept of the f(S) relation between f=0.7 at S=8 and f=0.2 at S=4

double f_goodness_n_p(double n_particles) {
    // double r_val=0.65;
    /**/
    double r_val = 0.;
    if(n_particles >= F_GOODNESS_N_HI)
        r_val = F_GOODNESS_HI;
    else if(n_particles <= F_GOODNESS_N_LO)
        r_val = F_GOODNESS_LO;
    else {
        double s_n = maximum_match_score(n_particles);
        r_val      = F_GOODNESS_M * s_n + F_GOODNESS_B;
    }
    /**/
    return (r_val);
}
