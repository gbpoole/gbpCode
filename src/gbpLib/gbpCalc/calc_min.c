#include <gbpSID.h>
#include <gbpCalc.h>

void calc_min(void *data, void *result, size_t n_data, SID_Datatype type, int mode) {

    // Initialize to zero
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
        SID_exit_error("Unknown variable type in calc_max", SID_ERROR_LOGIC);

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
            SID_exit_error("Unknown variable type in calc_max", SID_ERROR_LOGIC);
    } else if(SID_CHECK_BITFIELD_SWITCH(mode, CALC_MODE_RETURN_DOUBLE)) {
        if(type == SID_DOUBLE)
            ((double *)result)[0] = (double)((double *)data)[0];
        else if(type == SID_FLOAT)
            ((double *)result)[0] = (double)((float *)data)[0];
        else if(type == SID_INT)
            ((double *)result)[0] = (double)((int *)data)[0];
        else if(type == SID_UNSIGNED)
            ((double *)result)[0] = (double)((unsigned int *)data)[0];
        else if(type == SID_SIZE_T)
            ((double *)result)[0] = (double)((size_t *)data)[0];
        else
            SID_exit_error("Unknown variable type in calc_max\n", SID_ERROR_LOGIC);
        if(SID_CHECK_BITFIELD_SWITCH(mode, CALC_MODE_ABS)) {
            if(type == SID_DOUBLE)
                ((double *)result)[0] = GBP_IABS((double)((double *)data)[0]);
            else if(type == SID_FLOAT)
                ((double *)result)[0] = GBP_IABS((double)((float *)data)[0]);
            else if(type == SID_INT)
                ((double *)result)[0] = GBP_IABS((double)((int *)data)[0]);
            else if(type == SID_UNSIGNED)
                ((double *)result)[0] = GBP_IABS((double)((unsigned int *)data)[0]);
            else if(type == SID_SIZE_T)
                ((double *)result)[0] = GBP_IABS((double)((size_t *)data)[0]);
            else
                SID_exit_error("Unknown variable type in calc_max\n", SID_ERROR_LOGIC);
            if(type == SID_DOUBLE) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if(GBP_IABS((double)(((double *)data)[i_data])) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((double *)data)[i_data]);
            } else if(type == SID_FLOAT) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if(GBP_IABS((double)(((float *)data)[i_data])) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((float *)data)[i_data]);
            } else if(type == SID_INT) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if(GBP_IABS((double)(((int *)data)[i_data])) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((int *)data)[i_data]);
            } else if(type == SID_UNSIGNED) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if(GBP_IABS((double)(((unsigned int *)data)[i_data])) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((unsigned int *)data)[i_data]);
            } else if(type == SID_SIZE_T) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if(GBP_IABS((double)(((size_t *)data)[i_data])) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((size_t *)data)[i_data]);
            }
        } else {
            if(type == SID_DOUBLE)
                ((double *)result)[0] = (double)((double *)data)[0];
            else if(type == SID_FLOAT)
                ((double *)result)[0] = (double)((float *)data)[0];
            else if(type == SID_INT)
                ((double *)result)[0] = (double)((int *)data)[0];
            else if(type == SID_UNSIGNED)
                ((double *)result)[0] = (double)((unsigned int *)data)[0];
            else if(type == SID_SIZE_T)
                ((double *)result)[0] = (double)((size_t *)data)[0];
            else
                SID_exit_error("Unknown variable type in calc_max\n", SID_ERROR_LOGIC);
            if(type == SID_DOUBLE) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if((double)(((double *)data)[i_data]) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((double *)data)[i_data]);
            } else if(type == SID_FLOAT) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if((double)(((float *)data)[i_data]) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((float *)data)[i_data]);
            } else if(type == SID_INT) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if((double)(((int *)data)[i_data]) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((int *)data)[i_data]);
            } else if(type == SID_UNSIGNED) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if((double)(((unsigned int *)data)[i_data]) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((unsigned int *)data)[i_data]);
            } else if(type == SID_SIZE_T) {
                for(size_t i_data = 1; i_data < n_data; i_data++)
                    if((double)(((size_t *)data)[i_data]) < (double)(((double *)result)[0]))
                        ((double *)result)[0] = (double)(((size_t *)data)[i_data]);
            }
        }
    } else {
        float result_f;
        if(type == SID_DOUBLE)
            ((double *)result)[0] = ((double *)data)[0];
        else if(type == SID_FLOAT)
            result_f = ((float *)data)[0];
        else if(type == SID_INT)
            ((int *)result)[0] = ((int *)data)[0];
        else if(type == SID_UNSIGNED)
            ((unsigned int *)result)[0] = ((unsigned int *)data)[0];
        else if(type == SID_SIZE_T)
            ((size_t *)result)[0] = ((size_t *)data)[0];
        else
            SID_exit_error("Unknown variable type in calc_max\n", SID_ERROR_LOGIC);
        if(type == SID_DOUBLE) {
            for(size_t i_data = 1; i_data < n_data; i_data++)
                if(((double *)data)[i_data] < ((double *)result)[0])
                    ((double *)result)[0] = ((double *)data)[i_data];
        } else if(type == SID_FLOAT) {
            for(size_t i_data = 1; i_data < n_data; i_data++)
                if(((float *)data)[i_data] < result_f)
                    result_f = ((float *)data)[i_data];
            ((float *)result)[0] = result_f;
        } else if(type == SID_INT) {
            for(size_t i_data = 1; i_data < n_data; i_data++)
                if(((int *)data)[i_data] < ((int *)result)[0])
                    ((int *)result)[0] = ((int *)data)[i_data];
        } else if(type == SID_UNSIGNED) {
            for(size_t i_data = 1; i_data < n_data; i_data++)
                if(((unsigned int *)data)[i_data] < ((unsigned int *)result)[0])
                    ((unsigned int *)result)[0] = ((unsigned int *)data)[i_data];
        } else if(type == SID_SIZE_T) {
            for(size_t i_data = 1; i_data < n_data; i_data++)
                if(((size_t *)data)[i_data] < ((size_t *)result)[0])
                    ((size_t *)result)[0] = ((size_t *)data)[i_data];
        }
    }
}
