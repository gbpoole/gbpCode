#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void set_particle_map_quantities(render_info *        render,
                                 map_quantities_info *mq,
                                 int                  mode,
                                 size_t               i_particle,
                                 float                box_size,
                                 float                half_box,
                                 float *              x_i,
                                 float *              y_i,
                                 float *              z_i,
                                 float *              h_i,
                                 float *              v_i,
                                 float *              w_i) {
    if(render->n_interpolate == 1) {
        (*x_i) = mq->x[0][i_particle];
        (*y_i) = mq->y[0][i_particle];
        (*z_i) = mq->z[0][i_particle];
    } else if(render->n_interpolate == 2) {
        float dx = (float)(mq->x[1][i_particle] - mq->x[0][i_particle]);
        float dy = (float)(mq->y[1][i_particle] - mq->y[0][i_particle]);
        float dz = (float)(mq->z[1][i_particle] - mq->z[0][i_particle]);
        if(dx > half_box)
            dx -= box_size;
        if(dx < -half_box)
            dx += box_size;
        if(dy > half_box)
            dy -= box_size;
        if(dy < -half_box)
            dy += box_size;
        if(dz > half_box)
            dz -= box_size;
        if(dz < -half_box)
            dz += box_size;
        (*x_i) = mq->x[0][i_particle] + render->f_interpolate * dx;
        (*y_i) = mq->y[0][i_particle] + render->f_interpolate * dy;
        (*z_i) = mq->z[0][i_particle] + render->f_interpolate * dz;
    } else
        SID_trap_error("n_interpolate>2 not supported (yet).", SID_ERROR_LOGIC);
    if(mode == GBP_TRUE) {
        if(render->n_interpolate == 1)
            (*h_i) = mq->h_smooth[0][i_particle];
        else if(render->n_interpolate == 2)
            (*h_i) = mq->h_smooth[0][i_particle] + render->f_interpolate * (mq->h_smooth[1][i_particle] - mq->h_smooth[0][i_particle]);
        // Set the particle weighting
        switch(mq->w_mode) {
            case MAKE_MAP_MODE_RHO:
                if(render->n_interpolate == 1)
                    (*w_i) = mq->rho[0][i_particle];
                else if(render->n_interpolate == 2) {
                    double rho_0 = mq->rho[0][i_particle];
                    double rho_1 = mq->rho[1][i_particle];
                    (*w_i)       = take_alog10(rho_0 + render->f_interpolate * (rho_1 - rho_0));
                }
                if(mq->transfer_rho != NULL) {
                    switch(mq->flag_transfer_rho_log) {
                        case GBP_TRUE:
                            (*w_i) *= GBP_MAX(0., GBP_MIN(1., interpolate(mq->transfer_rho, take_log10((double)(*w_i)))));
                            break;
                        case GBP_FALSE:
                            (*w_i) *= GBP_MAX(0., GBP_MIN(1., interpolate(mq->transfer_rho, (double)(*w_i))));
                            break;
                    }
                }
                /*
                if(!mq->flag_comoving)
                   (*w_i)*=mq->inv_expansion_factor_cubed;
                */
                break;
            case MAKE_MAP_INV_SIGMA:
                if(render->n_interpolate == 1)
                    (*w_i) = 1. / mq->sigma[0][i_particle];
                else if(render->n_interpolate == 2) {
                    double sigma_0 = 1. / mq->sigma[0][i_particle];
                    double sigma_1 = 1. / mq->sigma[1][i_particle];
                    (*w_i)         = take_alog10(sigma_0 + render->f_interpolate * (sigma_1 - sigma_0));
                }
                if(mq->transfer_sigma != NULL) {
                    switch(mq->flag_transfer_sigma_log) {
                        case GBP_TRUE:
                            (*w_i) *= GBP_MAX(0., GBP_MIN(1., interpolate(mq->transfer_sigma, take_log10((double)(*w_i)))));
                            break;
                        case GBP_FALSE:
                            (*w_i) *= GBP_MAX(0., GBP_MIN(1., interpolate(mq->transfer_sigma, (double)(*w_i))));
                            break;
                    }
                }
                break;
            case MAKE_MAP_NO_WEIGHTING:
                (*w_i) = 1.;
                break;
            default:
                SID_trap_error("Unknown w_mode (%d) in make_map.", SID_ERROR_LOGIC, mq->v_mode);
                break;
        }
        // Set the particle value
        switch(mq->v_mode) {
            case MAKE_MAP_MODE_SIGMA:
                if(render->n_interpolate == 1)
                    (*v_i) = mq->sigma[0][i_particle];
                else if(render->n_interpolate == 2) {
                    double sigma_0 = mq->sigma[0][i_particle];
                    double sigma_1 = mq->sigma[1][i_particle];
                    (*v_i)         = take_alog10(sigma_0 + render->f_interpolate * (sigma_1 - sigma_0));
                }
                if(mq->transfer_sigma != NULL) {
                    switch(mq->flag_transfer_sigma_log) {
                        case GBP_TRUE:
                            (*v_i) *= GBP_MAX(0., GBP_MIN(1., interpolate(mq->transfer_rho, take_log10((double)(*v_i)))));
                            break;
                        case GBP_FALSE:
                            (*v_i) *= GBP_MAX(0., GBP_MIN(1., interpolate(mq->transfer_rho, (double)(*v_i))));
                            break;
                    }
                }
                break;
            default:
                SID_trap_error("Unknown v_mode (%d) in make_map.", SID_ERROR_LOGIC, mq->v_mode);
                break;
        }
    }
}
