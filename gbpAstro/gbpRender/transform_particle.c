#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void transform_particle(GBPREAL *x_i,
                        GBPREAL *y_i,
                        GBPREAL *z_i,
                        double   x_o,
                        double   y_o,
                        double   z_o,
                        double   x_hat,
                        double   y_hat,
                        double   z_hat,
                        double   d_o,
                        double   stereo_offset,
                        double   theta,
                        double   theta_roll,
                        double   box_size,
                        double   expansion_factor,
                        double   focus_shift_x,
                        double   focus_shift_y,
                        int      flag_comoving,
                        int      flag_force_periodic) {
    // Shift to object-centred coordinates
    (*x_i) -= (GBPREAL)x_o;
    (*y_i) -= (GBPREAL)y_o;
    (*z_i) -= (GBPREAL)z_o;

    // Centre the periodic box
    if(!flag_comoving || flag_force_periodic) {
        force_periodic(x_i, -0.5 * (GBPREAL)box_size, (GBPREAL)box_size);
        force_periodic(y_i, -0.5 * (GBPREAL)box_size, (GBPREAL)box_size);
        force_periodic(z_i, -0.5 * (GBPREAL)box_size, (GBPREAL)box_size);
    }

    // Rotate about the origen to place camera position on z-axis
    rotate_particle(x_hat, y_hat, z_hat, theta, x_i, y_i, z_i);

    // Apply roll angle
    rotate_particle(0., 0., 1., (-theta_roll), x_i, y_i, z_i);

    // Convert to proper coordinates?
    if(!flag_comoving) {
        (*x_i) *= expansion_factor;
        (*y_i) *= expansion_factor;
        (*z_i) *= expansion_factor;
    }

    // Apply focus shift
    (*x_i) -= focus_shift_x;
    (*y_i) -= focus_shift_y;

    // Shift zero to the camera position
    (*z_i) += d_o;
}
