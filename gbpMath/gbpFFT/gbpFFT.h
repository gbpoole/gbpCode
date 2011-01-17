#ifndef GBPFFT_AWAKE
#define GBPFFT_AWAKE
#include <gbpDomain.h>
#ifndef GBPFFTW_AWAKE
  #define GBPFFTW_AWAKE
  #ifdef USE_MPI
    #ifdef USE_DOUBLE
      #include <drfftw_mpi.h>
    #else
      #include <srfftw_mpi.h>
    #endif
  #else
    #ifdef USE_DOUBLE
      #include <drfftw.h>
    #else
      #include <srfftw.h>
    #endif
  #endif
#endif

// Function definitions
void   compute_FFT(field_info *FFT);
void   compute_iFFT(field_info *FFT);
void   add_buffer_FFT_R(field_info *FFT);
void   add_buffer_FFT_k(field_info *FFT);
void   remove_buffer_FFT_R(field_info *FFT);
void   remove_buffer_FFT_k(field_info *FFT);
size_t index_FFT_R(field_info *FFT,int *i_R);
size_t index_FFT_k(field_info *FFT,int *i_k);
size_t index_local_FFT_R(field_info *FFT,int *i_R);
size_t index_local_FFT_k(field_info *FFT,int *i_k);
size_t pad_index_FFT_R(field_info *FFT,size_t index);
size_t pad_index_FFT_k(field_info *FFT,size_t index);
void   k_field_FFT(field_info *FFT,int *i_k,double *k_field);
void   R_field_FFT(field_info *FFT,int *i_R,double *R_field);
double k_mag_field_FFT(field_info *FFT,int *i_k);
void   index2indices_FFT_R(field_info *FFT,size_t index,int *i_R);
void   index2indices_FFT_k(field_info *FFT,size_t index,int *i_k);
#endif