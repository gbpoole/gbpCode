#include <gbpLib.h>
#include <gbpFFT.h>

size_t index_FFT_k(field_info *FFT, int *i_k) {
    int    i_d, j_d;
    size_t index_out;
    for(j_d = 0, index_out = 0; j_d < FFT->n_d; j_d++) {
        i_d = j_d;
#if USE_MPI
        if(j_d == 1)
            i_d = 0;
        else if(j_d == 0)
            i_d = 1;
#endif
        if(i_d == FFT->n_d - 1 && FFT->flag_padded)
            index_out *= (FFT->n_k_local[i_d] + 1);
        else
            index_out *= FFT->n_k_local[i_d];
        index_out += i_k[i_d];
    }
    return (index_out);
}
