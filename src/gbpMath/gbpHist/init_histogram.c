#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <gbpLib.h>
#include <gbpHist.h>

void init_histogram(hist_info *hist, int mode, ...) {
    va_list vargs;
    va_start(vargs, mode);

    // Determine what type of vargs we have been passed
    int          flag_use_gbp_vargs = GBP_FALSE;
    gbp_va_list *vargs_gbp          = NULL;
    if(SID_CHECK_BITFIELD_SWITCH(mode, GBP_HISTOGRAM_GBP_VARGS)) {
        vargs_gbp = (gbp_va_list *)va_arg(vargs, gbp_va_list *);
        SID_va_start(vargs_gbp);
        flag_use_gbp_vargs = GBP_TRUE;
    }

    // Set some initial defaults
    hist->flag_bin_order_inverted = GBP_FALSE;

    // Set things dependant on mode
    if(SID_CHECK_BITFIELD_SWITCH(mode, GBP_HISTOGRAM_FIXED)) {
        if(flag_use_gbp_vargs) {
            SID_fetch_va_arg(vargs_gbp, sizeof(double), &(hist->x_min));
            SID_fetch_va_arg(vargs_gbp, sizeof(double), &(hist->dx));
            SID_fetch_va_arg(vargs_gbp, sizeof(int), &(hist->n_bins));
        } else {
            hist->x_min  = (double)va_arg(vargs, double);
            hist->dx     = (double)va_arg(vargs, double);
            hist->n_bins = (int)va_arg(vargs, int);
        }
        hist->x_lo = NULL;
        hist->x_hi = NULL;
    } else if(SID_CHECK_BITFIELD_SWITCH(mode, GBP_HISTOGRAM_IRREGULAR_XLO_DEFINED)) {
        double *x_lo_in;
        if(flag_use_gbp_vargs) {
            SID_fetch_va_arg(vargs_gbp, sizeof(double *), &x_lo_in);
            SID_fetch_va_arg(vargs_gbp, sizeof(int), &(hist->n_bins));
        } else {
            x_lo_in      = (double *)va_arg(vargs, double *);
            hist->n_bins = (int)va_arg(vargs, int);
        }
        hist->x_lo = (double *)SID_malloc(sizeof(double) * hist->n_bins);
        hist->x_hi = NULL;
        memcpy(hist->x_lo, x_lo_in, sizeof(double) * hist->n_bins);
        // Check that the x_min values are sorted properly
        int flag_error = GBP_FALSE;
        for(int i_bin = 0; i_bin < (hist->n_bins - 1); i_bin++) {
            if(hist->x_lo[i_bin + 1] < hist->x_lo[i_bin])
                flag_error = GBP_TRUE;
        }
        // Try reversing axis order if they are not
        if(flag_error) {
            flag_error = GBP_FALSE;
            for(int i_bin = 0; i_bin < (hist->n_bins - 1); i_bin++) {
                if(hist->x_lo[i_bin] < hist->x_lo[i_bin + 1])
                    flag_error = GBP_TRUE;
            }
            if(!flag_error)
                hist->flag_bin_order_inverted = GBP_TRUE;
        }
        // If reversing bin order failed, then this array is not sorted
        if(flag_error) {
            for(int i_bin = 0; i_bin < hist->n_bins; i_bin++)
                SID_log("%d %le", SID_LOG_COMMENT, i_bin, x_lo_in[i_bin]);
            SID_exit_error("The x_lo array passed to init_histogram() is not sorted.", SID_ERROR_LOGIC);
        }
    } else
        SID_exit_error("Invalid mode (%d) specified in init_histogram().", SID_ERROR_LOGIC, mode);

    // Set things common to all modes
    hist->mode           = mode;
    hist->count_hist     = 0;
    hist->count_all      = 0;
    hist->bin_count      = (size_t *)SID_calloc(sizeof(size_t) * hist->n_bins);
    hist->flag_finalized = GBP_FALSE;

    va_end(vargs);
}
