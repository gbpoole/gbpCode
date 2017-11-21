#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpRNG.h>
#include <gbpMCMC.h>

void set_MCMC_mode(MCMC_info *MCMC, int mode) {
    MCMC->mode = mode;

    // Set the local chain number
    if(SID_CHECK_BITFIELD_SWITCH(MCMC->mode, MCMC_MODE_PARALLEL)) {
        MCMC->my_chain = SID.My_rank;
        MCMC->n_chains = SID.n_proc;
    } else {
        MCMC->my_chain = SID_MASTER_RANK;
        MCMC->n_chains = 1;
    }
    MCMC->flag_no_map_write = SID_CHECK_BITFIELD_SWITCH(mode, MCMC_MODE_NO_MAP_WRITE);
}
