#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpHist.h>

int calc_histogram_index(hist_info *hist, double x) {
    int r_val = -1;
    if(SID_CHECK_BITFIELD_SWITCH(hist->mode, GBP_HISTOGRAM_FIXED)) {
        r_val = (int)((x - hist->x_min) / hist->dx);
        // Deal with bin-order swapping
        switch(hist->flag_bin_order_inverted) {
            case GBP_TRUE:
                r_val = (hist->n_bins - r_val - 1);
                break;
        }
    } else if(SID_CHECK_BITFIELD_SWITCH(hist->mode, GBP_HISTOGRAM_IRREGULAR_XLO_DEFINED)) {
        int flag_done = GBP_FALSE;
        switch(hist->flag_bin_order_inverted) {
            case GBP_FALSE:
                for(int i_bin = 0; i_bin < hist->n_bins && !flag_done; i_bin++) {
                    if(x >= histogram_bin_x_lo(hist, i_bin)) {
                        if(x < histogram_bin_x_hi(hist, i_bin)) {
                            r_val     = i_bin;
                            flag_done = GBP_TRUE;
                        }
                    } else
                        flag_done = GBP_TRUE;
                }
                break;
            case GBP_TRUE:
                for(int i_bin = (hist->n_bins - 1); i_bin >= 0 && !flag_done; i_bin++) {
                    if(x >= histogram_bin_x_lo(hist, i_bin)) {
                        if(x < histogram_bin_x_hi(hist, i_bin)) {
                            r_val     = i_bin;
                            flag_done = GBP_TRUE;
                        }
                    } else
                        flag_done = GBP_TRUE;
                }
                break;
        }
    } else
        SID_exit_error("Invalid mode (%d) specified in clear_histogram().", SID_ERROR_LOGIC, hist->mode);

    return (r_val);
}
