#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void init_particle_map_quantities(map_quantities_info *mq, render_info *render, ADaPS *transfer_list, int flag_comoving, double expansion_factor) {
    int i_snap;

    SID_log("Initializing quantities...", SID_LOG_OPEN);

    // Defaults
    mq->flag_weigh              = GBP_FALSE;
    mq->flag_line_integral      = GBP_FALSE;
    mq->flag_transfer_sigma_log = GBP_FALSE;
    mq->flag_transfer_rho_log   = GBP_FALSE;
    mq->n_particles             = 0;
    mq->mass_array              = 0.;
    mq->h_smooth                = (float **)SID_malloc(sizeof(float *) * render->n_interpolate);
    mq->x                       = (GBPREAL **)SID_malloc(sizeof(float *) * render->n_interpolate);
    mq->y                       = (GBPREAL **)SID_malloc(sizeof(float *) * render->n_interpolate);
    mq->z                       = (GBPREAL **)SID_malloc(sizeof(float *) * render->n_interpolate);
    mq->rho                     = (float **)SID_malloc(sizeof(float *) * render->n_interpolate);
    mq->sigma                   = (float **)SID_malloc(sizeof(float *) * render->n_interpolate);
    mq->transfer_sigma          = NULL;
    mq->transfer_rho            = NULL;

    // Comoving?
    mq->flag_comoving = flag_comoving;
    if(mq->flag_comoving)
        mq->inv_expansion_factor_cubed = 1 / (expansion_factor * expansion_factor * expansion_factor);
    else
        mq->inv_expansion_factor_cubed = 1.;

    // Initialize the array of used particle types
    int i_type;
    mq->ptype_used = (int *)SID_malloc(sizeof(int) * N_GADGET_TYPE);
    for(i_type = 0; i_type < N_GADGET_TYPE; i_type++)
        mq->ptype_used[i_type] = GBP_FALSE;

    // Render a dark matter velocity dispersion map
    if(!strcmp(render->camera->RGB_param, "sigma_v_dark") && !strcmp(render->camera->Y_param, "tau_dark")) {
        mq->ptype_used[GADGET_TYPE_DARK] = GBP_TRUE;
        mq->flag_weigh                   = GBP_TRUE;
        mq->flag_line_integral           = GBP_TRUE;
        mq->n_particles                  = ((size_t *)ADaPS_fetch(render->plist_list[0]->data, "n_dark"))[0];
        mq->v_mode                       = MAKE_MAP_MODE_SIGMA;
        mq->w_mode                       = MAKE_MAP_MODE_RHO;
        if(ADaPS_exist(render->plist_list[0]->data, "rho_dark")) {
            for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                mq->rho[i_snap] = (float *)ADaPS_fetch(render->plist_list[i_snap]->data, "rho_dark");
            if(ADaPS_exist(transfer_list, "rho_dark")) {
                if(ADaPS_exist(transfer_list, "rho_dark_log"))
                    mq->flag_transfer_rho_log = GBP_TRUE;
                mq->transfer_rho = (interp_info *)ADaPS_fetch(transfer_list, "rho_dark");
            } else {
                mq->flag_transfer_rho_log = GBP_FALSE;
                mq->transfer_rho          = NULL;
            }
        } else
            SID_exit_error("No densities available in make_map.", SID_ERROR_LOGIC);

        // Use sigma_v for values
        if(ADaPS_exist(render->plist_list[0]->data, "sigma_v_dark")) {
            for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                mq->sigma[i_snap] = (float *)ADaPS_fetch(render->plist_list[i_snap]->data, "sigma_v_dark");
            if(ADaPS_exist(transfer_list, "sigma_v_dark")) {
                if(ADaPS_exist(transfer_list, "sigma_v_dark_log"))
                    mq->flag_transfer_sigma_log = GBP_TRUE;
                mq->transfer_sigma = (interp_info *)ADaPS_fetch(transfer_list, "sigma_v_dark");
            } else {
                mq->flag_transfer_sigma_log = GBP_FALSE;
                mq->transfer_sigma          = NULL;
            }
        } else
            SID_exit_error("No sigma_v's available in make_map.", SID_ERROR_LOGIC);
    } else if(!strcmp(render->camera->Y_param, "sigma_v_dark") && !strcmp(render->camera->RGB_param, "tau_dark")) {
        mq->ptype_used[GADGET_TYPE_DARK] = GBP_TRUE;
        mq->flag_weigh                   = GBP_TRUE;
        mq->flag_line_integral           = GBP_TRUE;
        mq->n_particles                  = ((size_t *)ADaPS_fetch(render->plist_list[0]->data, "n_dark"))[0];
        mq->v_mode                       = MAKE_MAP_MODE_SIGMA;
        mq->w_mode                       = MAKE_MAP_MODE_RHO;
        if(ADaPS_exist(render->plist_list[0]->data, "sigma_v_dark")) {
            for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                mq->rho[i_snap] = (float *)ADaPS_fetch(render->plist_list[i_snap]->data, "sigma_v_dark");
            if(ADaPS_exist(transfer_list, "sigma_v_dark")) {
                if(ADaPS_exist(transfer_list, "sigma_v_dark_log"))
                    mq->flag_transfer_rho_log = GBP_TRUE;
                mq->transfer_rho = (interp_info *)ADaPS_fetch(transfer_list, "sigma_v_dark");
            } else {
                mq->flag_transfer_rho_log = GBP_FALSE;
                mq->transfer_rho          = NULL;
            }
        } else
            SID_exit_error("No densities available in make_map.", SID_ERROR_LOGIC);

        // Use rho for values
        if(ADaPS_exist(render->plist_list[0]->data, "rho_dark")) {
            for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                mq->sigma[i_snap] = (float *)ADaPS_fetch(render->plist_list[i_snap]->data, "rho_dark");
            if(ADaPS_exist(transfer_list, "rho_dark")) {
                if(ADaPS_exist(transfer_list, "rho_dark_log"))
                    mq->flag_transfer_sigma_log = GBP_TRUE;
                mq->transfer_sigma = (interp_info *)ADaPS_fetch(transfer_list, "rho_dark");
            } else {
                mq->flag_transfer_sigma_log = GBP_FALSE;
                mq->transfer_sigma          = NULL;
            }
        } else
            SID_exit_error("No sigma_v's available in make_map.", SID_ERROR_LOGIC);
    } else if(!strcmp(render->camera->RGB_param, "sigma_v_dark") && render->camera->flag_velocity_space) {
        mq->ptype_used[GADGET_TYPE_DARK] = GBP_TRUE;
        mq->flag_weigh                   = GBP_FALSE;
        mq->flag_line_integral           = GBP_TRUE;
        mq->n_particles                  = ((size_t *)ADaPS_fetch(render->plist_list[0]->data, "n_dark"))[0];
        mq->v_mode                       = MAKE_MAP_MODE_SIGMA;
        mq->w_mode                       = MAKE_MAP_INV_SIGMA;

        // Use sigma_v for values
        if(ADaPS_exist(render->plist_list[0]->data, "sigma_v_dark")) {
            for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                mq->sigma[i_snap] = (float *)ADaPS_fetch(render->plist_list[i_snap]->data, "sigma_v_dark");
            if(ADaPS_exist(transfer_list, "sigma_v_dark")) {
                if(ADaPS_exist(transfer_list, "sigma_v_dark_log"))
                    mq->flag_transfer_sigma_log = GBP_TRUE;
                mq->transfer_sigma = (interp_info *)ADaPS_fetch(transfer_list, "sigma_v_dark");
            } else {
                mq->flag_transfer_sigma_log = GBP_FALSE;
                mq->transfer_sigma          = NULL;
            }
        } else
            SID_exit_error("No sigma_v's available in make_map.", SID_ERROR_LOGIC);
    } else
        SID_exit_error("Unknown rendering configuration RGB={%s} Y={%s} w/ flag_velocity_space=%d.", SID_ERROR_LOGIC,
                       render->camera->RGB_param, render->camera->Y_param, render->camera->flag_velocity_space);

    // Initialize arrays
    int n_type_used = 0;
    for(i_type = 0; i_type < N_GADGET_TYPE; i_type++) {
        if(mq->ptype_used[i_type]) {
            // This code can use one-and-only-one particle type at a time at the moment
            n_type_used++;
            if(n_type_used > 1)
                SID_exit_error("An invalid number of particle types (%d) are being used in make_map.", SID_ERROR_LOGIC,
                               n_type_used);
            if(render->camera->flag_velocity_space) {
                for(i_snap = 0; i_snap < render->n_interpolate; i_snap++) {
                    mq->x[i_snap] = (GBPREAL *)ADaPS_fetch(render->plist_list[i_snap]->data, "vx_%s", render->plist_list[i_snap]->species[i_type]);
                    mq->y[i_snap] = (GBPREAL *)ADaPS_fetch(render->plist_list[i_snap]->data, "vy_%s", render->plist_list[i_snap]->species[i_type]);
                    mq->z[i_snap] = (GBPREAL *)ADaPS_fetch(render->plist_list[i_snap]->data, "vz_%s", render->plist_list[i_snap]->species[i_type]);
                }
                for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                    mq->h_smooth[i_snap] =
                        (float *)ADaPS_fetch(render->plist_list[i_snap]->data, "sigma_v_%s", render->plist_list[i_snap]->species[i_type]);
            } else {
                for(i_snap = 0; i_snap < render->n_interpolate; i_snap++) {
                    mq->x[i_snap] = (GBPREAL *)ADaPS_fetch(render->plist_list[i_snap]->data, "x_%s", render->plist_list[i_snap]->species[i_type]);
                    mq->y[i_snap] = (GBPREAL *)ADaPS_fetch(render->plist_list[i_snap]->data, "y_%s", render->plist_list[i_snap]->species[i_type]);
                    mq->z[i_snap] = (GBPREAL *)ADaPS_fetch(render->plist_list[i_snap]->data, "z_%s", render->plist_list[i_snap]->species[i_type]);
                }
                for(i_snap = 0; i_snap < render->n_interpolate; i_snap++)
                    mq->h_smooth[i_snap] =
                        (float *)ADaPS_fetch(render->plist_list[i_snap]->data, "r_smooth_%s", render->plist_list[i_snap]->species[i_type]);
            }
        }
    }
    SID_log("%zd eligible for rendering...Done.", SID_LOG_CLOSE, mq->n_particles);
}
