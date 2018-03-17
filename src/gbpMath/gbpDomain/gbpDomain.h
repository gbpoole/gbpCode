#ifndef GBPDOMAIN_AWAKE
#define GBPDOMAIN_AWAKE
#ifndef GBPFFTW_AWAKE
#define GBPFFTW_AWAKE
#include <stddef.h>

#if USE_FFTW
 #if USE_FFTW2
  #if USE_MPI
   #if USE_DOUBLE
    #include <drfftw_mpi.h>
   #else
    #include <srfftw_mpi.h>
   #endif
  #else
   #if USE_DOUBLE
    #include <drfftw.h>
   #else
    #include <srfftw.h>
   #endif
  #endif
 #else
  #if USE_MPI
   #include <fftw3-mpi.h>
  #else
   #include <fftw3.h>
  #endif
 #endif
#endif
#include <gbpLib.h> // Needed for GBPREAL definition

typedef struct slab_info slab_info;
struct slab_info {
    double x_min_local;
    double x_max_local;
    double x_max;
    int    n_x_local;
    int    i_x_start_local;
    int    i_x_stop_local;
    int    rank_to_left;
    int    rank_to_right;
};

#if USE_FFTW
#if USE_FFTW2
#define gbpFFT_real    fftw_real
#define gbpFFT_complex fftw_complex
#else
#define gbpFFT_real    GBPREAL
#define gbpFFT_complex fftw_complex
#endif
#else
#define gbpFFT_real    GBPREAL
#define gbpFFT_complex GBPREAL
#endif

typedef struct field_info field_info;
struct field_info {
    // Array storing the field
    gbpFFT_real *   field_local;
    gbpFFT_complex *cfield_local;
    // Field domain
    double * L;
    double * dR;
    double * dk;
    double **R_field;
    double **k_field;
    double * k_Nyquist;
    // Field sizes
    int    n_d;
    ptrdiff_t *  n;
    ptrdiff_t *  n_R_local;
    ptrdiff_t *  n_k_local;
    ptrdiff_t *  i_R_start_local;
    ptrdiff_t *  i_k_start_local;
    ptrdiff_t *  i_R_stop_local;
    ptrdiff_t *  i_k_stop_local;
    size_t n_field;
    size_t n_field_R_local;
    size_t n_field_k_local;
    size_t total_local_size;
    int    pad_size_R;
    int    pad_size_k;
    // flags
    int flag_padded;
// FFTW plans
#if USE_FFTW
#if USE_FFTW2
#if USE_MPI
    rfftwnd_mpi_plan plan;
    rfftwnd_mpi_plan iplan;
#else
    rfftwnd_plan plan;
    rfftwnd_plan iplan;
#endif
#else
#ifdef USE_DOUBLE
    fftw_plan plan;
    fftw_plan iplan;
#else
    fftwf_plan plan;
    fftwf_plan iplan;
#endif
#endif
#endif
    // Slab info
    slab_info slab;
};

// Function declarations
#ifdef __cplusplus
extern "C" {
#endif
void init_field(int n_d, int *n, double *L, field_info *FFT);
void free_field(field_info *FFT);
void clear_field(field_info *FFT);
void exchange_slab_buffer_left(void *     send_buffer,
                               size_t     send_buffer_size,
                               void *     receive_buffer,
                               size_t *   receive_buffer_size,
                               slab_info *slab);
void exchange_slab_buffer_right(void *     send_buffer,
                                size_t     send_buffer_size,
                                void *     receive_buffer,
                                size_t *   receive_buffer_size,
                                slab_info *slab);
#ifdef __cplusplus
}
#endif

#endif
#endif
