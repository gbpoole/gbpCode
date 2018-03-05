#include <math.h>
#include <gbpSID.h>
#include <gbpCalc.h>

void calc_stddev_global(void *data, void *result, size_t n_data_local, SID_Datatype type, int mode, SID_Comm *comm) {
    double stddev;
    double mean;
    size_t n_data;

    SID_Allreduce(&n_data_local, &n_data, 1, SID_SIZE_T, SID_SUM, comm);
    if(n_data < 1) {
        if(type == SID_DOUBLE || SID_CHECK_BITFIELD_SWITCH(mode, CALC_MODE_RETURN_DOUBLE))
            ((double *)result)[0] = 0.;
        else if(type == SID_FLOAT)
            ((float *)result)[0] = 0.;
        else if(type == SID_INT)
            ((int *)result)[0] = 0;
        else if(type == SID_UNSIGNED)
            ((unsigned int *)result)[0] = 0;
        else if(type == SID_SIZE_T)
            ((size_t *)result)[0] = 0;
        else
            SID_exit_error("Unknown variable type in calc_min", SID_ERROR_LOGIC);
    } else {
        if(SID_CHECK_BITFIELD_SWITCH(mode, CALC_MODE_ABS)) {
            calc_mean_global(data, &mean, n_data_local, type, CALC_MODE_RETURN_DOUBLE | CALC_MODE_ABS, comm);
            for(size_t i_data = 0, stddev = 0.; i_data < n_data_local; i_data++) {
                if(type == SID_DOUBLE)
                    stddev += pow(GBP_IABS((double)((double *)data)[i_data]) - mean, 2.);
                else if(type == SID_FLOAT)
                    stddev += pow(GBP_IABS((double)((float *)data)[i_data]) - mean, 2.);
                else if(type == SID_INT)
                    stddev += pow(GBP_IABS((double)((int *)data)[i_data]) - mean, 2.);
                else if(type == SID_UNSIGNED)
                    stddev += pow(GBP_IABS((double)((unsigned int *)data)[i_data]) - mean, 2.);
                else if(type == SID_SIZE_T)
                    stddev += pow(GBP_IABS((double)((size_t *)data)[i_data]) - mean, 2.);
                else
                    SID_exit_error("Unknown variable type in calc_stddev", SID_ERROR_LOGIC);
            }
        } else {
            calc_mean_global(data, &mean, n_data_local, type, CALC_MODE_RETURN_DOUBLE, comm);
            for(size_t i_data = 0, stddev = 0.; i_data < n_data_local; i_data++) {
                if(type == SID_DOUBLE)
                    stddev += pow((double)((double *)data)[i_data] - mean, 2.);
                else if(type == SID_FLOAT)
                    stddev += pow((double)((float *)data)[i_data] - mean, 2.);
                else if(type == SID_INT)
                    stddev += pow((double)((int *)data)[i_data] - mean, 2.);
                else if(type == SID_UNSIGNED)
                    stddev += pow((double)((unsigned int *)data)[i_data] - mean, 2.);
                else if(type == SID_SIZE_T)
                    stddev += pow((double)((size_t *)data)[i_data] - mean, 2.);
                else
                    SID_exit_error("Unknown variable type in calc_stddev", SID_ERROR_LOGIC);
            }
        }
        SID_Allreduce(SID_IN_PLACE, &stddev, 1, SID_DOUBLE, SID_SUM, comm);
        stddev = sqrt(stddev / (double)n_data);
        if(type == SID_DOUBLE || SID_CHECK_BITFIELD_SWITCH(mode, CALC_MODE_RETURN_DOUBLE))
            ((double *)result)[0] = (double)stddev;
        else if(type == SID_FLOAT)
            ((float *)result)[0] = (float)stddev;
        else if(type == SID_INT)
            ((int *)result)[0] = (int)stddev;
        else if(type == SID_UNSIGNED)
            ((unsigned int *)result)[0] = (unsigned int)stddev;
        else if(type == SID_SIZE_T)
            ((size_t *)result)[0] = (size_t)stddev;
        else
            SID_exit_error("Unknown variable type in calc_min", SID_ERROR_LOGIC);
    }
}
