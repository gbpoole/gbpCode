#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpSPH.h>
#include <gbpCosmo.h>
#include <gbpRender.h>

void init_make_map_abs(render_info *render,
                       int          stereo_offset_dir,
                       double       unit_factor,
                       const char * unit_text,
                       double       f_image_plane,
                       double       box_size,
                       double       FOV_x_in,
                       double       FOV_y_in,
                       double       xmin,
                       double       ymin,
                       double       pixel_size_x,
                       double       pixel_size_y,
                       double       radius_kernel_max,
                       int          nx,
                       int          ny,
                       double       expansion_factor,
                       double       focus_shift_x,
                       double       focus_shift_y,
                       double       d_near_field,
                       double       stereo_offset,
                       int          flag_comoving,
                       int          flag_force_periodic,
                       int          camera_mode,
                       int *        flag_weigh,
                       int *        flag_line_integral,
                       float **     x,
                       float **     y,
                       float **     z,
                       float **     h_smooth,
                       float **     f_stretch,
                       float **     value,
                       float **     weight,
                       char **      colour,
                       size_t **    z_index,
                       int *        i_x_min_local_return,
                       int *        i_x_max_local_return,
                       size_t *     n_particles) {
    int *   ptype_used;
    int     i_type;
    float * rho;
    double *mass;
    float * sigma_v;
    size_t  i_particle;
    size_t  j_particle;
    size_t  k_particle;
    double  mass_array;
    size_t  n_particles_species;
    float * x_temp;
    float * y_temp;
    float * z_temp;
    float * h_smooth_temp;
    int     flag_log;
    double  z_test;
    int     flag_use_Gadget;
    float   box_size_float = (float)box_size;
    float   half_box       = 0.5 * box_size_float;

    SID_log("Initializing projection-space...", SID_LOG_OPEN | SID_LOG_TIMER);

    // Plane parallel projection?
    int flag_plane_parallel;
    if(check_mode_for_flag(camera_mode, CAMERA_PLANE_PARALLEL))
        flag_plane_parallel = GBP_TRUE;
    else
        flag_plane_parallel = GBP_FALSE;

    // Report the state of some flags
    if(flag_plane_parallel)
        SID_log("Plane-parallel  projection: ON", SID_LOG_COMMENT);
    else
        SID_log("Plane-parallel  projection: OFF", SID_LOG_COMMENT);
    if(flag_comoving)
        SID_log("Comoving        projection: ON", SID_LOG_COMMENT);
    else
        SID_log("Comoving        projection: OFF", SID_LOG_COMMENT);
    if(flag_force_periodic)
        SID_log("Centre periodic projection: ON", SID_LOG_COMMENT);
    else
        SID_log("Centre periodic projection: OFF", SID_LOG_COMMENT);

    // Initialize the mapping quantities
    map_quantities_info mq;
    init_particle_map_quantities(&mq, render, render->camera->transfer_list, flag_comoving, expansion_factor);
    (*n_particles)        = mq.n_particles;
    (*flag_weigh)         = mq.flag_weigh;
    (*flag_line_integral) = mq.flag_line_integral;
    ptype_used            = mq.ptype_used;

    double FOV_x      = FOV_x_in;
    double FOV_y      = FOV_y_in;
    double d_o        = 0.;
    double x_o        = 0.;
    double y_o        = 0.;
    double z_o        = 0.;
    double x_c        = 0.;
    double y_c        = 0.;
    double z_c        = 0.;
    double x_hat      = 0.;
    double y_hat      = 0.;
    double z_hat      = 0.;
    double theta      = 0.;
    double theta_roll = 0.;
    compute_perspective_transformation(
        render, stereo_offset_dir, &FOV_x, &FOV_y, &d_o, &x_o, &y_o, &z_o, &x_c, &y_c, &z_c, &x_hat, &y_hat, &z_hat, &theta, &theta_roll);
    d_o *= unit_factor;
    x_o *= unit_factor;
    y_o *= unit_factor;
    z_o *= unit_factor;
    x_c = unit_factor;
    y_c = unit_factor;
    z_c = unit_factor;

    // The previous line sets d_o to the object distance.  Compute the distance to the image plane.
    double d_image_plane = d_o * f_image_plane;

    // Set mark arrays
    int    flag_mark_on = GBP_FALSE;
    char **mark         = (char **)SID_malloc(sizeof(char *) * N_GADGET_TYPE);
    for(i_type = 0; i_type < N_GADGET_TYPE; i_type++) {
        if(ADaPS_exist(render->plist_list[0]->data, "mark_%s", render->plist_list[0]->species[i_type])) {
            mark[i_type] = (char *)ADaPS_fetch(render->plist_list[0]->data, "mark_%s", render->plist_list[0]->species[i_type]);
            flag_mark_on = GBP_TRUE;
        } else
            mark[i_type] = NULL;
    }

    // Determine how many particles are contributing to each
    //    column of the image
    float   x_i;
    float   y_i;
    float   z_i;
    float   h_i;
    float   f_i;
    float   v_i;
    float   w_i;
    char    c_i;
    int     i_x;
    size_t *n_column;
    n_column = (size_t *)SID_calloc(sizeof(size_t) * nx);
    SID_log("Compute slab domain decomposition...", SID_LOG_OPEN | SID_LOG_TIMER);
    for(i_type = 0, j_particle = 0; i_type < N_GADGET_TYPE; i_type++) {
        if(ptype_used[i_type] && ADaPS_exist(render->plist_list[0]->data, "n_%s", render->plist_list[0]->species[i_type])) {
            n_particles_species = ((size_t *)ADaPS_fetch(render->plist_list[0]->data, "n_%s", render->plist_list[0]->species[i_type]))[0];
            for(i_particle = 0, k_particle = j_particle; i_particle < n_particles_species; i_particle++, k_particle++) {
                if(check_if_particle_marked(mark, i_type, i_particle, &c_i)) {
                    // Set the preoperties of the particle to be mapped (mode is GBP_TRUE because we need the angular size of the particle)
                    set_particle_map_quantities(render, &mq, GBP_TRUE, k_particle, box_size_float, half_box, &x_i, &y_i, &z_i, &h_i, &v_i, &w_i);

                    // Transform particle to render-coordinates
                    transform_particle(&x_i,
                                       &y_i,
                                       &z_i,
                                       x_o,
                                       y_o,
                                       z_o,
                                       x_hat,
                                       y_hat,
                                       z_hat,
                                       d_o,
                                       stereo_offset,
                                       theta,
                                       theta_roll,
                                       box_size,
                                       expansion_factor,
                                       focus_shift_x,
                                       focus_shift_y,
                                       flag_comoving,
                                       flag_force_periodic);

                    if(z_i > d_near_field) {
                        double radius2_norm;
                        double radius_kernel;
                        double part_pos_x;
                        double part_pos_y;
                        int    kx_min;
                        int    kx_max;
                        int    ky_min;
                        int    ky_max;
                        int    n_ky;
                        f_i = (float)compute_f_stretch(d_image_plane, z_i, flag_plane_parallel);
                        set_pixel_space(h_i,
                                        x_i,
                                        y_i,
                                        f_i,
                                        xmin,
                                        ymin,
                                        FOV_x,
                                        FOV_y,
                                        pixel_size_x,
                                        pixel_size_y,
                                        radius_kernel_max,
                                        &radius2_norm,
                                        &radius_kernel,
                                        &part_pos_x,
                                        &part_pos_y,
                                        &kx_min,
                                        &kx_max,
                                        &ky_min,
                                        &ky_max);
                        kx_min = GBP_MAX(kx_min, 0);
                        kx_max = GBP_MIN(kx_max, nx - 1);
                        ky_min = GBP_MAX(ky_min, 0);
                        ky_max = GBP_MIN(ky_max, ny - 1);
                        n_ky   = (ky_max - ky_min + 1);
                        // It's not clear which of these two scalings should be used.  Scaling
                        //   with no. of pixels yields imperfect results.
                        if(n_ky > 0) {
                            for(i_x = kx_min; i_x <= kx_max; i_x++)
                                // n_column[i_x]+=n_ky; // Scale with no. of pixels
                                n_column[i_x]++; // Scale with no. of particles
                        }
                    }
                }
            }
            j_particle += n_particles_species;
        }
    }
    SID_Allreduce(SID_IN_PLACE, n_column, nx, SID_SIZE_T, SID_SUM, SID.COMM_WORLD);

    // Create cumulative histogram
    for(i_x = 1; i_x < nx; i_x++)
        n_column[i_x] += n_column[i_x - 1];

    // Decide on a range of columns to assign to each rank
    int    i_x_min_local;
    int    i_x_max_local;
    size_t norm      = n_column[nx - 1];
    size_t n_used    = 0;
    int    i_x_start = 1;
    int    i_rank;
    size_t n_target;
    for(i_rank = 0, i_x = 0; i_rank < SID.n_proc; i_rank++) {
        i_x_start = i_x;
        n_target  = n_used + (size_t)((float)(norm - n_used) / (float)(SID.n_proc - i_rank));
        while(i_x < (nx - 1) && n_column[i_x] < n_target)
            i_x++;
        if(i_rank == SID.My_rank) {
            i_x_min_local = i_x_start;
            i_x_max_local = i_x;
        }
        n_used = n_column[i_x];
        i_x++;
    }
    if(SID.I_am_last_rank)
        if(i_x_max_local < nx)
            i_x_max_local = nx - 1;
    SID_log("Done.", SID_LOG_CLOSE);

    // Create a couple arrays storing the domain decomposition for all
    int  i_x_min_send;
    int  i_x_max_send;
    int *i_x_min_rank;
    int *i_x_max_rank;
    i_x_min_rank = (int *)SID_malloc(sizeof(int) * SID.n_proc);
    i_x_max_rank = (int *)SID_malloc(sizeof(int) * SID.n_proc);
    for(i_rank = 0; i_rank < SID.n_proc; i_rank++) {
        i_x_min_send = i_x_min_local;
        i_x_max_send = i_x_max_local;
        SID_Bcast(&i_x_min_send, 1, SID_INT, i_rank, SID.COMM_WORLD);
        SID_Bcast(&i_x_max_send, 1, SID_INT, i_rank, SID.COMM_WORLD);
        i_x_min_rank[i_rank] = i_x_min_send;
        i_x_max_rank[i_rank] = i_x_max_send;
    }

    // Count the number of particles contributing to each rank
    size_t *n_rank;
    size_t *n_rank_local;
    size_t  n_particles_local;
    n_rank       = (size_t *)SID_calloc(sizeof(size_t) * SID.n_proc);
    n_rank_local = (size_t *)SID_calloc(sizeof(size_t) * SID.n_proc);
    SID_log("Count particles...", SID_LOG_OPEN | SID_LOG_TIMER);
    for(i_type = 0, j_particle = 0; i_type < N_GADGET_TYPE; i_type++) {
        if(ptype_used[i_type] && ADaPS_exist(render->plist_list[0]->data, "n_%s", render->plist_list[0]->species[i_type])) {
            n_particles_species = ((size_t *)ADaPS_fetch(render->plist_list[0]->data, "n_%s", render->plist_list[0]->species[i_type]))[0];
            for(i_particle = 0, k_particle = j_particle; i_particle < n_particles_species; i_particle++, k_particle++) {
                if(check_if_particle_marked(mark, i_type, i_particle, &c_i)) {
                    // Set the preoperties of the particle to be mapped
                    set_particle_map_quantities(render, &mq, GBP_TRUE, k_particle, box_size_float, half_box, &x_i, &y_i, &z_i, &h_i, &v_i, &w_i);

                    // Transform particle to render-coordinates
                    transform_particle(&x_i,
                                       &y_i,
                                       &z_i,
                                       x_o,
                                       y_o,
                                       z_o,
                                       x_hat,
                                       y_hat,
                                       z_hat,
                                       d_o,
                                       stereo_offset,
                                       theta,
                                       theta_roll,
                                       box_size,
                                       expansion_factor,
                                       focus_shift_x,
                                       focus_shift_y,
                                       flag_comoving,
                                       flag_force_periodic);

                    if(z_i > d_near_field) {
                        double radius2_norm;
                        double radius_kernel;
                        double part_pos_x;
                        double part_pos_y;
                        int    kx_min;
                        int    kx_max;
                        int    ky_min;
                        int    ky_max;
                        int    n_ky;
                        f_i = (float)compute_f_stretch(d_image_plane, z_i, flag_plane_parallel);
                        set_pixel_space(h_i,
                                        x_i,
                                        y_i,
                                        f_i,
                                        xmin,
                                        ymin,
                                        FOV_x,
                                        FOV_y,
                                        pixel_size_x,
                                        pixel_size_y,
                                        radius_kernel_max,
                                        &radius2_norm,
                                        &radius_kernel,
                                        &part_pos_x,
                                        &part_pos_y,
                                        &kx_min,
                                        &kx_max,
                                        &ky_min,
                                        &ky_max);
                        kx_min = GBP_MAX(kx_min, 0);
                        kx_max = GBP_MIN(kx_max, nx - 1);
                        ky_min = GBP_MAX(ky_min, 0);
                        ky_max = GBP_MIN(ky_max, ny - 1);
                        n_ky   = (ky_max - ky_min + 1);
                        if(n_ky > 0) {
                            for(i_rank = 0; i_rank < SID.n_proc; i_rank++) {
                                if(kx_min <= i_x_max_rank[i_rank] && kx_max >= i_x_min_rank[i_rank])
                                    n_rank_local[i_rank]++;
                            }
                        }
                    }
                }
            }
            j_particle += n_particles_species;
        }
    }
    SID_Allreduce(n_rank_local, n_rank, SID.n_proc, SID_SIZE_T, SID_SUM, SID.COMM_WORLD);
    n_particles_local = n_rank[SID.My_rank];
    (*n_particles)    = n_particles_local;
    SID_free(SID_FARG n_column);
    SID_log("Done.", SID_LOG_CLOSE);

    // Report decomposition results
    if(SID.n_proc > 1) {
        SID_log("Results of domain decomposition:", SID_LOG_OPEN);
        for(i_rank = 0; i_rank < SID.n_proc; i_rank++)
            SID_log("Rank %03d image range: %5d->%5d n_particles=%zd",
                    SID_LOG_COMMENT,
                    i_rank,
                    i_x_min_rank[i_rank],
                    i_x_max_rank[i_rank],
                    n_rank[i_rank]);
        SID_log("", SID_LOG_CLOSE | SID_LOG_NOPRINT);
    }

    // Determine the needed size of the comm buffers
    size_t n_buffer;
    size_t n_buffer_max;
    calc_max(n_rank_local, &n_buffer, SID.n_proc, SID_SIZE_T, CALC_MODE_DEFAULT);
    SID_Allreduce(&n_buffer, &n_buffer_max, 1, SID_SIZE_T, SID_MAX, SID.COMM_WORLD);

    // Broadcast particles
    SID_log("Broadcast particles...(buffer=%zd particles)...", SID_LOG_OPEN | SID_LOG_TIMER, n_buffer_max);
    float *x_buffer;
    float *y_buffer;
    float *z_buffer;
    float *h_buffer;
    float *f_buffer;
    float *v_buffer;
    float *w_buffer;
    char * c_buffer;
    (*x)         = (float *)SID_malloc(sizeof(float) * n_particles_local);
    (*y)         = (float *)SID_malloc(sizeof(float) * n_particles_local);
    (*z)         = (float *)SID_malloc(sizeof(float) * n_particles_local);
    (*h_smooth)  = (float *)SID_malloc(sizeof(float) * n_particles_local);
    (*f_stretch) = (float *)SID_malloc(sizeof(float) * n_particles_local);
    (*value)     = (float *)SID_malloc(sizeof(float) * n_particles_local);
    (*weight)    = (float *)SID_malloc(sizeof(float) * n_particles_local);
    x_buffer     = (float *)SID_malloc(sizeof(float) * n_buffer);
    y_buffer     = (float *)SID_malloc(sizeof(float) * n_buffer);
    z_buffer     = (float *)SID_malloc(sizeof(float) * n_buffer);
    h_buffer     = (float *)SID_malloc(sizeof(float) * n_buffer);
    f_buffer     = (float *)SID_malloc(sizeof(float) * n_buffer);
    v_buffer     = (float *)SID_malloc(sizeof(float) * n_buffer);
    w_buffer     = (float *)SID_malloc(sizeof(float) * n_buffer);
    if(flag_mark_on) {
        (*colour) = (char *)SID_malloc(sizeof(char) * n_particles_local);
        c_buffer  = (char *)SID_malloc(sizeof(char) * n_buffer);
    } else {
        (*colour) = NULL;
        c_buffer  = NULL;
    }
    size_t i_particle_rank;
    size_t j_particle_rank = 0;
    for(i_rank = 0; i_rank < SID.n_proc; i_rank++) {
        int rank_to;
        int rank_from;
        set_exchange_ring_ranks(&rank_to, &rank_from, i_rank);
        i_particle_rank = 0;
        for(i_type = 0, j_particle = 0; i_type < N_GADGET_TYPE; i_type++) {
            if(ptype_used[i_type]) {
                n_particles_species = ((size_t *)ADaPS_fetch(render->plist_list[0]->data, "n_%s", render->plist_list[0]->species[i_type]))[0];
                for(i_particle = 0, k_particle = j_particle; i_particle < n_particles_species; i_particle++, k_particle++) {
                    if(check_if_particle_marked(mark, i_type, i_particle, &c_i)) {
                        // Set the preoperties of the particle to be mapped
                        set_particle_map_quantities(render, &mq, GBP_TRUE, k_particle, box_size_float, half_box, &x_i, &y_i, &z_i, &h_i, &v_i, &w_i);

                        // Transform particle to render-coordinates
                        transform_particle(&x_i,
                                           &y_i,
                                           &z_i,
                                           x_o,
                                           y_o,
                                           z_o,
                                           x_hat,
                                           y_hat,
                                           z_hat,
                                           d_o,
                                           stereo_offset,
                                           theta,
                                           theta_roll,
                                           box_size,
                                           expansion_factor,
                                           focus_shift_x,
                                           focus_shift_y,
                                           flag_comoving,
                                           flag_force_periodic);

                        if(z_i > d_near_field) {
                            double radius2_norm;
                            double radius_kernel;
                            double part_pos_x;
                            double part_pos_y;
                            int    kx_min;
                            int    kx_max;
                            int    ky_min;
                            int    ky_max;
                            int    n_ky;
                            f_i = (float)compute_f_stretch(d_image_plane, z_i, flag_plane_parallel);
                            set_pixel_space(h_i,
                                            x_i,
                                            y_i,
                                            f_i,
                                            xmin,
                                            ymin,
                                            FOV_x,
                                            FOV_y,
                                            pixel_size_x,
                                            pixel_size_y,
                                            radius_kernel_max,
                                            &radius2_norm,
                                            &radius_kernel,
                                            &part_pos_x,
                                            &part_pos_y,
                                            &kx_min,
                                            &kx_max,
                                            &ky_min,
                                            &ky_max);
                            kx_min = GBP_MAX(kx_min, 0);
                            kx_max = GBP_MIN(kx_max, nx - 1);
                            ky_min = GBP_MAX(ky_min, 0);
                            ky_max = GBP_MIN(ky_max, ny - 1);
                            n_ky   = (ky_max - ky_min + 1);
                            if(n_ky > 0) {
                                if(kx_min <= i_x_max_rank[rank_to] && kx_max >= i_x_min_rank[rank_to]) {
                                    x_buffer[i_particle_rank] = x_i;
                                    y_buffer[i_particle_rank] = y_i;
                                    z_buffer[i_particle_rank] = z_i;
                                    h_buffer[i_particle_rank] = h_i;
                                    f_buffer[i_particle_rank] = f_i;
                                    w_buffer[i_particle_rank] = w_i;
                                    v_buffer[i_particle_rank] = v_i;
                                    if(c_buffer != NULL)
                                        c_buffer[i_particle_rank] = (char)c_i;
                                    i_particle_rank++;
                                }
                            }
                        }
                    }
                }
                j_particle += n_particles_species;
            }
        } // loop over particles

        // Perform exchanges
        size_t n_exchange;
        exchange_ring_buffer(x_buffer, sizeof(float), n_rank_local[rank_to], &((*x)[j_particle_rank]), &n_exchange, i_rank);
        exchange_ring_buffer(y_buffer, sizeof(float), n_rank_local[rank_to], &((*y)[j_particle_rank]), &n_exchange, i_rank);
        exchange_ring_buffer(z_buffer, sizeof(float), n_rank_local[rank_to], &((*z)[j_particle_rank]), &n_exchange, i_rank);
        exchange_ring_buffer(h_buffer, sizeof(float), n_rank_local[rank_to], &((*h_smooth)[j_particle_rank]), &n_exchange, i_rank);
        exchange_ring_buffer(f_buffer, sizeof(float), n_rank_local[rank_to], &((*f_stretch)[j_particle_rank]), &n_exchange, i_rank);
        exchange_ring_buffer(w_buffer, sizeof(float), n_rank_local[rank_to], &((*weight)[j_particle_rank]), &n_exchange, i_rank);
        exchange_ring_buffer(v_buffer, sizeof(float), n_rank_local[rank_to], &((*value)[j_particle_rank]), &n_exchange, i_rank);
        if(c_buffer != NULL)
            exchange_ring_buffer(c_buffer, sizeof(char), n_rank_local[rank_to], &((*colour)[j_particle_rank]), &n_exchange, i_rank);
        j_particle_rank += n_exchange;
    }

    // Sanity checks
    if(j_particle_rank != n_particles_local)
        SID_exit_error("The wrong number of particles were received (ie. %zd!=%zd) on rank %d.", SID_ERROR_LOGIC,
                       j_particle_rank, n_particles_local, SID.My_rank);

    SID_log("Done.", SID_LOG_CLOSE);

    // Sort the local particles by position
    merge_sort((*z), (size_t)(*n_particles), z_index, SID_FLOAT, SORT_COMPUTE_INDEX, GBP_FALSE);

    // Clean-up
    SID_free(SID_FARG n_rank);
    SID_free(SID_FARG n_rank_local);
    SID_free(SID_FARG i_x_min_rank);
    SID_free(SID_FARG i_x_max_rank);
    SID_free(SID_FARG x_buffer);
    SID_free(SID_FARG y_buffer);
    SID_free(SID_FARG z_buffer);
    SID_free(SID_FARG h_buffer);
    SID_free(SID_FARG f_buffer);
    SID_free(SID_FARG v_buffer);
    SID_free(SID_FARG w_buffer);
    SID_free(SID_FARG c_buffer);
    free_particle_map_quantities(&mq);
    SID_free(SID_FARG mark);

    (*i_x_min_local_return) = i_x_min_local;
    (*i_x_max_local_return) = i_x_max_local;

    SID_log("Done.", SID_LOG_CLOSE);
}
