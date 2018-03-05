#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

// Compute the angle and the axis of rotation
//   needed to place the camera at (0,0,-d_o)
//   with the object at (0,0,0)
void compute_perspective_transformation(render_info *render,
                                        int          flag_stereo_offset,
                                        double *     FOV_x, // return the object-plane FOV
                                        double *     FOV_y, // return the object-plane FOV
                                        double *     d_o,
                                        double *     x_o_out,
                                        double *     y_o_out,
                                        double *     z_o_out,
                                        double *     x_c_out,
                                        double *     y_c_out,
                                        double *     z_c_out,
                                        double *     x_hat,
                                        double *     y_hat,
                                        double *     z_hat,
                                        double *     theta,
                                        double *     theta_roll) {
    // Fetch a few things
    double x_o           = render->camera->perspective->p_o[0];
    double y_o           = render->camera->perspective->p_o[1];
    double z_o           = render->camera->perspective->p_o[2];
    double x_c           = render->camera->perspective->p_c[0];
    double y_c           = render->camera->perspective->p_c[1];
    double z_c           = render->camera->perspective->p_c[2];
    double f_image_plane = render->camera->f_image_plane;

    // Set the stero offset
    double stereo_offset = render->camera->perspective->stereo_offset * (double)flag_stereo_offset;

    // Precompute a few things for efficiency
    double d_x_o    = x_o - x_c;
    double d_y_o    = y_o - y_c;
    double d_z_o    = z_o - z_c;
    double d_x_o_2  = d_x_o * d_x_o;
    double d_y_o_2  = d_y_o * d_y_o;
    double d_z_o_2  = d_z_o * d_z_o;
    double d_xy     = sqrt(d_x_o_2 + d_y_o_2);
    double inv_d_xy = 1. / d_xy;

    // Set the distance between the object and the camera
    (*d_o) = sqrt(d_x_o_2 + d_y_o_2 + d_z_o_2);

    // Set the unit vector pointing between the two
    (*x_hat) = d_y_o * inv_d_xy;
    (*y_hat) = -d_x_o * inv_d_xy;
    (*z_hat) = 0.;

    // Set the roll angle
    (*theta) = acos(d_z_o / (*d_o));
    if((d_x_o_2 + d_y_o_2) > 0.) {
        (*theta_roll) = acos(-d_y_o / sqrt(d_x_o_2 + d_y_o_2));
        if(d_x_o < 0.)
            (*theta_roll) = TWO_PI - (*theta_roll);
    } else
        (*theta_roll) = 0.;

    // Apply stereo offsets to the
    //    object and camera positions
    (*x_o_out) = x_o;
    (*y_o_out) = y_o;
    (*z_o_out) = z_o;
    (*x_c_out) = x_c;
    (*y_c_out) = y_c;
    (*z_c_out) = z_c;
    if(stereo_offset != 0.) {
        double Dx_stereo;
        double Dy_stereo;
        double Dz_stereo;
        double theta_roll_stereo = 0.;
        double cos_theta_over_d_xy;
        SID_log("Forcing theta=0 in stereo projection.", SID_LOG_COMMENT);
        if(d_x_o > 0.) {
            if(d_y_o > 0.) {
                Dx_stereo = -stereo_offset * cos(theta_roll_stereo) * fabs(d_y_o) / d_xy;
                Dy_stereo = stereo_offset * cos(theta_roll_stereo) * fabs(d_x_o) / d_xy;
            } else {
                Dx_stereo = stereo_offset * cos(theta_roll_stereo) * fabs(d_y_o) / d_xy;
                Dy_stereo = stereo_offset * cos(theta_roll_stereo) * fabs(d_x_o) / d_xy;
            }
        } else {
            if(d_y_o > 0.) {
                Dx_stereo = -stereo_offset * cos(theta_roll_stereo) * fabs(d_y_o) / d_xy;
                Dy_stereo = -stereo_offset * cos(theta_roll_stereo) * fabs(d_x_o) / d_xy;
            } else {
                Dx_stereo = stereo_offset * cos(theta_roll_stereo) * fabs(d_y_o) / d_xy;
                Dy_stereo = -stereo_offset * cos(theta_roll_stereo) * fabs(d_x_o) / d_xy;
            }
        }
        if(theta_roll_stereo > 0.)
            Dz_stereo = sqrt(stereo_offset * stereo_offset - Dx_stereo * Dx_stereo - Dy_stereo * Dy_stereo);
        else if(theta_roll_stereo == 0.)
            Dz_stereo = 0.;
        else
            Dz_stereo = -sqrt(stereo_offset * stereo_offset - Dx_stereo * Dx_stereo - Dy_stereo * Dy_stereo);
        (*x_o_out) += Dx_stereo;
        (*y_o_out) += Dy_stereo;
        (*z_o_out) += Dz_stereo;
        (*x_c_out) += Dx_stereo;
        (*y_c_out) += Dy_stereo;
        (*z_c_out) += Dz_stereo;
    }

    // Recompute d_o to place the object in front/behind of the image plane
    // If f_image_plane>1, then the object is in front of the image plane
    //    by that factor.  FOV is input at the object plane but used at the image
    //    plane, so we need to make it bigger by that factor as well.
    (*FOV_x) = render->camera->perspective->FOV_x_image_plane;
    (*FOV_y) = render->camera->perspective->FOV_y_image_plane;
    (*d_o) /= f_image_plane;
    (*FOV_x) *= f_image_plane; // return the object-plane FOV
    (*FOV_y) *= f_image_plane; // return the object-plane FOV
}
