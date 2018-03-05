#include <gbpSID.h>
#include <gbpCalc.h>

void calc_mean(void *data, void *result, size_t n_data, SID_Datatype type, int mode) {
    double temp;
    int    flag_abs;
    if(SID_CHECK_BITFIELD_SWITCH(mode, CALC_MODE_ABS))
        flag_abs = CALC_MODE_ABS;
    else
        flag_abs = GBP_FALSE;
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
        calc_sum(data, &temp, n_data, type, CALC_MODE_RETURN_DOUBLE | flag_abs);
        temp /= (double)n_data;
        if(type == SID_DOUBLE)
            ((double *)result)[0] = (double)temp;
        else if(type == SID_FLOAT)
            ((float *)result)[0] = (float)temp;
        else if(type == SID_UNSIGNED)
            ((unsigned int *)result)[0] = (unsigned int)temp;
        else if(type == SID_SIZE_T)
            ((size_t *)result)[0] = (size_t)temp;
    }
}
