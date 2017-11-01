#include <gbpLib.h>
#include <gbpFFT.h>

void compute_FFT(field_info *FFT) {
    // Add the FFTW padding if we need to.  The log message
    //   is only needed if we do add a buffer.
    int flag_log_message = GBP_FALSE;
    if(add_buffer_FFT_R(FFT)) {
        SID_log("Performing FFT...", SID_LOG_OPEN | SID_LOG_TIMER);
        flag_log_message = GBP_TRUE;
    }

// Perform the FFT
#ifdef USE_DOUBLE
    fftw_execute((const fftw_plan)FFT->plan);
#else
    fftwf_execute((const fftwf_plan)FFT->plan);
#endif

    if(flag_log_message)
        SID_log("Done.", SID_LOG_CLOSE);
}
