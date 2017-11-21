#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void render_frame(render_info *render) {
    // Check that the camera is sealed
    check_camera_sealed(render->camera, GBP_TRUE);

    // Fetch some needed simulation stuff
    double box_size         = ((double *)ADaPS_fetch(render->plist_list[0]->data,
                                             "box_size"))[0]; // This way, we are sure that box_size is in the same units as the particle quantities
    double h_Hubble         = render->h_Hubble;
    double expansion_factor = render->camera->perspective->time;

    // Set some things depending on what space we're rendering in
    char   unit_text[32];
    int    kernel_flag         = SPH_KERNEL_GADGET;
    double unit_factor         = 1.;
    int    flag_velocity_space = render->camera->flag_velocity_space;
    if(flag_velocity_space) {
        SID_log("Rendering in velocity-space.", SID_LOG_COMMENT);
        sprintf(unit_text, "km/s");
        kernel_flag = SPH_KERNEL_GAUSSIAN;
        unit_factor = 1e3;
    } else {
        SID_log("Rendering in real-space.", SID_LOG_COMMENT);
        sprintf(unit_text, "Mpc/h");
        kernel_flag = SPH_KERNEL_GADGET;
        unit_factor = M_PER_MPC / h_Hubble;
    }

    // Put all the render distances/sizes into the Gadget
    //    units of the relevant space (r or v-space)
    double x_o               = unit_factor * render->camera->perspective->p_o[0];
    double y_o               = unit_factor * render->camera->perspective->p_o[1];
    double z_o               = unit_factor * render->camera->perspective->p_o[2];
    double x_c               = unit_factor * render->camera->perspective->p_c[0];
    double y_c               = unit_factor * render->camera->perspective->p_c[1];
    double z_c               = unit_factor * render->camera->perspective->p_c[2];
    double FOV_x_image_plane = unit_factor * render->camera->perspective->FOV_x_image_plane;
    double FOV_y_image_plane = unit_factor * render->camera->perspective->FOV_y_image_plane;
    double focus_shift_x     = unit_factor * render->camera->perspective->focus_shift_x;
    double focus_shift_y     = unit_factor * render->camera->perspective->focus_shift_y;
    double d_near_field      = unit_factor * render->camera->perspective->d_near_field;
    double d_taper_field     = unit_factor * render->camera->perspective->d_taper_field;
    double d_image_plane     = unit_factor * render->camera->perspective->d_image_plane;
    double stereo_offset     = unit_factor * render->camera->perspective->stereo_offset;
    if(d_near_field > 0.)
        SID_log("Near  field  = %le [%s]", SID_LOG_COMMENT, d_near_field / unit_factor, unit_text);
    if(d_taper_field > 0.)
        SID_log("Taper field  = %le [%s]", SID_LOG_COMMENT, d_taper_field / unit_factor, unit_text);
    if(d_near_field > 0. || d_taper_field > 0.)
        SID_log("Image plane  = %le [%s]", SID_LOG_COMMENT, d_image_plane / unit_factor, unit_text);

    // Sanity check the taper distances
    double taper_width = d_taper_field - d_near_field;
    if(taper_width < 0.)
        SID_exit_error(
                "The near-field distance (%le [%s]) must be less than the taper distance (%le [%s]) if non-zero.",
                SID_ERROR_LOGIC, d_near_field * unit_factor, unit_text, d_taper_field * unit_factor, unit_text);

    // Make sure absorption is >=0
    double f_absorption = render->f_absorption;
    if(f_absorption < 0.)
        f_absorption = 0.;
    if(f_absorption > 0.)
        SID_log("f_absorption = %le", SID_LOG_COMMENT, f_absorption);

    // Fetch a bunch of other things dictating how/what we are rendering
    int    flag_comoving       = render->flag_comoving;
    int    flag_fade           = render->flag_fade;
    int    flag_force_periodic = render->flag_force_periodic;
    int    flag_add_absorption = render->flag_add_absorption;
    int    v_mode              = render->v_mode;
    int    w_mode              = render->w_mode;
    double alpha_fade          = render->alpha_fade;
    int    camera_mode         = render->camera->camera_mode;
    double f_image_plane       = render->camera->f_image_plane;

    // Set image scales
    int    nx           = render->camera->width;
    int    ny           = render->camera->height;
    int    n_pixels     = nx * ny;
    double pixel_size_x = FOV_x_image_plane / (double)nx;
    double pixel_size_y = FOV_y_image_plane / (double)ny;
    double pixel_size   = 0.5 * (pixel_size_x + pixel_size_y);
    double pixel_area   = pixel_size_x * pixel_size_y;
    if(fabs((pixel_size_x - pixel_size_y) / pixel_size_x) > 1e-4)
        SID_log_warning("pixels are not square by %7.3f%%", 0, fabs((pixel_size_x - pixel_size_y) / pixel_size_x) * 1e2);

    // Generate (or read) the smoothing kernal
    set_sph_kernel(
        &(render->kernel_radius), &(render->kernel_table_3d), &(render->kernel_table), &(render->kernel_table_avg), kernel_flag | SPH_KERNEL_2D);
    double *kernel_radius      = render->kernel_radius;
    double *kernel_table       = render->kernel_table;
    double  kernel_table_avg   = render->kernel_table_avg;
    double  radius_kernel_max  = kernel_radius[N_KERNEL_TABLE];
    double  radius_kernel_max2 = radius_kernel_max * radius_kernel_max;

    // Loop over the left/right stereo pair (if necessary)
    int i_image = 1;
    if(SID_CHECK_BITFIELD_SWITCH(camera_mode, CAMERA_STEREO))
        i_image = 0;
    for(; i_image < 2; i_image++) {
        SID_log("Projecting to a %dx%d pixel array...", SID_LOG_OPEN | SID_LOG_TIMER, nx, ny);

        // Set the stereo offset
        int stereo_offset_dir = 0; // descibes the sence of the stereo offset (-1 is left, 0 is none, +1 is right)
        switch(i_image) {
            case 0:
                stereo_offset_dir = -1;
                break;
            case 1:
                if(SID_CHECK_BITFIELD_SWITCH(camera_mode, CAMERA_STEREO))
                    stereo_offset_dir = 1;
                else
                    stereo_offset_dir = 0;
                break;
            // Shouldn't make it here
            default:
                SID_exit_error("Invalid value for i_image (%d).", SID_ERROR_LOGIC, i_image);
                break;
        }

        // Generate depth array
        set_camera_depths(render, stereo_offset_dir);
        int n_depth = render->camera->n_depth;

        // Fetch pointers to arrays of the images we want to produce
        double **temp_image = NULL;
        double **RGB_image  = NULL;
        double **Y_image    = NULL;
        double **RY_image   = NULL;
        double **GY_image   = NULL;
        double **BY_image   = NULL;
        switch(i_image) {
            // Left image
            case 0:
                if(render->camera->image_RGB_left != NULL) {
                    RGB_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                    for(int i_depth = 0; i_depth < n_depth; i_depth++)
                        fetch_image_array(render->camera->image_RGB_left, i_depth, &(RGB_image[i_depth]));
                }
                if(render->camera->image_RGBY_left != NULL) {
                    temp_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                    for(int i_depth = 0; i_depth < n_depth; i_depth++)
                        fetch_image_array(render->camera->image_RGBY_left, i_depth, &(temp_image[i_depth]));
                }
                if(render->camera->image_Y_left != NULL) {
                    Y_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                    for(int i_depth = 0; i_depth < n_depth; i_depth++)
                        fetch_image_array(render->camera->image_Y_left, i_depth, &(Y_image[i_depth]));
                }
                if(render->camera->image_RY_left != NULL) {
                    RY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                    for(int i_depth = 0; i_depth < n_depth; i_depth++)
                        fetch_image_array(render->camera->image_RY_left, i_depth, &(RY_image[i_depth]));
                }
                if(render->camera->image_GY_left != NULL) {
                    GY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                    for(int i_depth = 0; i_depth < n_depth; i_depth++)
                        fetch_image_array(render->camera->image_GY_left, i_depth, &(GY_image[i_depth]));
                }
                if(render->camera->image_BY_left != NULL) {
                    BY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                    for(int i_depth = 0; i_depth < n_depth; i_depth++)
                        fetch_image_array(render->camera->image_BY_left, i_depth, &(BY_image[i_depth]));
                }
                break;
            case 1:
                // Right image
                if(SID_CHECK_BITFIELD_SWITCH(camera_mode, CAMERA_STEREO)) {
                    if(render->camera->image_RGB_right != NULL) {
                        RGB_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_RGB_right, i_depth, &(RGB_image[i_depth]));
                    }
                    if(render->camera->image_RGBY_right != NULL) {
                        temp_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_RGBY_right, i_depth, &(temp_image[i_depth]));
                    }
                    if(render->camera->image_Y_right != NULL) {
                        Y_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_Y_right, i_depth, &(Y_image[i_depth]));
                    }
                    if(render->camera->image_RY_right != NULL) {
                        RY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_RY_right, i_depth, &(RY_image[i_depth]));
                    }
                    if(render->camera->image_GY_right != NULL) {
                        GY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_GY_right, i_depth, &(GY_image[i_depth]));
                    }
                    if(render->camera->image_BY_right != NULL) {
                        BY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_BY_right, i_depth, &(BY_image[i_depth]));
                    }
                }
                // Mono image (stereo turned off)
                else {
                    if(render->camera->image_RGB != NULL) {
                        RGB_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_RGB, i_depth, &(RGB_image[i_depth]));
                    }
                    if(render->camera->image_RGBY != NULL) {
                        temp_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_RGBY, i_depth, &(temp_image[i_depth]));
                    }
                    if(render->camera->image_Y != NULL) {
                        Y_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_Y, i_depth, &(Y_image[i_depth]));
                    }
                    if(render->camera->image_RY != NULL) {
                        RY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_RY, i_depth, &(RY_image[i_depth]));
                    }
                    if(render->camera->image_GY != NULL) {
                        GY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_GY, i_depth, &(GY_image[i_depth]));
                    }
                    if(render->camera->image_BY != NULL) {
                        BY_image = (double **)SID_malloc(sizeof(double *) * n_depth);
                        for(int i_depth = 0; i_depth < n_depth; i_depth++)
                            fetch_image_array(render->camera->image_BY, i_depth, &(BY_image[i_depth]));
                    }
                }
                break;
        }

        // Set stereo offset
        double camera_stereo_offset = stereo_offset * (double)stereo_offset_dir;

        // Compute geometrical information needed for the projection (this needs to be
        //    done separately in this loop because it changes for mono/left/right cameras)
        double FOV_x_object_plane = 0.;
        double FOV_y_object_plane = 0.;
        double d_o                = 0.;
        double x_o_out            = 0.;
        double y_o_out            = 0.;
        double z_o_out            = 0.;
        double x_c_out            = 0.;
        double y_c_out            = 0.;
        double z_c_out            = 0.;
        double x_hat              = 0.;
        double y_hat              = 0.;
        double z_hat              = 0.;
        double theta              = 0.;
        double theta_roll         = 0.;
        compute_perspective_transformation(render,
                                           stereo_offset_dir,
                                           &FOV_x_object_plane,
                                           &FOV_y_object_plane,
                                           &d_o, // Includes f_image_plane correction
                                           &x_o_out,
                                           &y_o_out,
                                           &z_o_out,
                                           &x_c_out,
                                           &y_c_out,
                                           &z_c_out,
                                           &x_hat,
                                           &y_hat,
                                           &z_hat,
                                           &theta,
                                           &theta_roll);
        FOV_x_object_plane *= unit_factor;
        FOV_y_object_plane *= unit_factor;

        // Cast depth array into the units of the render
        double *depth_array = (double *)SID_malloc(sizeof(double) * n_depth);
        for(int i_depth = 0; i_depth < n_depth; i_depth++)
            depth_array[i_depth] = GBP_MAX(0., unit_factor * render->camera->depth_array[i_depth]);

        // Set physical image-plane domain
        double xmin = -0.5 * FOV_x_image_plane; // Things will be centred on (x_o,y_o,z_o) later
        double ymin = -0.5 * FOV_y_image_plane; // Things will be centred on (x_o,y_o,z_o) later
        xmin -= camera_stereo_offset;

        // Initialize arrays needed for rendering (perform transformations
        //   of positions, generate weightings, etc.)
        float * weight             = NULL;
        float * value              = NULL;
        char *  colour             = NULL;
        float * x                  = NULL;
        float * y                  = NULL;
        float * z                  = NULL;
        float * h_smooth           = NULL;
        float * f_stretch          = NULL;
        size_t *z_index            = NULL;
        int     i_x_min_local      = 0;
        int     i_x_max_local      = 0;
        size_t  n_particles        = 0;
        int     flag_line_integral = GBP_TRUE;
        int     flag_weigh         = GBP_TRUE;

        // If we are including absorption, then the particles
        //   will need to be distributed differently, rendered
        //   in order for most distant to closest, etc ... so
        //   we have to treat the two cases separately.
        if(flag_add_absorption)
            init_make_map_abs(render,
                              stereo_offset_dir,
                              unit_factor,
                              unit_text,
                              f_image_plane,
                              box_size,
                              FOV_x_object_plane,
                              FOV_y_object_plane,
                              xmin,
                              ymin,
                              pixel_size_x,
                              pixel_size_y,
                              radius_kernel_max,
                              nx,
                              ny,
                              expansion_factor,
                              focus_shift_x,
                              focus_shift_y,
                              d_near_field,
                              camera_stereo_offset,
                              flag_comoving,
                              flag_force_periodic,
                              camera_mode,
                              &flag_weigh,
                              &flag_line_integral,
                              &x,
                              &y,
                              &z,
                              &h_smooth,
                              &f_stretch,
                              &value,
                              &weight,
                              &colour,
                              &z_index,
                              &i_x_min_local,
                              &i_x_max_local,
                              &n_particles);
        else
            init_make_map_noabs(render,
                                stereo_offset_dir,
                                unit_factor,
                                unit_text,
                                f_image_plane,
                                box_size,
                                FOV_x_object_plane,
                                FOV_y_object_plane,
                                xmin,
                                ymin,
                                pixel_size_x,
                                pixel_size_y,
                                radius_kernel_max,
                                nx,
                                ny,
                                expansion_factor,
                                focus_shift_x,
                                focus_shift_y,
                                d_near_field,
                                camera_stereo_offset,
                                flag_comoving,
                                flag_force_periodic,
                                camera_mode,
                                &flag_weigh,
                                &flag_line_integral,
                                &x,
                                &y,
                                &z,
                                &h_smooth,
                                &f_stretch,
                                &value,
                                &weight,
                                &colour,
                                &z_index,
                                &i_x_min_local,
                                &i_x_max_local,
                                &n_particles);

        // Clear image arrays
        for(int i_depth = 0; i_depth < n_depth; i_depth++) {
            for(int i_pixel = 0; i_pixel < n_pixels; i_pixel++) {
                if(RGB_image != NULL)
                    RGB_image[i_depth][i_pixel] = 0.;
                if(temp_image != NULL)
                    temp_image[i_depth][i_pixel] = 0.;
                if(Y_image != NULL)
                    Y_image[i_depth][i_pixel] = 0.;
                if(RY_image != NULL)
                    RY_image[i_depth][i_pixel] = 0.;
                if(GY_image != NULL)
                    GY_image[i_depth][i_pixel] = 0.;
                if(BY_image != NULL)
                    BY_image[i_depth][i_pixel] = 0.;
            }
        }

        // Allocate and clear a mask array
        char *mask = (char *)SID_malloc(sizeof(char) * n_pixels);
        for(int i_pixel = 0; i_pixel < n_pixels; i_pixel++)
            mask[i_pixel] = GBP_FALSE;

        // Initialize a progress counter
        size_t        n_particles_used_local = 0;
        size_t        n_particles_used       = 0;
        pcounter_info pcounter;
        SID_init_pcounter(&pcounter, n_particles, 10);

        // This array keeps track of which depth planes a particle gets added to
        char *depth_flags = (char *)SID_malloc(sizeof(char) * n_depth);

        // Perform projection
        SID_log("Performing projection...", SID_LOG_OPEN | SID_LOG_TIMER);
        n_particles_used_local = 0;
        for(size_t ii_particle = 0; ii_particle < n_particles; ii_particle++) {
            size_t i_particle = z_index[n_particles - 1 - ii_particle];
            double z_i        = (double)z[i_particle];

            // Only render particles that are behind
            //    the near-field cut-off
            if(z_i > d_near_field) {
                // Set pixel-space ranges and positions
                double part_h_z      = (double)h_smooth[i_particle];
                double part_h_xy     = part_h_z * f_stretch[i_particle];
                double radius_kernel = part_h_xy;
                if(fpclassify(radius_kernel) == FP_NORMAL) {
                    double radius2_norm = 1. / (part_h_xy * part_h_xy);
                    double part_pos_x   = (double)(x[i_particle] * f_stretch[i_particle]);
                    double part_pos_y   = (double)(y[i_particle] * f_stretch[i_particle]);
                    int    kx_min       = (int)((part_pos_x - radius_kernel - xmin) / pixel_size_x);
                    int    kx_max       = (int)((part_pos_x + radius_kernel - xmin) / pixel_size_x + ONE_HALF);
                    int    ky_min       = (int)((part_pos_y - radius_kernel - ymin) / pixel_size_y);
                    int    ky_max       = (int)((part_pos_y + radius_kernel - ymin) / pixel_size_y + ONE_HALF);

                    // Compute any potential fading (alpha_fade=2 for normal inverse-square fading)
                    double f_fade = 1.;
                    if(flag_fade)
                        f_fade = pow(d_image_plane / z_i, -alpha_fade);

                    // Compute any potential tapering
                    double f_taper = 1.;
                    if(taper_width > 0. && z_i < d_taper_field)
                        f_taper = (z_i - d_near_field) / taper_width;

                    // Combine dimming factors into one
                    double f_dim = f_taper * f_fade;

                    // Set the particle values and weights
                    double R_i = 1.;
                    double G_i = 1.;
                    double B_i = 1.;
                    if(colour != NULL) {
                        R_i = RGB_lookup(render, colour[i_particle], 0);
                        G_i = RGB_lookup(render, colour[i_particle], 1);
                        B_i = RGB_lookup(render, colour[i_particle], 2);
                    }
                    double v_i  = (double)value[i_particle];
                    double w_i  = (double)weight[i_particle];
                    double vw_i = v_i * w_i;
                    double zw_i = z_i * w_i;

                    // Determine which image depth(s)
                    //    to add this particle to
                    int flag_particle_used     = GBP_FALSE;
                    int flag_particle_in_depth = GBP_FALSE;
                    for(int i_depth = 0; i_depth < n_depth; i_depth++) {
                        if(z_i >= depth_array[i_depth]) {
                            depth_flags[i_depth]   = GBP_TRUE;
                            flag_particle_in_depth = GBP_TRUE;
                        } else
                            depth_flags[i_depth] = GBP_FALSE;
                    }

                    // Loop over the kernal
                    double pixel_pos_x = xmin + (kx_min + 0.5) * pixel_size_x;
                    for(int kx = kx_min; kx <= kx_max && flag_particle_in_depth; kx++, pixel_pos_x += pixel_size_x) {
                        if(kx >= 0 && kx < nx) {
                            double pixel_pos_y = ymin + (ky_min + 0.5) * pixel_size_y;
                            for(int ky = ky_min; ky <= ky_max; ky++, pixel_pos_y += pixel_size_y) {
                                if(ky >= 0 && ky < ny) {
                                    double radius2 = (pixel_pos_x - part_pos_x) * (pixel_pos_x - part_pos_x) +
                                                     (pixel_pos_y - part_pos_y) * (pixel_pos_y - part_pos_y);
                                    radius2 *= radius2_norm;
                                    if(radius2 < radius_kernel_max2) {
                                        // Perform kernel interpolation
                                        int    pos     = ky + kx * ny;
                                        double f_table = sqrt(radius2) / radius_kernel_max;
                                        int    i_table = (int)(f_table * (double)N_KERNEL_TABLE);
                                        double kernel  = kernel_table[i_table] + (kernel_table[i_table + 1] - kernel_table[i_table]) *
                                                                                    (f_table - kernel_radius[i_table]) * (double)N_KERNEL_TABLE;
                                        double w_k = w_i * kernel;

                                        // Perform addition.  Add to all image depths <= the one the particle
                                        //   is in so that the depths accumulate.  This way, absorption
                                        //   works the way it is coded and the first image represents the
                                        //   final sum.
                                        flag_particle_used = GBP_TRUE; // depth has already been checked for
                                        if(Y_image != NULL) {
                                            for(int i_depth = 0; i_depth < n_depth; i_depth++) {
                                                if(depth_flags[i_depth])
                                                    Y_image[i_depth][pos] += (f_dim - f_absorption * Y_image[i_depth][pos]) * w_k;
                                            }
                                        }
                                        if(temp_image != NULL) {
                                            for(int i_depth = 0; i_depth < n_depth; i_depth++)
                                                if(depth_flags[i_depth])
                                                    temp_image[i_depth][pos] += (f_dim * v_i - f_absorption * temp_image[i_depth][pos]) * w_k;
                                        }
                                        if(RY_image != NULL) {
                                            for(int i_depth = 0; i_depth < n_depth; i_depth++)
                                                if(depth_flags[i_depth])
                                                    RY_image[i_depth][pos] += (f_dim * R_i - f_absorption * RY_image[i_depth][pos]) * w_k;
                                        }
                                        if(GY_image != NULL) {
                                            for(int i_depth = 0; i_depth < n_depth; i_depth++)
                                                if(depth_flags[i_depth])
                                                    GY_image[i_depth][pos] += (f_dim * G_i - f_absorption * GY_image[i_depth][pos]) * w_k;
                                        }
                                        if(BY_image != NULL) {
                                            for(int i_depth = 0; i_depth < n_depth; i_depth++)
                                                if(depth_flags[i_depth])
                                                    BY_image[i_depth][pos] += (f_dim * B_i - f_absorption * BY_image[i_depth][pos]) * w_k;
                                        }
                                        mask[pos] = GBP_TRUE;
                                    }
                                }
                            } // loop over kernel
                        }
                    } // loop over kernel
                    // Count the particles used
                    if(flag_particle_used)
                        n_particles_used_local++;
                } // check on fp_classify
            }     // near-field selection
            SID_check_pcounter(&pcounter, ii_particle);
        } // loop over particles
        SID_Barrier(SID_COMM_WORLD);
        SID_Allreduce(&n_particles_used_local, &n_particles_used, 1, SID_SIZE_T, SID_SUM, SID_COMM_WORLD);
        SID_log("n_particles_used=%zd", SID_LOG_COMMENT, n_particles_used);
        SID_log("Done.", SID_LOG_CLOSE);

        SID_log("Image normalization, etc...", SID_LOG_OPEN | SID_LOG_TIMER);

        // Add results from all ranks if this is being run in parallel
        //   First, clear the parts of the image not in this rank's domain ...
        for(int i_depth = 0; i_depth < n_depth; i_depth++) {
            for(int kx = 0; kx < nx; kx++) {
                if(!(kx >= i_x_min_local && kx <= i_x_max_local)) {
                    for(int ky = 0; ky < ny; ky++) {
                        int pos = ky + kx * ny;
                        if(temp_image != NULL)
                            temp_image[i_depth][pos] = 0.;
                        if(Y_image != NULL)
                            Y_image[i_depth][pos] = 0.;
                        if(RY_image != NULL) {
                            RY_image[i_depth][pos] = 0.;
                            GY_image[i_depth][pos] = 0.;
                            BY_image[i_depth][pos] = 0.;
                        }
                    }
                }
            }
        }

// Join mask results.  The MPI 1.1 standard does not specify MPI_SUM
//   on MPI_CHAR types so we need to do this awkward thing for the mask array ...
#if USE_MPI
        int *mask_buffer = (int *)SID_malloc(sizeof(int) * nx);
        for(int i_y = 0; i_y < ny; i_y++) {
            for(int i_x = 0, i_pixel = i_y * nx; i_x < nx; i_x++, i_pixel++)
                mask_buffer[i_x] = (int)mask[i_pixel];
            SID_Allreduce(SID_IN_PLACE, mask_buffer, nx, SID_INT, SID_MAX, SID_COMM_WORLD);
            for(int i_x = 0, i_pixel = i_y * nx; i_x < nx; i_x++, i_pixel++) {
                if(mask_buffer[i_x])
                    mask[i_pixel] = GBP_TRUE;
            }
        }
        SID_free(SID_FARG mask_buffer);
#endif
        for(int i_depth = 0; i_depth < n_depth; i_depth++) {
            SID_Allreduce(SID_IN_PLACE, temp_image[i_depth], n_pixels, SID_DOUBLE, SID_SUM, SID_COMM_WORLD);
            SID_Allreduce(SID_IN_PLACE, Y_image[i_depth], n_pixels, SID_DOUBLE, SID_SUM, SID_COMM_WORLD);
            if(RY_image != NULL) {
                SID_Allreduce(SID_IN_PLACE, RY_image[i_depth], n_pixels, SID_DOUBLE, SID_SUM, SID_COMM_WORLD);
                SID_Allreduce(SID_IN_PLACE, GY_image[i_depth], n_pixels, SID_DOUBLE, SID_SUM, SID_COMM_WORLD);
                SID_Allreduce(SID_IN_PLACE, BY_image[i_depth], n_pixels, SID_DOUBLE, SID_SUM, SID_COMM_WORLD);
            }
        }

        // Create final normalized images and clear the temp_image which has been used as a buffer
        if(RGB_image != NULL) {
            for(int i_depth = 0; i_depth < n_depth; i_depth++) {
                for(int i_pixel = 0; i_pixel < n_pixels; i_pixel++) {
                    if(mask[i_pixel])
                        RGB_image[i_depth][i_pixel] = temp_image[i_depth][i_pixel] / Y_image[i_depth][i_pixel];
                    temp_image[i_depth][i_pixel] = 0.;
                }
            }
        }
        if(RY_image != NULL) {
            for(int i_depth = 0; i_depth < n_depth; i_depth++) {
                for(int i_pixel = 0; i_pixel < n_pixels; i_pixel++) {
                    if(mask[i_pixel])
                        RY_image[i_depth][i_pixel] = RY_image[i_depth][i_pixel] / Y_image[i_depth][i_pixel];
                }
            }
        }
        if(GY_image != NULL) {
            for(int i_depth = 0; i_depth < n_depth; i_depth++) {
                for(int i_pixel = 0; i_pixel < n_pixels; i_pixel++) {
                    if(mask[i_pixel])
                        GY_image[i_depth][i_pixel] = GY_image[i_depth][i_pixel] / Y_image[i_depth][i_pixel];
                }
            }
        }
        if(BY_image != NULL) {
            for(int i_depth = 0; i_depth < n_depth; i_depth++) {
                for(int i_pixel = 0; i_pixel < n_pixels; i_pixel++) {
                    if(mask[i_pixel])
                        BY_image[i_depth][i_pixel] = BY_image[i_depth][i_pixel] / Y_image[i_depth][i_pixel];
                }
            }
        }

        // Compute some image statistics
        double min_RGB_image = 0.;
        double max_RGB_image = 0.;
        double min_Y_image   = 0.;
        double max_Y_image   = 0.;
        double min_RY_image  = 0.;
        double max_RY_image  = 0.;
        double min_GY_image  = 0.;
        double max_GY_image  = 0.;
        double min_BY_image  = 0.;
        double max_BY_image  = 0.;
        int    n_unmasked    = 0;
        for(int i_pixel = 0; i_pixel < n_pixels; i_pixel++)
            if(mask[i_pixel])
                n_unmasked++;
        if(RGB_image != NULL) {
            calc_min(RGB_image[0], &min_RGB_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
            calc_max(RGB_image[0], &max_RGB_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
        }
        if(Y_image != NULL) {
            calc_min(Y_image[0], &min_Y_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
            calc_max(Y_image[0], &max_Y_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
        }
        if(RY_image != NULL) {
            calc_min(RY_image[0], &min_RY_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
            calc_max(RY_image[0], &max_RY_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
        }
        if(GY_image != NULL) {
            calc_min(GY_image[0], &min_GY_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
            calc_max(GY_image[0], &max_GY_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
        }
        if(BY_image != NULL) {
            calc_min(BY_image[0], &min_BY_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
            calc_max(BY_image[0], &max_BY_image, n_pixels, SID_DOUBLE, CALC_MODE_DEFAULT);
        }
        SID_log("Done.", SID_LOG_CLOSE);

        // Report image statistics
        if(n_unmasked > 0) {
            SID_log("Image statistics:", SID_LOG_OPEN);
            SID_log("coverage=%3d%%", SID_LOG_COMMENT, (int)(100. * (double)n_unmasked / (double)n_pixels));
            if(RGB_image != NULL) {
                SID_log("RGB min =%le", SID_LOG_COMMENT, min_RGB_image);
                SID_log("RGB max =%le", SID_LOG_COMMENT, max_RGB_image);
            }
            if(Y_image != NULL) {
                SID_log("Y   min =%le", SID_LOG_COMMENT, min_Y_image);
                SID_log("Y   max =%le", SID_LOG_COMMENT, max_Y_image);
            }
            if(RY_image != NULL) {
                SID_log("RY  min =%le", SID_LOG_COMMENT, min_RY_image);
                SID_log("RY  max =%le", SID_LOG_COMMENT, max_RY_image);
            }
            if(GY_image != NULL) {
                SID_log("GY  min =%le", SID_LOG_COMMENT, min_GY_image);
                SID_log("GY  max =%le", SID_LOG_COMMENT, max_GY_image);
            }
            if(BY_image != NULL) {
                SID_log("BY  min =%le", SID_LOG_COMMENT, min_BY_image);
                SID_log("BY  max =%le", SID_LOG_COMMENT, max_BY_image);
            }
            SID_log("", SID_LOG_SILENT_CLOSE);
        } else
            SID_log("IMAGE IS EMPTY.", SID_LOG_COMMENT);

        // Clean-up
        SID_free(SID_FARG x);
        SID_free(SID_FARG y);
        SID_free(SID_FARG z);
        SID_free(SID_FARG h_smooth);
        SID_free(SID_FARG f_stretch);
        SID_free(SID_FARG z_index);
        SID_free(SID_FARG value);
        SID_free(SID_FARG weight);
        SID_free(SID_FARG colour);
        SID_free(SID_FARG mask);
        SID_free(SID_FARG temp_image);
        SID_free(SID_FARG RGB_image);
        SID_free(SID_FARG Y_image);
        SID_free(SID_FARG RY_image);
        SID_free(SID_FARG GY_image);
        SID_free(SID_FARG BY_image);
        SID_free(SID_FARG depth_flags);
        SID_free(SID_FARG depth_array);

        SID_log("Done.", SID_LOG_CLOSE);
    }
}
