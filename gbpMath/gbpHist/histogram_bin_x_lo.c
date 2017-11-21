#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <gbpLib.h>
#include <gbpHist.h>

double histogram_bin_x_lo(hist_info *hist, int bin) {
    double r_val;

    // Sanity check
    if(!is_histogram_index_in_range(hist, bin))
        SID_exit_error("Invalid bin requested (%d) in histogram_bin_x_hi().", SID_ERROR_LOGIC, bin);

    if(SID_CHECK_BITFIELD_SWITCH(hist->mode, GBP_HISTOGRAM_FIXED)) {
        switch(hist->flag_bin_order_inverted) {
            case GBP_FALSE:
                r_val = hist->x_min + ((double)(bin)) * hist->dx;
                break;
            case GBP_TRUE:
                r_val = hist->x_min + ((double)(hist->n_bins - bin - 1)) * hist->dx;
                break;
        }
    } else if(SID_CHECK_BITFIELD_SWITCH(hist->mode, GBP_HISTOGRAM_IRREGULAR_XLO_DEFINED))
        r_val = hist->x_lo[bin];
    else
        SID_exit_error("Invalid mode (%d) specified in histogram_bin_x_lo().", SID_ERROR_LOGIC, hist->mode);
    return (r_val);
}
