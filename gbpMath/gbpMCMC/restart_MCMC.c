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

void restart_MCMC(MCMC_info *MCMC) {
    SID_log("Resetting MCMC structure for a fresh run...", SID_LOG_OPEN);

    MCMC->flag_integrate_on     = GBP_FALSE;
    MCMC->flag_analysis_on      = GBP_TRUE;
    MCMC->first_map_call        = GBP_TRUE;
    MCMC->first_link_call       = GBP_TRUE;
    MCMC->flag_init_chain       = GBP_TRUE;
    MCMC->first_chain_call      = GBP_TRUE;
    MCMC->first_parameter_call  = GBP_TRUE;
    MCMC->first_likelihood_call = GBP_TRUE;
    MCMC->ln_likelihood_last    = 0.;
    MCMC->ln_likelihood_new     = 0.;
    MCMC->ln_likelihood_chain   = 0.;
    MCMC->n_success             = 0;
    MCMC->n_propositions        = 0;
    MCMC->n_map_calls           = 0;

    free_MCMC_covariance(MCMC);
    free_MCMC_arrays(MCMC);

    SID_log("Done.", SID_LOG_CLOSE);
}
