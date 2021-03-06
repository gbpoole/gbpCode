#include <gbpLib.h>
#include <gbpDomain.h>
#include <gbpFFT.h>

void compute_iFFT(field_info *FFT) {

    SID_log("Performing iFFT...", SID_LOG_OPEN | SID_LOG_TIMER);

// Perform the inverse FFT
#if FFTW_V2
#if USE_MPI
    rfftwnd_mpi(FFT->iplan, 1, FFT->field_local, NULL, FFTW_TRANSPOSED_ORDER);
#else
    rfftwnd_one_complex_to_real(FFT->iplan, FFT->cfield_local, NULL);
#endif
#else
#ifdef USE_DOUBLE
    fftw_execute((const fftw_plan)FFT->iplan);
#else
    fftwf_execute((const fftwf_plan)FFT->iplan);
#endif
#endif

    // Divide-out the FFTW scaling with N
    for(size_t i_k = 0; i_k < FFT->total_local_size; i_k++)
        FFT->field_local[i_k] /= (GBPREAL)FFT->n_field;

    // Remove the FFTW padding if we need to
    remove_buffer_FFT_R(FFT);

    SID_log("Done.", SID_LOG_CLOSE);
}
