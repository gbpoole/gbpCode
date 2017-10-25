// Free memory allocated by initialize_field()
#include <stdio.h>
#include <stdlib.h>
#include <gbpLib.h>
#include <gbpDomain.h>

void free_field(field_info *FFT) {
    int i_d;

    SID_free(SID_FARG FFT->n_R_local);
    SID_free(SID_FARG FFT->n_k_local);
    SID_free(SID_FARG FFT->i_R_start_local);
    SID_free(SID_FARG FFT->i_k_start_local);
    SID_free(SID_FARG FFT->i_R_stop_local);
    SID_free(SID_FARG FFT->i_k_stop_local);

    // Free FFTs
#ifdef USE_DOUBLE
    fftw_destroy_plan(FFT->plan);
    fftw_destroy_plan(FFT->iplan);
#if USE_MPI
    fftw_mpi_cleanup();
#else
    fftw_cleanup();
#endif
#else
    fftwf_destroy_plan(FFT->plan);
    fftwf_destroy_plan(FFT->iplan);
#if USE_MPI
    fftwf_mpi_cleanup();
#else
    fftwf_cleanup();
#endif
#endif

    // Free field arrays
    for(i_d = 0; i_d < FFT->n_d; i_d++) {
        SID_free(SID_FARG FFT->k_field[i_d]);
        SID_free(SID_FARG FFT->R_field[i_d]);
    }
    SID_free(SID_FARG FFT->k_field);
    SID_free(SID_FARG FFT->R_field);
    SID_free(SID_FARG FFT->field_local);
    SID_free(SID_FARG FFT->dR);
    SID_free(SID_FARG FFT->dk);
    SID_free(SID_FARG FFT->k_Nyquist);
    SID_free(SID_FARG FFT->n);
    SID_free(SID_FARG FFT->L);
}
