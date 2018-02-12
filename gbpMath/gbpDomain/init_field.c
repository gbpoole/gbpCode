// Initialize the field, k/real-space domains and FFTW plans
#include <stdio.h>
#include <stdlib.h>
#include <gbpLib.h>
#include <gbpDomain.h>

void init_field(int n_d, int *n, double *L, field_info *FFT) {
    int  i_d;
    int  i_i;
    ptrdiff_t  n_x_local;
    ptrdiff_t  i_x_start_local;
    ptrdiff_t  n_y_transpose_local;
    ptrdiff_t  i_y_start_transpose_local;
    int *n_x_rank;
    int  i_rank;
    int  j_rank;
    int  flag_active;
    int  n_active;
    int  min_size, max_size;

    SID_log("Initializing ", SID_LOG_OPEN);
    for(i_d = 0; i_d < n_d; i_d++) {
        if(i_d < (n_d - 1))
            SID_log("%dx", SID_LOG_CONTINUE, n[i_d]);
        else
            SID_log("%d element %d-d FFT ", SID_LOG_CONTINUE, n[i_d], n_d);
    }
    SID_log("(%d byte precision)...", SID_LOG_CONTINUE, (int)sizeof(GBPREAL));

    // Initialize FFT sizes
    FFT->n_d             = n_d;
    FFT->n               = (int *)SID_calloc(sizeof(int) * FFT->n_d);
    FFT->L               = (double *)SID_calloc(sizeof(double) * FFT->n_d);
    FFT->n_k_local       = (int *)SID_calloc(sizeof(int) * FFT->n_d);
    FFT->n_R_local       = (int *)SID_calloc(sizeof(int) * FFT->n_d);
    FFT->i_R_start_local = (int *)SID_calloc(sizeof(int) * FFT->n_d);
    FFT->i_k_start_local = (int *)SID_calloc(sizeof(int) * FFT->n_d);
    FFT->i_R_stop_local  = (int *)SID_calloc(sizeof(int) * FFT->n_d);
    FFT->i_k_stop_local  = (int *)SID_calloc(sizeof(int) * FFT->n_d);
    for(i_d = 0; i_d < FFT->n_d; i_d++) {
        FFT->n[i_d]               = n[i_d];
        FFT->L[i_d]               = L[i_d];
        FFT->i_R_start_local[i_d] = 0;
        FFT->i_k_start_local[i_d] = 0;
        FFT->n_R_local[i_d]       = FFT->n[i_d];
        FFT->n_k_local[i_d]       = FFT->n[i_d];
    }
    FFT->n_k_local[FFT->n_d - 1] = FFT->n[FFT->n_d - 1] / 2 + 1;

// Initialize FFTW
#if USE_MPI
#if USE_FFTW2
    int total_local_size_int;
    int n_x_local_int;
    int i_x_start_local_int;
    int n_y_transpose_local_int;
    int i_y_start_transpose_local_int;
    FFT->plan  = rfftwnd_mpi_create_plan(SID.COMM_WORLD->comm, FFT->n_d, FFT->n, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
    FFT->iplan = rfftwnd_mpi_create_plan(SID.COMM_WORLD->comm, FFT->n_d, FFT->n, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);
    rfftwnd_mpi_local_sizes(FFT->plan,
                            &(n_x_local_int),
                            &(i_x_start_local_int),
                            &(n_y_transpose_local_int),
                            &(i_y_start_transpose_local_int),
                            &total_local_size_int);
    n_x_local=n_x_local_int;
    i_x_start_local=i_x_start_local_int;
    n_y_transpose_local=n_y_transpose_local_int;
    i_y_start_transpose_local=i_y_start_transpose_local_int;
    FFT->total_local_size = (size_t)total_local_size_int;
#else
#ifdef USE_DOUBLE
    fftw_mpi_init();
    FFT->total_local_size = fftw_mpi_local_size_many_transposed(FFT->n_d,
                                                                FFT->n,
                                                                1,
                                                                FFTW_MPI_DEFAULT_BLOCK,
                                                                FFTW_MPI_DEFAULT_BLOCK,
                                                                SID_COMM_WORLD->comm,
                                                                &(n_x_local),
                                                                &(i_x_start_local),
                                                                &(n_y_transpose_local),
                                                                &(i_y_start_transpose_local));
#else
    fftwf_mpi_init();
    FFT->total_local_size   = fftwf_mpi_local_size_many_transposed(FFT->n_d,
                                                                 FFT->n,
                                                                 1,
                                                                 FFTW_MPI_DEFAULT_BLOCK,
                                                                 FFTW_MPI_DEFAULT_BLOCK,
                                                                 SID_COMM_WORLD->comm,
                                                                 &(n_x_local),
                                                                 &(i_x_start_local),
                                                                 &(n_y_transpose_local),
                                                                 &(i_y_start_transpose_local));
#endif
#endif

    // Set empty slabs to start at 0 to make ignoring them simple.
    if(n_x_local == 0)
        i_x_start_local = 0;
    if(n_y_transpose_local == 0)
        i_y_start_transpose_local = 0;

    // Modify the local slab dimensions according to what FFTW chose.
    FFT->i_R_start_local[0] = i_x_start_local;
    FFT->n_R_local[0]       = n_x_local;
    if(FFT->n_d > 1) {
        FFT->i_k_start_local[1] = i_y_start_transpose_local;
        FFT->n_k_local[1]       = n_y_transpose_local;
    }

    // Allocate field
#if USE_FFTW2
        FFT->field_local = (GBPREAL *)SID_malloc(sizeof(GBPREAL *)*FFT->total_local_size);
#else
        FFT->field_local = fftwf_alloc_real(FFT->total_local_size);
#endif
    FFT->cfield_local = (fftw_complex *)FFT->field_local;

// Generate plans
#if !USE_FFTW2
#ifdef USE_DOUBLE
    FFT->plan  = fftw_mpi_plan_dft_r2c(FFT->n_d, FFT->n, FFT->field_local, FFT->cfield_local, SID_COMM_WORLD->comm, FFTW_ESTIMATE);
    FFT->iplan = fftw_mpi_plan_dft_c2r(FFT->n_d, FFT->n, FFT->cfield_local, FFT->field_local, SID_COMM_WORLD->comm, FFTW_ESTIMATE);
#else
    FFT->plan  = fftwf_mpi_plan_dft_r2c(FFT->n_d, FFT->n, FFT->field_local, FFT->cfield_local, SID_COMM_WORLD->comm, FFTW_ESTIMATE);
    FFT->iplan = fftwf_mpi_plan_dft_c2r(FFT->n_d, FFT->n, FFT->cfield_local, FFT->field_local, SID_COMM_WORLD->comm, FFTW_ESTIMATE);
#endif
#endif
#else

    for(i_d = 0, FFT->total_local_size = 1; i_d < FFT->n_d; i_d++) {
        if(i_d < FFT->n_d - 1)
            FFT->total_local_size *= FFT->n[i_d];
        else
            FFT->total_local_size *= 2 * (FFT->n[i_d] / 2 + 1);
    }
#if USE_FFTW2
    FFT->plan         = rfftwnd_create_plan(FFT->n_d, FFT->n, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE | FFTW_IN_PLACE);
    FFT->iplan        = rfftwnd_create_plan(FFT->n_d, FFT->n, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE | FFTW_IN_PLACE);
#else
#ifdef USE_DOUBLE
    FFT->plan  = fftw_plan_dft_r2c(FFT->n_d, FFT->n, FFT->field_local, FFT->cfield_local, FFTW_ESTIMATE);
    FFT->iplan = fftw_plan_dft_c2r(FFT->n_d, FFT->n, FFT->cfield_local, FFT->field_local, FFTW_ESTIMATE);
#else
    FFT->plan  = fftwf_plan_dft_r2c(FFT->n_d, FFT->n, FFT->field_local, FFT->cfield_local, FFTW_ESTIMATE);
    FFT->iplan = fftwf_plan_dft_c2r(FFT->n_d, FFT->n, FFT->cfield_local, FFT->field_local, FFTW_ESTIMATE);
#endif
#endif
#endif

    // Upper limits of slab decomposition
    for(i_d = 0; i_d < FFT->n_d; i_d++) {
        FFT->i_R_stop_local[i_d] = FFT->i_R_start_local[i_d] + FFT->n_R_local[i_d] - 1;
        FFT->i_k_stop_local[i_d] = FFT->i_k_start_local[i_d] + FFT->n_k_local[i_d] - 1;
    }

    // FFTW padding sizes
    if(FFT->n_d > 1) {
        FFT->pad_size_R = 2 * (FFT->n_R_local[FFT->n_d - 1] / 2 + 1) - FFT->n_R_local[FFT->n_d - 1];
        FFT->pad_size_k = 0;
    } else {
        FFT->pad_size_R = 0;
        FFT->pad_size_k = 0;
    }

    // Number of elements (global and local) in the FFT
    for(i_d = 0, FFT->n_field = 1, FFT->n_field_R_local = 1, FFT->n_field_k_local = 1; i_d < FFT->n_d; i_d++) {
        FFT->n_field *= (size_t)FFT->n[i_d];
        FFT->n_field_R_local *= (size_t)FFT->n_R_local[i_d];
        FFT->n_field_k_local *= (size_t)FFT->n_k_local[i_d];
    }

    // Clear the field
    clear_field(FFT);

    // Initialize the FFT's real-space grid
    FFT->R_field = (double **)SID_malloc(sizeof(double *) * FFT->n_d);
    FFT->dR      = (double *)SID_malloc(sizeof(double *) * FFT->n_d);
    for(i_d = 0; i_d < FFT->n_d; i_d++) {
        FFT->R_field[i_d] = (double *)SID_malloc(sizeof(double) * (FFT->n[i_d] + 1));
        FFT->dR[i_d]      = FFT->L[i_d] / (double)(FFT->n[i_d]);
        for(i_i = 0; i_i < FFT->n[i_d]; i_i++)
            FFT->R_field[i_d][i_i] = FFT->L[i_d] * ((double)i_i / (double)(FFT->n[i_d]));
        FFT->R_field[i_d][FFT->n[i_d]] = FFT->L[i_d];
    }

    // Initialize the FFT's k-space grid
    FFT->k_field   = (double **)SID_malloc(sizeof(double *) * FFT->n_d);
    FFT->dk        = (double *)SID_malloc(sizeof(double *) * FFT->n_d);
    FFT->k_Nyquist = (double *)SID_malloc(sizeof(double *) * FFT->n_d);
    for(i_d = 0; i_d < FFT->n_d; i_d++) {
        FFT->k_field[i_d]   = (double *)SID_malloc(sizeof(double) * FFT->n[i_d]);
        FFT->dk[i_d]        = TWO_PI / FFT->L[i_d];
        FFT->k_Nyquist[i_d] = TWO_PI * (double)(FFT->n[i_d]) / FFT->L[i_d] / 2.;
        for(i_i = 0; i_i < FFT->n[i_d]; i_i++) {
            if(i_i >= FFT->n[i_d] / 2)
                FFT->k_field[i_d][i_i] = TWO_PI * (double)(i_i - FFT->n[i_d]) / FFT->L[i_d];
            else
                FFT->k_field[i_d][i_i] = TWO_PI * (double)(i_i) / FFT->L[i_d];
        }
    }

    // Flags
    FFT->flag_padded = GBP_FALSE;

    // Slab info
    FFT->slab.n_x_local       = FFT->n_R_local[0];
    FFT->slab.i_x_start_local = FFT->i_R_start_local[0];
    FFT->slab.i_x_stop_local  = FFT->i_R_stop_local[0];
    FFT->slab.x_min_local     = FFT->R_field[0][FFT->i_R_start_local[0]];
    if(FFT->slab.n_x_local > 0)
        FFT->slab.x_max_local = FFT->R_field[0][FFT->i_R_stop_local[0] + 1];
    else
        FFT->slab.x_max_local = FFT->slab.x_min_local;
    SID_Allreduce(&(FFT->slab.x_max_local), &(FFT->slab.x_max), 1, SID_DOUBLE, SID_MAX, SID_COMM_WORLD);

#if USE_MPI
    // All ranks are not necessarily assigned any slices, so
    //   we need to figure out what ranks are to the right and the left for
    //   buffer exchanges
    n_x_rank              = (int *)SID_malloc(sizeof(int) * SID.n_proc);
    n_x_rank[SID.My_rank] = FFT->slab.n_x_local;
    if(n_x_rank[SID.My_rank] > 0)
        flag_active = GBP_TRUE;
    else
        flag_active = GBP_FALSE;
    SID_Allreduce(&flag_active, &n_active, 1, SID_INT, SID_SUM, SID_COMM_WORLD);
    SID_Allreduce(&n_x_rank[SID.My_rank], &min_size, 1, SID_INT, SID_MIN, SID_COMM_WORLD);
    SID_Allreduce(&n_x_rank[SID.My_rank], &max_size, 1, SID_INT, SID_MAX, SID_COMM_WORLD);
    for(i_rank = 0; i_rank < SID.n_proc; i_rank++)
        SID_Bcast(&(n_x_rank[i_rank]), 1, SID_INT, i_rank, SID_COMM_WORLD);
    FFT->slab.rank_to_right = -1;
    for(i_rank = SID.My_rank + 1; i_rank < SID.My_rank + SID.n_proc && FFT->slab.rank_to_right < 0; i_rank++) {
        j_rank = i_rank % SID.n_proc;
        if(n_x_rank[j_rank] > 0)
            FFT->slab.rank_to_right = j_rank;
    }
    if(FFT->slab.rank_to_right < 0)
        FFT->slab.rank_to_right = SID.My_rank;
    FFT->slab.rank_to_left = -1;
    for(i_rank = SID.My_rank - 1; i_rank > SID.My_rank - SID.n_proc && FFT->slab.rank_to_left < 0; i_rank--) {
        if(i_rank < 0)
            j_rank = i_rank + SID.n_proc;
        else
            j_rank = i_rank;
        if(n_x_rank[j_rank] > 0)
            FFT->slab.rank_to_left = j_rank;
    }
    if(FFT->slab.rank_to_left < 0)
        FFT->slab.rank_to_left = SID.My_rank;
    free(n_x_rank);
    SID_log("(%d cores unused, min/max slab size=%d/%d)...", SID_LOG_CONTINUE, SID.n_proc - n_active, min_size, max_size);
#else
    FFT->slab.rank_to_right = SID.My_rank;
    FFT->slab.rank_to_left  = SID.My_rank;
    if(FFT->slab.n_x_local > 0) {
        flag_active = GBP_TRUE;
        n_active    = 1;
        min_size    = FFT->slab.n_x_local;
        max_size    = FFT->slab.n_x_local;
    } else {
        flag_active = GBP_FALSE;
        n_active    = 0;
        min_size    = 0;
        max_size    = 0;
    }
#endif

    SID_log("Done.", SID_LOG_CLOSE);
}
