#include <gbpLib.h>
#include <gbpFFT.h>

size_t index_local_FFT_k(field_info *FFT, int *i_k) {
    int    i_d, j_d;
    size_t index;
    for(j_d = 0, index = 0; j_d < FFT->n_d; j_d++) {
        i_d = j_d;
// Invert the x-y place for FFTW
#if USE_MPI
        if(j_d == 1)
            i_d = 0;
        else if(j_d == 0)
            i_d = 1;
#endif
        if(i_d == FFT->n_d - 1 && FFT->flag_padded)
            index *= (FFT->n_k_local[i_d] + 1);
        else
            index *= FFT->n_k_local[i_d];
        if(i_k[i_d] < 0 || i_k[i_d] > FFT->i_k_stop_local[i_d])
            SID_trap_error("Index (%d;i_d=%d) out of local slab's range (%d->%d).",
                           SID_ERROR_LOGIC,
                           i_k[i_d],
                           i_d,
                           FFT->i_k_start_local[i_d],
                           FFT->i_k_stop_local[i_d]);
        index += (i_k[i_d] - FFT->i_k_start_local[i_d]);
    }
    return (index);
}
