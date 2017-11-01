#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpRNG.h>
#include <gbpMCMC.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_interp.h>

void generate_MCMC_parameters(MCMC_info *MCMC) {
    int                i_P;
    int                j_P;
    double             factor_i;
    int                flag_continue;
    static int         reflect_priors;
    static int         n_P;
    static double      factor;
    static double *    P_new;
    static double *    P_chain;
    static double *    P_init;
    static double *    P_limit_min;
    static double *    P_limit_max;
    static gsl_vector *b;
    static gsl_matrix *m;
    static RNG_info *  RNG;

    // Initialize a few things on the first call
    switch(MCMC->first_parameter_call) {
        case GBP_TRUE:
            n_P                        = MCMC->n_P;
            P_new                      = MCMC->P_new;
            P_chain                    = MCMC->P_chain;
            P_init                     = MCMC->P_init;
            P_limit_min                = MCMC->P_limit_min;
            P_limit_max                = MCMC->P_limit_max;
            b                          = MCMC->b;
            m                          = MCMC->m;
            RNG                        = MCMC->RNG;
            factor                     = 2.4 / sqrt((double)MCMC->n_M_total);
            MCMC->first_parameter_call = GBP_FALSE;
            reflect_priors             = check_mode_for_flag(MCMC->mode, MCMC_MODE_REFLECT_PRIORS);
            break;
    }

    // Loop until a satisfactory set of parameters has been selected
    flag_continue = GBP_TRUE;
    while(flag_continue) {
        // Generate a random displacement vector
        factor_i = MCMC->temperature * factor;
        for(i_P = 0; i_P < n_P; i_P++)
            gsl_vector_set(b, i_P, factor_i * random_gaussian(RNG));

        // Use the rotated covariance matrix (if available)
        if(m != NULL) {
            memcpy(P_new, P_chain, n_P * sizeof(double));
            for(i_P = 0; i_P < n_P; i_P++) {
                for(j_P = 0; j_P < n_P; j_P++)
                    P_new[i_P] += gsl_matrix_get(m, i_P, j_P) * gsl_vector_get(b, j_P);
            }
        } else {
            for(j_P = 0; j_P < n_P; j_P++)
                P_new[j_P] = P_init[j_P] * (1. + gsl_vector_get(b, j_P));
        }

        // Enforce parameter limits
        if(reflect_priors) {
            for(i_P = 0; i_P < n_P; i_P++) {
                if(P_new[i_P] < P_limit_min[i_P])
                    P_new[i_P] = 2 * (P_limit_min[i_P]) - P_new[i_P];
                if(P_new[i_P] > P_limit_max[i_P])
                    P_new[i_P] = 2 * (P_limit_max[i_P]) - P_new[i_P];
            }
        } else {
            for(i_P = 0, flag_continue = GBP_FALSE; i_P < n_P; i_P++)
                if(P_new[i_P] < P_limit_min[i_P] || P_new[i_P] > P_limit_max[i_P])
                    flag_continue = GBP_TRUE;
        }
    }

    if(!check_mode_for_flag(MCMC->mode, MCMC_MODE_PARALLEL))
        SID_Bcast(P_new, n_P, SID_DOUBLE, MCMC->comm, SID_MASTER_RANK);
}
