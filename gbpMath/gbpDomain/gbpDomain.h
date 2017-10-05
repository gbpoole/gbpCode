#ifndef GBPDOMAIN_AWAKE
#define GBPDOMAIN_AWAKE
#if USE_MPI
   #include <fftw3-mpi.h>
#else
   #include <fftw3.h>
#endif
#include <gbpLib.h> // Needed for GBPREAL definition

typedef GBPREAL gbpFFT_complex[2];

typedef struct slab_info slab_info;
struct slab_info{
  double x_min_local;
  double x_max_local;
  double x_max;
  int    n_x_local;
  int    i_x_start_local;
  int    i_x_stop_local;
  int    rank_to_left;
  int    rank_to_right;
};

typedef struct field_info field_info;
struct field_info{
  // Array storing the field
  GBPREAL        *field_local;
  gbpFFT_complex *cfield_local;
  // Field domain
  double           *L;
  double           *dR;
  double           *dk;
  double          **R_field;
  double          **k_field;
  double           *k_Nyquist;
  // Field sizes
  int               n_d;
  int              *n;
  int              *n_R_local;
  int              *n_k_local;
  int              *i_R_start_local;
  int              *i_k_start_local;
  int              *i_R_stop_local;
  int              *i_k_stop_local;
  size_t            n_field;
  size_t            n_field_R_local;
  size_t            n_field_k_local;
  size_t            total_local_size;
  int               pad_size_R;
  int               pad_size_k;
  // flags
  int               flag_padded;
  // FFTW plans
#ifdef USE_DOUBLE
  fftw_plan plan;
  fftw_plan iplan;
#else
  fftwf_plan plan;
  fftwf_plan iplan;
#endif
  // Slab info
  slab_info         slab;
};

// Function declarations
#ifdef __cplusplus
extern "C" {
#endif
void init_field(int       n_d,
                int      *n,
                double   *L,
                field_info *FFT);
void free_field(field_info *FFT);
void clear_field(field_info *FFT);
void set_exchange_ring_ranks(int *rank_to,
                             int *rank_from,
                             int  i_rank);
void exchange_ring_buffer(void     *send_buffer,
                          size_t    buffer_type_size,
                          size_t    send_count,
                          void     *receive_buffer,
                          size_t   *receive_count,
                          int       i_rank);
void exchange_slab_buffer_left(void      *send_buffer,
                               size_t     send_buffer_size,
                               void      *receive_buffer,
                               size_t    *receive_buffer_size,
                               slab_info *slab);
void exchange_slab_buffer_right(void      *send_buffer,
                                size_t     send_buffer_size,
                                void      *receive_buffer,
                                size_t    *receive_buffer_size,
                                slab_info *slab);
#ifdef __cplusplus
}
#endif

#endif

