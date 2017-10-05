#include <gbpLib.h>
#include <gbpFFT.h>

void compute_iFFT(field_info *FFT){
  int i_k;

  SID_log("Performing iFFT...",SID_LOG_OPEN|SID_LOG_TIMER);

  // Perform the inverse FFT
  #ifdef USE_DOUBLE
    fftw_execute((const fftw_plan)FFT->iplan);
  #else
    fftwf_execute((const fftwf_plan)FFT->iplan);
  #endif

  // Divide-out the FFTW scaling with N
  for(i_k=0;i_k<FFT->total_local_size;i_k++)
    FFT->field_local[i_k]/=(GBPREAL)FFT->n_field;

  // Remove the FFTW padding if we need to
  remove_buffer_FFT_R(FFT);

  SID_log("Done.",SID_LOG_CLOSE);  
}
