// Free memory allocated by initialize_field()
#include <gbpLib.h>
#include <gbpPHKs.h>

void compute_PHK_volume_keys(int PHK_bit_size, PHK_t key_in, int shell_min, int shell_max, int *n_keys_return, PHK_t **keys_return_in) {
    hikey_t  basecoord[3];
    hikey_t  coord[3];
    unsigned bits;

    // Some type conversions
    hikey_t   base;
    hikey_t **keys_return;
    bits        = (unsigned)PHK_bit_size;
    base        = (hikey_t)key_in;
    keys_return = (hikey_t **)keys_return_in;

    // Calculate the number of possible Key values per dimension
    hikey_t num1dim = (hikey_t)PHK_DIM_SIZE(bits);

    // Sanity checks
    if(shell_min < 0)
        SID_exit_error("shell_min<0 (ie %d<0) in compute_PHK_volume_keys", SID_ERROR_LOGIC, shell_min);
    if((hikey_t)shell_max > num1dim)
        SID_exit_error("shell_max>num1dim (ie %d>%d) in compute_PHK_volume_keys", SID_ERROR_LOGIC, shell_max, num1dim);
    if(shell_max < shell_min)
        SID_exit_error("shell_max<shell_min (ie %d<%d) in compute_PHK_volume_keys", SID_ERROR_LOGIC, shell_max,
                       shell_min);

    // Compute the number of keys to be returned
    (*n_keys_return) = (2 * shell_max + 1) * (2 * shell_max + 1) * (2 * shell_max + 1);
    if(shell_min > 0)
        (*n_keys_return) -= (2 * shell_min - 1) * (2 * shell_min - 1) * (2 * shell_min - 1);

    // Allocate for the key array only if it is NULL
    if((*keys_return_in) == NULL)
        (*keys_return_in) = (PHK_t *)SID_malloc((*n_keys_return) * sizeof(PHK_t));

    // Convert the Hilbert Key to a position
    hilbert_i2c(3, bits, base, basecoord);

    // Find keys
    size_t count = 0;
    for(size_t i = num1dim - shell_max; i <= num1dim + shell_max; i++) {
        coord[0] = (basecoord[0] + i) % num1dim;
        for(size_t j = num1dim - shell_max; j <= num1dim + shell_max; j++) {
            coord[1] = (basecoord[1] + j) % num1dim;
            for(size_t k = num1dim - shell_max; k <= num1dim + shell_max; k++) {
                // Check that the cell is not in the centrally excluded zone
                if((i <= num1dim - shell_min) || (i >= num1dim + shell_min) || (j <= num1dim - shell_min) || (j >= num1dim + shell_min) ||
                   (k <= num1dim - shell_min) || (k >= num1dim + shell_min)) {
                    coord[2]                = (basecoord[2] + k) % num1dim;
                    (*keys_return)[count++] = hilbert_c2i(3, bits, coord);
                }
            }
        }
    }

    // Sanity check
    if(count != (size_t)(*n_keys_return))
        SID_exit_error("Resulting key count in compute_PHK_from_Cartesian does not make sense (ie %d!=%d)",
                       SID_ERROR_LOGIC, count, (*n_keys_return));
}
